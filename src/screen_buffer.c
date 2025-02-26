#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include "macros.h"
#include "private_headers.h"
#include "../headers/renderer.h"

struct winsize volatile WINSIZE;
Buffer* buff;

void alloc_buff() {
  buff = malloc(sizeof(Buffer));

  buff->cols = WINSIZE.ws_col;
  buff->rows = WINSIZE.ws_row;

  buff->pixels = (Color*)malloc(buff->cols * buff->rows * sizeof(Color));
  buff->bg = (Color*)malloc(buff->cols * buff->rows * sizeof(Color));
  buff->modified = (bool*)malloc(buff->cols * buff->rows * sizeof(bool));

  memset(buff->modified, 1, buff->cols * buff->rows * sizeof(bool));
}

void resize_buff() {
  buff->cols = WINSIZE.ws_col;
  buff->rows = WINSIZE.ws_row;

  buff->pixels =
      (Color*)realloc(buff->pixels, buff->cols * buff->rows * sizeof(Color));
  buff->bg = (Color*)realloc(buff->bg, buff->cols * buff->rows * sizeof(Color));
  buff->modified =
      (bool*)realloc(buff->modified, buff->cols * buff->rows * sizeof(bool));
}

void free_buff() {
  free(buff->pixels);
  free(buff->bg);
  free(buff->modified);
  free(buff);
}

bool _check_pix_in_bound(unsigned int x, unsigned int y) {
  return (x < buff->cols && y < buff->rows);
}

void pix_to_buff(unsigned int x, unsigned int y, Color color) {
  if (!_check_pix_in_bound(x, y)) return;
  buff->pixels[x * buff->rows + y] = color;
  buff->modified[x * buff->rows + y] = true;
}

void pix_to_bg(unsigned int x, unsigned int y, Color color) {
  if (!_check_pix_in_bound(x, y)) return;
  buff->bg[x * buff->rows + y] = color;
}

void remove_pix(unsigned int x, unsigned int y) {
  if (!_check_pix_in_bound(x, y)) return;
  buff->pixels[x * buff->rows + y] = buff->bg[x * buff->rows + y];
  buff->modified[x * buff->rows + y] = true;
}

Color _get_pix(unsigned int x, unsigned int y) {
  if (!_check_pix_in_bound(x, y))
    return ((Color){0, 0, 0});
  else
    return buff->pixels[x * buff->rows + y];
}

bool _color_eq(Color color1, Color color2) {
  return (color1.r == color2.r && color1.g == color2.g && color1.b == color2.b);
}

bool _check_pix_modified(unsigned int x, unsigned int y) {
  return (_check_pix_in_bound(x, y) && buff->modified[x * buff->rows + y]);
}

void draw_buff() {
  Color color1;
  Color color2;

  for (int y = 0; y < buff->rows; y += 2) {
    for (int x = 0; x < buff->cols; x++) {
      if (buff->modified[x * buff->rows + y] ||
          buff->modified[x * buff->rows + y + 1]) {
        MOVE_CUR_TO(x, y);
        color1 = buff->pixels[x * buff->rows + y];
        color2 = buff->pixels[x * buff->rows + y + 1];

        if (_color_eq(color1, color2)) {
          CHG_BG_COLOR_TO(color1);
          printf(" ");
        }
        // else if (color2.a == 0) { CHG_BG_COLOR_TO(BLACK);
        // CHG_FG_COLOR_TO(color1); printf("▀"); } else if (color1.a == 0) {
        // CHG_BG_COLOR_TO(BLACK);     CHG_FG_COLOR_TO(color2); printf("▄"); }
        else {
          CHG_BG_COLOR_TO(color1);
          CHG_FG_COLOR_TO(color2);
          printf("▄");
        }

        buff->modified[x * buff->rows + y] = false;
        buff->modified[x * buff->rows + y + 1] = false;
      }
    }
  }
}