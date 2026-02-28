#include "apps.h"
#include "font.h"
#include "graphics.h"
#include "i18n.h"
#include "window.h"

static void on_paint(window_t *win) {
  gfx_fill_rect_buffer(win->buffer, win->width, 0, 0, win->width, win->height,
                       COLOR_MAIN);

  int y = 20, x = 20;
  font_draw_string_buffer(win->buffer, win->width, x, y, "ADNWS Settings",
                          RGBA(0, 100, 200, 255), 0);
  y += 40;

  /* Sidebar / Options */
  font_draw_string_buffer(win->buffer, win->width, 20, 50, "> Display & Ratios",
                          COLOR_TEXT_DARK, 0);
  font_draw_string_buffer(win->buffer, win->width, 20, 80,
                          "> Raisu-Glass Theme", COLOR_TEXT_DARK, 0);
  font_draw_string_buffer(win->buffer, win->width, 20, 110, "> Language (i18n)",
                          COLOR_TEXT_DARK, 0);

  /* ADNWS Sub-Branding Section */
  gfx_fill_rounded_rect_buffer(win->buffer, win->width, win->height, 20, 150,
                               win->width - 40, 80, 8,
                               RGBA(240, 245, 255, 200));
  font_draw_string_buffer(win->buffer, win->width, 30, 160,
                          "Subsystem: ADNWS v1.0.0", COLOR_SUB, 0);
  font_draw_string_buffer(win->buffer, win->width, 30, 180,
                          "Build: 2026.02.28-v1.0.0", RGBA(150, 150, 160, 255),
                          0);
  font_draw_string_buffer(win->buffer, win->width, 30, 200,
                          "Status: Hyper-Enhanced", RGBA(100, 200, 150, 255),
                          0);
}

void app_settings_launch(void) {
  window_t *win = wm_create_window("Settings", 300, 200, 350, 260,
                                   WIN_FLAG_MOVABLE | WIN_FLAG_CLOSABLE |
                                       WIN_FLAG_DECORATED);
  if (win)
    win->on_paint = on_paint;
}
