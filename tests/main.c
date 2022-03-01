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
    fill_background(BLACK);
    end_drawing();

    int width =  get_screen_width();
    int height = get_screen_height();
    
    // draw_ellipse(40, 50, 30, 40, PURPLE);
    // draw_ellipse_boundary(39, 49, 31, 12, GOLD);
    draw_circle(width/2+30, height/2+30, 40, BLUE);


    // draw_circle(30, 30, 30, GOLD);



    Color color = GOLD;
    int rad = 0;
    for (int i = 0; i<width; i++) {
            color.r = MIN(color.r+1, 255);
            color.g = MAX(color.g-1, 0);
            color.b = MIN(color.b+1, 255);
            rad++;
            // draw_ellipse(i, i, rad, rad, GOLD);
            draw_circle_boundary(i, i, rad, color);
            end_drawing();
            usleep(15000);
            draw_circle_boundary(i, i, rad, BLACK);
    }


    float angle;
    int r = 30;
    for (int i = 0; i<=360; i++) {
        begin_drawing();
        draw_rect(width/2 - r, height/2 - r, 2*r+1, 2*r+1, BLACK);

        width = get_screen_width();
        height = get_screen_height();

        angle = (float) i * PI / 180.0;
        draw_line(width/2, height/2, width/2 + (int) round(r * cos(angle)), height/2 + (int) round(r * sin(angle)), GREEN);
        end_drawing();

        usleep(5000);
    }



    // draw_rect(10, 10, 10, 10, GOLD);
    // draw_rect_boundary(10, 10, 10, 10, PURPLE);

    end_drawing();

    wait_and_leave_window();

    return 0;
}