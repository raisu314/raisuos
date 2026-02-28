#include "desktop.h"
#include "graphics.h"
#include "launcher.h"
#include "mouse.h"
#include "taskbar.h"
#include "vbe.h"
#include "window.h"

/* Mouse cursor */
static const uint8_t cursor_map[12][9] = {
    {1, 0, 0, 0, 0, 0, 0, 0, 0}, {1, 1, 0, 0, 0, 0, 0, 0, 0},
    {1, 2, 1, 0, 0, 0, 0, 0, 0}, {1, 2, 2, 1, 0, 0, 0, 0, 0},
    {1, 2, 2, 2, 1, 0, 0, 0, 0}, {1, 2, 2, 2, 2, 1, 0, 0, 0},
    {1, 2, 2, 2, 2, 2, 1, 0, 0}, {1, 2, 2, 2, 2, 2, 2, 1, 0},
    {1, 2, 2, 1, 1, 1, 1, 1, 1}, {1, 2, 1, 0, 0, 0, 0, 0, 0},
    {1, 1, 0, 0, 0, 0, 0, 0, 0}, {1, 0, 0, 0, 0, 0, 0, 0, 0}};

static uint32_t cursor_x = 512;
static uint32_t cursor_y = 384;
static bool btn_left = false;
static bool btn_right = false;

static void draw_cursor(void) {
  for (int r = 0; r < 12; r++) {
    for (int c = 0; c < 9; c++) {
      if (cursor_map[r][c] == 1) {
        vbe_set_pixel(cursor_x + c, cursor_y + r, RGBA(0, 0, 0, 255));
      } else if (cursor_map[r][c] == 2) {
        vbe_set_pixel(cursor_x + c, cursor_y + r, RGBA(255, 255, 255, 255));
      }
    }
  }
}

void desktop_init(void) {
  wm_init();
  taskbar_init();
  launcher_init();
}

void desktop_draw(void) {
  vbe_info_t *vbe = vbe_get_info();

  /* Deep Space Premium Gradient */
  gfx_draw_gradient_v(0, 0, vbe->width, vbe->height, RGBA(15, 20, 35, 255),
                      RGBA(45, 60, 100, 255));

  wm_draw_all();
  taskbar_draw();
  launcher_draw();
  draw_cursor();
}

void desktop_update(void) { /* Handle window animations if any */ }

static bool dragged = false;
static int drag_off_x = 0;
static int drag_off_y = 0;

void desktop_handle_mouse(int x, int y, bool left, bool right) {
  cursor_x = x;
  cursor_y = y;

  vbe_info_t *vbe = vbe_get_info();

  /* Handle drag */
  if (left && !btn_left) {
    btn_left = true;

    if (launcher_is_open()) {
      launcher_handle_mouse(x, y, left, right);
    } else if (y >=
               (int)(vbe->height - TASKBAR_HEIGHT - 20)) { /* Floating area */
      taskbar_handle_mouse(x, y, left, right);
    } else {
      wm_handle_mouse(x, y, left, right);
      window_t *act = wm_get_focused();
      if (act && act->flags & WIN_FLAG_MOVABLE) {
        if (y < act->y + TITLE_BAR_HEIGHT) { /* Hit title bar */
          dragged = true;
          drag_off_x = x - act->x;
          drag_off_y = y - act->y;
        }
      }
    }
  } else if (!left && btn_left) {
    btn_left = false;
    dragged = false;
  }

  if (dragged && btn_left) {
    window_t *act = wm_get_focused();
    if (act && act->flags & WIN_FLAG_MOVABLE) {
      act->x = x - drag_off_x;
      act->y = y - drag_off_y;

      /* Snap bounds */
      if (act->x < 0)
        act->x = 0;
      if (act->y < 0)
        act->y = 0;
      if (act->x + act->width > (int)vbe_get_info()->width)
        act->x = vbe_get_info()->width - act->width;
    }
  }

  btn_right = right;
}

void desktop_handle_key(char key, uint8_t keycode, bool pressed, bool ctrl,
                        bool alt, bool shift) {
  if (launcher_is_open() && pressed && keycode == 0x01) { /* ESC */
    launcher_hide();
    return;
  }
  taskbar_handle_key(key, keycode, pressed);
  wm_handle_key(key, keycode, pressed, ctrl, alt, shift);
}
