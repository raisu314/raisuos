#include "apps.h"
#include "font.h"
#include "graphics.h"
#include "heap.h"
#include "i18n.h"
#include "string.h"

#define MAX_LINES 100
#define MAX_COLS 60

typedef struct {
  char lines[MAX_LINES][MAX_COLS];
  int cursor_x;
  int cursor_y;
} editor_state_t;

static void editor_paint(window_t *win) {
  editor_state_t *state = (editor_state_t *)win->app_data;

  /* Background: ADNWS Pure White */
  gfx_fill_rect_buffer(win->buffer, win->width, 0, 0, win->width, win->height,
                       COLOR_MAIN);

  int start_y = 10;

  for (int r = 0; r < MAX_LINES; r++) {
    if (start_y + r * 12 > win->height)
      break;

    for (int c = 0; c < MAX_COLS; c++) {
      char ch = state->lines[r][c];
      if (ch) {
        font_draw_char_buffer(win->buffer, win->width, 5 + c * 8,
                              start_y + r * 12, ch, COLOR_TEXT_DARK, 0);
      }
    }
  }

  /* Cursor: Sky Blue Accent */
  if (win->flags & WIN_FLAG_FOCUSED) {
    gfx_fill_rect_buffer(win->buffer, win->width, 5 + state->cursor_x * 8,
                         start_y + state->cursor_y * 12, 2, 10, COLOR_SUB);
  }
}

static void editor_key(window_t *win, char key, bool pressed) {
  if (!pressed)
    return;
  editor_state_t *state = (editor_state_t *)win->app_data;

  if (key == '\b') {
    if (state->cursor_x > 0) {
      state->cursor_x--;
      state->lines[state->cursor_y][state->cursor_x] = 0;
    } else if (state->cursor_y > 0) {
      state->cursor_y--;
      state->cursor_x = strlen(state->lines[state->cursor_y]);
    }
  } else if (key == '\n') {
    if (state->cursor_y < MAX_LINES - 1) {
      state->cursor_y++;
      state->cursor_x = 0;
    }
  } else if (key >= 32 && key <= 126) {
    if (state->cursor_x < MAX_COLS - 1) {
      state->lines[state->cursor_y][state->cursor_x] = key;
      state->cursor_x++;
      state->lines[state->cursor_y][state->cursor_x] = 0;
    }
  }
}

static void editor_close(window_t *win) {
  if (win->app_data)
    kfree(win->app_data);
  win->app_data = NULL;
  wm_destroy_window(win);
}

void app_editor_launch(void) {
  window_t *win = wm_create_window(i18n_get(STR_EDITOR), 200, 150, 500, 400,
                                   WIN_FLAG_MOVABLE | WIN_FLAG_CLOSABLE |
                                       WIN_FLAG_DECORATED);
  editor_state_t *state = kmalloc(sizeof(editor_state_t));
  memset(state, 0, sizeof(editor_state_t));
  win->app_data = state;
  win->on_paint = editor_paint;
  win->on_key = editor_key;
  win->on_close = editor_close;
}
