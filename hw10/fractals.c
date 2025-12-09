// fractals.c
// Name: Dannylo Correia


#include "gfx.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void sierpinski( int x1, int y1, int x2, int y2, int x3, int y3 );    // Class Code
void drawTriangle( int x1, int y1, int x2, int y2, int x3, int y3 );  // Class Code
void drawSquare(int cx, int cy, int side);
void shrinkingSquares(int cx, int cy, int side);
void spiralSquares(double center_x, double center_y, double radius, double angle_deg, double size);
void circularLace(int cx, int cy, int radius);

void tree(int x1, int y1, double angle, double length);
void fern(int x1, int y1, double angle, double length);
void spiralOfSpirals(int x, int y, double angle, double size);


int main()
{
  int width = 850, height = 850, mrgn = 20;

  gfx_open(width, height, "Fractals");

  int event = gfx_wait();

  while(1) {
    gfx_clear();

    if (event == '1') {
      sierpinski(mrgn, mrgn, width-mrgn, mrgn, width/2, height-mrgn);
    }

    if (event == '2') {
      shrinkingSquares(width/2, height/2, 300);
    }

    if (event == '3') {
    spiralSquares(width/2, height/2,300, 0, 120);
    }

    if (event == '4') {
    circularLace(width/2, height/2, 200);
  }

    if (event == '5') {
      printf("Snowflake");
    }

    if (event == '6') {
      tree(width/2, height - 60, M_PI / 2, 240);
    }

    if (event == '7') {
      fern(width/2, height - 50, M_PI/2, 180);
    }

    if (event == '8') {
      int x = width / 2;
      int y = height / 2;
      spiralOfSpirals(x, y, 0, 300);
    }

    
    if ( gfx_wait() == 'q' ) break;
  }
}

void sierpinski( int x1, int y1, int x2, int y2, int x3, int y3 )     // Class Code
{
  // Base case. 
  if( abs(x2-x1) < 5 ) return;

  // Draw a triangle
  drawTriangle( x1, y1, x2, y2, x3, y3 );

  // Recursive calls
  sierpinski( x1, y1, (x1+x2)/2, (y1+y2)/2, (x1+x3)/2, (y1+y3)/2 );
  sierpinski( (x1+x2)/2, (y1+y2)/2, x2, y2, (x2+x3)/2, (y2+y3)/2 );
  sierpinski( (x1+x3)/2, (y1+y3)/2, (x2+x3)/2, (y2+y3)/2, x3, y3 );
}

void drawTriangle( int x1, int y1, int x2, int y2, int x3, int y3 )   // Class Code
{
  gfx_line(x1,y1,x2,y2);
  gfx_line(x2,y2,x3,y3);
  gfx_line(x3,y3,x1,y1);
}

void drawSquare(int cx, int cy, int side) {
    int h = (side / 2);

    gfx_line(cx - h, cy - h,  cx + h, cy - h);
    gfx_line(cx + h, cy - h,  cx + h, cy + h);
    gfx_line(cx + h, cy + h,  cx - h, cy + h);
    gfx_line(cx - h, cy + h,  cx - h, cy - h);
}

void shrinkingSquares(int cx, int cy, int side) {
    if (side < 5) return;

    drawSquare(cx, cy, side);

    int newSide = side / 2;
    int half = side / 2;

    // child centers = EXACT parent corners
    shrinkingSquares(cx - half, cy - half, newSide);  // top-left
    shrinkingSquares(cx + half, cy - half, newSide);  // top-right
    shrinkingSquares(cx - half, cy + half, newSide);  // bottom-left
    shrinkingSquares(cx + half, cy + half, newSide);  // bottom-right
}

void spiralSquares(double center_x, double center_y, double radius, double angle_deg, double size)
{
    if (size < 2) return;

    double angle_rad = angle_deg * M_PI / 180.0;

    double cx = center_x + radius * cos(angle_rad);
    double cy = center_y + radius * sin(angle_rad);

    drawSquare((int)cx, (int)cy, (int)size);

    spiralSquares(center_x, center_y, radius * 0.9, angle_deg + 40.0, size * 0.9);
}

void circularLace(int cx, int cy, int radius)
{
    if (radius < 5) return;

    // Draw the main circle
    gfx_circle(cx, cy, radius);

    // Place 6 smaller circles evenly around the perimeter
    for (int i = 0; i < 6; i++) {
        double angle = i * M_PI / 3.0;  // 0, 60, 120, 180, 240, 300 degrees

        int newX = cx + radius * cos(angle);
        int newY = cy + radius * sin(angle);

        circularLace(newX, newY, radius / 3);
    }
}



void tree(int x1, int y1, double angle, double length)
{
    if (length < 4) return;   // stop when branch becomes very small

    int x2 = x1 + (int)(length * cos(angle));
    int y2 = y1 - (int)(length * sin(angle));  

    gfx_line(x1, y1, x2, y2);

    double newLength = length * 0.72;  // perfect scaling for 800×800
    double delta = M_PI / 6;           // 30 degrees

    tree(x2, y2, angle + delta, newLength);
    tree(x2, y2, angle - delta, newLength);
}

void fern(int x1, int y1, double angle, double length)
{
    if (length < 2) return;   // base case: stop when segments get tiny

    // Compute endpoint of this segment
    int x2 = x1 + (int)(length * cos(angle));
    int y2 = y1 - (int)(length * sin(angle));  // negative = upward

    // Draw main segment
    gfx_line(x1, y1, x2, y2);

    // Shrinking factor for next main segment
    double mainShrink = 0.85;
    double sideShrink = 0.40;

    // Side angle (frond deviation)
    double sideAngle = M_PI / 6;   // 30 degrees

    // Continue main stalk
    fern(x2, y2, angle, length * mainShrink);

    // Left frond
    fern(x2, y2, angle + sideAngle, length * sideShrink);

    // Right frond
    fern(x2, y2, angle - sideAngle, length * sideShrink);
}

void spiralOfSpirals(int x, int y, double angle, double size)
{
    if (size < 2) return;   

    double rad = angle * M_PI / 180.0;

    double x2 = x + cos(rad) * (size * 0.5);
    double y2 = y + sin(rad) * (size * 0.5);

    gfx_point(x2, y2);

    spiralOfSpirals(x2, y2, angle + 30, size * 0.90);
    spiralOfSpirals(x2, y2, angle + 30, size * 0.30);
}
