#include "timer.h"
#include "idt.h"
#include "io.h"

static uint32_t timer_ticks = 0;

static void timer_callback(registers_t *regs) {
  UNUSED(regs);
  timer_ticks++;
}

void timer_init(uint32_t frequency) {
  irq_install_handler(0, timer_callback);

  uint32_t divisor = 1193180 / frequency;

  outb(0x43, 0x36);
  uint8_t l = (uint8_t)(divisor & 0xFF);
  uint8_t h = (uint8_t)((divisor >> 8) & 0xFF);
  outb(0x40, l);
  outb(0x40, h);
}

uint32_t timer_get_ticks(void) { return timer_ticks; }

void timer_sleep(uint32_t ticks) {
  uint32_t start = timer_ticks;
  while (timer_ticks < start + ticks) {
    __asm__ volatile("hlt");
  }
}
