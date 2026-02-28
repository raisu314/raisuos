#ifndef ICONS_H
#define ICONS_H

#include "types.h"

/* ICON IDs */
#define ICON_TERMINAL 1
#define ICON_EDITOR 2
#define ICON_SYSMON 3
#define ICON_CALC 4
#define ICON_CALENDAR 5
#define ICON_WEATHER 6
#define ICON_NOTES 7
#define ICON_CLOCK 8
#define ICON_MUSIC 9
#define ICON_PHOTOS 10
#define ICON_SETTINGS 11

void icons_draw(uint32_t *buffer, int bw, int bh, int x, int y, int size,
                int icon_id);

#endif
