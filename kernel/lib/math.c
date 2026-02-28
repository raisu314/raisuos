#include "types.h"

/*
 * GCC intrinsics for 64-bit division and modulo.
 * These are required when linking without libgcc on x86_64.
 */

uint64_t __udivdi3(uint64_t n, uint64_t d) {
  if (d == 0)
    return 0;
  uint64_t q = 0, r = 0;
  for (int i = 63; i >= 0; i--) {
    r <<= 1;
    r |= (n >> i) & 1;
    if (r >= d) {
      r -= d;
      q |= (1ULL << i);
    }
  }
  return q;
}

uint64_t __umoddi3(uint64_t n, uint64_t d) {
  if (d == 0)
    return 0;
  uint64_t r = 0;
  for (int i = 63; i >= 0; i--) {
    r <<= 1;
    r |= (n >> i) & 1;
    if (r >= d) {
      r -= d;
    }
  }
  return r;
}

int64_t __divdi3(int64_t n, int64_t d) {
  uint64_t un = (n < 0) ? -n : n;
  uint64_t ud = (d < 0) ? -d : d;
  uint64_t uq = __udivdi3(un, ud);
  if ((n < 0) ^ (d < 0))
    return -(int64_t)uq;
  return (int64_t)uq;
}

int64_t __moddi3(int64_t n, int64_t d) {
  uint64_t un = (n < 0) ? -n : n;
  uint64_t ud = (d < 0) ? -d : d;
  uint64_t ur = __umoddi3(un, ud);
  if (n < 0)
    return -(int64_t)ur;
  return (int64_t)ur;
}
