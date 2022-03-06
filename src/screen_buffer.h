#ifndef SCREEN_BUFFER_H
#define SCREEN_BUFFER_H


#include <stdbool.h>

#include "../headers/renderer.h"


typedef struct _buffer {
    Color* pixels;
    Color* bg;
    bool* modified;
    unsigned short width;
    unsigned short height;
} Buffer;


void alloc_buff();
void resize_buff();
void free_buff();
void pix_to_buff(unsigned int x, unsigned int y, Color color);
void pix_to_bg(unsigned int x, unsigned int y, Color color);
void remove_pix(unsigned int x, unsigned int y);
void draw_buff();
void flush_buff();


#endif // #ifndef SCREEN_BUFFER_H