#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include "../headers/renderer.h"
#include "../src/macros.h"

#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))
#define MAX(X, Y) (((X) > (Y)) ? (X) : (Y))


int main() {
    init_window();
    begin_drawing(); 
    end_drawing();

    int width =  get_screen_width();
    int height = get_screen_height();
    
    draw_circle(width/2+30, height/2+30, 40, BLUE);

    set_bg_to_current();
    
    Color color = GOLD;
    int rad = 0;
    for (int i = 1; i<width; i++) {
            color.r = MIN(color.r+1, 255);
            color.g = MAX(color.g-1, 0);
            color.b = MIN(color.b+1, 255);
            rad++;
            draw_circle_boundary(i, i, rad, color);
            end_drawing();
            usleep(15000);
            remove_circle_boundary(i, i, rad);
    }

    float angle;
    int r = 30;
    for (int i = 0; i<=360; i++) {
        begin_drawing();
        for (int x = width/2-r; x < width/2-r + 2*r+1; x++) {
            for (int y = height/2-r; y < height/2-r + 2*r+1; y++) {
                remove_point(x, y);
            }
        }
        // draw_rect(width/2 - r, height/2 - r, 2*r+1, 2*r+1, BLACK);

        width = get_screen_width();
        height = get_screen_height();

        angle = (float) i * PI / 180.0;
        draw_line(width/2, height/2, width/2 + (int) round(r * cos(angle)), height/2 + (int) round(r * sin(angle)), GREEN);
        end_drawing();

        usleep(5000);
    }
    end_drawing();

    wait_and_leave_window();
    return 0;
}