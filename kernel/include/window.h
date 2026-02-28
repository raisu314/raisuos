#ifndef WINDOW_H
#define WINDOW_H

#include "types.h"

#define MAX_WINDOWS 32
#define TITLE_BAR_HEIGHT 24
#define BUTTON_SIZE 16

#define WIN_FLAG_VISIBLE (1 << 0)
#define WIN_FLAG_FOCUSED (1 << 1)
#define WIN_FLAG_MINIMIZED (1 << 2)
#define WIN_FLAG_MOVABLE (1 << 3)
#define WIN_FLAG_DECORATED (1 << 4)
#define WIN_FLAG_CLOSABLE (1 << 5)

struct window;
typedef void (*window_paint_t)(struct window *win);
typedef void (*window_mouse_t)(struct window *win, int x, int y, bool left,
                               bool right);
typedef void (*window_key_t)(struct window *win, char key, bool pressed);
typedef void (*window_close_t)(struct window *win);

typedef struct window {
  int id;
  int x, y;
  int width, height;
  int content_width, content_height;
  uint32_t flags;
  char title[64];
  uint32_t *buffer;
  int desktop_id;

  uint32_t anim_progress;
  int anim_type;

  void *app_data;

  window_paint_t on_paint;
  window_mouse_t on_mouse;
  window_key_t on_key;
  window_close_t on_close;
} window_t;

void wm_init(void);
window_t *wm_create_window(const char *title, int x, int y, int w, int h,
                           uint32_t flags);
void wm_destroy_window(window_t *win);
void wm_focus_window(window_t *win);
void wm_draw_all(void);
void wm_handle_mouse(int x, int y, bool left, bool right);
void wm_handle_key(char key, uint8_t keycode, bool pressed, bool ctrl, bool alt,
                   bool shift);

window_t *wm_get_focused(void);
int wm_get_desktop(void);
void wm_set_desktop(int id);

#endif
