#include "apps.h"
#include "font.h"
#include "graphics.h"
#include "window.h"

static void on_paint(window_t *win) {
  /* Mica / Light Background */
  gfx_fill_rect_buffer(win->buffer, win->width, win->height, 0, 0, win->width,
                       win->height, COLOR_MAIN);
  gfx_draw_iridescent_line(win->buffer, win->width, win->height, 0, 0,
                           win->width);

  /* Photo Grid Placeholder */
  for (int r = 0; r < 2; r++) {
    for (int c = 0; c < 2; c++) {
      int x = 10 + c * 100;
      int y = 40 + r * 80;
      /* Rainbow Segmented Placeholder */
      gfx_fill_rounded_rect_buffer(win->buffer, win->width, win->height, x, y,
                                   90, 70, 4, RGBA(240, 245, 255, 255));
      gfx_fill_circle_buffer(win->buffer, win->width, win->height, x + 45,
                             y + 35, 15, COLOR_SUB);
    }
  }

  font_draw_string_buffer(win->buffer, win->width, win->height, 10, 210,
                          "4 Items - Camera Roll", RGBA(150, 150, 160, 255), 0);
}

void app_photos_launch(void) {
  window_t *win = wm_create_window("Photo Gallery", 150, 200, 220, 240,
                                   WIN_FLAG_DECORATED | WIN_FLAG_CLOSABLE |
                                       WIN_FLAG_MOVABLE);
  if (win)
    win->on_paint = on_paint;
}
