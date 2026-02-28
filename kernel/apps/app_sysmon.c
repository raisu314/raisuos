#include "apps.h"
#include "font.h"
#include "graphics.h"
#include "heap.h"
#include "i18n.h"
#include "string.h"
#include "timer.h"


static void sysmon_paint(window_t *win) {
  gfx_fill_rect_buffer(win->buffer, win->width, 0, TITLE_BAR_HEIGHT, win->width,
                       win->height - TITLE_BAR_HEIGHT,
                       RGBA(250, 250, 250, 255));

  int y = TITLE_BAR_HEIGHT + 20;
  int x = 20;

  font_draw_string_buffer(win->buffer, win->width, x, y,
                          "All Device Native Work System",
                          RGBA(0, 50, 150, 255), 0);
  y += 30;

  char uptime[64] = "Uptime: ";
  char num[16];
  itoa(timer_get_ticks() / 1000, num, 10);
  strcat(uptime, num);
  strcat(uptime, " seconds");

  font_draw_string_buffer(win->buffer, win->width, x, y, uptime,
                          RGBA(50, 50, 50, 255), 0);
  y += 20;

  font_draw_string_buffer(win->buffer, win->width, x, y,
                          "Architecture: x86_64 (Long Mode)",
                          RGBA(50, 50, 50, 255), 0);
  y += 20;

  font_draw_string_buffer(win->buffer, win->width, x, y,
                          "Minimum RAM Required: 4096 MB",
                          RGBA(50, 50, 50, 255), 0);
  y += 20;

  font_draw_string_buffer(win->buffer, win->width, x, y,
                          "Minimum ROM Required: 65536 MB",
                          RGBA(50, 50, 50, 255), 0);
  y += 20;

  /* Draw memory bar */
  uint64_t used = heap_get_used() / 1024;
  uint64_t free = heap_get_free() / 1024;
  uint64_t total = used + free;

  char mem[64] = "OS Heap: ";
  itoa(used, num, 10);
  strcat(mem, num);
  strcat(mem, " KB / ");
  itoa(total, num, 10);
  strcat(mem, num);
  strcat(mem, " KB");

  y += 20;
  font_draw_string_buffer(win->buffer, win->width, x, y, mem,
                          RGBA(50, 50, 50, 255), 0);

  y += 15;
  int bar_w = win->width - 40;
  gfx_draw_rect_buffer(win->buffer, win->width, x, y, bar_w, 20, 1,
                       RGBA(150, 150, 150, 255));

  int fill_w = (total > 0) ? (used * bar_w / total) : 0;
  gfx_fill_rect_buffer(win->buffer, win->width, x + 1, y + 1, fill_w, 18,
                       RGBA(50, 150, 250, 255));
}

static void sysmon_close(window_t *win) { wm_destroy_window(win); }

static char *strcat(char *dest, const char *src) {
  char *rd = dest;
  while (*dest)
    dest++;
  while ((*dest++ = *src++))
    ;
  return rd;
}

void app_sysmon_launch(void) {
  window_t *win = wm_create_window(i18n_get(STR_SYSMON), 150, 150, 350, 250,
                                   WIN_FLAG_MOVABLE | WIN_FLAG_CLOSABLE |
                                       WIN_FLAG_DECORATED);
  win->on_paint = sysmon_paint;
  win->on_close = sysmon_close;
}
