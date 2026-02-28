#include "apps.h"
#include "font.h"
#include "graphics.h"
#include "window.h"

static void on_paint(window_t *win) {
  /* Mica / Pure White Body */
  gfx_fill_rect_buffer(win->buffer, win->width, win->height, 0, 0, win->width,
                       win->height, COLOR_MAIN);

  /* Iridescent Top Accent */
  gfx_draw_iridescent_line(win->buffer, win->width, win->height, 0, 0,
                           win->width);

  /* Weather UI: Large Sun/Cloud Icon Placeholder */
  gfx_fill_circle_buffer(win->buffer, win->width, win->height, win->width / 2,
                         80, 40, 0xFFFFD700); /* Sun */

  font_draw_string_buffer(win->buffer, win->width, win->height,
                          win->width / 2 - 30, 140, "22°C", COLOR_TEXT_DARK, 0);
  font_draw_string_buffer(win->buffer, win->width, win->height,
                          win->width / 2 - 40, 170, "Sunny Sky",
                          RGBA(100, 100, 120, 255), 0);

  /* Weekly Forecast Mini-Grid */
  for (int i = 0; i < 3; i++) {
    gfx_fill_rounded_rect_buffer(win->buffer, win->width, win->height,
                                 20 + i * 60, 210, 50, 70, 8,
                                 RGBA(240, 245, 255, 255));
    font_draw_string_buffer(win->buffer, win->width, win->height, 30 + i * 60,
                            220, "Mon", COLOR_TEXT_DARK, 0);
  }
}

void app_weather_launch(void) {
  window_t *win = wm_create_window("Weather Hub", 400, 150, 220, 300,
                                   WIN_FLAG_DECORATED | WIN_FLAG_CLOSABLE |
                                       WIN_FLAG_MOVABLE);
  if (win)
    win->on_paint = on_paint;
}
