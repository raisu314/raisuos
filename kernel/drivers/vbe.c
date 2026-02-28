#include "vbe.h"
#include "heap.h"
#include "serial.h"
#include "string.h"

/* Multiboot2 tag structures */
struct multiboot_tag {
  uint32_t type;
  uint32_t size;
};

struct multiboot_tag_framebuffer {
  uint32_t type;
  uint32_t size;
  uint64_t framebuffer_addr;
  uint32_t framebuffer_pitch;
  uint32_t framebuffer_width;
  uint32_t framebuffer_height;
  uint8_t framebuffer_bpp;
  uint8_t framebuffer_type;
  uint16_t reserved;
};

static vbe_info_t current_vbe;

void vbe_init(uint64_t mboot_info_ptr) {
  /* Fallback defaults */
  current_vbe.width = 1024;
  current_vbe.height = 768;
  current_vbe.pitch = 1024 * 4;
  current_vbe.bpp = 32;
  current_vbe.framebuffer = (uint32_t *)0xFD000000;

  if (!mboot_info_ptr) {
    serial_printf("[VBE] Multiboot2 info missing!\n");
    return;
  }

  uint32_t total_size = *(uint32_t *)(uintptr_t)mboot_info_ptr;
  uint64_t end = mboot_info_ptr + total_size;
  uint64_t addr = mboot_info_ptr + 8; /* Skip total size and reserved */

  while (addr < end) {
    struct multiboot_tag *tag = (struct multiboot_tag *)(uintptr_t)addr;
    if (tag->type == 0 && tag->size == 8) {
      break; /* End tag */
    }

    if (tag->type == 8) { /* Type 8 = Framebuffer tag */
      struct multiboot_tag_framebuffer *fb =
          (struct multiboot_tag_framebuffer *)tag;
      current_vbe.width = fb->framebuffer_width;
      current_vbe.height = fb->framebuffer_height;
      current_vbe.pitch = fb->framebuffer_pitch;
      current_vbe.bpp = fb->framebuffer_bpp;
      current_vbe.framebuffer = (uint32_t *)(uintptr_t)fb->framebuffer_addr;

      serial_printf("[VBE] Multiboot2 Framebuffer found: %dx%dx%d at 0x%lX\n",
                    current_vbe.width, current_vbe.height, current_vbe.bpp,
                    (uint64_t)current_vbe.framebuffer);
      break;
    }

    /* Next tag is aligned to 8 bytes */
    addr += (tag->size + 7) & ~7;
  }

  /* Allocate the backbuffer in kernel heap */
  uint32_t fb_size =
      current_vbe.width * current_vbe.height * (current_vbe.bpp / 8);
  current_vbe.backbuffer = (uint32_t *)kmalloc_aligned(fb_size, 4096);

  if (current_vbe.backbuffer) {
    memset(current_vbe.backbuffer, 0, fb_size);
    serial_printf("[VBE] Backbuffer allocated: %d bytes\n", fb_size);
  } else {
    serial_printf("[VBE] ERROR: Failed to allocate backbuffer!\n");
  }
}

vbe_info_t *vbe_get_info(void) { return &current_vbe; }

void vbe_swap_buffers(void) {
  if (!current_vbe.backbuffer || !current_vbe.framebuffer)
    return;
  uint32_t count = current_vbe.width * current_vbe.height;
  memcpy(current_vbe.framebuffer, current_vbe.backbuffer,
         count * sizeof(uint32_t));
}

void vbe_set_pixel(int x, int y, uint32_t color) {
  if (x < 0 || y < 0 || (uint32_t)x >= current_vbe.width ||
      (uint32_t)y >= current_vbe.height)
    return;
  if (current_vbe.backbuffer) {
    current_vbe.backbuffer[y * current_vbe.width + x] = color;
  }
}

bool vbe_is_portrait(void) { return current_vbe.height > current_vbe.width; }

uint32_t vbe_get_aspect_ratio(void) {
  if (current_vbe.height == 0)
    return 0;
  return (current_vbe.width * 1000) / current_vbe.height;
}
