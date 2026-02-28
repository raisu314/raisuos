#ifndef GDT_H
#define GDT_H

#include "types.h"

/* GDT entry structure (64-bit format) */
typedef struct {
  uint16_t limit_low;
  uint16_t base_low;
  uint8_t base_middle;
  uint8_t access;
  uint8_t granularity;
  uint8_t base_high;
} __attribute__((packed)) gdt_entry_t;

/* GDT pointer structure (64-bit address) */
typedef struct {
  uint16_t limit;
  uint64_t base;
} __attribute__((packed)) gdt_ptr_t;

void gdt_init(void);
void gdt_flush(uint64_t gdt_ptr);

#endif
