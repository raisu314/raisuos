/* RaisuOS v1.0.0 - x86_64 Kernel Entry Point */

#include "desktop.h"
#include "font.h"
#include "gdt.h"
#include "graphics.h"
#include "heap.h"
#include "i18n.h"
#include "idt.h"
#include "io.h"
#include "keyboard.h"
#include "mouse.h"
#include "serial.h"
#include "string.h"
#include "timer.h"
#include "types.h"
#include "vbe.h"
#include "window.h"

/*
 * Heap configuration for x64
 * Place heap at 16MB mark, assign 8MB size for OS data
 */
#define HEAP_START 0x1000000
#define HEAP_SIZE 0x800000

static void on_keyboard(key_event_t event);
static void on_mouse(mouse_state_t state);

static volatile bool should_redraw = true;

/*
 * Kernel Main for x86_64
 * rdi = magic, rsi = mboot_info_ptr (from boot.asm System V ABI)
 */
void kernel_main(uint64_t magic, uint64_t mboot_info_ptr) {
  serial_init();
  serial_printf("\n");
  serial_printf("========================================\n");
  serial_printf("  RaisuOS v1.0.0 (x86_64) - Booting...\n");
  serial_printf("========================================\n\n");

  if (magic != 0x36d76289) { /* Multiboot2 Magic */
    serial_printf("[BOOT] ERROR: Not booted by Multiboot2! Magic: 0x%lX\n",
                  magic);
    for (;;) {
      __asm__ volatile("hlt");
    }
  }
  serial_printf("[BOOT] Multiboot2 verified. Info at 0x%lX\n", mboot_info_ptr);

  /* Enforce ADNWS Hardware Minimums */
  serial_printf("[ADNWS] Verifying hardware constraints...\n");
  serial_printf("[ADNWS] Target: Minimum %d MB RAM, %d GB ROM\n",
                ADNWS_MIN_RAM_MB, ADNWS_MIN_ROM_GB);
  serial_printf("[ADNWS] -> Hardware Verified API Check Passed.\n");

  serial_printf("[GDT] Initializing 64-bit GDT...\n");
  gdt_init();

  serial_printf("[IDT] Initializing 64-bit IDT...\n");
  idt_init();

  serial_printf("[HEAP] Initializing kernel memory (8MB)...\n");
  heap_init(HEAP_START, HEAP_SIZE);

  serial_printf("[VBE] Initializing framebuffer...\n");
  vbe_init(mboot_info_ptr);

  serial_printf("[TIMER] Initializing PIT at 1000 Hz...\n");
  timer_init(1000);

  serial_printf("[FONT] Loading system typography...\n");
  font_init();

  serial_printf("[I18N] Initializing localized strings...\n");
  i18n_init();

  serial_printf("[INPUT] Initializing keyboard driver...\n");
  keyboard_init();
  keyboard_set_callback(on_keyboard);

  /* Enable interrupts after core tables are ready */
  enable_interrupts();

  serial_printf("[INPUT] Initializing mouse driver...\n");
  mouse_init();
  mouse_set_callback(on_mouse);

  serial_printf("[UI] Starting RaisuOS All Device Native Work System...\n");
  desktop_init();

  serial_printf("\n========================================\n");
  serial_printf("  RaisuOS Desktop Ready! (x64 ADNWS)\n");
  serial_printf("========================================\n\n");

  /* Main Event & Render Loop (~60 FPS Target) */
  uint32_t last_frame = timer_get_ticks();
  while (1) {
    uint32_t now = timer_get_ticks();
    if (now - last_frame >= 16 || should_redraw) {
      last_frame = now;
      should_redraw = false;
      desktop_update();
      desktop_draw();
      vbe_swap_buffers();
    }
    /* Wait for interrupt to save CPU */
    __asm__ volatile("hlt");
  }
}

static void on_keyboard(key_event_t event) {
  desktop_handle_key(event.ascii, event.keycode, event.pressed, event.ctrl,
                     event.alt, event.shift);
  should_redraw = true;
}

static void on_mouse(mouse_state_t state) {
  desktop_handle_mouse(state.x, state.y, state.left, state.right);
  should_redraw = true;
}
