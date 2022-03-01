#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <unistd.h>
#include "../headers/renderer.h"

#define BALL_RADIUS 4
#define PAD_WIDTH  10
#define PAD_HEIGHT 40
#define HALF_PAD_WIDTH  PAD_WIDTH  / 2
#define HALF_PAD_HEIGHT PAD_HEIGHT / 2
#define WIDTH get_screen_width()
#define HEIGHT get_screen_height()

typedef struct _ball {
    int posx;
    int posy;
    float velx;
    float vely;
} Ball;

typedef struct _paddle {
    float posx;
    float posy;
    float vel;
} Paddle;

typedef struct _score {
    unsigned int l;
    unsigned int r;
} Score;


static Ball ball;
static Paddle paddle1, paddle2;
static Score score;


// Borne non atteinte
int rand_int(int lower, int upper){
    return (rand() % (upper - lower)) + lower;
}


void ball_init(int right) {

    ball.posx = WIDTH/2;
    ball.posy = HEIGHT/2;

    int horz = rand_int(2, 4);
    int vert = rand_int(1, 3);

    if (!right) horz = -horz;

    ball.velx = horz;
    ball.vely = -vert;
}


void init() {
    fill_background(BLACK);


    paddle1.posx = HALF_PAD_WIDTH - 1;
    paddle1.posy = HEIGHT/2;
    score.l = 0;
    score.r = 0;

    ball_init(rand_int(0, 2));
}


void draw() {
    begin_drawing();

    // Erase old stuff
    draw_circle(ball.posx, ball.posy, BALL_RADIUS, BLACK);
    draw_rect(paddle1.posx - HALF_PAD_WIDTH, paddle1.posy - HALF_PAD_HEIGHT, 
              PAD_WIDTH, PAD_HEIGHT, GREEN);
    draw_rect(paddle2.posx - HALF_PAD_WIDTH, paddle2.posy - HALF_PAD_HEIGHT, 
              PAD_WIDTH, PAD_HEIGHT, GREEN);

    // Draw terrain
    draw_line(WIDTH/2, 0, WIDTH/2, HEIGHT , WHITE);
    draw_line(PAD_WIDTH, 0, PAD_WIDTH, HEIGHT , WHITE);
    draw_line(WIDTH - PAD_WIDTH, 0, WIDTH - PAD_WIDTH, HEIGHT , WHITE);
    draw_circle(WIDTH/2, HEIGHT/2, 10, WHITE);


    


    // Update the paddles' y pos
    if (paddle1.posy > HALF_PAD_HEIGHT && paddle1.posy < HEIGHT - HALF_PAD_HEIGHT)
        paddle1.posy += paddle1.vel;
    else if (paddle1.posy == HALF_PAD_HEIGHT && paddle1.vel > 0)
        paddle1.posy += paddle1.vel;
    else if (paddle1.posy == HEIGHT - HALF_PAD_HEIGHT && paddle1.vel < 0)
        paddle1.posy += paddle1.vel;

    if (paddle2.posy > HALF_PAD_HEIGHT && paddle2.posy < HEIGHT - HALF_PAD_HEIGHT)
        paddle2.posy += paddle2.vel;
    else if (paddle2.posy == HALF_PAD_HEIGHT && paddle2.vel > 0)
        paddle2.posy += paddle2.vel;
    else if (paddle2.posy == HEIGHT - HALF_PAD_HEIGHT && paddle2.vel < 0)
        paddle2.posy += paddle2.vel;
    
    // Update ball
    ball.posx += (int) ball.velx;
    ball.posy += (int) ball.vely;


    // Draw paddles and ball
    draw_circle(ball.posx, ball.posy, BALL_RADIUS, RED);
    draw_rect(paddle1.posx - HALF_PAD_WIDTH, paddle1.posy - HALF_PAD_HEIGHT, 
              PAD_WIDTH, PAD_HEIGHT, GREEN);
    draw_rect(paddle2.posx - HALF_PAD_WIDTH, paddle2.posy - HALF_PAD_HEIGHT, 
              PAD_WIDTH, PAD_HEIGHT, GREEN);

    // Ball collision check on top and bottom walls
    if ((int) ball.posy <= BALL_RADIUS)
        ball.vely = -ball.vely;
    else if ((int) ball.posy >= HEIGHT - BALL_RADIUS)
        ball.vely = -ball.vely;
    
    // Ball collision check on gutters or paddles
    if ((int) ball.posx <= BALL_RADIUS + PAD_WIDTH 
        && (int) ball.posy >= paddle1.posy - HALF_PAD_HEIGHT 
        && (int) ball.posy  < paddle1.posy + HALF_PAD_HEIGHT) {

        ball.velx = -ball.velx;
        ball.velx *= 1.1;
        ball.velx *= 1.1;
    }
    else if ((int) ball.posx <= BALL_RADIUS + PAD_WIDTH) {
        score.r++;
        draw_circle(ball.posx, ball.posy, BALL_RADIUS, BLACK);
        ball_init(1);
    }

    if ((int) ball.posx >= WIDTH + 1 - BALL_RADIUS - PAD_WIDTH 
        && (int) ball.posy >= paddle2.posy - HALF_PAD_HEIGHT 
        && (int) ball.posy  < paddle2.posy + HALF_PAD_HEIGHT) {
    
        ball.velx = -ball.velx;
        ball.velx *= 1.1;
        ball.velx *= 1.1;
    }
    else if ((int) ball.posx >= WIDTH - BALL_RADIUS - PAD_WIDTH) {
        score.l++;
        draw_circle(ball.posx, ball.posy, BALL_RADIUS, BLACK);
        ball_init(0);
    }


    // Update scores

    end_drawing();
}



int main() {
    srand(time(NULL));
    init_window();
    init();

    while(1) {
        draw();
        usleep(30000);
    }


    leave_window();
}