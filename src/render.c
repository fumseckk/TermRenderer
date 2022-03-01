#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <signal.h>
#include "macros.h"
#include "private_headers.h"
#include "../headers/renderer.h"


extern struct winsize volatile WINSIZE;
static struct termios term_original;
static struct termios term_settings;


// --------- LOCAL HEADERS ----------

// --------- LOCAL HEADERS ----------



void update_winsize() {
    ioctl(0, TIOCGWINSZ, &WINSIZE);
    // TODO: uncomment quand les pixels seront carré
    WINSIZE.ws_row *= 2;    
}

void sigwinch_handler(int sig) {
    update_winsize();
}

void sigint_handler(int Sig) {
    leave_window();
    exit(EXIT_SUCCESS);
}


// --------- LIB FUNCTIONS ----------

unsigned int get_screen_width() {
    return (unsigned int) WINSIZE.ws_col;
}


unsigned int get_screen_height() {
    return (unsigned int) WINSIZE.ws_row;
}


void init_window() {


    FATAL_ERROR(tcgetattr(STDIN, &term_settings) < 0, "Could not retrieve terminal settings for stdin");
    term_original = term_settings;
    // ICANON: disables cannonical mode
    // ECHO: disables echoing input characters (like arrow keys, HOME, etc)
    term_settings.c_lflag &= ~(ECHO | ICANON);
    FATAL_ERROR(tcsetattr(STDIN, 0, &term_settings) < 0, "Could not set terminal settings for stdin");

    update_winsize();

    alloc_buff();

    // Signal handlers for resize and Ctrl+C 
    FATAL_ERROR(signal(SIGWINCH, sigwinch_handler) == SIG_ERR || signal(SIGINT, sigint_handler) == SIG_ERR,
                "Could not init: handling SIGWINCH and SIGINT signals raised SIG_ERR");

    SAVE_CUR_POS();
    HIDE_CUR();
    USE_ALT_BUFF();
    MOVE_CUR_HOME();
}


void leave_window() {
    FATAL_ERROR(tcsetattr(STDIN, 0, &term_settings) < 0, "[CNSR] Could not set config for stdin");
    free_buff();
    fflush(stdout);
    CLEAR_SCREEN();
    USE_MAIN_BUFF();
    SHOW_CUR();
    RESTORE_CUR_POS();
}


void wait_and_leave_window() {
    // Waits for SIGINT signal, which is handled using signal function. SIGINT will call the leave_window function.
    while(1) sleep(1);
}


void begin_drawing() {
    // nothing to do lol
}


void end_drawing() {
    draw_pix_buff();
    fflush(stdout);
}


void draw_point(unsigned int x, unsigned int y, Color color) {
    push_pix_buff(x, y, color);
}


void draw_line(unsigned int x0, unsigned int y0, unsigned int x1, unsigned int y1, Color color) {
    int dx, dy, sx, sy, err, e2;

    dx =  abs (x1 - x0);
    dy = -abs (y1 - y0);
    sy = y0 < y1 ? 1 : -1; 
    sx = x0 < x1 ? 1 : -1;
    
    err = dx + dy; // error value e_xy

    for (;;){  // boucle infinie
        draw_point(x0, y0, color);
        if (x0 == x1 && y0 == y1) break;

        e2 = 2 * err;

        if (e2 >= dy)  { 
            err += dy;
            x0 += sx; // e_xy + e_x > 0
        }
        if (e2 <= dx) {
            err += dx;
            y0 += sy; // e_xy + e_y < 0
        }
    }
}


void draw_rect_boundary(unsigned int x, unsigned int y, unsigned int width, unsigned int height, Color color) {
    for (int posx = x; posx < x + width;    posx++) draw_point(posx, y, color);
    for (int posx = x; posx < x + width;    posx++) draw_point(posx, y + height, color);
    for (int posy = y; posy < y + height;   posy++) draw_point(x, posy, color);
    for (int posy = y; posy < y + height+1; posy++) draw_point(x + width, posy, color);
}


void draw_rect(unsigned int x, unsigned int y, unsigned int width, unsigned int height, Color color) {
    for (int posx = x; posx < x + width; posx++) {
        for (int posy = y; posy < y + height; posy++) {
            draw_point(posx, posy, color);
        }
    }
}


void draw_ellipse(int x0, int y0, int width, int height, Color color) {
    int x1 = x0 + width;
    int y1 = y0 + height;
    int b1 = height & 1; // values of diameter 
    long dx = 4*(1 - width)  * height * height;
    long dy = 4*(b1 + 1) * width * width; // error increment
    long err = dx + dy + b1*width*width;
    long e2; // error of 1.step

    if (x0 > x1) { x0 = x1; x1 += width; } // if called with swapped points
    if (y0 > y1) y0 = y1; // .. exchange them 
    y0 += (height + 1) / 2;
    y1 = y0-b1;   // starting pixel
    width *= 8 * width;
    b1 = 8 * height * height;

    do {
        for (int i = x0; i <= x1; i++) draw_point(i, y0, color);
        for (int i = x0; i <= x1; i++) draw_point(i, y1, color);
        e2 = 2 * err;

        if (e2 >= dx) {
            x0++;
            x1--;
            err += dx += b1;
        } // x step

        if (e2 <= dy) {
            y0++;
            y1--;
            err += dy += width;
        }  // y step 
    } while (x0 <= x1);

   while (y0-y1 < height) {  // too early stop of flat ellipses a=1
    
       draw_point(x0-1, y0, color); // -> finish tip of ellipse
       draw_point(x1+1, y0++, color); 
       draw_point(x0-1, y1, color);
       draw_point(x1+1, y1--, color); 
   }
}


void draw_ellipse_boundary(int x0, int y0, int width, int height, Color color) {
    int x1 = x0 + width;
    int y1 = y0 + height;
    int b1 = height & 1; // values of diameter 
    long dx = 4*(1 - width)  * height * height;
    long dy = 4*(b1 + 1) * width * width; // error increment
    long err = dx + dy + b1*width*width;
    long e2; // error of 1.step

    if (x0 > x1) { x0 = x1; x1 += width; } // if called with swapped points
    if (y0 > y1) y0 = y1; // .. exchange them 
    y0 += (height + 1) / 2;
    y1 = y0-b1;   // starting pixel
    width *= 8 * width;
    b1 = 8 * height * height;

    do {
        draw_point(x1, y0, color); /*   I. Quadrant */
        draw_point(x0, y0, color); /*  II. Quadrant */
        draw_point(x0, y1, color); /* III. Quadrant */
        draw_point(x1, y1, color); /*  IV. Quadrant */
        e2 = 2 * err;

        if (e2 >= dx) {
            x0++;
            x1--;
            err += dx += b1;
        } // x step

        if (e2 <= dy) {
            y0++;
            y1--;
            err += dy += width;
        }  // y step 
    } while (x0 <= x1);

   while (y0-y1 < height) {  // too early stop of flat ellipses a=1
       draw_point(x0-1, y0, color); // -> finish tip of ellipse
       draw_point(x1+1, y0++, color); 
       draw_point(x0-1, y1, color);
       draw_point(x1+1, y1--, color); 
   }
}


void draw_circle(unsigned int cx, unsigned int cy, unsigned int radius, Color color) {
    if (!radius) return;
    int error = -radius;
    int x = radius;
    int y = 0;

    while (x > y) {
        for (int i = cx - x; i < cx + x; i++) draw_point(i, cy + y, color);
        for (int i = cx - x; i < cx + x; i++) draw_point(i, cy - y, color);
        for (int i = cx - y; i < cx + y; i++) draw_point(i, cy + x, color);
        for (int i = cx - y; i < cx + y; i++) draw_point(i, cy - x, color);

        error += y;
        ++y;
        error += y;
        if (error >= 0) {
            --x;
            error -= x;
            error -= x;
        }
    }
    for (int i = cx - x; i < cx + x; i++) draw_point(i, cy + y, color);
    for (int i = cx - x; i < cx + x; i++) draw_point(i, cy - y, color);
}


void draw_circle_boundary(unsigned int cx, unsigned int cy, unsigned int radius, Color color) {
    if (!radius) return;
    int error = -radius;
    int x = radius;
    int y = 0;

    while (x > y) {
        draw_point (cx + x, cy + y, color);
        if (x != 0) draw_point (cx - x, cy + y, color);
        if (y != 0) draw_point (cx + x, cy - y, color);
        draw_point (cx - x, cy - y, color);

        draw_point (cx + y, cy + x, color);
        if (y != 0) draw_point (cx - y, cy + x, color);
        if (x != 0) draw_point (cx + y, cy - x, color);
        draw_point (cx - y, cy - x, color);

        error += y;
        ++y;
        error += y;
        if (error >= 0) {
            --x;
            error -= x;
            error -= x;
        }
    }
    draw_point (cx + x, cy + y, color);
    if (x != 0) draw_point (cx - x, cy + y, color);
    if (y != 0) draw_point (cx + x, cy - y, color);
    draw_point (cx - x, cy - y, color);
}


void fill_background(Color color) {
    draw_rect(0, 0, get_screen_width(), get_screen_height(), color);
}

// --------- LIB FUNCTIONS ----------
