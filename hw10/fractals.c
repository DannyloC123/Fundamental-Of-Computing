// fractals.c
// Name: Dannylo Correia


#include "gfx.h"
#include <stdio.h>
#include <stdlib.h>


void sierpinski( int x1, int y1, int x2, int y2, int x3, int y3 );    // Class Code
void drawTriangle( int x1, int y1, int x2, int y2, int x3, int y3 );  // Class Code
void drawSquare(int cx, int cy, int side);
void shrinkingSquares(int cx, int cy, int side);
//void spiralSquare();

int main()
{
  int width = 800, height = 800, mrgn = 20;

  gfx_open(width, height, "Sierpinski's Triangle");

  int event = gfx_wait();

  while(1) {
    gfx_clear();

    if (event == 's') {
      sierpinski(mrgn, mrgn, width-mrgn, mrgn, width/2, height-mrgn);
    }

    if (event == 't') {
      shrinkingSquares(width/2, height/2, 300);
    }

    if (event == '3') {
      //spiralSquares();
      printf("Spiral Squares");
    }

    if (event == '4') {
      printf("Circular Lace");
    }

    if (event == '5') {
      printf("Snowflake");
    }

    if (event == '6') {
      printf("Tree");
    }

    if (event == '7') {
      printf("Fern");
    }

    if (event == '8') {
      printf("Spiral of Spirals");
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



