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
  win->anim_progress = 0.0f;
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

  /* Frosted glass title bar imitation: Light grey */
  gfx_fill_rect_buffer(win->buffer, win->width, 0, 0, win->width,
                       TITLE_BAR_HEIGHT, RGBA(220, 230, 240, 255));

  uint32_t text_col = (win->flags & WIN_FLAG_FOCUSED)
                          ? RGBA(0, 0, 0, 255)
                          : RGBA(100, 100, 100, 255);
  font_draw_string_buffer(win->buffer, win->width, 10,
                          (TITLE_BAR_HEIGHT - 8) / 2, win->title, text_col, 0);

  /* Close Button */
  if (win->flags & WIN_FLAG_CLOSABLE) {
    gfx_draw_rect_buffer(win->buffer, win->width, win->width - BUTTON_SIZE - 10,
                         (TITLE_BAR_HEIGHT - BUTTON_SIZE) / 2, BUTTON_SIZE,
                         BUTTON_SIZE, 1, RGBA(255, 50, 50, 255));
  }
}

void wm_draw_all(void) {
  /* Draw background desktop here or handled via desktop.c */

  /* Draw windows backwards (painters algorithm) */
  for (int i = 0; i < window_count; i++) {
    window_t *win = &windows[i];
    if (!(win->flags & WIN_FLAG_VISIBLE))
      continue;
    if (win->flags & WIN_FLAG_MINIMIZED)
      continue;
    if (win->desktop_id != current_desktop && win->desktop_id != -1)
      continue;

    /* Erase background */
    memset(win->buffer, 0, win->width * win->height * sizeof(uint32_t));

    draw_window_decorations(win);

    if (win->on_paint)
      win->on_paint(win);

    /* Compositing step to the global VBE Double Buffer */
    vbe_info_t *vbe = vbe_get_info();
    for (int row = 0; row < win->height; row++) {
      for (int col = 0; col < win->width; col++) {
        int screen_x = win->x + col;
        int screen_y = win->y + row;

        if (screen_x >= 0 && screen_y >= 0 && (uint32_t)screen_x < vbe->width &&
            (uint32_t)screen_y < vbe->height) {
          uint32_t clr = win->buffer[row * win->width + col];
          if (clr != 0) { /* Treat 0 as transparent */
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
