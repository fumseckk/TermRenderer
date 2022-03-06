#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "macros.h"
#include "../headers/renderer.h"
#include "screen_buffer.h"


// DRAWING SHAPES

void draw_point(int x, int y, Color color) {
    _pix_to_buff(x, y, color);
}


void draw_line(int x0, int y0, int x1, int y1, Color color) {
    int dx, dy, sx, sy, err, e2;

    dx =  abs(x1 - x0);
    dy = -abs(y1 - y0);
    sy = y0 < y1 ? 1 : -1; 
    sx = x0 < x1 ? 1 : -1;
    
    err = dx + dy; // error value e_xy

    while (1) {  // boucle infinie
        draw_point(x0, y0, color);
        if (x0 == x1 && y0 == y1) break;

        e2 = 2 * err;

        if (e2 >= dy)  { 
            err += dy;
            x0 += sx; // e_xy + e_x > 0
        }
        if (e2 <= dx) {
            err += dx;
            y0 += sy; // e_xy + e_y < 0
        }
    }
}


void draw_rect_boundary(int x, int y, int width, int height, Color color) {
    for (int posx = x; posx < x + width;    posx++) draw_point(posx, y, color);
    for (int posx = x; posx < x + width;    posx++) draw_point(posx, y + height, color);
    for (int posy = y; posy < y + height;   posy++) draw_point(x, posy, color);
    for (int posy = y; posy < y + height+1; posy++) draw_point(x + width, posy, color);
}


void draw_rect(int x, int y, int width, int height, Color color) {
    for (int posx = x; posx < x + width; posx++) {
        for (int posy = y; posy < y + height; posy++) {
            draw_point(posx, posy, color);
        }
    }
}


void draw_ellipse(int x0, int y0, int width, int height, Color color) {
    int x1   = x0 + width;
    int y1   = y0 + height;
    int b1   = height & 1; // values of diameter 
    long dx  = 4*(1 - width) * height * height;
    long dy  = 4*(b1 + 1) * width * width; // error increment
    long err = dx + dy + b1 * width * width;
    long e2; // error of 1.step

    if (x0 > x1) { x0 = x1; x1 += width; } // if called with swapped points
    if (y0 > y1) y0 = y1; // .. exchange them 
    y0 += (height + 1) / 2;
    y1 = y0 - b1;   // starting pixel
    width *= 8 * width;
    b1 = 8 * height * height;

    do {
        for (int i = x0; i <= x1; i++) draw_point(i, y0, color);
        for (int i = x0; i <= x1; i++) draw_point(i, y1, color);
        e2 = 2 * err;

        if (e2 >= dx) {
            x0++;
            x1--;
            err += dx += b1;
        } // x step

        if (e2 <= dy) {
            y0++;
            y1--;
            err += dy += width;
        }  // y step 
    } while (x0 <= x1);

   while (y0-y1 < height) {  // too early stop of flat ellipses a=1
       draw_point(x0-1, y0, color); // -> finish tip of ellipse
       draw_point(x1+1, y0++, color); 
       draw_point(x0-1, y1, color);
       draw_point(x1+1, y1--, color); 
   }
}


void draw_ellipse_boundary(int x0, int y0, int width, int height, Color color) {
    int x1   = x0 + width;
    int y1   = y0 + height;
    int b1   = height & 1; // values of diameter 
    long dx  = 4*(1 - width) * height * height;
    long dy  = 4*(b1 + 1) * width * width; // error increment
    long err = dx + dy + b1 * width * width;
    long e2; // error of 1.step

    if (x0 > x1) { x0 = x1; x1 += width; } // if called with swapped points
    if (y0 > y1) y0 = y1; // .. exchange them 
    y0 += (height + 1) / 2;
    y1 = y0 - b1;   // starting pixel
    width *= 8 * width;
    b1 = 8 * height * height;

    do {
        draw_point(x1, y0, color); /*   I. Quadrant */
        draw_point(x0, y0, color); /*  II. Quadrant */
        draw_point(x0, y1, color); /* III. Quadrant */
        draw_point(x1, y1, color); /*  IV. Quadrant */
        e2 = 2 * err;

        if (e2 >= dx) {
            x0++;
            x1--;
            err += dx += b1;
        } // x step

        if (e2 <= dy) {
            y0++;
            y1--;
            err += dy += width;
        }  // y step 
    } while (x0 <= x1);

   while (y0-y1 < height) {  // too early stop of flat ellipses a=1
       draw_point(x0-1, y0, color); // -> finish tip of ellipse
       draw_point(x1+1, y0++, color); 
       draw_point(x0-1, y1, color);
       draw_point(x1+1, y1--, color); 
   }
}


void draw_circle(int cx, int cy, int radius, Color color) {
    int x = radius;
    int y = 0;
    int xChange = 1 - (radius << 1);
    int yChange = 0;
    int radiusError = 0;

    while (x >= y)
    {
        for (int i = cx - x; i <= cx + x; i++)
        {
            draw_point(i, cy + y, color);
            draw_point(i, cy - y, color);
        }
        for (int i = cx - y; i <= cx + y; i++)
        {
            draw_point(i, cy + x, color);
            draw_point(i, cy - x, color);
        }

        y++;
        radiusError += yChange;
        yChange += 2;
        if (((radiusError << 1) + xChange) > 0)
        {
            x--;
            radiusError += xChange;
            xChange += 2;
        }
    }
}


void draw_circle_boundary(int cx, int cy, int radius, Color color) {
    if (!radius) return;
    int error = -radius;
    int x = radius;
    int y = 0;

    while (x > y) {
        draw_point(cx + x, cy + y, color);
        if (x != 0) draw_point(cx - x, cy + y, color);
        if (y != 0) draw_point(cx + x, cy - y, color);
        draw_point(cx - x, cy - y, color);

        draw_point(cx + y, cy + x, color);
        if (y != 0) draw_point(cx - y, cy + x, color);
        if (x != 0) draw_point(cx + y, cy - x, color);
        draw_point(cx - y, cy - x, color);

        error += y;
        ++y;
        error += y;
        if (error >= 0) {
            --x;
            error -= x;
            error -= x;
        }
    }
    draw_point(cx + x, cy + y, color);
    if (x != 0) draw_point(cx - x, cy + y, color);
    if (y != 0) draw_point(cx + x, cy - y, color);
    draw_point(cx - x, cy - y, color);
}




// REMOVING SHAPES

void remove_point(int x, int y) {
    _remove_pix(x, y);
}


void remove_line(int x0, int y0, int x1, int y1) {
    int dx, dy, sx, sy, err, e2;

    dx =  abs(x1 - x0);
    dy = -abs(y1 - y0);
    sy = y0 < y1 ? 1 : -1; 
    sx = x0 < x1 ? 1 : -1;
    
    err = dx + dy; // error value e_xy

    while (1) {  // boucle infinie
        remove_point(x0, y0);
        if (x0 == x1 && y0 == y1) break;

        e2 = 2 * err;

        if (e2 >= dy)  { 
            err += dy;
            x0 += sx; // e_xy + e_x > 0
        }
        if (e2 <= dx) {
            err += dx;
            y0 += sy; // e_xy + e_y < 0
        }
    }
}


void remove_rect_boundary(int x, int y, int width, int height) {
    for (int posx = x; posx < x + width;    posx++) remove_point(posx, y);
    for (int posx = x; posx < x + width;    posx++) remove_point(posx, y + height);
    for (int posy = y; posy < y + height;   posy++) remove_point(x, posy);
    for (int posy = y; posy < y + height+1; posy++) remove_point(x + width, posy);
}


void remove_rect(int x, int y, int width, int height) {
    for (int posx = x; posx < x + width; posx++) {
        for (int posy = y; posy < y + height; posy++) {
            remove_point(posx, posy);
        }
    }
}


void remove_ellipse(int x0, int y0, int width, int height) {
    int x1   = x0 + width;
    int y1   = y0 + height;
    int b1   = height & 1; // values of diameter 
    long dx  = 4*(1 - width) * height * height;
    long dy  = 4*(b1 + 1) * width * width; // error increment
    long err = dx + dy + b1 * width * width;
    long e2; // error of 1.step

    if (x0 > x1) { x0 = x1; x1 += width; } // if called with swapped points
    if (y0 > y1) y0 = y1; // .. exchange them 
    y0 += (height + 1) / 2;
    y1 = y0 - b1;   // starting pixel
    width *= 8 * width;
    b1 = 8 * height * height;

    do {
        for (int i = x0; i <= x1; i++) remove_point(i, y0);
        for (int i = x0; i <= x1; i++) remove_point(i, y1);
        e2 = 2 * err;

        if (e2 >= dx) {
            x0++;
            x1--;
            err += dx += b1;
        } // x step

        if (e2 <= dy) {
            y0++;
            y1--;
            err += dy += width;
        }  // y step 
    } while (x0 <= x1);

   while (y0-y1 < height) {  // too early stop of flat ellipses a=1
       remove_point(x0-1, y0); // -> finish tip of ellipse
       remove_point(x1+1, y0++); 
       remove_point(x0-1, y1);
       remove_point(x1+1, y1--); 
   }
}


void remove_ellipse_boundary(int x0, int y0, int width, int height) {
    int x1   = x0 + width;
    int y1   = y0 + height;
    int b1   = height & 1; // values of diameter 
    long dx  = 4*(1 - width) * height * height;
    long dy  = 4*(b1 + 1) * width * width; // error increment
    long err = dx + dy + b1 * width * width;
    long e2; // error of 1.step

    if (x0 > x1) { x0 = x1; x1 += width; } // if called with swapped points
    if (y0 > y1) y0 = y1; // .. exchange them 
    y0 += (height + 1) / 2;
    y1 = y0 - b1;   // starting pixel
    width *= 8 * width;
    b1 = 8 * height * height;

    do {
        remove_point(x1, y0); /*   I. Quadrant */
        remove_point(x0, y0); /*  II. Quadrant */
        remove_point(x0, y1); /* III. Quadrant */
        remove_point(x1, y1); /*  IV. Quadrant */
        e2 = 2 * err;

        if (e2 >= dx) {
            x0++;
            x1--;
            err += dx += b1;
        } // x step

        if (e2 <= dy) {
            y0++;
            y1--;
            err += dy += width;
        }  // y step 
    } while (x0 <= x1);

   while (y0-y1 < height) {  // too early stop of flat ellipses a=1
       remove_point(x0-1, y0); // -> finish tip of ellipse
       remove_point(x1+1, y0++); 
       remove_point(x0-1, y1);
       remove_point(x1+1, y1--); 
   }
}


void remove_circle(int cx, int cy, int radius) {
    int x = radius;
    int y = 0;
    int xChange = 1 - (radius << 1);
    int yChange = 0;
    int radiusError = 0;

    while (x >= y)
    {
        for (int i = cx - x; i <= cx + x; i++)
        {
            remove_point(i, cy + y);
            remove_point(i, cy - y);
        }
        for (int i = cx - y; i <= cx + y; i++)
        {
            remove_point(i, cy + x);
            remove_point(i, cy - x);
        }

        y++;
        radiusError += yChange;
        yChange += 2;
        if (((radiusError << 1) + xChange) > 0)
        {
            x--;
            radiusError += xChange;
            xChange += 2;
        }
    }
}


void remove_circle_boundary(int cx, int cy, int radius) {
    if (!radius) return;
    int error = -radius;
    int x = radius;
    int y = 0;

    while (x > y) {
        remove_point(cx + x, cy + y);
        if (x != 0) remove_point(cx - x, cy + y);
        if (y != 0) remove_point(cx + x, cy - y);
        remove_point(cx - x, cy - y);

        remove_point(cx + y, cy + x);
        if (y != 0) remove_point(cx - y, cy + x);
        if (x != 0) remove_point(cx + y, cy - x);
        remove_point(cx - y, cy - x);

        error += y;
        ++y;
        error += y;
        if (error >= 0) {
            --x;
            error -= x;
            error -= x;
        }
    }
    remove_point(cx + x, cy + y);
    if (x != 0) remove_point(cx - x, cy + y);
    if (y != 0) remove_point(cx + x, cy - y);
    remove_point(cx - x, cy - y);
}
