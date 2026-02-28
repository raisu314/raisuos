#ifndef IO_H
#define IO_H

#include "types.h"

/* Assembly-defined port I/O functions */
extern uint8_t inb(uint16_t port);
extern void outb(uint16_t port, uint8_t val);
extern uint16_t inw(uint16_t port);
extern void outw(uint16_t port, uint16_t val);
extern uint32_t inl(uint16_t port);
extern void outl(uint16_t port, uint32_t val);

/* Interrupt control */
extern void enable_interrupts(void);
extern void disable_interrupts(void);
extern void io_wait(void);

#endif
