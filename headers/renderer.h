#ifndef RENDERER_H
#define RENDERER_H

// Ce header contient l'ensemble des fonctions utilisables (publiques) de la
// librairie

// TODO:
// Fix window resize
// Managing input
// Screen buffer: ability to "set as default background"
// Rendering text
// Importing and rendering images -> ppm first, then other formats, existing
// libs Framerate along with main loop function? Ability to have sprite object
// you can just move. Level of precedence of sprites (bg/fg).

// Idée: à chaque fois qu'on dessine une chape, ajouté dans une structure
// "Object" Permet: ctrl+z, remove sans faire pleins de fonctions, ...

typedef struct _color {
  unsigned int r;
  unsigned int g;
  unsigned int b;
} Color;

// Les coordonnées sont 0-indexed

#define PI 3.1415926535

#define BLACK (Color){0, 0, 0}            // Black
#define WHITE (Color){255, 255, 255}      // White
#define LIGHTGRAY (Color){200, 200, 200}  // Light Gray
#define GRAY (Color){130, 130, 130}       // Gray
#define DARKGRAY (Color){80, 80, 80}      // Dark Gray
#define YELLOW (Color){253, 249, 0}       // Yellow
#define GOLD (Color){255, 203, 0}         // Gold
#define ORANGE (Color){255, 161, 0}       // Orange
#define PINK (Color){255, 109, 194}       // Pink
#define RED (Color){230, 41, 55}          // Red
#define MAROON (Color){190, 33, 55}       // Maroon
#define GREEN (Color){0, 228, 48}         // Green
#define LIME (Color){0, 158, 47}          // Lime
#define DARKGREEN (Color){0, 117, 44}     // Dark Green
#define SKYBLUE (Color){102, 191, 255}    // Sky Blue
#define BLUE (Color){0, 121, 241}         // Blue
#define DARKBLUE (Color){0, 82, 172}      // Dark Blue
#define PURPLE (Color){200, 122, 255}     // Purple
#define VIOLET (Color){135, 60, 190}      // Violet
#define DARKPURPLE (Color){112, 31, 126}  // Dark Purple
#define BEIGE (Color){211, 176, 131}      // Beige
#define BROWN (Color){127, 106, 79}       // Brown
#define DARKBROWN (Color){76, 63, 47}     // Dark Brown
#define MAGENTA (Color){255, 0, 255}      // Magenta

void init_window();
void leave_window();
void wait_and_leave_window();
void begin_drawing();
void end_drawing();

unsigned int get_screen_width();
unsigned int get_screen_height();

void draw_point(unsigned int x, unsigned int y, Color color);
void draw_line(unsigned int x0, unsigned int y0, unsigned int x1,
               unsigned int y1, Color color);

void draw_rect(unsigned int x, unsigned int y, unsigned int width,
               unsigned int height, Color color);
void draw_rect_boundary(unsigned int x, unsigned int y, unsigned int width,
                        unsigned int height, Color color);

void draw_ellipse(int x0, int y0, int width, int height, Color color);
void draw_ellipse_boundary(int x0, int y0, int width, int height, Color color);

void draw_circle(int cx, int cy, int radius, Color color);
void draw_circle_boundary(unsigned int cx, unsigned int cy, unsigned int radius,
                          Color color);

void remove_point(unsigned int x, unsigned int y);
void remove_line(unsigned int x0, unsigned int y0, unsigned int x1,
                 unsigned int y1);

void remove_rect(unsigned int x, unsigned int y, unsigned int width,
                 unsigned int height);
void remove_rect_boundary(unsigned int x, unsigned int y, unsigned int width,
                          unsigned int height);

void remove_ellipse(int x0, int y0, int width, int height);
void remove_ellipse_boundary(int x0, int y0, int width, int height);

void remove_circle(int cx, int cy, int radius);
void remove_circle_boundary(unsigned int cx, unsigned int cy,
                            unsigned int radius);

void set_bg_to_color(Color color);
void set_bg_to_current();

// void draw_circle();
// void draw_ellipse();

#endif