#include "taskbar.h"
#include "desktop.h"
#include "font.h"
#include "graphics.h"
#include "heap.h"
#include "i18n.h"
#include "launcher.h"
#include "string.h"
#include "timer.h"
#include "vbe.h"
#include "window.h"

static int dock_x = 0;
static int dock_w = 0;
static int tb_y = 0;
static bool launcher_open = false;

void taskbar_init(void) {
  vbe_info_t *vbe = vbe_get_info();
  /* Responsive Dock Width: 60% on landscape, 90% on portrait */
  int scale = vbe_is_portrait() ? 90 : 60;
  dock_w = vbe->width * scale / 100;
  dock_x = (vbe->width - dock_w) / 2;
  tb_y = vbe->height - TASKBAR_HEIGHT - 10;
}

void taskbar_draw(void) {
  vbe_info_t *vbe = vbe_get_info();
  int scale = vbe_is_portrait() ? 90 : 60;
  dock_w = vbe->width * scale / 100;
  dock_x = (vbe->width - dock_w) / 2;
  tb_y = vbe->height - TASKBAR_HEIGHT - 10;

  /* 1. Shadow for the dock */
  gfx_draw_shadow_buffer(vbe->backbuffer, vbe->width, vbe->height, dock_x, tb_y,
                         dock_w, TASKBAR_HEIGHT, 15, 12);

  /* 2. Frost Glass Wall (Blur) - Enhanced for Light Mode */
  gfx_blur_rect(dock_x, tb_y, dock_w, TASKBAR_HEIGHT, 3);

  /* 3. "Premium Light" Mica Dock (High-translucency White) */
  gfx_fill_rounded_rect_buffer(vbe->backbuffer, vbe->width, vbe->height, dock_x,
                               tb_y, dock_w, TASKBAR_HEIGHT, 15, COLOR_MICA);

  /* 4. Launcher Icon (ADNWS Blue Accent) */
  int icon_y = tb_y + (TASKBAR_HEIGHT - 32) / 2;
  gfx_fill_rounded_rect_buffer(vbe->backbuffer, vbe->width, vbe->height,
                               dock_x + 15, icon_y, 32, 32, 16, COLOR_SUB);
  font_draw_string(dock_x + 15 + 10, icon_y + 8, "R", COLOR_TEXT_DARK, 0);

  /* 5. Desktop Indicator (Subtle Blue Text) */
  char d_buf[8];
  itoa(wm_get_desktop() + 1, d_buf, 10);
  font_draw_string(dock_x + 65, tb_y + 14, "SPACE", RGBA(100, 160, 220, 255),
                   0);
  font_draw_string(dock_x + 120, tb_y + 14, d_buf, COLOR_TEXT_DARK, 0);

  /* 6. Premium Clock (Slate Gray Contrast) */
  uint32_t ticks = timer_get_ticks();
  uint32_t secs = ticks / 1000;
  uint32_t mins = secs / 60;
  char time_str[16];
  itoa(mins % 60, time_str, 10);
  int tlen = strlen(time_str);
  time_str[tlen++] = ':';
  itoa(secs % 60, time_str + tlen, 10);
  font_draw_string(dock_x + dock_w - 90, tb_y + 14, time_str, COLOR_TEXT_DARK,
                   0);
}

void taskbar_handle_mouse(int x, int y, bool left, bool right) {
  UNUSED(right);
  if (!left)
    return;

  /* Check Launcher click */
  if (x >= dock_x + 15 && x <= dock_x + 15 + 32 && y >= tb_y &&
      y <= tb_y + TASKBAR_HEIGHT) {
    launcher_open = !launcher_open;
    if (launcher_open)
      launcher_show();
    else
      launcher_hide();
  }
}

void taskbar_handle_key(char key, uint8_t keycode, bool pressed) {
  UNUSED(key);
  UNUSED(keycode);
  UNUSED(pressed);
}
