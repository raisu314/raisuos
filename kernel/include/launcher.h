#ifndef LAUNCHER_H
#define LAUNCHER_H

#include "types.h"

void launcher_init(void);
void launcher_show(void);
void launcher_hide(void);
void launcher_draw(void);
void launcher_handle_mouse(int x, int y, bool left, bool right);
bool launcher_is_open(void);

#endif
