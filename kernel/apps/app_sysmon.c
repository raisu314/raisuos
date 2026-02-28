#include "apps.h"
#include "font.h"
#include "graphics.h"
#include "heap.h"
#include "i18n.h"
#include "string.h"
#include "timer.h"

static void sysmon_paint(window_t *win) {
  /* High-Pure White Background */
  gfx_fill_rect_buffer(win->buffer, win->width, win->height, 0, 0, win->width,
                       win->height, COLOR_MAIN);

  int y = 20;
  int x = 20;

  /* ADNWS Blue Accent for Headers */
  font_draw_string_buffer(win->buffer, win->width, x, y,
                          "ADNWS System Overview", RGBA(0, 100, 200, 255), 0);
  y += 30;

  char uptime[64] = "Uptime: ";
  char num[16];
  itoa(timer_get_ticks() / 1000, num, 10);
  strcat(uptime, num);
  strcat(uptime, " seconds");

  font_draw_string_buffer(win->buffer, win->width, x, y, uptime,
                          COLOR_TEXT_DARK, 0);
  y += 20;

  font_draw_string_buffer(win->buffer, win->width, x, y,
                          "Architecture: x64 v1.0.0", COLOR_TEXT_DARK, 0);
  y += 20;

  /* Memory Statistics */
  uint64_t used = heap_get_used() / 1024;
  uint64_t total = (heap_get_used() + heap_get_free()) / 1024;

  char mem[64] = "Memory Utilization: ";
  itoa(used, num, 10);
  strcat(mem, num);
  strcat(mem, " KB / ");
  itoa(total, num, 10);
  strcat(mem, num);
  strcat(mem, " KB");

  font_draw_string_buffer(win->buffer, win->width, x, y, mem, COLOR_TEXT_DARK,
                          0);
  y += 20;

  /* Premium Progress Bar (Sky Blue) */
  int bar_w = win->width - 40;
  gfx_fill_rounded_rect_buffer(win->buffer, win->width, win->height, x, y,
                               bar_w, 16, 8, RGBA(240, 240, 245, 255));

  int fill_w = (total > 0) ? (used * bar_w / total) : 0;
  if (fill_w > 0) {
    gfx_fill_rounded_rect_buffer(win->buffer, win->width, win->height, x, y,
                                 fill_w, 16, 8, COLOR_SUB);
  }
}

static void sysmon_close(window_t *win) { wm_destroy_window(win); }

void app_sysmon_launch(void) {
  window_t *win = wm_create_window(i18n_get(STR_SYSMON), 150, 150, 350, 250,
                                   WIN_FLAG_MOVABLE | WIN_FLAG_CLOSABLE |
                                       WIN_FLAG_DECORATED);
  win->on_paint = sysmon_paint;
  win->on_close = sysmon_close;
}
