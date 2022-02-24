#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <stdbool.h>
#include "../headers/renderer.h"
#include "../src/macros.h"


int main() {
    init_window();

    for (int x = 0; x<=get_screen_width(); x++) {
        for (int y = 0; y<=get_screen_height(); y++) {
            begin_drawing();
            draw_point(x, y, RED);
            end_drawing();
        }
        usleep(100000);
    }

    wait_and_leave_window();
    return 0;
}