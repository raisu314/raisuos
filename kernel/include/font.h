#ifndef FONT_H
#define FONT_H

#include "types.h"

typedef struct {
  uint8_t width;
  uint8_t height;
  const uint8_t *data;
} font_t;

void font_init(void);

/* UTF-8 decoding helper */
uint32_t font_utf8_to_unicode(const char **s);

/* Rendering functions */
void font_draw_char(int x, int y, uint32_t unicode, uint32_t fg, uint32_t bg);
void font_draw_string(int x, int y, const char *str, uint32_t fg, uint32_t bg);

void font_draw_char_buffer(uint32_t *buffer, int bw, int bh, int x, int y,
                           uint32_t unicode, uint32_t fg, uint32_t bg);
void font_draw_string_buffer(uint32_t *buffer, int bw, int bh, int x, int y,
                             const char *str, uint32_t fg, uint32_t bg);

#endif
