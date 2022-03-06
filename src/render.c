#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include "macros.h"
#include "private_headers.h"
#include "../headers/renderer.h"


extern Buffer* buff;
bool initialized = false;
extern struct winsize volatile WINSIZE;
static struct termios term_original;
static struct termios term_settings;


// --------- LOCAL HEADERS ----------

// --------- LOCAL HEADERS ----------



void update_winsize() {
    if (!initialized) {
        ioctl(0, TIOCGWINSZ, &WINSIZE);
        WINSIZE.ws_row *= 2;
    }
    else {
        int old_cols = WINSIZE.ws_col;
        int old_rows = WINSIZE.ws_row;
        ioctl(0, TIOCGWINSZ, &WINSIZE);
        WINSIZE.ws_row *= 2;
        if (old_cols != WINSIZE.ws_col || old_rows != WINSIZE.ws_row) {
            resize_buff();
        }
    }
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
    if (!initialized) update_winsize();
    return (unsigned int) WINSIZE.ws_col;
}

unsigned int get_screen_height() {
    if (!initialized) update_winsize();
    return (unsigned int) WINSIZE.ws_row;
}


void init_window() {



    FATAL_ERROR(tcgetattr(STDIN, &term_settings) < 0, "Could not retrieve terminal settings for stdin");
    term_original = term_settings;
    // ICANON: disables cannonical mode
    // ECHO: disables echoing input characters (like arrow keys, HOME, etc)
    term_settings.c_lflag &= ~(ECHO | ICANON);
    FATAL_ERROR(tcsetattr(STDIN, 0, &term_settings) < 0, "Could not set terminal settings for stdin");

    ioctl(0, TIOCGWINSZ, &WINSIZE);
    WINSIZE.ws_row *= 2;

    alloc_buff();
    initialized = true;

    // Signal handlers for resize and Ctrl+C 
    FATAL_ERROR(signal(SIGWINCH, sigwinch_handler) == SIG_ERR || signal(SIGINT, sigint_handler) == SIG_ERR,
                "Could not init: handling SIGWINCH and SIGINT signals raised SIG_ERR");

    SAVE_CUR_POS();
    HIDE_CUR();
    USE_ALT_BUFF();
    MOVE_CUR_HOME();

    draw_buff();
}


void leave_window() {
    FATAL_ERROR(tcsetattr(STDIN, 0, &term_settings) < 0, "[CNSR] Could not set config for stdin");
    free_buff();
    fflush(stdout);
    initialized = false;
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
    draw_buff();
    fflush(stdout);
}


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


void set_bg_to_current() {
    int width = get_screen_width();
    int height = get_screen_height();
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            pix_to_bg(x, y, buff->pixels[x * buff->rows + y]);
        }
    }
}

// --------- LIB FUNCTIONS ----------
