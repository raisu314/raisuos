#include "serial.h"
#include "io.h"
#include "string.h"
#include <stdarg.h>

void serial_init(void) {
  outb(PORT_COM1 + 1, 0x00);
  outb(PORT_COM1 + 3, 0x80);
  outb(PORT_COM1 + 0, 0x03);
  outb(PORT_COM1 + 1, 0x00);
  outb(PORT_COM1 + 3, 0x03);
  outb(PORT_COM1 + 2, 0xC7);
  outb(PORT_COM1 + 4, 0x0B);
}

void serial_write(char a) {
  while ((inb(PORT_COM1 + 5) & 0x20) == 0)
    ;
  outb(PORT_COM1, a);
}

void serial_print(const char *str) {
  while (*str)
    serial_write(*str++);
}

void serial_printf(const char *format, ...) {
  va_list args;
  va_start(args, format);

  while (*format != '\0') {
    if (*format == '%') {
      format++;
      if (*format == 'd') {
        int64_t i = va_arg(args, int64_t); /* Read 64-bit cleanly on x64 */
        char buffer[32];
        itoa(i, buffer, 10);
        serial_print(buffer);
      } else if (*format == 's') {
        const char *s = va_arg(args, const char *);
        serial_print(s);
      } else if (*format == 'x' || *format == 'X') {
        uint64_t i = va_arg(args, uint64_t);
        char buffer[32];
        itoa(i, buffer, 16);
        serial_print(buffer);
      } else if (*format == 'c') {
        char c = (char)va_arg(args, int);
        serial_write(c);
      } else if (*format == 'l' && *(format + 1) == 'X') {
        uint64_t i = va_arg(args, uint64_t);
        format++;
        char buffer[64];
        itoa(i, buffer, 16);
        serial_print(buffer);
      } else {
        serial_write('%');
        serial_write(*format);
      }
    } else {
      serial_write(*format);
    }
    format++;
  }

  va_end(args);
}
