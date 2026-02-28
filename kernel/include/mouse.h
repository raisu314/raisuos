#ifndef MOUSE_H
#define MOUSE_H

#include "types.h"

typedef struct {
  int x, y;
  bool left, right, middle;
} mouse_state_t;

typedef void (*mouse_callback_t)(mouse_state_t state);

void mouse_init(void);
void mouse_set_callback(mouse_callback_t cb);

#endif
