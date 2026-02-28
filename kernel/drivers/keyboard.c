#include "keyboard.h"
#include "idt.h"
#include "io.h"

static keyboard_callback_t key_cb = NULL;
static bool shift_pressed = false;
static bool ctrl_pressed = false;
static bool alt_pressed = false;

/* US QWERTY Scancode map */
static const char unshifted[128] = {
    0,   27,   '1',  '2', '3',  '4', '5', '6', '7', '8', '9', '0', '-',
    '=', '\b', '\t', 'q', 'w',  'e', 'r', 't', 'y', 'u', 'i', 'o', 'p',
    '[', ']',  '\n', 0,   'a',  's', 'd', 'f', 'g', 'h', 'j', 'k', 'l',
    ';', '\'', '`',  0,   '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',',
    '.', '/',  0,    '*', 0,    ' ', 0,   0,   0,   0,   0,   0,   0,
    0,   0,    0,    0,   0,    0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,    0,    0,   0,    0,   0,   0};

static const char shifted[128] = {
    0,   27,   '!',  '@', '#', '$', '%', '^', '&', '*', '(', ')', '_',
    '+', '\b', '\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P',
    '{', '}',  '\n', 0,   'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L',
    ':', '"',  '~',  0,   '|', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<',
    '>', '?',  0,    '*', 0,   ' ', 0,   0,   0,   0,   0,   0,   0,
    0,   0,    0,    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,    0,    0,   0,   0,   0,   0};

char keyboard_get_ascii(uint8_t scancode, bool shift) {
  if (scancode >= 128)
    return 0;
  return shift ? shifted[scancode] : unshifted[scancode];
}

static void keyboard_callback(registers_t *regs) {
  UNUSED(regs);
  uint8_t scancode = inb(0x60);
  bool pressed = !(scancode & 0x80);
  uint8_t code = scancode & 0x7F;

  if (code == 0x2A || code == 0x36)
    shift_pressed = pressed;
  if (code == 0x1D)
    ctrl_pressed = pressed;
  if (code == 0x38)
    alt_pressed = pressed;

  if (key_cb) {
    key_event_t ev;
    ev.keycode = code;
    ev.pressed = pressed;
    ev.shift = shift_pressed;
    ev.ctrl = ctrl_pressed;
    ev.alt = alt_pressed;
    ev.ascii = keyboard_get_ascii(code, ev.shift);
    key_cb(ev);
  }
}

void keyboard_init(void) { irq_install_handler(1, keyboard_callback); }

void keyboard_set_callback(keyboard_callback_t cb) { key_cb = cb; }
