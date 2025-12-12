// SpaceInvaders.c
// Name: Dannylo Correia
// Mini Final Project - Space Invaders

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "gfx.h"

// -----------------------------
// CONSTANTS
// -----------------------------
#define WIDTH 800
#define HEIGHT 900
#define INVADER_ROWS 4
#define INVADER_COLS 8

// -----------------------------
// STRUCTS
// -----------------------------
typedef struct {
    int x, y;
    int width, height;
} Player;

typedef struct {
    int x, y;
    int alive;
} Invader;

typedef struct {
    int x, y;
    int width;
    int height;
    int active;
} Bullet;

typedef struct {
    int leftHeld;
    int rightHeld;
    int shootPressed;
} Input;

// -----------------------------
// INITIALIZATION
// -----------------------------
Player initPlayer() {
    Player p;
    p.x = WIDTH / 2;
    p.y = HEIGHT - 60;
    p.width = 25;
    p.height = 40;
    return p;
}

Bullet initBullet() {
    Bullet b;
    b.x = 0;
    b.y = 0;
    b.active = 0;
    b.width = 10;
    b.height = 5;
    return b;
}

Input initInput() {
    Input in;
    in.leftHeld = 0;
    in.rightHeld = 0;
    in.shootPressed = 0;
    return in;
}

void initInvaders(Invader inv[INVADER_ROWS][INVADER_COLS]) {
    for (int r = 0; r < INVADER_ROWS; r++) {
        for (int c = 0; c < INVADER_COLS; c++) {
            inv[r][c].x = 70 + c * 90;
            inv[r][c].y = 50 + r * 75;
            inv[r][c].alive = 1;
        }
    }
}

// -----------------------------
// DRAWING HELPERS
// -----------------------------
void gfx_fill_rect(int x, int y, int w, int h) {
    for (int i = 0; i < h; i++) {
        gfx_line(x, y + i, x + w, y + i);
    }
}

void drawPlayer(Player p) {
    gfx_color(0, 255, 0);
    gfx_fill_rect(p.x, p.y, p.width, p.height);
}

void drawInvaders(Invader inv[INVADER_ROWS][INVADER_COLS]) {
    gfx_color(255, 0, 0);
    for (int r = 0; r < INVADER_ROWS; r++) {
        for (int c = 0; c < INVADER_COLS; c++) {
            if (inv[r][c].alive) {
                gfx_fill_rect(inv[r][c].x, inv[r][c].y, 50, 50);
            }
        }
    }
}

void drawBullet(Bullet b) {
    if (!b.active) return;
    gfx_color(255, 255, 0);
    gfx_fill_rect(b.x, b.y, 4, 10);
}

// -----------------------------
// GAME LOGIC
// -----------------------------
void shoot(Bullet *b, Player *p) {
    b->active = 1;
    b->x = p->x + p->width / 2;
    b->y = p->y;
}

void updateBullet(Bullet *b) {
    if (!b->active) return;

    b->y -= 10;
    if (b->y < 0)
        b->active = 0;
}

// -----------------------------
// MAIN GAME LOOP
// -----------------------------
int main() {
    Player player = initPlayer();
    Input in = initInput();
    Bullet bullets[3];
    for (int i = 0; i < 3; i++)
        bullets[i] = initBullet();

    Invader invaders[INVADER_ROWS][INVADER_COLS];
    initInvaders(invaders);

    gfx_open(WIDTH, HEIGHT, "Space Invaders");

    int Running = 1;

    while (Running) {
        gfx_clear();

        drawPlayer(player);
        drawInvaders(invaders);
        for (int i = 0; i < 3; i++)
            drawBullet(bullets[i]);

        gfx_flush();
        usleep(16000); // 60 FPS


        in.leftHeld = 0;
        in.rightHeld = 0;


        while (gfx_event_waiting()) {
            char key = gfx_wait();

            if (key == 'q') Running = 0;
            if (key == 'a') in.leftHeld = 1;
            if (key == 'd') in.rightHeld = 1;
            if (key == ' ') in.shootPressed = 1;
        }


        if (in.leftHeld && player.x > 10)
            player.x -= 10;

        if (in.rightHeld && player.x + player.width + 15 < WIDTH)
            player.x += 10;


        if (in.shootPressed) {
            for (int i = 0; i < 3; i++) {
                if (!bullets[i].active) {
                    shoot(&bullets[i], &player);
                    break;
                }
            }
            in.shootPressed = 0;
        }

        for (int i = 0; i < 3; i++)
            updateBullet(&bullets[i]);
    }

    return 0;
}