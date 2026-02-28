#ifndef DESKTOP_H
#define DESKTOP_H

#include "types.h"

void desktop_init(void);
void desktop_draw(void);
void desktop_update(void);
void desktop_handle_mouse(int x, int y, bool left, bool right);
void desktop_handle_key(char key, uint8_t keycode, bool pressed, bool ctrl,
                        bool alt, bool shift);

#endif
