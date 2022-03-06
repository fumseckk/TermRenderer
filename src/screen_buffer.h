#ifndef SCREEN_BUFFER_H
#define SCREEN_BUFFER_H


#include <stdbool.h>

#include "../headers/renderer.h"


typedef struct _buffer {
    Color* pixels;
    Color* bg;
    bool* modified;
    short width;
    short height;
} Buffer;


void _alloc_buff();
void _resize_buff();
void _free_buff();
Color _get_pix(int x, int y);
void _pix_to_buff(int x, int y, Color color);
void _pix_to_bg(int x, int y, Color color);
void _remove_pix(int x, int y);
void _draw_buff();
void _flush_buff();


#endif // #ifndef SCREEN_BUFFER_H