#ifndef RENDERER_H
#define RENDERER_H


typedef struct _color {
    unsigned int r;
    unsigned int g;
    unsigned int b;
    unsigned int a;
} Color;

// Ce header contiendra l'ensemble des fonctions utilisables (publiques) de la librairie

#define WHITE      (Color){ 255, 255, 255, 255 }        // White
#define BLACK      (Color){ 0, 0, 0, 255 }              // Black
#define LIGHTGRAY  (Color){ 200, 200, 200, 255 }        // Light Gray
#define GRAY       (Color){ 130, 130, 130, 255 }        // Gray
#define DARKGRAY   (Color){ 80, 80, 80, 255 }           // Dark Gray
#define YELLOW     (Color){ 253, 249, 0, 255 }          // Yellow
#define GOLD       (Color){ 255, 203, 0, 255 }          // Gold
#define ORANGE     (Color){ 255, 161, 0, 255 }          // Orange
#define PINK       (Color){ 255, 109, 194, 255 }        // Pink
#define RED        (Color){ 230, 41, 55, 255 }          // Red
#define MAROON     (Color){ 190, 33, 55, 255 }          // Maroon
#define GREEN      (Color){ 0, 228, 48, 255 }           // Green
#define LIME       (Color){ 0, 158, 47, 255 }           // Lime
#define DARKGREEN  (Color){ 0, 117, 44, 255 }           // Dark Green
#define SKYBLUE    (Color){ 102, 191, 255, 255 }        // Sky Blue
#define BLUE       (Color){ 0, 121, 241, 255 }          // Blue
#define DARKBLUE   (Color){ 0, 82, 172, 255 }           // Dark Blue
#define PURPLE     (Color){ 200, 122, 255, 255 }        // Purple
#define VIOLET     (Color){ 135, 60, 190, 255 }         // Violet
#define DARKPURPLE (Color){ 112, 31, 126, 255 }         // Dark Purple
#define BEIGE      (Color){ 211, 176, 131, 255 }        // Beige
#define BROWN      (Color){ 127, 106, 79, 255 }         // Brown
#define DARKBROWN  (Color){ 76, 63, 47, 255 }           // Dark Brown
#define MAGENTA    (Color){ 255, 0, 255, 255 }          // Magenta


// Fonction à appeler au début du programme, sert à initialiser les valeurs de la république
void init_window();

// Fonction à appeler en fin de programme, sert à rétablir ces dernières comme au temps de Pétain
void leave_window();

// Leaves the window when stopping signals are received.
void wait_and_leave_window();

// Fonction à appeler au moment où on commence à dessiner un marteau et une faucille 
void begin_drawing();

// Fonction à appeler à la fin de l'autre en gros
void end_drawing();

// Renvoie la largeur actuelle du terminal
unsigned int get_screen_width();

// Renvoie la hauteur actuelle du terminal
unsigned int get_screen_height();

// Dessine un point aux coordonnées (x, y) de couleur `color`
void draw_point(unsigned int x, unsigned int y, Color color);


void draw_line(); // Dessine une ligne de points
void draw_rect(); // t'as compris
void draw_circle();
void draw_ellipse();
void draw_ellipse();

#endif