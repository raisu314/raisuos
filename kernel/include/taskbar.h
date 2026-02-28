#ifndef TASKBAR_H
#define TASKBAR_H

#include "types.h"

#define TASKBAR_HEIGHT 40

void taskbar_init(void);
void taskbar_draw(void);
void taskbar_handle_mouse(int x, int y, bool left, bool right);
void taskbar_handle_key(char key, uint8_t keycode, bool pressed);

#endif
