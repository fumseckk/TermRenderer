#include <alloca.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <stdbool.h>

#include "core.h"

#include "macros.h"
#include "datatypes/Buffer2D.h"
#include "../headers/renderer.h"
#include "../headers/debug.h"
#include "../headers/utils.h"

#include "servers/main_server.h"


const bool FALSE = false;
const bool TRUE  = true;

// --------- Variables ---------

static Buffer2D pixels;
static Buffer2D background;
static Buffer2D modified;

static bool initialized = false;
static struct termios term_original;
static struct termios term_settings;

static _Atomic int wind_width  = 0;
static _Atomic int wind_height = 0;

static void (*loop)(double);

#ifndef NDEBUG
static FILE* debout;
#endif  // #ifndef NDEBUG


// --------- Private Functions ---------


void _alloc_buff() {
    int width = get_screen_width();
    int height = get_screen_height();

    pixels     = create_buffer2d(width, height, sizeof(Color));
    background = create_buffer2d(width, height, sizeof(Color));
    modified   = create_buffer2d(width, height / 2, sizeof(bool));
    buffer2d_fill(modified, &TRUE);
}


void _resize_buff() {
    resize_buffer2d(pixels, get_screen_width(), get_screen_height());
    resize_buffer2d(background, get_screen_width(), get_screen_height());
    resize_buffer2d(modified, get_screen_width() / 2, get_screen_height());
}


void _free_buff() {
    free_buffer2d(pixels);
    free_buffer2d(background);
    free_buffer2d(modified);
}


void _draw_buff() {
    Color color_up;
    Color color_down;

    for (int y = 0; y < pixels->height; y += 2) {
        for (int x = 0; x < pixels->width; x++) {
            if (*(bool*)buffer2d_get(modified, x, y / 2)) {
                MOVE_CUR_TO(x, y);
                color_up = *(Color*)buffer2d_get(pixels, x, y);
                color_down = *(Color*)buffer2d_get(pixels, x, y+1);

                if (color_eq(color_up, color_down)) { 
                    CHG_BG_COLOR_TO(color_up);
                    printf(" ");
                } else {
                    CHG_BG_COLOR_TO(color_up);
                    CHG_FG_COLOR_TO(color_down);
                    printf("▄");
                }

                buffer2d_set(modified, x, y / 2, &FALSE);
            }
        }
    }
}


bool _check_pixel_out_of_bound(int x, int y) {
    return (x < 0 || y < 0 || x >= pixels->width || y >= pixels->height);
}


// --------- Internal Functions ---------


void _update_winsize() {
    const struct winsize WINSIZE;
    if (!initialized) {
        ioctl(0, TIOCGWINSZ, &WINSIZE);
        wind_width = WINSIZE.ws_col;
        wind_height = WINSIZE.ws_row * 2;
    }
    else {
        ioctl(0, TIOCGWINSZ, &WINSIZE);
        if (wind_width != WINSIZE.ws_col || wind_height != WINSIZE.ws_row * 2) {
            wind_width = WINSIZE.ws_col;
            wind_height = WINSIZE.ws_row * 2;
            _resize_buff();
        }
    }
}


void _sigwinch_handler(int sig) {
    _update_winsize();
}


void _sigint_handler(int sig) {
    _leave_window();
    _end_debug();
    exit(EXIT_SUCCESS);
}


void _init_window() {
    FATAL_ERROR(tcgetattr(STDIN, &term_settings) < 0, "Could not retrieve terminal settings for stdin");
    term_original = term_settings;
    // ICANON: disables cannonical mode
    // ECHO: disables echoing input characters (like arrow keys, HOME, etc)
    term_settings.c_lflag &= ~(ECHO | ICANON);
    FATAL_ERROR(tcsetattr(STDIN, 0, &term_settings) < 0, "Could not set terminal settings for stdin");

    _update_winsize();
    _alloc_buff();
    initialized = true;

    // Signal handlers for resize and Ctrl+C 
    FATAL_ERROR(signal(SIGWINCH, _sigwinch_handler) == SIG_ERR || signal(SIGINT, _sigint_handler) == SIG_ERR,
                "Could not init: handling SIGWINCH and SIGINT signals raised SIG_ERR");

    SAVE_CUR_POS();
    HIDE_CUR();
    USE_ALT_BUFF();
    MOVE_CUR_HOME();

    _draw_buff();
}


void _leave_window() {
    FATAL_ERROR(tcsetattr(STDIN, 0, &term_settings) < 0, "Could not set config for stdin");
    _free_buff();
    fflush(stdout);
    initialized = false;
    CLEAR_SCREEN();
    USE_MAIN_BUFF();
    SHOW_CUR();
    RESTORE_CUR_POS();
}


void _begin_drawing() { /* nothing to do lol */ }


void _end_drawing() {
    _draw_buff();
    fflush(stdout);
}


void _loop(double delta) {
    _begin_drawing();
    loop(delta);
    _end_drawing();
}


void _init_debug() {
#ifndef NDEBUG
    debout = fopen("logs.txt", "w");
    dup2(fileno(debout), STDERR_FILENO);
#endif
}


void _end_debug() {
#ifndef NDEBUG
    fflush(debout);
    fclose(debout);
#endif
}


// --------- Library functions ---------


int get_screen_width() {
    if (!initialized) _update_winsize();
    return wind_width;
}


int get_screen_height() {
    if (!initialized) _update_winsize();
    return wind_height;
}


void wait_and_leave_window() {
    // Waits for SIGINT signal, which is handled using signal function. SIGINT will call the leave_window function.
    while(1) sleep(1);
}


void init(void (loop_function)(double)) {
    loop = loop_function;
    _init_debug();
    _init_window();
}


void start() {
    _main_server_loop(); // start main loop
}


void _set_point(int x, int y, Color c) {
    if (_check_pixel_out_of_bound(x, y)) return;
    if (!color_eq(*(Color*)buffer2d_get(pixels, x, y), c)) {
        buffer2d_set(pixels, x, y, &c);
        buffer2d_set(modified, x, y / 2, &TRUE);
    }
}


Color _get_point(int x, int y) {
    if (_check_pixel_out_of_bound(x, y)) return BLACK;
    return *(Color*)buffer2d_get(pixels, x, y);
}


void _remove_point(int x, int y) {
    if (_check_pixel_out_of_bound(x, y)) return;
    _set_point(x, y, *(Color*)buffer2d_get(background, x, y));
}


void _set_bg_point(int x, int y, Color c) {
    if (_check_pixel_out_of_bound(x, y)) return;
    if (!color_eq(*(Color*)buffer2d_get(background, x, y), c)) {
        buffer2d_set(background, x, y, &c);
    }
}


Color _get_bg_point(int x, int y) {
    if (_check_pixel_out_of_bound(x, y)) return BLACK;
    return *(Color*)buffer2d_get(background, x, y);   
}