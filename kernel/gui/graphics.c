#include "graphics.h"
#include "string.h"

void gfx_fill_rect(int x, int y, int w, int h, uint32_t color) {
  vbe_info_t *vbe = vbe_get_info();
  if (!vbe->backbuffer)
    return;

  for (int i = 0; i < h; i++) {
    for (int j = 0; j < w; j++) {
      vbe_set_pixel(x + j, y + i, color);
    }
  }
}

void gfx_fill_rect_buffer(uint32_t *buffer, int bw, int x, int y, int w, int h,
                          uint32_t color) {
  for (int i = 0; i < h; i++) {
    for (int j = 0; j < w; j++) {
      buffer[(y + i) * bw + (x + j)] = color;
    }
  }
}

void gfx_draw_rect(int x, int y, int w, int h, int thickness, uint32_t color) {
  gfx_fill_rect(x, y, w, thickness, color);
  gfx_fill_rect(x, y + h - thickness, w, thickness, color);
  gfx_fill_rect(x, y, thickness, h, color);
  gfx_fill_rect(x + w - thickness, y, thickness, h, color);
}

void gfx_draw_rect_buffer(uint32_t *buffer, int bw, int x, int y, int w, int h,
                          int thickness, uint32_t color) {
  gfx_fill_rect_buffer(buffer, bw, x, y, w, thickness, color);
  gfx_fill_rect_buffer(buffer, bw, x, y + h - thickness, w, thickness, color);
  gfx_fill_rect_buffer(buffer, bw, x, y, thickness, h, color);
  gfx_fill_rect_buffer(buffer, bw, x + w - thickness, y, thickness, h, color);
}

void gfx_draw_line(int x0, int y0, int x1, int y1, uint32_t color) {
  int dx = (x1 > x0) ? (x1 - x0) : (x0 - x1);
  int dy = (y1 > y0) ? (y1 - y0) : (y0 - y1);
  int sx = (x0 < x1) ? 1 : -1;
  int sy = (y0 < y1) ? 1 : -1;
  int err = dx - dy;

  while (1) {
    vbe_set_pixel(x0, y0, color);
    if (x0 == x1 && y0 == y1)
      break;
    int e2 = 2 * err;
    if (e2 > -dy) {
      err -= dy;
      x0 += sx;
    }
    if (e2 < dx) {
      err += dx;
      y0 += sy;
    }
  }
}

/* Linear color interpolation (Integer based) */
static uint32_t interpolate_color(uint32_t c1, uint32_t c2, int t, int max) {
  uint32_t r1 = (c1 >> 16) & 0xFF, g1 = (c1 >> 8) & 0xFF, b1 = c1 & 0xFF;
  uint32_t r2 = (c2 >> 16) & 0xFF, g2 = (c2 >> 8) & 0xFF, b2 = c2 & 0xFF;

  uint32_t r = r1 + (r2 - r1) * t / max;
  uint32_t g = g1 + (g2 - g1) * t / max;
  uint32_t b = b1 + (b2 - b1) * t / max;

  return RGBA(r, g, b, 255);
}

void gfx_draw_gradient_v(int x, int y, int w, int h, uint32_t top_color,
                         uint32_t bot_color) {
  if (h <= 1)
    return;
  for (int i = 0; i < h; i++) {
    uint32_t color = interpolate_color(top_color, bot_color, i, h - 1);
    gfx_fill_rect(x, y + i, w, 1, color);
  }
}

/* Fast Box Blur implementation for Frost Glass effect */
void gfx_blur_rect(int x, int y, int w, int h, int radius) {
  vbe_info_t *vbe = vbe_get_info();
  if (!vbe->backbuffer)
    return;

  for (int i = y; i < y + h; i++) {
    for (int j = x; j < x + w; j++) {
      if (i < 0 || j < 0 || (uint32_t)i >= vbe->height ||
          (uint32_t)j >= vbe->width)
        continue;

      uint32_t count = 0, r = 0, g = 0, b = 0;

      for (int dy = -radius; dy <= radius; dy++) {
        for (int dx = -radius; dx <= radius; dx++) {
          int ny = i + dy, nx = j + dx;
          if (ny >= 0 && nx >= 0 && (uint32_t)ny < vbe->height &&
              (uint32_t)nx < vbe->width) {
            uint32_t color = vbe->backbuffer[ny * vbe->width + nx];
            r += (color >> 16) & 0xFF;
            g += (color >> 8) & 0xFF;
            b += color & 0xFF;
            count++;
          }
        }
      }
      if (count > 0) {
        uint32_t final_col = RGBA(r / count, g / count, b / count, 255);
        vbe->backbuffer[i * vbe->width + j] = final_col;
      }
    }
  }
}

/* Alpha blending for two colors */
uint32_t gfx_blend(uint32_t foreground, uint32_t background, uint8_t alpha) {
  uint32_t r_f = (foreground >> 16) & 0xFF, g_f = (foreground >> 8) & 0xFF,
           b_f = foreground & 0xFF;
  uint32_t r_b = (background >> 16) & 0xFF, g_b = (background >> 8) & 0xFF,
           b_b = background & 0xFF;

  uint32_t r = (r_f * alpha + r_b * (255 - alpha)) / 255;
  uint32_t g = (g_f * alpha + g_b * (255 - alpha)) / 255;
  uint32_t b = (b_f * alpha + b_b * (255 - alpha)) / 255;

  return RGBA(r, g, b, 255);
}

void gfx_fill_rounded_rect_buffer(uint32_t *buffer, int bw, int bh, int x,
                                  int y, int w, int h, int r, uint32_t color) {
  for (int i = 0; i < h; i++) {
    int py = y + i;
    if (py < 0 || py >= bh)
      continue;
    for (int j = 0; j < w; j++) {
      int px = x + j;
      if (px < 0 || px >= bw)
        continue;

      bool inside = true;
      /* Check corners */
      if (j < r && i < r) { /* Top Left */
        if ((j - r) * (j - r) + (i - r) * (i - r) > r * r)
          inside = false;
      } else if (j >= w - r && i < r) { /* Top Right */
        if ((j - (w - r - 1)) * (j - (w - r - 1)) + (i - r) * (i - r) > r * r)
          inside = false;
      } else if (j < r && i >= h - r) { /* Bottom Left */
        if ((j - r) * (j - r) + (i - (h - r - 1)) * (i - (h - r - 1)) > r * r)
          inside = false;
      } else if (j >= w - r && i >= h - r) { /* Bottom Right */
        if ((j - (w - r - 1)) * (j - (w - r - 1)) +
                (i - (h - r - 1)) * (i - (h - r - 1)) >
            r * r)
          inside = false;
      }

      if (inside) {
        buffer[py * bw + px] = color;
      }
    }
  }
}

void gfx_draw_shadow_buffer(uint32_t *buffer, int bw, int bh, int x, int y,
                            int w, int h, int r, int strength) {
  UNUSED(r);
  /* Draw a subtle dark glow around the rect */
  for (int i = -strength; i < h + strength; i++) {
    int py = y + i;
    if (py < 0 || py >= bh)
      continue;
    for (int j = -strength; j < w + strength; j++) {
      int px = x + j;
      if (px < 0 || px >= bw)
        continue;

      /* Don't draw over the element itself */
      if (i >= 0 && i < h && j >= 0 && j < w)
        continue;

      int dist_x = 0;
      if (j < 0)
        dist_x = -j;
      else if (j >= w)
        dist_x = j - w + 1;

      int dist_y = 0;
      if (i < 0)
        dist_y = -i;
      else if (i >= h)
        dist_y = i - h + 1;

      int dist = (dist_x > dist_y) ? dist_x : dist_y;

      if (dist <= strength) {
        uint8_t alpha = (strength - dist) * 40 / strength;
        buffer[py * bw + px] = gfx_blend(0x000000, buffer[py * bw + px], alpha);
      }
    }
  }
}
