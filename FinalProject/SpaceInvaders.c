// SpaceInvaders.c
// Name: Dannylo Correia
// Mini Final Project - Space Invaders

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "gfx.h"

#define WIDTH 800
#define HEIGHT 900
#define INVADER_ROWS 4
#define INVADER_COLS 8
#define MAX_BULLET 10
#define PLAYER_SPEED 10

typedef struct {
    int x, y;
    int width, height;
} Player;

typedef struct {
    int x, y, alive;
} Invader;

typedef struct {
    int x, y;
    int width, height;
    int active;
} Bullet;

Player initPlayer() {
    Player p;
    p.x = WIDTH / 2;
    p.y = HEIGHT - 60;
    p.width = 25;
    p.height = 40;
    return p;
}

Bullet initBullet() {
    Bullet b = {0, 0, 10, 5, 0};
    return b;
}

void initInvaders(Invader inv[INVADER_ROWS][INVADER_COLS]) {
    for (int r = 0; r < INVADER_ROWS; r++)
        for (int c = 0; c < INVADER_COLS; c++)
            inv[r][c] = (Invader){70 + c*90, 50 + r*75, 1};
}

void gfx_fill_rect(int x, int y, int w, int h) {
    for (int i = 0; i < h; i++)
        gfx_line(x, y + i, x + w, y + i);
}

void drawPlayer(Player p) {
    gfx_color(0,255,0);
    gfx_fill_rect(p.x, p.y, p.width, p.height);
}

void drawInvaders(Invader inv[INVADER_ROWS][INVADER_COLS]) {
    gfx_color(255,0,0);
    for (int r = 0; r < INVADER_ROWS; r++)
        for (int c = 0; c < INVADER_COLS; c++)
            if (inv[r][c].alive)
                gfx_fill_rect(inv[r][c].x, inv[r][c].y, 50, 50);
}

void drawBullet(Bullet b) {
    if (!b.active) return;
    gfx_color(255,255,0);
    gfx_fill_rect(b.x, b.y, 4, 10);
}


int rectCollide(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2) {
    return !( x1 + w1 < x2 || x1 > x2 + w2 || y1 + h1 < y2 || y1 > y2 + h2 );
}

void handleBulletCollision(Bullet *b, Invader inv[INVADER_ROWS][INVADER_COLS]) {

    if (!b->active) return;

    for (int r = 0; r < INVADER_ROWS; r++) {
        for (int c = 0; c < INVADER_COLS; c++) {

            if (!inv[r][c].alive) continue;

            if (rectCollide(
                    b->x, b->y, b->width, b->height,
                    inv[r][c].x, inv[r][c].y, 50, 50)) {

                inv[r][c].alive = 0;
                b->active = 0;
                return;
            }
        }
    }
}




void shoot(Bullet *b, Player *p) {
    b->active = 1;
    b->x = p->x + p->width/2 - 2;
    b->y = p->y;
}

void updateBullet(Bullet *b) {
    if (!b->active) return;
    b->y -= 10;
    if (b->y < 0) b->active = 0;
}

int main() {
    Player player = initPlayer();
    Bullet bullets[MAX_BULLET];
    for (int i = 0; i < MAX_BULLET; i++)
        bullets[i] = initBullet();

    Invader invaders[INVADER_ROWS][INVADER_COLS];
    initInvaders(invaders);

    gfx_open(WIDTH, HEIGHT, "Space Invaders");

    int Running = 1;
    int keyLeft = 0;
    int keyRight = 0;
    int lastSpaceState = 0;

    while (Running) {
        gfx_clear();

        drawPlayer(player);
        drawInvaders(invaders);
        for (int i = 0; i < MAX_BULLET; i++)
            drawBullet(bullets[i]);

        gfx_flush();


        while (gfx_event_waiting()) {
            char c = gfx_wait();
                        
            if (c == 'q') {
                Running = 0;
            }
            
            if (c == 'a') {
                keyLeft = 1;
                keyRight = 0;  // Stop other direction
            }
            else if (c == 'd') {
                keyRight = 1;
                keyLeft = 0;
            }
            else if (c == ' ') {
                for (int i = 0; i < MAX_BULLET; i++) {
                    if (!bullets[i].active) {
                        shoot(&bullets[i], &player);
                        break;
                    }
                }
            }
            else if ((unsigned char)c > 127) {
                char released = c - 128;
                if (released == 'a') keyLeft = 0;
                if (released == 'd') keyRight = 0;
            }
        }

        if (keyLeft && player.x > 10) {
            player.x -= PLAYER_SPEED;
        }
        if (keyRight && player.x + player.width + 15 < WIDTH) {
            player.x += PLAYER_SPEED;
        }

        for (int i = 0; i < MAX_BULLET; i++) {
            if (bullets[i].active) {
                updateBullet(&bullets[i]);
                handleBulletCollision(&bullets[i], invaders);
            }
        }

        usleep(16000);  // ~60 FPS
    }

    return 0;
}