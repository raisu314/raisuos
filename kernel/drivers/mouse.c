#include "mouse.h"
#include "idt.h"
#include "io.h"
#include "vbe.h"

static uint8_t mouse_cycle = 0;
static uint8_t mouse_byte[3];
static mouse_state_t mouse_state;
static mouse_callback_t mouse_cb = NULL;

static void mouse_wait(uint8_t a_type) {
  uint32_t timeout = 100000;
  if (a_type == 0) {
    while (timeout--) {
      if ((inb(0x64) & 1) == 1)
        return;
    }
  } else {
    while (timeout--) {
      if ((inb(0x64) & 2) == 0)
        return;
    }
  }
}

static void mouse_write(uint8_t write) {
  mouse_wait(1);
  outb(0x64, 0xD4);
  mouse_wait(1);
  outb(0x60, write);
}

static uint8_t mouse_read(void) {
  mouse_wait(0);
  return inb(0x60);
}

static void mouse_callback(registers_t *regs) {
  UNUSED(regs);
  uint8_t status = inb(0x64);
  if (!(status & 0x20))
    return;

  mouse_byte[mouse_cycle++] = inb(0x60);
  if (mouse_cycle == 3) {
    mouse_cycle = 0;

    bool left = mouse_byte[0] & 0x1;
    bool right = mouse_byte[0] & 0x2;
    int dx = mouse_byte[1] - ((mouse_byte[0] << 4) & 0x100);
    int dy = mouse_byte[2] - ((mouse_byte[0] << 3) & 0x100);

    vbe_info_t *vbe = vbe_get_info();
    mouse_state.x += dx;
    mouse_state.y -= dy; /* Y is inverted in PS/2 */

    /* Clamp to screen */
    if (mouse_state.x < 0)
      mouse_state.x = 0;
    if (mouse_state.y < 0)
      mouse_state.y = 0;
    if ((uint32_t)mouse_state.x >= vbe->width)
      mouse_state.x = vbe->width - 1;
    if ((uint32_t)mouse_state.y >= vbe->height)
      mouse_state.y = vbe->height - 1;

    mouse_state.left = left;
    mouse_state.right = right;

    if (mouse_cb)
      mouse_cb(mouse_state);
  }
}

void mouse_init(void) {
  mouse_state.x = 512;
  mouse_state.y = 384;
  mouse_state.left = false;
  mouse_state.right = false;

  mouse_wait(1);
  outb(0x64, 0xA8);

  mouse_wait(1);
  outb(0x64, 0x20);
  mouse_wait(0);
  uint8_t status = inb(0x60) | 2;
  mouse_wait(1);
  outb(0x64, 0x60);
  mouse_wait(1);
  outb(0x60, status);

  mouse_write(0xF6);
  mouse_read();
  mouse_write(0xF4);
  mouse_read();

  irq_install_handler(12, mouse_callback);
}

void mouse_set_callback(mouse_callback_t cb) { mouse_cb = cb; }
