#ifndef VBE_H
#define VBE_H

#include "types.h"

typedef struct {
  uint32_t *framebuffer;
  uint32_t *backbuffer;
  uint32_t width;
  uint32_t height;
  uint32_t pitch;
  uint32_t bpp;
} vbe_info_t;

void vbe_init(uint64_t mboot_info_ptr);
vbe_info_t *vbe_get_info(void);
void vbe_swap_buffers(void);
void vbe_set_pixel(int x, int y, uint32_t color);

/* Responsive Helpers */
bool vbe_is_portrait(void);
float vbe_get_aspect_ratio(void);

#endif
