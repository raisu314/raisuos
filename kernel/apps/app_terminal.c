#include "apps.h"
#include "font.h"
#include "graphics.h"
#include "heap.h"
#include "i18n.h"
#include "string.h"


#define TERM_COLS 80
#define TERM_ROWS 25

typedef struct {
  char buffer[TERM_ROWS][TERM_COLS];
  int cursor_x;
  int cursor_y;
  char input_line[TERM_COLS];
  int input_len;
} term_state_t;

static void term_print(term_state_t *state, const char *str) {
  while (*str) {
    if (*str == '\n') {
      state->cursor_x = 0;
      state->cursor_y++;
    } else {
      if (state->cursor_x >= TERM_COLS) {
        state->cursor_x = 0;
        state->cursor_y++;
      }
      state->buffer[state->cursor_y][state->cursor_x++] = *str;
    }

    if (state->cursor_y >= TERM_ROWS) {
      /* Scroll up */
      for (int i = 1; i < TERM_ROWS; i++) {
        memcpy(state->buffer[i - 1], state->buffer[i], TERM_COLS);
      }
      memset(state->buffer[TERM_ROWS - 1], 0, TERM_COLS);
      state->cursor_y = TERM_ROWS - 1;
    }
    str++;
  }
}

static void term_paint(window_t *win) {
  term_state_t *state = (term_state_t *)win->app_data;

  /* Background */
  gfx_fill_rect_buffer(win->buffer, win->width, 0, TITLE_BAR_HEIGHT, win->width,
                       win->height - TITLE_BAR_HEIGHT, RGBA(30, 30, 30, 255));

  int start_y = TITLE_BAR_HEIGHT + 5;

  for (int r = 0; r < TERM_ROWS; r++) {
    for (int c = 0; c < TERM_COLS; c++) {
      char ch = state->buffer[r][c];
      if (ch) {
        font_draw_char_buffer(win->buffer, win->width, 5 + c * 8,
                              start_y + r * 10, ch, RGBA(200, 200, 200, 255),
                              0);
      }
    }
  }

  /* Draw cursor */
  if (win->flags & WIN_FLAG_FOCUSED) {
    gfx_fill_rect_buffer(win->buffer, win->width, 5 + state->cursor_x * 8,
                         start_y + state->cursor_y * 10, 8, 10,
                         RGBA(255, 255, 255, 128));
  }
}

static void term_execute(term_state_t *state) {
  term_print(state, "\n");
  if (strcmp(state->input_line, "help") == 0) {
    term_print(state, "Commands: help, clear, sysinfo\n");
  } else if (strcmp(state->input_line, "clear") == 0) {
    memset(state->buffer, 0, sizeof(state->buffer));
    state->cursor_x = 0;
    state->cursor_y = 0;
  } else if (strcmp(state->input_line, "sysinfo") == 0) {
    term_print(state, "RaisuOS v2.0 (ADNWS / x86_64)\n");
    term_print(state, "Memory: 4096 MB / RAM\n");
  } else if (state->input_len > 0) {
    term_print(state, "Command not found.\n");
  }

  state->input_len = 0;
  memset(state->input_line, 0, TERM_COLS);
  term_print(state, "raisu> ");
}

static void term_key(window_t *win, char key, bool pressed) {
  if (!pressed)
    return;
  term_state_t *state = (term_state_t *)win->app_data;

  if (key == '\b') {
    if (state->input_len > 0) {
      state->input_len--;
      state->input_line[state->input_len] = '\0';
      if (state->cursor_x > 7) {
        state->cursor_x--;
        state->buffer[state->cursor_y][state->cursor_x] = 0;
      }
    }
  } else if (key == '\n') {
    term_execute(state);
  } else if (key >= 32 && key <= 126 && state->input_len < TERM_COLS - 1) {
    state->input_line[state->input_len++] = key;
    state->input_line[state->input_len] = '\0';
    char str[2] = {key, 0};
    term_print(state, str);
  }
}

static void term_close(window_t *win) {
  if (win->app_data)
    kfree(win->app_data);
  win->app_data = NULL;
  wm_destroy_window(win);
}

void app_terminal_launch(void) {
  window_t *win = wm_create_window(i18n_get(STR_TERMINAL), 100, 100, 650, 300,
                                   WIN_FLAG_MOVABLE | WIN_FLAG_CLOSABLE |
                                       WIN_FLAG_DECORATED);

  term_state_t *state = kmalloc(sizeof(term_state_t));
  memset(state, 0, sizeof(term_state_t));
  win->app_data = state;

  win->on_paint = term_paint;
  win->on_key = term_key;
  win->on_close = term_close;

  term_print(state, "RaisuOS Terminal (ADNWS)\n");
  term_print(state, "raisu> ");
}
