#include "launcher.h"
#include "apps.h"
#include "font.h"
#include "graphics.h"
#include "i18n.h"
#include "taskbar.h"
#include "vbe.h"


static bool is_open = false;

void launcher_init(void) { is_open = false; }

void launcher_show(void) { is_open = true; }

void launcher_hide(void) { is_open = false; }

bool launcher_is_open(void) { return is_open; }

void launcher_draw(void) {
  if (!is_open)
    return;

  vbe_info_t *vbe = vbe_get_info();

  int w = 300;
  int h = 400;
  int x = 10;
  int y = vbe->height - TASKBAR_HEIGHT - h - 10;

  /* Launcher Box */
  gfx_fill_rect(x, y, w, h, RGBA(240, 245, 250, 230));
  gfx_draw_rect(x, y, w, h, 2, RGBA(200, 200, 200, 255));

  font_draw_string(x + 20, y + 20, "RaisuOS Applications", RGBA(0, 0, 0, 255),
                   0);

  int item_y = y + 50;

  /* App List */
  font_draw_string(x + 20, item_y, "1. Terminal", RGBA(0, 0, 200, 255), 0);
  item_y += 30;
  font_draw_string(x + 20, item_y, "2. Text Editor", RGBA(0, 0, 200, 255), 0);
  item_y += 30;
  font_draw_string(x + 20, item_y, "3. System Monitor", RGBA(0, 0, 200, 255),
                   0);
  item_y += 30;
  font_draw_string(x + 20, item_y, "4. About", RGBA(0, 0, 200, 255), 0);
  item_y += 30;
}

void launcher_handle_mouse(int mx, int my, bool left, bool right) {
  UNUSED(right);
  if (!is_open || !left)
    return;

  vbe_info_t *vbe = vbe_get_info();
  int h = 400;
  int y_start = vbe->height - TASKBAR_HEIGHT - h - 10 + 50;

  if (mx >= 10 && mx <= 310) {
    if (my >= y_start && my < y_start + 30) {
      app_terminal_launch();
      launcher_hide();
    } else if (my >= y_start + 30 && my < y_start + 60) {
      app_editor_launch();
      launcher_hide();
    } else if (my >= y_start + 60 && my < y_start + 90) {
      app_sysmon_launch();
      launcher_hide();
    } else if (my >= y_start + 90 && my < y_start + 120) {
      app_about_launch();
      launcher_hide();
    }
  } else {
    /* Click outside launcher closes it */
    launcher_hide();
  }
}
