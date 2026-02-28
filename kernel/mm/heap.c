#include "heap.h"
#include "serial.h"

/* Simple bump allocator for RaisuOS x86_64 */

static uint64_t heap_start = 0;
static uint64_t heap_size = 0;
static uint64_t heap_current = 0;

void heap_init(uint64_t start, uint64_t size) {
  heap_start = start;
  heap_size = size;
  heap_current = start;
}

void *kmalloc(size_t size) {
  if (size == 0)
    return NULL;

  /* Align to 8 bytes for 64-bit safety */
  size = (size + 7) & ~7;

  if (heap_current + size > heap_start + heap_size) {
    serial_printf("[HEAP] OOM: Attempted to allocate %lu bytes\n",
                  (unsigned long)size);
    return NULL;
  }

  void *ptr = (void *)heap_current;
  heap_current += size;

  return ptr;
}

void *kmalloc_aligned(size_t size, size_t alignment) {
  if (size == 0 || alignment == 0)
    return NULL;

  /* Ensure alignment is a power of 2 */
  if ((alignment & (alignment - 1)) != 0)
    return NULL;

  uint64_t offset = heap_current % alignment;
  if (offset != 0) {
    heap_current += (alignment - offset);
  }

  return kmalloc(size);
}

void *kcalloc(size_t count, size_t size) {
  size_t total = count * size;
  void *ptr = kmalloc(total);
  if (ptr) {
    uint8_t *p = (uint8_t *)ptr;
    for (size_t i = 0; i < total; i++) {
      p[i] = 0;
    }
  }
  return ptr;
}

void kfree(void *ptr) {
  UNUSED(ptr);
  /* Bump allocator does not free individual blocks. */
}

uint64_t heap_get_used(void) { return heap_current - heap_start; }

uint64_t heap_get_free(void) { return heap_size - (heap_current - heap_start); }
