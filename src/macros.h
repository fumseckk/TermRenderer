#ifndef MACROS_H
#define MACROS_H

#include <stdlib.h> // Necessary for EXIT_FAILURE


#define STDIN  0
#define STDOUT 1


#define ESC "\x1b"
#define CSI "\x1b["



#define CHG_COLOR_TO(r, g, b) printf(CSI "38;2;%d;%d;%dm", r, g, b);
#define PRINT_PIX() printf("█");


// ANSI colors
#define ANSGREY    CSI "90m"
#define ANSRED     CSI "31m"
#define ANSGREEN   CSI "32m"
#define ANSMAGENTA CSI "35m"
#define ANSCYAN    CSI "36m"
#define ANSBOLD    CSI "1m"
#define ANSRESET   CSI "0m"


// CSI command macros
#define CLEAR_SCREEN() printf(CSI "H" CSI "J");

#define MOVE_CUR_TO(__x, __y) printf(CSI "%d;%dH", ++__y, ++__x);
#define MOVE_CUR_HOME()   printf(CSI "H");
#define MOVE_CUR_UP(__x)    printf(CSI "%dA", __x);
#define MOVE_CUR_DOWN(__x)  printf(CSI "%dB", __x);
#define MOVE_CUR_RIGHT(__x) printf(CSI "%dC", __x);
#define MOVE_CUR_LEFT(__x)  printf(CSI "%dD", __x);

#define HIDE_CUR()        printf(CSI "?25l"); 
#define SHOW_CUR()        printf(CSI "?25h");

#define USE_ALT_BUFF()  printf(CSI "?1049h");
#define USE_MAIN_BUFF() printf(CSI "?1049l");

#define SAVE_CUR_POS()    printf(ESC "7");
#define RESTORE_CUR_POS() printf(ESC "8");



#define SLEEP() (usleep((useconds_t) 1000000/FPS);)

#define FATAL_ERROR(cond, error_message) ({        \
    if (cond) {                                    \
        printf("FATAL_ERROR: %s\n", error_message);\
        exit(EXIT_FAILURE);                        \
    }                                              \
})



#endif // MACROS_H