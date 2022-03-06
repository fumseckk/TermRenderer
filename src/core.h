#ifndef RENDER_H
#define RENDER_H


// renderer system functions
void _init_window();
void _leave_window();
void _begin_drawing();
void _end_drawing();

void _init_debug();
void _end_debug();

void _loop(double delta);


#endif //#ifndef RENDER_H