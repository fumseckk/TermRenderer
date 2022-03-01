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
    
    buff->pixels =  (Color**) calloc(buff->cols, sizeof(Color*));
    buff->modified = (bool**) calloc(buff->cols, sizeof(bool*));

    for (int i = 0; i < buff->cols; i++) {
        buff->pixels[i] =  (Color*) calloc(buff->rows, sizeof(Color));
        buff->modified[i] = (bool*) calloc(buff->rows, sizeof(bool));
    }
}


void free_buff() {
    for (int i = 0; i < buff->cols; i++) {
        free(buff->pixels[i]);
        free(buff->modified[i]);
    }
    free(buff->pixels);
    free(buff->modified);
    free(buff);
}


bool _check_pix_in_bound(unsigned int x, unsigned int y) {
    return (x < buff->cols && y < buff->rows);
}


void push_pix_buff(unsigned int x, unsigned int y, Color color) {
    if (!_check_pix_in_bound(x, y)) return;
    buff->pixels[x][y] = color;
    buff->modified[x][y] = true;
}



Color _get_pix(unsigned int x, unsigned int y) {
    if (!_check_pix_in_bound(x, y)) return ( (Color){ 0, 0, 0, 0 } );
    else return buff->pixels[x][y];
}


bool _color_eq(Color color1, Color color2) {
    return (color1.r == color2.r && color1.g == color2.g && color1.b == color2.b && color1.a == color2.a);
}


bool _check_pix_modified(unsigned int x, unsigned int y) {
    return (_check_pix_in_bound(x, y) && buff->modified[x][y]);
}

void draw_pix_buff() {
    Color color1;
    Color color2;

    LOG("%d ", (int) buff->modified[0][0]);

    for (int y = 0; y < buff->rows; y += 2) {
        for (int x = 0; x < buff->cols; x++) {
            if (buff->modified[x][y] || buff->modified[x][y+1]) {
            if(x == 0 && y == 0) LOG("on print en (0, 0) avec la couleur %d,%d,%d,%d", 
                                        buff->pixels[x][y].r, buff->pixels[x][y].g, buff->pixels[x][y].b, buff->pixels[x][y].a);
                MOVE_CUR_TO(x, y);
                color1 = buff->pixels[x][y];
                color2 = buff->pixels[x][y+1];

                if      (color1.a == 0 && color2.a == 0) { RESET_FG_BG_COLOR();     printf(" ");                          }
                else if (color2.a == 0)                  { RESET_FG_BG_COLOR();     CHG_FG_COLOR_TO(color1); printf("▀"); }
                else if (color1.a == 0)                  { RESET_FG_BG_COLOR();     CHG_FG_COLOR_TO(color2); printf("▄"); }
                else if (_color_eq(color1, color2))      { CHG_BG_COLOR_TO(color1); printf(" ");                          }
                else                                     { CHG_BG_COLOR_TO(color1); CHG_FG_COLOR_TO(color2); printf("▄"); }

                buff->modified[x][y] =  false;
                buff->modified[x][y+1] = false;
            }
        }

    }
}