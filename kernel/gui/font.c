#include "../include/font.h"
#include "string.h"
#include "types.h"
#include "vbe.h"

extern font_t font_national_park;
extern font_t font_kosugi_maru;
extern const uint8_t *font_get_ja_glyph(uint32_t unicode);

void font_init(void) { /* Fonts initialized via linking */ }

uint32_t font_utf8_to_unicode(const char **s) {
  const uint8_t *p = (const uint8_t *)*s;
  uint32_t res = 0;
  int len = 0;

  if (*p == 0)
    return 0;

  if ((*p & 0x80) == 0) {
    res = *p;
    len = 1;
  } else if ((*p & 0xE0) == 0xC0) {
    res = *p & 0x1F;
    len = 2;
  } else if ((*p & 0xF0) == 0xE0) {
    res = *p & 0x0F;
    len = 3;
  } else if ((*p & 0xF8) == 0xF0) {
    res = *p & 0x07;
    len = 4;
  } else {
    /* Invalid */
    *s += 1;
    return '?';
  }

  for (int i = 1; i < len; i++) {
    if ((p[i] & 0xC0) != 0x80) {
      *s += 1;
      return '?';
    }
    res = (res << 6) | (p[i] & 0x3F);
  }

  *s += len;
  return res;
}

static void draw_glyph(int x, int y, uint32_t unicode, uint32_t fg, uint32_t bg,
                       uint32_t *buffer, int bw, int bh) {
  const uint8_t *glyph_data = NULL;
  int gw = 8, gh = 16;

  if (unicode < 128) {
    /* Use National Park */
    glyph_data = font_national_park.data + (unicode * 16);
    gw = 8;
    gh = 16;
  } else if (unicode >= 0x3000 && unicode <= 0x9FFF) {
    /* Use Kosugi Maru for Japanese ranges */
    glyph_data = font_get_ja_glyph(unicode);
    gw = 16;
    gh = 16;
    if (!glyph_data) {
      /* Fallback or draw empty box */
      unicode = '?';
      glyph_data = font_national_park.data + ('?' * 16);
      gw = 8;
      gh = 16;
    }
  } else {
    /* Fallback */
    unicode = '?';
    glyph_data = font_national_park.data + ('?' * 16);
    gw = 8;
    gh = 16;
  }

  for (int row = 0; row < gh; row++) {
    uint16_t row_bits;
    if (gw == 8) {
      row_bits = glyph_data[row];
    } else {
      /* 16-bit row data */
      row_bits = (glyph_data[row * 2] << 8) | glyph_data[row * 2 + 1];
    }

    for (int col = 0; col < gw; col++) {
      if (row_bits & (1 << (gw - 1 - col))) {
        if (buffer) {
          int px = x + col, py = y + row;
          if (px >= 0 && px < bw && py >= 0 && py < bh)
            buffer[py * bw + px] = fg;
        } else {
          vbe_set_pixel(x + col, y + row, fg);
        }
      } else if (bg != 0) {
        if (buffer) {
          int px = x + col, py = y + row;
          if (px >= 0 && px < bw && py >= 0 && py < bh)
            buffer[py * bw + px] = bg;
        } else {
          vbe_set_pixel(x + col, y + row, bg);
        }
      }
    }
  }
}

void font_draw_char(int x, int y, uint32_t unicode, uint32_t fg, uint32_t bg) {
  draw_glyph(x, y, unicode, fg, bg, NULL, 0, 0);
}

void font_draw_string(int x, int y, const char *str, uint32_t fg, uint32_t bg) {
  int cx = x;
  const char *p = str;
  while (*p) {
    if (*p == '\n') {
      cx = x;
      y += 18;
      p++;
      continue;
    }
    uint32_t unicode = font_utf8_to_unicode(&p);
    font_draw_char(cx, y, unicode, fg, bg);
    cx += (unicode < 128) ? 8 : 16;
  }
}

void font_draw_char_buffer(uint32_t *buffer, int bw, int bh, int x, int y,
                           uint32_t unicode, uint32_t fg, uint32_t bg) {
  draw_glyph(x, y, unicode, fg, bg, buffer, bw, bh);
}

void font_draw_string_buffer(uint32_t *buffer, int bw, int bh, int x, int y,
                             const char *str, uint32_t fg, uint32_t bg) {
  int cx = x;
  const char *p = str;
  while (*p) {
    if (*p == '\n') {
      cx = x;
      y += 18;
      p++;
      continue;
    }
    uint32_t unicode = font_utf8_to_unicode(&p);
    font_draw_char_buffer(buffer, bw, bh, cx, y, unicode, fg, bg);
    cx += (unicode < 128) ? 8 : 16;
  }
}
