#ifndef RENDER_H
#define RENDER_H

#include "../headers/renderer.h"

// renderer system functions
void _init_window();
void _leave_window();
void _begin_drawing();
void _end_drawing();

void _init_debug();
void _end_debug();

void _loop(double delta);


void _set_point(int x, int y, Color c);
Color _get_point(int x, int y);
void _set_bg_point(int x, int y, Color c);
Color _get_bg_point(int x, int y);
void _remove_point(int x, int y);


#endif //#ifndef RENDER_H