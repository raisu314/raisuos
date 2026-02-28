#include "apps.h"
#include "font.h"
#include "graphics.h"
#include "window.h"

static void on_paint(window_t *win) {
  /* Dark Mica / Iridescent Background */
  gfx_fill_rect_buffer(win->buffer, win->width, win->height, 0, 0, win->width,
                       win->height, RGBA(30, 35, 45, 255));
  gfx_draw_iridescent_line(win->buffer, win->width, win->height, 0,
                           win->height - 4, win->width);

  /* Spectrum Visualizer (Placeholder) */
  for (int i = 0; i < 30; i++) {
    int h = 20 + (i % 5) * 10;
    gfx_fill_rect_buffer(win->buffer, win->width, win->height, 20 + i * 10,
                         150 - h, 6, h, COLOR_SUB);
  }

  font_draw_string_buffer(win->buffer, win->width, 20, 180,
                          "Raisu Beats - LoFi", COLOR_MAIN, 0);
  font_draw_string_buffer(win->buffer, win->width, 20, 200, "02:45 / 04:00",
                          RGBA(180, 180, 190, 255), 0);
}

void app_music_launch(void) {
  window_t *win = wm_create_window("Music Player", 50, 50, 350, 240,
                                   WIN_FLAG_DECORATED | WIN_FLAG_CLOSABLE |
                                       WIN_FLAG_MOVABLE);
  if (win)
    win->on_paint = on_paint;
}
