#include "apps.h"
#include "font.h"
#include "graphics.h"
#include "window.h"

static void on_paint(window_t *win) {
  gfx_fill_rect_buffer(win->buffer, win->width, 0, 0, win->width, win->height,
                       COLOR_MAIN);

  /* Calendar Header */
  font_draw_string_buffer(win->buffer, win->width, 10, 10, "February 2026",
                          COLOR_TEXT_DARK, 0);

  /* Simple Day Grid */
  uint32_t accent = COLOR_SUB;
  for (int i = 0; i < 7; i++) {
    for (int j = 0; j < 5; j++) {
      int day = i + j * 7 + 1;
      if (day > 28)
        break;
      char buf[4];
      itoa(day, buf, 10);
      gfx_fill_rounded_rect_buffer(
          win->buffer, win->width, win->height, 10 + i * 35, 40 + j * 35, 30,
          30, 4, (day == 28) ? accent : RGBA(245, 245, 250, 255));
      font_draw_string_buffer(win->buffer, win->width, 15 + i * 35, 48 + j * 35,
                              buf, COLOR_TEXT_DARK, 0);
    }
  }
}

void app_calendar_launch(void) {
  window_t *win = wm_create_window("Calendar", 200, 100, 270, 240,
                                   WIN_FLAG_DECORATED | WIN_FLAG_CLOSABLE);
  if (win) {
    win->on_paint = on_paint;
  }
}
