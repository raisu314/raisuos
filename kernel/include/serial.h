#ifndef SERIAL_H
#define SERIAL_H

#include "types.h"

#define PORT_COM1 0x3F8

void serial_init(void);
void serial_write(char a);
void serial_print(const char *str);
void serial_printf(const char *format, ...);

#endif
