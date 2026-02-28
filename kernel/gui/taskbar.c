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


static int tb_y = 0;
static bool launcher_open = false;

void taskbar_init(void) {
  vbe_info_t *vbe = vbe_get_info();
  tb_y = vbe->height - TASKBAR_HEIGHT;
}

void taskbar_draw(void) {
  vbe_info_t *vbe = vbe_get_info();
  tb_y = vbe->height - TASKBAR_HEIGHT; /* Recalculate if VBE changes entirely */

  /* Semi-transparent Frost Glass background */
  gfx_fill_rect(0, tb_y, vbe->width, TASKBAR_HEIGHT, RGBA(255, 255, 255, 180));
  gfx_draw_line(0, tb_y, vbe->width, tb_y, RGBA(200, 200, 200, 255));

  /* Start/Launcher Button (ADNWS scaled) */
  gfx_fill_rect(10, tb_y + 4, 32, 32, RGBA(50, 150, 255, 255));
  font_draw_char(10 + 12, tb_y + 12, 'R', RGBA(255, 255, 255, 255), 0);

  /* Draw Desktop Number */
  char desk_str[16];
  strcpy(desk_str, "D");
  char num[2];
  itoa(wm_get_desktop() + 1, num, 10);
  strcpy(desk_str + 1, num);
  font_draw_string(60, tb_y + 16, desk_str, RGBA(50, 50, 50, 255), 0);

  /* Clock */
  uint32_t ticks = timer_get_ticks();
  uint32_t secs = ticks / 1000;
  uint32_t mins = secs / 60;
  secs %= 60;
  char time_str[32];
  itoa(mins, time_str, 10);
  int len = strlen(time_str);
  time_str[len] = ':';
  if (secs < 10) {
    time_str[len + 1] = '0';
    itoa(secs, time_str + len + 2, 10);
  } else {
    itoa(secs, time_str + len + 1, 10);
  }
  font_draw_string(vbe->width - 60, tb_y + 16, time_str, RGBA(0, 0, 0, 255), 0);
}

void taskbar_handle_mouse(int x, int y, bool left, bool right) {
  UNUSED(right);
  if (!left)
    return;

  if (x >= 10 && x <= 42 && y >= tb_y + 4 && y <= tb_y + 36) {
    launcher_open = !launcher_open;
    if (launcher_open) {
      launcher_show();
    } else {
      launcher_hide();
    }
  }
}
