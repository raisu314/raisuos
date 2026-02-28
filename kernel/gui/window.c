#include "window.h"
#include "font.h"
#include "graphics.h"
#include "heap.h"
#include "string.h"

static window_t windows[MAX_WINDOWS];
static int window_count = 0;
static window_t *focused_window = NULL;
static int current_desktop = 0;

void wm_init(void) {
  memset(windows, 0, sizeof(windows));
  window_count = 0;
  focused_window = NULL;
}

window_t *wm_create_window(const char *title, int x, int y, int w, int h,
                           uint32_t flags) {
  if (window_count >= MAX_WINDOWS)
    return NULL;

  window_t *win = &windows[window_count++];
  win->id = window_count;
  win->x = x;
  win->y = y;
  win->width = w;
  win->height = h;
  win->flags = flags | WIN_FLAG_VISIBLE;

  strncpy(win->title, title, sizeof(win->title) - 1);

  win->content_width = w;
  win->content_height =
      (flags & WIN_FLAG_DECORATED) ? (h - TITLE_BAR_HEIGHT) : h;

  /* Allocate rendering buffer safely using 64-bit size_t */
  size_t buf_size = win->width * win->height * sizeof(uint32_t);
  win->buffer = (uint32_t *)kmalloc_aligned(buf_size, 4096);
  memset(win->buffer, 0, buf_size);

  win->desktop_id = current_desktop;
  win->anim_progress = 0;
  win->anim_type = 1; /* Opening */

  wm_focus_window(win);
  return win;
}

void wm_destroy_window(window_t *win) {
  if (!win)
    return;
  win->flags &= ~WIN_FLAG_VISIBLE;

  /* Reassign focus if needed */
  if (focused_window == win) {
    focused_window = NULL;
    for (int i = window_count - 1; i >= 0; i--) {
      if ((windows[i].flags & WIN_FLAG_VISIBLE) &&
          windows[i].desktop_id == current_desktop) {
        wm_focus_window(&windows[i]);
        break;
      }
    }
  }
}

void wm_focus_window(window_t *win) {
  if (focused_window)
    focused_window->flags &= ~WIN_FLAG_FOCUSED;
  if (win)
    win->flags |= WIN_FLAG_FOCUSED;
  focused_window = win;
}

static void draw_window_decorations(window_t *win) {
  if (!(win->flags & WIN_FLAG_DECORATED))
    return;

  /* Premium Title Bar: Slate Blue Depth */
  uint32_t title_col = (win->flags & WIN_FLAG_FOCUSED) ? RGBA(45, 55, 75, 255)
                                                       : RGBA(40, 40, 45, 255);

  /* Clear window buffer with transparent rounded base first?
   * Actually, windows are usually rects, but we'll round the decorations. */
  gfx_fill_rounded_rect_buffer(win->buffer, win->width, win->height, 0, 0,
                               win->width, win->height, 12,
                               RGBA(25, 25, 30, 255));

  gfx_fill_rounded_rect_buffer(win->buffer, win->width, win->height, 0, 0,
                               win->width, TITLE_BAR_HEIGHT, 12, title_col);
  /* Patch bottom of title bar rounding to keep content area square */
  gfx_fill_rect_buffer(win->buffer, win->width, 0, 10, win->width,
                       TITLE_BAR_HEIGHT - 10, title_col);

  uint32_t text_col = RGBA(240, 240, 255, 255);
  font_draw_string_buffer(win->buffer, win->width, 15,
                          (TITLE_BAR_HEIGHT - 8) / 2, win->title, text_col, 0);

  /* Premium Close Button: Circular Glow */
  if (win->flags & WIN_FLAG_CLOSABLE) {
    int bx = win->width - 25;
    int by = (TITLE_BAR_HEIGHT - 12) / 2;
    gfx_fill_rounded_rect_buffer(win->buffer, win->width, win->height, bx, by,
                                 12, 12, 6, RGBA(255, 80, 80, 255));
  }
}

void wm_draw_all(void) {
  vbe_info_t *vbe = vbe_get_info();

  /* Draw windows backwards (painters algorithm) */
  for (int i = 0; i < window_count; i++) {
    window_t *win = &windows[i];
    if (!(win->flags & WIN_FLAG_VISIBLE))
      continue;
    if (win->flags & WIN_FLAG_MINIMIZED)
      continue;
    if (win->desktop_id != current_desktop && win->desktop_id != -1)
      continue;

    /* Erase window buffer with "Transparent" (0) */
    memset(win->buffer, 0, win->width * win->height * sizeof(uint32_t));

    /* Shadow on the main VBE buffer BEFORE drawing the window */
    gfx_draw_shadow_buffer(vbe->backbuffer, vbe->width, vbe->height, win->x,
                           win->y, win->width, win->height, 12, 10);

    draw_window_decorations(win);

    if (win->on_paint)
      win->on_paint(win);

    /* Compositing with Alpha/Transparency Check */
    for (int row = 0; row < win->height; row++) {
      for (int col = 0; col < win->width; col++) {
        int screen_x = win->x + col;
        int screen_y = win->y + row;

        if (screen_x >= 0 && screen_y >= 0 && (uint32_t)screen_x < vbe->width &&
            (uint32_t)screen_y < vbe->height) {
          uint32_t clr = win->buffer[row * win->width + col];
          if (clr != 0) {
            vbe_set_pixel(screen_x, screen_y, clr);
          }
        }
      }
    }
  }
}

void wm_handle_mouse(int mx, int my, bool left, bool right) {
  if (left) {
    /* Extremely crude hit testing - Top window wins */
    for (int i = window_count - 1; i >= 0; i--) {
      window_t *win = &windows[i];
      if (!(win->flags & WIN_FLAG_VISIBLE))
        continue;

      if (mx >= win->x && mx < win->x + win->width && my >= win->y &&
          my < win->y + win->height) {
        wm_focus_window(win);

        /* Titlebar click for closing */
        if (my < win->y + TITLE_BAR_HEIGHT) {
          if (mx >= win->x + win->width - BUTTON_SIZE - 10 &&
              mx <= win->x + win->width - 10) {
            if (win->on_close)
              win->on_close(win);
            else
              wm_destroy_window(win);
          }
        } else {
          if (win->on_mouse)
            win->on_mouse(win, mx - win->x, my - win->y - TITLE_BAR_HEIGHT,
                          left, right);
        }
        return; /* Block pass-through */
      }
    }
  }
}

void wm_handle_key(char key, uint8_t keycode, bool pressed, bool ctrl, bool alt,
                   bool shift) {
  UNUSED(shift);
  if (focused_window && focused_window->on_key) {
    focused_window->on_key(focused_window, key, pressed);
  }

  /* Desktop Switching Alt+Tab or Ctrl+Alt+Arrows */
  if (ctrl && alt && pressed) {
    if (keycode == 0x4B) { /* Left arrow */
      current_desktop = (current_desktop > 0) ? current_desktop - 1 : 3;
    } else if (keycode == 0x4D) { /* Right arrow */
      current_desktop = (current_desktop < 3) ? current_desktop + 1 : 0;
    }
  }
}

window_t *wm_get_focused(void) { return focused_window; }
int wm_get_desktop(void) { return current_desktop; }
void wm_set_desktop(int id) { current_desktop = id; }
