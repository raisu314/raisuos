#include "apps.h"
#include "font.h"
#include "graphics.h"
#include "window.h"

static void on_paint(window_t *win) {
  gfx_fill_rect_buffer(win->buffer, win->width, 0, 0, win->width, win->height,
                       COLOR_MAIN);

  /* Large Analog Clock Face */
  int cx = win->width / 2;
  int cy = win->height / 2;
  gfx_fill_circle_buffer(win->buffer, win->width, cx, cy, 80,
                         RGBA(245, 250, 255, 255));

  /* Iridescent Second Hand Simulation */
  gfx_draw_iridescent_line(win->buffer, win->width, cx, cy, 70);

  /* Digital Readout */
  font_draw_string_buffer(win->buffer, win->width, cx - 40, cy + 90, "20:52:50",
                          COLOR_TEXT_DARK, 0);
  font_draw_string_buffer(win->buffer, win->width, cx - 35, cy + 110,
                          "Tokyo, JP", RGBA(150, 150, 160, 255), 0);
}

void app_clock_launch(void) {
  window_t *win = wm_create_window("World Clock", 500, 300, 250, 300,
                                   WIN_FLAG_DECORATED | WIN_FLAG_CLOSABLE |
                                       WIN_FLAG_MOVABLE);
  if (win)
    win->on_paint = on_paint;
}
