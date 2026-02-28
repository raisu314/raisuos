#include "icons.h"
#include "graphics.h"

void icons_draw(uint32_t *buffer, int bw, int x, int y, int size, int icon_id) {
  uint32_t accent = COLOR_SUB;
  uint32_t bg = COLOR_MAIN;

  switch (icon_id) {
  case ICON_TERMINAL:
    /* Dark terminal box with prompt */
    gfx_fill_rounded_rect_buffer(buffer, bw, 0, x + size / 4, y + size / 4,
                                 size / 2, size / 2, size / 4, bg);
    break;

  case ICON_WEATHER:
    /* Sun with iridescent rays mask */
    gfx_fill_circle_buffer(buffer, bw, x + size / 2, y + size / 2, size / 3,
                           0xFFFFD700);
    gfx_draw_iridescent_line(buffer, bw, x, y + size / 2, size);
    break;

  case ICON_NOTES:
    /* Paper with folded corner */
    gfx_fill_rounded_rect_buffer(buffer, bw, 0, x + 4, y + 2, size - 8,
                                 size - 4, 2, RGBA(255, 255, 240, 255));
    gfx_fill_rect_buffer(buffer, bw, x + 8, y + 10, size - 16, 2, accent);
    break;

  case ICON_CLOCK:
    /* Circle with hands */
    gfx_fill_circle_buffer(buffer, bw, x + size / 2, y + size / 2, size / 2 - 2,
                           bg);
    gfx_draw_line(x + size / 2, y + size / 2, x + size / 2, y + 8, accent);
    gfx_draw_line(x + size / 2, y + size / 2, x + size - 8, y + size / 2,
                  RGBA(100, 100, 100, 255));
    break;

  case ICON_MUSIC:
    /* Note symbol on iridescent bar */
    gfx_draw_iridescent_line(buffer, bw, x, y + size - 4, size);
    gfx_fill_circle_buffer(buffer, bw, x + 10, y + size - 12, 6,
                           COLOR_TEXT_DARK);
    gfx_draw_line(x + 16, y + size - 12, x + 16, y + 4, COLOR_TEXT_DARK);
    break;

  case ICON_PHOTOS:
    /* Rainbow-colored segments */
    gfx_fill_rounded_rect_buffer(buffer, bw, 0, x, y, size, size, 4, bg);
    gfx_fill_rect_buffer(buffer, bw, x + 2, y + 2, size / 2 - 4, size / 2 - 4,
                         0xFFFF7675);
    gfx_fill_rect_buffer(buffer, bw, x + size / 2 + 2, y + 2, size / 2 - 4,
                         size / 2 - 4, 0xFFBBE2F9);
    break;

  case ICON_EDITOR:
    /* Document with lines */
    gfx_fill_rounded_rect_buffer(buffer, bw, 0, x, y, size, size, 4, bg);
    gfx_draw_rect_buffer(buffer, bw, x, y, size, size, 1,
                         RGBA(200, 200, 200, 255));
    for (int i = 0; i < 3; i++) {
      gfx_fill_rect_buffer(buffer, bw, x + 4, y + 6 + i * 6, size - 8, 2,
                           accent);
    }
    break;

  case ICON_SYSMON:
    /* Pulse line */
    gfx_fill_rounded_rect_buffer(buffer, bw, 0, x, y, size, size, 8,
                                 RGBA(40, 45, 55, 255));
    gfx_draw_line(x + 2, y + size / 2, x + size / 3, y + size / 2, accent);
    gfx_draw_line(x + size / 3, y + size / 2, x + size / 2, y + 5, accent);
    gfx_draw_line(x + size / 2, y + 5, x + 2 * size / 3, y + size - 5, accent);
    gfx_draw_line(x + 2 * size / 3, y + size - 5, x + size - 2, y + size / 2,
                  accent);
    break;

  case ICON_CALC:
    /* 2x2 grid */
    gfx_fill_rounded_rect_buffer(buffer, bw, 0, x, y, size, size, 6,
                                 RGBA(240, 245, 255, 255));
    gfx_fill_rect_buffer(buffer, bw, x + 4, y + 4, (size - 12) / 2,
                         (size - 12) / 2, accent);
    gfx_fill_rect_buffer(buffer, bw, x + size / 2 + 2, y + 4, (size - 12) / 2,
                         (size - 12) / 2, RGBA(150, 150, 160, 255));
    gfx_fill_rect_buffer(buffer, bw, x + 4, y + size / 2 + 2, (size - 12) / 2,
                         (size - 12) / 2, RGBA(150, 150, 160, 255));
    gfx_fill_rect_buffer(buffer, bw, x + size / 2 + 2, y + size / 2 + 2,
                         (size - 12) / 2, (size - 12) / 2,
                         RGBA(255, 150, 100, 255));
    break;

  case ICON_CALENDAR:
    /* Red top bar page */
    gfx_fill_rounded_rect_buffer(buffer, bw, 0, x, y, size, size, 4, bg);
    gfx_fill_rect_buffer(buffer, bw, x, y, size, size / 4,
                         RGBA(255, 100, 100, 255));
    /* Grid dots */
    for (int i = 0; i < 2; i++) {
      for (int j = 0; j < 2; j++) {
        gfx_fill_rect_buffer(buffer, bw, x + 6 + i * 10, y + 12 + j * 10, 4, 4,
                             RGBA(200, 200, 210, 255));
      }
    }
    break;

  case ICON_SETTINGS:
    /* Cog base */
    gfx_fill_rounded_rect_buffer(buffer, bw, 0, x, y, size, size, size / 2,
                                 RGBA(180, 185, 200, 255));
    gfx_fill_rounded_rect_buffer(buffer, bw, 0, x + size / 4, y + size / 4,
                                 size / 2, size / 2, size / 4, bg);
    break;
  }
}
