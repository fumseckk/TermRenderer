#include <stdlib.h>
#include <time.h>
#include "../headers/renderer.h"

#define BALL_RADIUS 20
#define PAD_WIDTH  4
#define PAD_HEIGHT 30
#define HALF_PAD_WIDTH  PAD_WIDTH  / 2
#define HALF_PAD_HEIGHT PAD_HEIGHT / 2
#define WIDTH get_screen_width()
#define HEIGHT get_screen_height()

typedef struct _ball {
    int posx;
    int posy;
    int velx;
    int vely;
} Ball;

typedef struct _paddle {
    float posx;
    float posy;
    float velx;
    float vely;
} Paddle;

typedef struct _score {
    unsigned int l;
    unsigned int r;
} Score;


static Ball *ball;
static Paddle *paddle1, *paddle2;
static Score score;


// Borne non atteinte
int rand_int(int lower, int upper){
    return (rand() % (upper - lower)) + lower;
}


void ball_init(right) {
    ball = malloc(sizeof(Ball));
    ball->posx = WIDTH/2;
    ball->posy = HEIGHT/2;

    int horz = rand_int(2, 4);
    int vert = rand_int(1, 3);

    if (!right) horz = -horz;

    ball->velx = horz;
    ball->vely = -vert;
}

void ball_free() {
    free(ball);
}


void init() {
    paddle1 = malloc(sizeof(Paddle));
    paddle2 = malloc(sizeof(Paddle));
    paddle1->posx = HALF_PAD_WIDTH - 1;
    paddle1->posy = HEIGHT/2;
    score.l = 0;
    score.r = 0;

    ball_init(rand_int(0, 2));
}


draw(canvas) {
    begin_drawing();
    fill_background(BLACK);
    draw_line(WIDTH/2, 0, WIDTH/2, HEIGHT , WHITE);
    draw_line(PAD_WIDTH, 0, PAD_WIDTH, HEIGHT , WHITE);
    draw_line(WIDTH - PAD_WIDTH, 0, WIDTH - PAD_WIDTH, HEIGHT , WHITE);
    draw_circle(WIDTH/2, HEIGHT/2, , , WHITE);


    
}



int main() {
    srand(time(NULL));
    init_window();




    leave_window();
}