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

  int w = 320;
  int h = 420;
  int x = 20;
  int y = vbe->height - TASKBAR_HEIGHT - h - 30;

  /* 1. Shadow */
  gfx_draw_shadow_buffer(vbe->backbuffer, vbe->width, vbe->height, x, y, w, h,
                         15, 12);

  /* 2. Glass Blur */
  gfx_blur_rect(x, y, w, h, 3);

  /* 3. Glass Tint (Dark) */
  gfx_fill_rounded_rect_buffer(vbe->backbuffer, vbe->width, vbe->height, x, y,
                               w, h, 15, RGBA(30, 30, 40, 180));

  font_draw_string(x + 25, y + 25, "ADNWS Native Hub", RGBA(255, 255, 255, 255),
                   0);

  int item_y = y + 70;
  const char *items[] = {"Terminal", "Text Editor", "System Monitor", "About"};

  for (int i = 0; i < 4; i++) {
    /* Item Background (Hover effect imitation: slight light) */
    gfx_fill_rounded_rect_buffer(vbe->backbuffer, vbe->width, vbe->height,
                                 x + 10, item_y - 10, w - 20, 40, 8,
                                 RGBA(255, 255, 255, 20));

    font_draw_string(x + 30, item_y, items[i], RGBA(200, 210, 255, 255), 0);
    item_y += 50;
  }
}

void launcher_handle_mouse(int mx, int my, bool left, bool right) {
  UNUSED(right);
  if (!is_open || !left)
    return;

  vbe_info_t *vbe = vbe_get_info();
  int h = 420;
  int y_base = vbe->height - TASKBAR_HEIGHT - h - 30;

  if (mx >= 20 && mx <= 340 && my >= y_base && my < y_base + h) {
    int rel_y = my - (y_base + 70);
    int index = rel_y / 50;

    if (index == 0)
      app_terminal_launch();
    else if (index == 1)
      app_editor_launch();
    else if (index == 2)
      app_sysmon_launch();
    else if (index == 3)
      app_about_launch();

    launcher_hide();
  } else {
    launcher_hide();
  }
}
