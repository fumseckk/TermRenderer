#ifndef LOCAL_H
#define LOCAL_H

#include <sys/ioctl.h> // Necessary for winsize struct type
#include <stdbool.h> // Necessary for Buffer struct
#include "../headers/renderer.h"



typedef struct _buffer {
    Color** pixels;
    bool** modified;
    unsigned short cols;
    unsigned short rows;
} Buffer;


extern struct winsize volatile WINSIZE;
extern Buffer* buff;

// render.c
void get_winsize();
void sigwinch_handler(int Sig);
void sigint_handler(int Sig);
void _draw_point_no_color(unsigned int x, unsigned int y);


// buffer.c
void alloc_buff();
void realloc_buff();
void free_buff();
void push_pix_buff(unsigned int x, unsigned int y, Color color);
void draw_pix_buff();
void flush_buff();


#endif