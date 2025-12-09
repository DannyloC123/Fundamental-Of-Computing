// fractals.c
// Name: Dannylo Correia


// Libraries
#include "gfx.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>


// Prototypes
void sierpinski( int x1, int y1, int x2, int y2, int x3, int y3 );    // Class Code
void drawTriangle( int x1, int y1, int x2, int y2, int x3, int y3 );  // Class Code
void drawSquare(int cx, int cy, int side);
void shrinkingSquares(int cx, int cy, int side);
void spiralSquares(double center_x, double center_y, double radius, double angle_deg, double size);
void circularLace(int cx, int cy, int radius);
void snowflake(double x, double y, double radius, int level);
void tree(int x1, int y1, double angle, double length);
void fern(int x1, int y1, double angle, double length);
void spiralOfSpirals(int x, int y, double angle, double size);


int main()
{
    int width = 800, height = 800, mrgn = 20;

    gfx_open(width, height, "Fractals");

    while (1) {

        gfx_clear();               // Clear the window for the next drawing
        int event = gfx_wait();    // Wait for a key press each loop

        if (event == '1') {
            // Classic Sierpinski triangle
            sierpinski(mrgn, mrgn, width-mrgn, mrgn, width/2, height-mrgn);
        }

        if (event == '2') {
            // Centered shrinking squares
            shrinkingSquares(width/2, height/2, 300);
        }

        if (event == '3') {
            // Spiral made out of squares
            spiralSquares(width/2, height/2, 300, 0, 120);
        }

        if (event == '4') {
            // Circular lace pattern
            circularLace(width/2, height/2, 200);
        }

        if (event == '5') {
            // Snowflake fractal (circle-based)
            snowflake(width/2, height/2, 150, 4);
        }

        if (event == '6') {
            // Symmetrical branching tree
            tree(width/2, height - 60, M_PI / 2, 240);
        }

        if (event == '7') {
            // Fern fractal with side fronds
            fern(width/2, height - 50, M_PI/2, 180);
        }

        if (event == '8') {
            // Spiral made out of tiny spirals
            spiralOfSpirals(width/2, height/2, 0, 80);
        }

        // Quit when 'q' is pressed
        if (event == 'q') break;
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
    // Compute half the side length so we can draw around the center
    int h = side / 2;

    // Draw four edges of the square
    gfx_line(cx - h, cy - h,  cx + h, cy - h);
    gfx_line(cx + h, cy - h,  cx + h, cy + h);
    gfx_line(cx + h, cy + h,  cx - h, cy + h);
    gfx_line(cx - h, cy + h,  cx - h, cy - h);
}

void shrinkingSquares(int cx, int cy, int side) {
    // Stop when the square becomes tiny
    if (side < 5) return;

    // Draw the current square
    drawSquare(cx, cy, side);

    // The next squares will be half the size
    int newSide = side / 2;
    int half = side / 2;

    // Recursively draw squares at all 4 corners
    shrinkingSquares(cx - half, cy - half, newSide); // top-left
    shrinkingSquares(cx + half, cy - half, newSide); // top-right
    shrinkingSquares(cx - half, cy + half, newSide); // bottom-left
    shrinkingSquares(cx + half, cy + half, newSide); // bottom-right
}

void spiralSquares(double center_x, double center_y, double radius, double angle_deg, double size)
{
    // Stop recursion when squares are too small
    if (size < 2) return;

    // Convert angle to radians for cosine/sine
    double angle_rad = angle_deg * M_PI / 180.0;

    // Compute the next square's center along a circular path
    double cx = center_x + radius * cos(angle_rad);
    double cy = center_y + radius * sin(angle_rad);

    // Draw the square at this new position
    drawSquare((int)cx, (int)cy, (int)size);

    // Move inward (smaller radius), rotate, and shrink the square
    spiralSquares(center_x, center_y, radius * 0.9, angle_deg + 40.0, size * 0.9);
}

void circularLace(int cx, int cy, int radius)
{
    // Stop when circles are tiny
    if (radius < 5) return;

    // Draw the current circle
    gfx_circle(cx, cy, radius);

    // Create six evenly spaced new circles around the edge
    for (int i = 0; i < 6; i++) {

        double angle = i * M_PI / 3.0; // 60° apart

        // Compute the next center location
        int newX = cx + radius * cos(angle);
        int newY = cy + radius * sin(angle);

        // Recursive lace pattern
        circularLace(newX, newY, radius / 3);
    }
}

void snowflake(double x, double y, double radius, int level)
{
    // Base case: draw a final circle
    if (level == 0) {
        gfx_circle(x, y, radius);
        return;
    }

    // Create six branches around the center
    for (int i = 0; i < 6; i++) {

        double angle = i * M_PI / 3.0; // 60° increments

        // Make each branch longer than its radius for a snowflake look
        double branch = radius * 1.5;

        // Compute endpoint of the branch
        double newX = x + cos(angle) * branch;
        double newY = y + sin(angle) * branch;

        // Draw main branch
        gfx_line(x, y, newX, newY);

        // Recur at end of branch
        snowflake(newX, newY, radius / 3.0, level - 1);
    }
}

void tree(int x1, int y1, double angle, double length)
{
    // Stop when branch is too small to see
    if (length < 4) return;

    // Compute endpoint of this branch
    double x2 = x1 + length * cos(angle);
    double y2 = y1 - length * sin(angle); // minus = upward

    // Draw branch
    gfx_line(x1, y1, x2, y2);

    // Shrink the branches for next recursion
    double newLength = length * 0.72;

    // Angle offset for symmetry (30°)
    double delta = M_PI / 6;

    // Left and right branches
    tree(x2, y2, angle + delta, newLength);
    tree(x2, y2, angle - delta, newLength);
}

void fern(int x1, int y1, double angle, double length)
{
    // Stop when too small
    if (length < 2) return;

    // Compute new endpoint
    int x2 = x1 + (int)(length * cos(angle));
    int y2 = y1 - (int)(length * sin(angle));

    // Draw main stem segment
    gfx_line(x1, y1, x2, y2);

    // How much each branch shrinks
    double mainShrink = 0.85; // main stem
    double sideShrink = 0.40; // side fronds

    double sideAngle = M_PI / 6; // 30° outward bends

    // Continue up the main fern stalk
    fern(x2, y2, angle, length * mainShrink);

    // Left frond
    fern(x2, y2, angle + sideAngle, length * sideShrink);

    // Right frond
    fern(x2, y2, angle - sideAngle, length * sideShrink);
}

void spiralOfSpirals(int x, int y, double angle, double size)
{
    // End when points are nearly invisible
    if (size < 2) return;

    // Convert angle to radians
    double rad = angle * M_PI / 180.0;

    // Move forward by an amount equal to the size
    double x2 = x + cos(rad) * size;
    double y2 = y + sin(rad) * size;

    // Draw a single dot at the new location
    gfx_point(x2, y2);

    // Main spiral path (slightly shrinking)
    spiralOfSpirals(x2, y2, angle + 30, size * 0.90);

    // Mini-spiral growing off each point
    spiralOfSpirals(x2, y2, angle + 30, size * 0.30);
}