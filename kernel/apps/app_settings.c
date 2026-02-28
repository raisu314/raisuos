#include "apps.h"
#include "font.h"
#include "graphics.h"
#include "i18n.h"


static void settings_paint(window_t *win) {
  gfx_fill_rect_buffer(win->buffer, win->width, 0, TITLE_BAR_HEIGHT, win->width,
                       win->height - TITLE_BAR_HEIGHT,
                       RGBA(255, 255, 255, 255));

  int y = TITLE_BAR_HEIGHT + 10;
  font_draw_string_buffer(win->buffer, win->width, 20, y, "Language: English",
                          RGBA(0, 0, 0, 255), 0);
  y += 25;
  font_draw_string_buffer(win->buffer, win->width, 20, y,
                          "Theme: Default Frost", RGBA(0, 0, 0, 255), 0);
  y += 25;
  font_draw_string_buffer(win->buffer, win->width, 20, y,
                          "Resolution: 1024x768x32", RGBA(0, 0, 0, 255), 0);
  y += 25;
}

void app_settings_launch(void) {
  window_t *win = wm_create_window(i18n_get(STR_SETTINGS), 400, 100, 350, 400,
                                   WIN_FLAG_MOVABLE | WIN_FLAG_CLOSABLE |
                                       WIN_FLAG_DECORATED);
  win->on_paint = settings_paint;
  win->on_close = (window_close_t)wm_destroy_window;
}
