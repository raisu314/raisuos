#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "types.h"

typedef struct {
  char ascii;
  uint8_t keycode;
  bool pressed;
  bool shift;
  bool ctrl;
  bool alt;
} key_event_t;

typedef void (*keyboard_callback_t)(key_event_t);

void keyboard_init(void);
void keyboard_set_callback(keyboard_callback_t cb);
char keyboard_get_ascii(uint8_t scancode, bool shift);

#endif
