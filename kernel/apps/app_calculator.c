#include "apps.h"
#include "font.h"
#include "graphics.h"
#include "window.h"

static void on_paint(window_t *win) {
  /* Mica Body */
  gfx_fill_rect_buffer(win->buffer, win->width, 0, 0, win->width, win->height,
                       COLOR_MAIN);

  /* Display area (Sky Blue Tint) */
  gfx_fill_rounded_rect_buffer(win->buffer, win->width, win->height, 10, 10,
                               win->width - 20, 40, 6, COLOR_SUB);
  font_draw_string_buffer(win->buffer, win->width, win->width - 40, 20, "0",
                          COLOR_TEXT_DARK, 0);

  /* Grid of buttons placeholder */
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      gfx_fill_rounded_rect_buffer(win->buffer, win->width, win->height,
                                   10 + i * 45, 60 + j * 45, 40, 40, 4,
                                   RGBA(240, 240, 245, 255));
    }
  }
}

void app_calculator_launch(void) {
  window_t *win = wm_create_window("Calculator", 150, 150, 210, 280,
                                   WIN_FLAG_DECORATED | WIN_FLAG_CLOSABLE);
  if (win) {
    win->on_paint = on_paint;
  }
}
