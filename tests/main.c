#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <stdbool.h>
#include "../headers/renderer.h"
#include "../src/macros.h"


int main() {
    init_window();

    begin_drawing();
    draw_rect(10, 10, 10, 10, GOLD);
    draw_rect_boundary(10, 10, 10, 10, PURPLE);
    draw_line(30, 30, 100, 12, BLUE);
    end_drawing();

    for (int x = 0; x<=get_screen_width(); x++) {
        begin_drawing();
        draw_line(x, 0, x, get_screen_height(), PURPLE);
        end_drawing();
        usleep(10000);
    }

    wait_and_leave_window();
    return 0;
}