// SpaceInvaders.c
// Dannylo Correia

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "gfx.h"

#define WIDTH 800
#define HEIGHT 900
#define INVADER_ROWS 4
#define INVADER_COLS 8
#define MAX_BULLET 10
#define MAX_ENEMY_BULLETS 8
#define PLAYER_SPEED 10

// ----------------------------
// STRUCTS
// ----------------------------
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
    int width, height;
    int active;
} Bullet;

typedef struct {
    int x, y;
    int width, height;
    int active;
} EnemyBullet;

// ----------------------------
// INITIALIZATION
// ----------------------------
Player initPlayer() {
    Player p = {WIDTH/2, HEIGHT - 60, 25, 40};
    return p;
}

Bullet initBullet() {
    Bullet b = {0, 0, 4, 10, 0};
    return b;
}

EnemyBullet initEnemyBullet() {
    EnemyBullet b = {0, 0, 4, 10, 0};
    return b;
}

void initInvaders(Invader inv[INVADER_ROWS][INVADER_COLS]) {
    for (int r = 0; r < INVADER_ROWS; r++) {
        for (int c = 0; c < INVADER_COLS; c++) {
            inv[r][c] = (Invader){70 + c*90, 50 + r*75, 1};
        }
    }
}

// ----------------------------
// DRAW HELPERS
// ----------------------------
void gfx_fill_rect(int x, int y, int w, int h) {
    for (int i = 0; i < h; i++) {
        gfx_line(x, y+i, x+w, y+i);
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
            if (inv[r][c].alive){
                gfx_fill_rect(inv[r][c].x, inv[r][c].y, 50, 50);
            }
        }
    }
}

void drawBullet(Bullet b) {
    if (!b.active) return;
    gfx_color(255, 255, 0);
    gfx_fill_rect(b.x, b.y, b.width, b.height);
}

void drawEnemyBullet(EnemyBullet b) {
    if (!b.active) return;
    gfx_color(255, 0, 0);
    gfx_fill_rect(b.x, b.y, b.width, b.height);
}

// ----------------------------
// COLLISION HELPERS
// ----------------------------
int rectCollide(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2) {
    return !(x1+w1 < x2 || x1 > x2+w2 || y1+h1 < y2 || y1 > y2+h2);
}

void handleBulletCollision(Bullet *b, Invader inv[INVADER_ROWS][INVADER_COLS]) {
    if (!b->active) return;

    for (int r = 0; r < INVADER_ROWS; r++) {
        for (int c = 0; c < INVADER_COLS; c++) {

            if (!inv[r][c].alive) continue;

            if (rectCollide(b->x, b->y, b->width, b->height,
                            inv[r][c].x, inv[r][c].y, 50, 50))
            {
                inv[r][c].alive = 0;
                b->active = 0;
                return;
            }
        }
    }
}

int checkPlayerHit(Player *p, EnemyBullet *b) {
    if (!b->active) return 0;

    if (rectCollide(b->x, b->y, b->width, b->height,
                    p->x, p->y, p->width, p->height))
    {
        b->active = 0;
        return 1;
    }
    return 0;
}

// ----------------------------
// SHOOTING & UPDATES
// ----------------------------
void shoot(Bullet *b, Player *p) {
    b->active = 1;
    b->x = p->x + p->width/2;
    b->y = p->y;
}

void enemyShoot(EnemyBullet *b, Invader *inv) {
    b->active = 1;
    b->x = inv->x + 25;
    b->y = inv->y + 50;
}

void updateBullet(Bullet *b) {
    if (!b->active) return;
    b->y -= 10;
    if (b->y < 0) b->active = 0;
}

void updateEnemyBullet(EnemyBullet *b) {
    if (!b->active) return;
    b->y += 7;
    if (b->y > HEIGHT) b->active = 0;
}

// ----------------------------
// DEATH / WIN SCREEN
// ----------------------------
void showDeathScreen() {
    gfx_clear();

    gfx_color(255, 0, 0);
    gfx_text(WIDTH/2 - 50, HEIGHT/2 - 20, "Game Over");

    gfx_color(255, 255, 255);
    gfx_text(WIDTH/2 - 70, HEIGHT/2 + 20, "Press R to Restart");
    gfx_text(WIDTH/2 - 63, HEIGHT/2 + 40, "Press Q to Quit");

    gfx_flush();

    while (1) {
        if (gfx_event_waiting()) {
            char key = gfx_wait();

            if (key == 'q' || key == 'Q')
                exit(0);

            if (key == 'r' || key == 'R')
                return;
        }
        usleep(16000);
    }
}

int allInvadersDead(Invader inv[INVADER_ROWS][INVADER_COLS]) {
    for (int r = 0; r < INVADER_ROWS; r++) {
        for (int c = 0; c < INVADER_COLS; c++) {
            if (inv[r][c].alive) return 0;
        }
    }
    return 1;
}

// ----------------------------
// MAIN GAME LOOP
// ----------------------------
int main() {
    Player player = initPlayer();
    Bullet bullets[MAX_BULLET];
    EnemyBullet enemyBullets[MAX_ENEMY_BULLETS];

    for (int i = 0; i < MAX_BULLET; i++) bullets[i] = initBullet();
    for (int i = 0; i < MAX_ENEMY_BULLETS; i++) enemyBullets[i] = initEnemyBullet();

    Invader invaders[INVADER_ROWS][INVADER_COLS];
    initInvaders(invaders);

    gfx_open(WIDTH, HEIGHT, "Space Invaders");

    int Running = 1;
    int keyLeft = 0, keyRight = 0;

    while (Running) {

        // ---- DRAW EVERYTHING ----
        gfx_clear();

        drawPlayer(player);
        drawInvaders(invaders);

        for (int i = 0; i < MAX_BULLET; i++) drawBullet(bullets[i]);
        for (int i = 0; i < MAX_ENEMY_BULLETS; i++) drawEnemyBullet(enemyBullets[i]);

        gfx_flush();

        // ---- INPUT ----
        while (gfx_event_waiting()) {
            char c = gfx_wait();

            if (c == 'q') Running = 0;

            if (c == 'a') { keyLeft = 1; keyRight = 0; }
            else if (c == 'd') { keyRight = 1; keyLeft = 0; }

            else if (c == ' ') {
                for (int i = 0; i < MAX_BULLET; i++)
                    if (!bullets[i].active) { shoot(&bullets[i], &player); break; }
            }

            else if ((unsigned char)c > 127) {
                char r = c - 128;
                if (r == 'a') keyLeft = 0;
                if (r == 'd') keyRight = 0;
            }
        }

        // ---- MOVE PLAYER ----
        if (keyLeft && player.x > 10)
            player.x -= PLAYER_SPEED;

        if (keyRight && player.x + player.width + 15 < WIDTH)
            player.x += PLAYER_SPEED;

        // ---- UPDATE PLAYER BULLETS ----
        for (int i = 0; i < MAX_BULLET; i++) {
            if (bullets[i].active) {
                updateBullet(&bullets[i]);
                handleBulletCollision(&bullets[i], invaders);
            }
        }

        // ---- ENEMY SHOOTING ----
        for (int r = 0; r < INVADER_ROWS; r++) {
            for (int c = 0; c < INVADER_COLS; c++) {
                if (!invaders[r][c].alive) continue;

                if (rand() % 500 == 0) {
                    for (int i = 0; i < MAX_ENEMY_BULLETS; i++) {
                        if (!enemyBullets[i].active) {
                            enemyShoot(&enemyBullets[i], &invaders[r][c]);
                            break;
                        }
                    }
                }
            }
        }

        // ---- UPDATE ENEMY BULLETS ----
        for (int i = 0; i < MAX_ENEMY_BULLETS; i++) {
            if (enemyBullets[i].active) {
                updateEnemyBullet(&enemyBullets[i]);

                if (checkPlayerHit(&player, &enemyBullets[i])) {

                    showDeathScreen();

                    player = initPlayer();
                    for (int i = 0; i < MAX_BULLET; i++) bullets[i] = initBullet();
                    for (int i = 0; i < MAX_ENEMY_BULLETS; i++) enemyBullets[i] = initEnemyBullet();
                    initInvaders(invaders);

                    break;
                }
            }
        }

        // ---- WIN CONDITION ----
        if (allInvadersDead(invaders)) {

            showDeathScreen();

            player = initPlayer();
            for (int i = 0; i < MAX_BULLET; i++) bullets[i] = initBullet();
            for (int i = 0; i < MAX_ENEMY_BULLETS; i++) enemyBullets[i] = initEnemyBullet();
            initInvaders(invaders);
        }

        usleep(16000);
    }

    return 0;
}
