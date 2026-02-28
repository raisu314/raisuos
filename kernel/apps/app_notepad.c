#include "apps.h"
#include "font.h"
#include "graphics.h"
#include "string.h"
#include "window.h"

static void on_paint(window_t *win) {
  /* Note Paper Aesthetic (Slightly warm white) */
  gfx_fill_rect_buffer(win->buffer, win->width, win->height, 0, 0, win->width,
                       win->height, RGBA(255, 255, 250, 255));

  /* Iridescent Accent */
  gfx_draw_iridescent_line(win->buffer, win->width, win->height, 0, 0,
                           win->width);

  /* Ruling Lines */
  for (int i = 0; i < 15; i++) {
    gfx_fill_rect_buffer(win->buffer, win->width, win->height, 0, 40 + i * 20,
                         win->width, 1, RGBA(230, 235, 245, 255));
  }

  font_draw_string_buffer(win->buffer, win->width, win->height, 10, 15, "Journal - Feb 28",
                          RGBA(150, 155, 170, 255), 0);
  font_draw_string_buffer(win->buffer, win->width, win->height, 20, 45,
                          "Today I am building RaisuOS v1.0.0!",
                          COLOR_TEXT_DARK, 0);
}

void app_notepad_launch(void) {
  window_t *win = wm_create_window("Note Pad", 100, 300, 320, 350,
                                   WIN_FLAG_DECORATED | WIN_FLAG_CLOSABLE |
                                       WIN_FLAG_MOVABLE);
  if (win)
    win->on_paint = on_paint;
}
