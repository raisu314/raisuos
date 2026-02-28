#include "apps.h"
#include "font.h"
#include "graphics.h"
#include "i18n.h"

static void files_paint(window_t *win) {
  gfx_fill_rect_buffer(
      win->buffer, win->width, win->height, 0, TITLE_BAR_HEIGHT, win->width,
      win->height - TITLE_BAR_HEIGHT, RGBA(240, 240, 240, 255));

  int y = TITLE_BAR_HEIGHT + 10;
  font_draw_string_buffer(win->buffer, win->width, 10, y, "A:/ (initrd)",
                          RGBA(0, 0, 0, 255), 0);
  y += 20;

  /* Mock filesystem */
  font_draw_string_buffer(win->buffer, win->width, 20, y, "[DIR]  System",
                          RGBA(50, 50, 200, 255), 0);
  y += 15;
  font_draw_string_buffer(win->buffer, win->width, 20, y, "[DIR]  Users",
                          RGBA(50, 50, 200, 255), 0);
  y += 15;
  font_draw_string_buffer(win->buffer, win->width, 20, y,
                          "[FILE] config.ini  (4 KB)", RGBA(50, 50, 50, 255),
                          0);
  y += 15;
  font_draw_string_buffer(win->buffer, win->width, 20, y,
                          "[FILE] readme.txt  (1 KB)", RGBA(50, 50, 50, 255),
                          0);
  y += 15;
}

void app_files_launch(void) {
  window_t *win = wm_create_window(i18n_get(STR_FILES), 120, 120, 400, 300,
                                   WIN_FLAG_MOVABLE | WIN_FLAG_CLOSABLE |
                                       WIN_FLAG_DECORATED);
  win->on_paint = files_paint;
  win->on_close = (window_close_t)wm_destroy_window;
}
