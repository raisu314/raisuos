#include "gdt.h"
#include "string.h"

#define GDT_ENTRIES 5

gdt_entry_t gdt_entries[GDT_ENTRIES];
gdt_ptr_t gdt_ptr;

static void gdt_set_gate(int num, uint64_t base, uint32_t limit, uint8_t access,
                         uint8_t gran) {
  gdt_entries[num].base_low = (base & 0xFFFF);
  gdt_entries[num].base_middle = (base >> 16) & 0xFF;
  gdt_entries[num].base_high = (base >> 24) & 0xFF;

  gdt_entries[num].limit_low = (limit & 0xFFFF);
  gdt_entries[num].granularity = (limit >> 16) & 0x0F;

  gdt_entries[num].granularity |= gran & 0xF0;
  gdt_entries[num].access = access;
}

void gdt_init(void) {
  gdt_ptr.limit = (sizeof(gdt_entry_t) * GDT_ENTRIES) - 1;
  gdt_ptr.base = (uint64_t)&gdt_entries;

  /* Null segment */
  gdt_set_gate(0, 0, 0, 0, 0);

  /* Code segment (exec/read, Long Mode) -> Access: 0x9A, Granularity: 0x20
   * (L=1) */
  gdt_set_gate(1, 0, 0, 0x9A, 0x20);

  /* Data segment (read/write) -> Access: 0x92, Granularity: 0x00 */
  gdt_set_gate(2, 0, 0, 0x92, 0x00);

  /* User code segment */
  gdt_set_gate(3, 0, 0, 0xFA, 0x20);

  /* User data segment */
  gdt_set_gate(4, 0, 0, 0xF2, 0x00);

  __asm__ volatile("lgdt %0" : : "m"(gdt_ptr));

  /* Reload data segment registers */
  __asm__ volatile("mov ax, 0x10\n"
                   "mov ds, ax\n"
                   "mov es, ax\n"
                   "mov fs, ax\n"
                   "mov gs, ax\n"
                   "mov ss, ax\n");
}
