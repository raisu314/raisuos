#ifndef HEAP_H
#define HEAP_H

#include "types.h"

void heap_init(uint64_t start, uint64_t size);
void *kmalloc(size_t size);
void *kmalloc_aligned(size_t size, size_t alignment);
void *kcalloc(size_t count, size_t size);
void kfree(void *ptr);

uint64_t heap_get_used(void);
uint64_t heap_get_free(void);

#endif
