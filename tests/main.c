#include <stdlib.h>
#include <unistd.h>
#include <math.h>

#include "../headers/debug.h"
#include "../headers/renderer.h"
#include "../src/macros.h"


#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))
#define MAX(X, Y) (((X) > (Y)) ? (X) : (Y))


void loop(double delta);


int main() {
    init(&loop);

    int width =  get_screen_width();
    int height = get_screen_height();

    draw_circle(width/2+30, height/2+30, 40, BLUE);
    set_bg_to_current();

    start();
    return 0;
}


Color color = GOLD;
int i = 0;
int r = 30;

int state = 0;


void loop(double delta) {
    int width =  get_screen_width();
    int height = get_screen_height();

    // should be implemented using other functions etc... but as an example it is fine
    switch (state) {
    case 0:
        set_framerate(16.5);

        remove_circle_boundary(i+1, i+1, i);

        color.r = MIN(color.r+1, 255);
        color.g = MAX(color.g-1, 0);
        color.b = MIN(color.b+1, 255);
        i++;

        draw_circle_boundary(i+1, i+1, i, color);

        if (i - 1 > width) {
            state++;
            i = 0;
        }
        break;
    case 1:
        set_framerate(7);
        for (int x = width/2-r; x < width/2-r + 2*r+1; x++) {
            for (int y = height/2-r; y < height/2-r + 2*r+1; y++) {
                remove_point(x, y);
            }
        }

        float angle = (float) i * PI / 180.0;

        draw_line(width/2, height/2, width/2 + (int) round(r * cos(angle)), height/2 + (int) round(r * sin(angle)), GREEN);
        i++;

        if (i > 360) {
            state++;
        }
        break;
    }
}