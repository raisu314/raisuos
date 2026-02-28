#include "apps.h"
#include "font.h"
#include "graphics.h"
#include "heap.h"
#include "i18n.h"
#include "string.h"

static void about_paint(window_t *win) {
  gfx_fill_rect_buffer(win->buffer, win->width, 0, TITLE_BAR_HEIGHT, win->width,
                       win->height - TITLE_BAR_HEIGHT,
                       RGBA(255, 255, 255, 255));

  int y = TITLE_BAR_HEIGHT + 20;

  /* Draw RaisuOS Logo mock */
  gfx_fill_rect_buffer(win->buffer, win->width, win->width / 2 - 20, y, 40, 40,
                       RGBA(50, 150, 250, 255));
  font_draw_char_buffer(win->buffer, win->width, win->width / 2 - 4, y + 16,
                        'R', RGBA(255, 255, 255, 255), 0);

  y += 60;

  const char *lines[] = {
      "RaisuOS v1.0.0 - ADNWS Edition", "Architecture: x86_64",
      "All Device Native Work System",  "(C) 2026 RaisuOS Project",
      "Zero Linux Dependencies",        NULL};

  for (int i = 0; lines[i] != NULL; i++) {
    int len = strlen(lines[i]) * 8;
    font_draw_string_buffer(win->buffer, win->width, (win->width - len) / 2, y,
                            lines[i], RGBA(0, 0, 0, 255), 0);
    y += 20;
  }
}

static void about_close(window_t *win) { wm_destroy_window(win); }

void app_about_launch(void) {
  window_t *win = wm_create_window(i18n_get(STR_ABOUT), 300, 200, 300, 250,
                                   WIN_FLAG_MOVABLE | WIN_FLAG_CLOSABLE |
                                       WIN_FLAG_DECORATED);
  win->on_paint = about_paint;
  win->on_close = about_close;
}
