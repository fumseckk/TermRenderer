#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

#include "core.h"

#include "macros.h"
#include "screen_buffer.h"
#include "../headers/renderer.h"
#include "../headers/debug.h"

#include "servers/main_server.h"


// --------- Global variables ---------


static bool initialized = false;
static struct termios term_original;
static struct termios term_settings;

static _Atomic int wind_width  = 0;
static _Atomic int wind_height = 0;

static void (*loop)(double);

#ifndef NDEBUG
static FILE* debout;
#endif  // #ifndef NDEBUG


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
            resize_buff();
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
    alloc_buff();
    initialized = true;

    // Signal handlers for resize and Ctrl+C 
    FATAL_ERROR(signal(SIGWINCH, _sigwinch_handler) == SIG_ERR || signal(SIGINT, _sigint_handler) == SIG_ERR,
                "Could not init: handling SIGWINCH and SIGINT signals raised SIG_ERR");

    SAVE_CUR_POS();
    HIDE_CUR();
    USE_ALT_BUFF();
    MOVE_CUR_HOME();

    draw_buff();
}


void _leave_window() {
    FATAL_ERROR(tcsetattr(STDIN, 0, &term_settings) < 0, "[CNSR] Could not set config for stdin");
    free_buff();
    fflush(stdout);
    initialized = false;
    CLEAR_SCREEN();
    USE_MAIN_BUFF();
    SHOW_CUR();
    RESTORE_CUR_POS();
}


void _begin_drawing() {
    // nothing to do lol
}


void _end_drawing() {
    draw_buff();
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


void set_bg_to_color(Color color) {
    int width = get_screen_width();
    int height = get_screen_height();
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            pix_to_bg(x, y, color);
            pix_to_buff(x, y, color);
        }
    }
    
}


/* TOFIX: change place of the function */

// void set_bg_to_current() {
//     int width = get_screen_width();
//     int height = get_screen_height();
//     for (int x = 0; x < width; x++) {
//         for (int y = 0; y < height; y++) {
//             pix_to_bg(x, y, buff->pixels[x * buff->rows + y]);
//         }
//     }
// }


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
    _init_window();
    _init_debug();
}


void start() {
    _main_server_loop(); // start main loop
}