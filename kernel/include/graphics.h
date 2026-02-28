#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "types.h"
#include "vbe.h"

/* Standard RGBA macro */
#define RGBA(r, g, b, a) (((a) << 24) | ((r) << 16) | ((g) << 8) | (b))

void gfx_fill_rect(int x, int y, int w, int h, uint32_t color);
void gfx_fill_rect_buffer(uint32_t *buffer, int bw, int x, int y, int w, int h,
                          uint32_t color);
void gfx_draw_rect(int x, int y, int w, int h, int thickness, uint32_t color);
void gfx_draw_rect_buffer(uint32_t *buffer, int bw, int x, int y, int w, int h,
                          int thickness, uint32_t color);
void gfx_draw_line(int x0, int y0, int x1, int y1, uint32_t color);

/* Advanced Rendering */
void gfx_draw_gradient_v(int x, int y, int w, int h, uint32_t top_color,
                         uint32_t bot_color);
void gfx_blur_rect(int x, int y, int w, int h, int radius);

#endif
