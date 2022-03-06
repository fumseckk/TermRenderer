#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

#include "screen_buffer.h"

#include "macros.h"
#include "../headers/renderer.h"


Buffer* buff;


// --------- Internal Functions ---------


void _alloc_buff() {
    buff = malloc(sizeof(Buffer));

    buff->width  = get_screen_width();
    buff->height = get_screen_height();
    
    buff->pixels   = (Color*) malloc(buff->width * buff->height * sizeof(Color));
    buff->bg       = (Color*) malloc(buff->width * buff->height * sizeof(Color));
    buff->modified = (bool*)  malloc(buff->width * buff->height * sizeof(bool));

    memset(buff->modified, 1, buff->width * buff->height * sizeof(bool));
}


void _resize_buff() {
    buff->width  = get_screen_width();
    buff->height = get_screen_height();

    buff->pixels   = (Color*) realloc(buff->pixels,   buff->width * buff->height * sizeof(Color));
    buff->bg       = (Color*) realloc(buff->bg,       buff->width * buff->height * sizeof(Color));
    buff->modified = (bool*)  realloc(buff->modified, buff->width * buff->height * sizeof(bool));
}


void _free_buff() {
    free(buff->pixels);
    free(buff->bg);
    free(buff->modified);
    free(buff);
}


bool _check_pix_in_bound(int x, int y) {
    return (x < buff->width && y < buff->height);
}


void _pix_to_buff(int x, int y, Color color) {
    if (!_check_pix_in_bound(x, y)) return;
    buff->pixels[x * buff->height + y] = color;
    buff->modified[x * buff->height + y] = true;
}


void _pix_to_bg(int x, int y, Color color) {
    if (!_check_pix_in_bound(x, y)) return;
    buff->bg[x * buff->height + y] = color;
}


void _remove_pix(int x, int y) {
    if (!_check_pix_in_bound(x, y)) return;
    buff->pixels[x * buff->height + y] = buff->bg[x * buff->height + y];
    buff->modified[x * buff->height + y] = true;
}


Color _get_pix(int x, int y) {
    if (!_check_pix_in_bound(x, y)) return ( (Color){ 0, 0, 0 } );
    else return buff->pixels[x * buff->height + y];
}


bool _color_eq(Color color1, Color color2) {
    return (color1.r == color2.r && color1.g == color2.g && color1.b == color2.b);
}


bool _check_pix_modified(int x, int y) {
    return (_check_pix_in_bound(x, y) && buff->modified[x * buff->height + y]);
}


void _draw_buff() {
    Color color1;
    Color color2;

    for (int y = 0; y < buff->height; y += 2) {
        for (int x = 0; x < buff->width; x++) {
            if (buff->modified[x * buff->height + y] || buff->modified[x * buff->height + y+1]) {
                MOVE_CUR_TO(x, y);
                color1 = buff->pixels[x * buff->height + y];
                color2 = buff->pixels[x * buff->height + y+1];

                if (_color_eq(color1, color2)) { 
                    CHG_BG_COLOR_TO(color1);
                    printf(" ");
                }
                else {
                    CHG_BG_COLOR_TO(color1);
                    CHG_FG_COLOR_TO(color2);
                    printf("▄");
                }

                buff->modified[x * buff->height + y]   = false;
                buff->modified[x * buff->height + y+1] = false;
            }
        }

    }
}
