#include "../include/font.h"
#include "types.h"

/*
 * Placeholder 16x16 bitmap font data representing "Kosugi Maru" status.
 * In a real scenario, this would be generated from a TTF/BDF.
 * 'あ' (U+3042)
 */
static const uint8_t font_kosugi_maru_あ[32] = {
    0x04, 0x00, 0x04, 0x00, 0x7E, 0x00, 0x04, 0x00, 0x04, 0x00, 0x0F,
    0xE0, 0x10, 0x10, 0x20, 0x08, 0x4F, 0x08, 0x81, 0x08, 0x81, 0x08,
    0x41, 0x10, 0x3E, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

/* Mapping of unicode points to glyph data */
typedef struct {
  uint32_t unicode;
  const uint8_t *data;
} unicode_glyph_t;

static const unicode_glyph_t kosugi_maru_table[] = {
    {0x3042, font_kosugi_maru_あ},
    /* ... More JP glyphs ... */
    {0, 0}};

const font_t font_kosugi_maru = {
    .width = 16,
    .height = 16,
    .data = 0 // Table driven
};

const uint8_t *font_get_ja_glyph(uint32_t unicode) {
  for (int i = 0; kosugi_maru_table[i].unicode != 0; i++) {
    if (kosugi_maru_table[i].unicode == unicode) {
      return kosugi_maru_table[i].data;
    }
  }
  return 0;
}
