#ifndef IDT_H
#define IDT_H

#include "types.h"

/* Registers pushed by ISR/IRQ stubs in 64-bit mode (System V ABI) */
typedef struct {
  uint64_t r15, r14, r13, r12, r11, r10, r9, r8;
  uint64_t rbp, rdi, rsi, rdx, rcx, rbx, rax;
  uint64_t int_no, err_code;
  uint64_t rip, cs, rflags, rsp, ss;
} __attribute__((packed)) registers_t;

/* IDT entry structure (64-bit is 16 bytes) */
typedef struct {
  uint16_t base_lo;
  uint16_t sel;
  uint8_t always0;
  uint8_t flags;
  uint16_t base_hi;
  uint32_t base_top;
  uint32_t reserved;
} __attribute__((packed)) idt_entry_t;

/* IDT pointer structure */
typedef struct {
  uint16_t limit;
  uint64_t base;
} __attribute__((packed)) idt_ptr_t;

typedef void (*irq_handler_t)(registers_t *);

void idt_init(void);
void irq_install_handler(int irq, irq_handler_t handler);
void irq_uninstall_handler(int irq);

#endif
