#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>
#include <signal.h>
#include "macros.h"
#include "../headers/renderer.h"


// static Color **pixels;
static struct winsize volatile WINSIZE;
static struct termios term_original;
static struct termios term_settings;


// --------- LOCAL HEADERS ----------
void get_winsize();
void sigwinch_handler(int Sig);
void sigint_handler(int Sig);
void _draw_point_no_color(unsigned int x, unsigned int y);
// --------- LOCAL HEADERS ----------



void update_winsize() {
    ioctl(0, TIOCGWINSZ, &WINSIZE);
    // TODO: uncomment quand les pixels seront carré
    // WINSIZE.ws_col *= 2; 
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
    term_settings.c_lflag &= ~ECHO;
    FATAL_ERROR(tcsetattr(STDIN, 0, &term_settings) < 0, "Could not set terminal settings for stdin");

    update_winsize();
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
    fflush(stdout);
}


void draw_point(unsigned int x, unsigned int y, Color color) {
    CHG_COLOR_TO(color);
    MOVE_CUR_TO(x, y);
    PRINT_PIX();

}


void _draw_point_no_color(unsigned int x, unsigned int y) {
    // TODO: enlever le /2 quand les pixels seront carrés
    MOVE_CUR_TO(x, y);
    PRINT_PIX();
}


void draw_line(unsigned int x0, unsigned int y0, unsigned int x1, unsigned int y1, Color color) {
    CHG_COLOR_TO(color);

    int dx, dy, sx, sy, err, e2;

    dx =  abs (x1 - x0);
    dy = -abs (y1 - y0);
    sy = y0 < y1 ? 1 : -1; 
    sx = x0 < x1 ? 1 : -1;
    
    err = dx + dy; // error value e_xy

    for (;;){  // boucle infinie
        _draw_point_no_color(x0, y0);
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
 CHG_COLOR_TO(color)
    for (int posx = x; posx < x + width;    posx++) _draw_point_no_color(posx, y);
    for (int posx = x; posx < x + width;    posx++) _draw_point_no_color(posx, y + height);
    for (int posy = y; posy < y + height;   posy++) _draw_point_no_color(x, posy);
    for (int posy = y; posy < y + height+1; posy++) _draw_point_no_color(x + width, posy);
}


void draw_rect(unsigned int x, unsigned int y, unsigned int width, unsigned int height, Color color) {
    CHG_COLOR_TO(color)
    for (int posx = x; posx < x + width; posx++) {
        for (int posy = y; posy < y + height; posy++) {
            _draw_point_no_color(posx, posy);
        }
    }
}

// --------- LIB FUNCTIONS ----------
