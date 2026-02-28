#include "launcher.h"
#include "apps.h"
#include "font.h"
#include "graphics.h"
#include "i18n.h"
#include "icons.h"
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

  /* Glass Mica Hub: Responsive 450x320 centered hub */
  int hub_w = vbe_is_portrait() ? vbe->width - 40 : 450;
  int hub_h = 320;
  int hub_x = (vbe->width - hub_w) / 2;
  int hub_y = (vbe->height - hub_h) / 2;

  /* 1. Shadow */
  gfx_draw_shadow_buffer(vbe->backbuffer, vbe->width, vbe->height, hub_x, hub_y,
                         hub_w, hub_h, 20, 15);

  /* 2. Glass Blur */
  gfx_blur_rect(hub_x, hub_y, hub_w, hub_h, 4);

  /* 3. Mica Glass Body */
  gfx_fill_rounded_rect_buffer(vbe->backbuffer, vbe->width, vbe->height, hub_x,
                               hub_y, hub_w, hub_h, 16, COLOR_MICA);

  /* 4. Top Iridescent Glow */
  gfx_draw_iridescent_line(vbe->backbuffer, vbe->width, hub_x + 20, hub_y,
                           hub_w - 40);

  /* Draw App Grid (4x3 layout) */
  for (int i = 0; i < 11; i++) {
    int row = i / 4;
    int col = i % 4;
    int ax = hub_x + 30 + col * 100;
    int ay = hub_y + 40 + row * 90;

    icons_draw(vbe->backbuffer, vbe->width, vbe->height, ax + 20, ay, 40,
               app_entries[i].icon_id);
    font_draw_string_buffer(vbe->backbuffer, vbe->width, ax + 15, ay + 50,
                            app_entries[i].name, COLOR_TEXT_DARK, 0);
  }
}

void launcher_handle_mouse(int x, int y, bool left, bool right) {
  UNUSED(right);
  if (!is_open || !left)
    return;

  vbe_info_t *vbe = vbe_get_info();
  int hub_w = vbe_is_portrait() ? vbe->width - 40 : 450;
  int hub_h = 320;
  int hub_x = (vbe->width - hub_w) / 2;
  int hub_y = (vbe->height - hub_h) / 2;

  for (int i = 0; i < 11; i++) {
    int row = i / 4;
    int col = i % 4;
    int ax = hub_x + 30 + col * 100;
    int ay = hub_y + 40 + row * 90;

    if (x >= ax && x < ax + 90 && y >= ay && y < ay + 80) {
      if (app_entries[i].launch) {
        app_entries[i].launch();
        is_open = false;
      }
      break;
    }
  }
}
