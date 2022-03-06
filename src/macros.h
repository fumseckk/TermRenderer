#ifndef MACROS_H
#define MACROS_H

#include <stdio.h> // Necessary for fprintf
#include <stdlib.h> // Necessary for EXIT_FAILURE


#define STDIN  0
#define STDOUT 1


#define ESC "\x1b"
#define CSI "\x1b["

#define RESET_FG_BG_COLOR()         printf(CSI "0m");
#define CHG_FG_COLOR_TO(__c_struct) printf(CSI "38;2;%d;%d;%dm", __c_struct.r, __c_struct.g, __c_struct.b);
#define CHG_BG_COLOR_TO(__c_struct) printf(CSI "48;2;%d;%d;%dm", __c_struct.r, __c_struct.g, __c_struct.b);


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

#define MOVE_CUR_TO(__x, __y) printf(CSI "%d;%dH", __y/2+1, __x+1);
#define MOVE_CUR_HOME()       printf(CSI "H");
#define MOVE_CUR_UP(__x)      printf(CSI "%dA", __x);
#define MOVE_CUR_DOWN(__x)    printf(CSI "%dB", __x);
#define MOVE_CUR_RIGHT(__x)   printf(CSI "%dC", __x);
#define MOVE_CUR_LEFT(__x)    printf(CSI "%dD", __x);

#define HIDE_CUR() printf(CSI "?25l"); 
#define SHOW_CUR() printf(CSI "?25h");

#define USE_ALT_BUFF()  printf(CSI "?1049h");
#define USE_MAIN_BUFF() printf(CSI "?1049l");

#define SAVE_CUR_POS()    printf(ESC "7");
#define RESTORE_CUR_POS() printf(ESC "8");

#endif // MACROS_H