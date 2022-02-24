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
    CHG_COLOR_TO(color.r, color.g, color.b);
    MOVE_CUR_TO(x, y);
    PRINT_PIX();

}


void _draw_point_no_color(unsigned int x, unsigned int y) {
    MOVE_CUR_TO(x, y);
    PRINT_PIX();
}

// --------- LIB FUNCTIONS ----------
