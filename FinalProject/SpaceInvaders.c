// SpaceInvaders.c
// Dannylo Correia
//
// This is my Space Invaders game for my Fundamentals of Computing final project.

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "gfx.h"

// Window + game constants
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

// Player rectangle (simple hitbox-based player)
typedef struct {
    int x, y;
    int width, height;
} Player;

// Basic invader structure — each invader is just a square w/ alive flag
typedef struct {
    int x, y;
    int alive;
} Invader;

// Player bullet structure
typedef struct {
    int x, y;
    int width, height;
    int active;  // 1 if bullet is on screen, 0 if not
} Bullet;

// Enemy bullet structure (same idea as player bullet)
typedef struct {
    int x, y;
    int width, height;
    int active;
} EnemyBullet;

// ----------------------------
// INITIALIZATION FUNCTIONS
// ----------------------------

// Create player at bottom center of screen
Player initPlayer() {
    Player p = {WIDTH/2, HEIGHT - 60, 25, 40};
    return p;
}

// Create a blank bullet (inactive)
Bullet initBullet() {
    Bullet b = {0, 0, 4, 10, 0};
    return b;
}

// Create a blank enemy bullet (inactive)
EnemyBullet initEnemyBullet() {
    EnemyBullet b = {0, 0, 4, 10, 0};
    return b;
}

// Create all invaders and place them in a grid
void initInvaders(Invader inv[INVADER_ROWS][INVADER_COLS]) {
    for (int r = 0; r < INVADER_ROWS; r++) {
        for (int c = 0; c < INVADER_COLS; c++) {
            // Spread them out evenly
            inv[r][c] = (Invader){70 + c*90, 50 + r*75, 1};
        }
    }
}

// ----------------------------
// DRAWING HELPERS
// ----------------------------

// My own rectangle fill since gfx doesn't have one
void gfx_fill_rect(int x, int y, int w, int h) {
    for (int i = 0; i < h; i++) {
        gfx_line(x, y+i, x+w, y+i);
    }
}

// Draw the player (green rectangle)
void drawPlayer(Player p) {
    gfx_color(0, 255, 0);
    gfx_fill_rect(p.x, p.y, p.width, p.height);
}

// Draw all invaders (red squares)
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

// Draw player's bullet
void drawBullet(Bullet b) {
    if (!b.active) return;
    gfx_color(255, 255, 0); // Yellow bullet
    gfx_fill_rect(b.x, b.y, b.width, b.height);
}

// Draw enemy bullet
void drawEnemyBullet(EnemyBullet b) {
    if (!b.active) return;
    gfx_color(255, 0, 0); // Red bullet
    gfx_fill_rect(b.x, b.y, b.width, b.height);
}

// ----------------------------
// COLLISION SYSTEM
// ----------------------------

// Basic axis-aligned rectangle collision detection
int rectCollide(int x1, int y1, int w1, int h1,
                int x2, int y2, int w2, int h2)
{
    return !(x1+w1 < x2 || x1 > x2+w2 ||
             y1+h1 < y2 || y1 > y2+h2);
}

// When a player bullet hits an invader
void handleBulletCollision(Bullet *b, Invader inv[INVADER_ROWS][INVADER_COLS]) {
    if (!b->active) return;

    // Check bullet against every invader
    for (int r = 0; r < INVADER_ROWS; r++) {
        for (int c = 0; c < INVADER_COLS; c++) {

            if (!inv[r][c].alive) continue;

            if (rectCollide(b->x, b->y, b->width, b->height,
                            inv[r][c].x, inv[r][c].y, 50, 50))
            {
                inv[r][c].alive = 0; // kill invader
                b->active = 0;       // delete bullet
                return;
            }
        }
    }
}

// Check if enemy bullet hit the player
int checkPlayerHit(Player *p, EnemyBullet *b) {
    if (!b->active) return 0;

    if (rectCollide(b->x, b->y, b->width, b->height,
                    p->x, p->y, p->width, p->height))
    {
        b->active = 0; // bullet disappears
        return 1;      // player was hit
    }
    return 0;
}

// ----------------------------
// SHOOTING + MOVEMENT UPDATES
// ----------------------------

// Player shooting
void shoot(Bullet *b, Player *p) {
    b->active = 1;
    b->x = p->x + p->width/2;
    b->y = p->y;
}

// Enemy shooting (same idea)
void enemyShoot(EnemyBullet *b, Invader *inv) {
    b->active = 1;
    b->x = inv->x + 25;
    b->y = inv->y + 50;
}

// Move player bullets upward
void updateBullet(Bullet *b) {
    if (!b->active) return;
    b->y -= 10;
    if (b->y < 0) b->active = 0;
}

// Move enemy bullets downward
void updateEnemyBullet(EnemyBullet *b) {
    if (!b->active) return;
    b->y += 7;
    if (b->y > HEIGHT) b->active = 0;
}

// ----------------------------
// DEATH / WIN SCREEN
// ----------------------------

// Simple blocking “Game Over” screen
void showDeathScreen() {
    gfx_clear();

    gfx_color(255, 0, 0);
    gfx_text(WIDTH/2 - 50, HEIGHT/2 - 20, "Game Over");

    gfx_color(255, 255, 255);
    gfx_text(WIDTH/2 - 70, HEIGHT/2 + 20, "Press R to Restart");
    gfx_text(WIDTH/2 - 63, HEIGHT/2 + 40, "Press Q to Quit");

    gfx_flush();

    // Wait for R or Q
    while (1) {
        if (gfx_event_waiting()) {
            char key = gfx_wait();

            if (key == 'q' || key == 'Q')
                exit(0);

            if (key == 'r' || key == 'R')
                return; // resume game loop
        }
        usleep(16000);
    }
}

// Check if all invaders are dead → win condition
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
    // Create player + all bullet arrays
    Player player = initPlayer();
    Bullet bullets[MAX_BULLET];
    EnemyBullet enemyBullets[MAX_ENEMY_BULLETS];

    // Initialize bullet arrays
    for (int i = 0; i < MAX_BULLET; i++) bullets[i] = initBullet();
    for (int i = 0; i < MAX_ENEMY_BULLETS; i++) enemyBullets[i] = initEnemyBullet();

    // Create all invaders
    Invader invaders[INVADER_ROWS][INVADER_COLS];
    initInvaders(invaders);

    printf("Click 'a' to move left, 'd' to move right and space bar to fight back\n");

    // Open window
    gfx_open(WIDTH, HEIGHT, "Space Invaders");

    int Running = 1;
    int keyLeft = 0, keyRight = 0;


    while (Running) {

        //
        // DRAW EVERYTHING
        //
        gfx_clear();

        drawPlayer(player);
        drawInvaders(invaders);

        for (int i = 0; i < MAX_BULLET; i++) drawBullet(bullets[i]);
        for (int i = 0; i < MAX_ENEMY_BULLETS; i++) drawEnemyBullet(enemyBullets[i]);

        gfx_flush();

        //
        // INPUT HANDLING
        //
        while (gfx_event_waiting()) {
            char c = gfx_wait();

            if (c == 'q') Running = 0;

            // Movement keys (hold-down logic)
            if (c == 'a') { keyLeft = 1; keyRight = 0; }
            else if (c == 'd') { keyRight = 1; keyLeft = 0; }

            // Space = shoot (find first free bullet slot)
            else if (c == ' ') {
                for (int i = 0; i < MAX_BULLET; i++)
                    if (!bullets[i].active) {
                        shoot(&bullets[i], &player);
                        break;
                    }
            }

            // Key-release events
            else if ((unsigned char)c > 127) {
                char r = c - 128;
                if (r == 'a') keyLeft = 0;
                if (r == 'd') keyRight = 0;
            }
        }

        //
        // MOVE PLAYER
        //
        if (keyLeft && player.x > 10)
            player.x -= PLAYER_SPEED;

        if (keyRight && player.x + player.width + 15 < WIDTH)
            player.x += PLAYER_SPEED;

        //
        // UPDATE PLAYER BULLETS
        //
        for (int i = 0; i < MAX_BULLET; i++) {
            if (bullets[i].active) {
                updateBullet(&bullets[i]);
                handleBulletCollision(&bullets[i], invaders);
            }
        }

        //
        // RANDOM ENEMY SHOOTING
        //
        for (int r = 0; r < INVADER_ROWS; r++) {
            for (int c = 0; c < INVADER_COLS; c++) {

                if (!invaders[r][c].alive) continue;

                // Random chance to shoot
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

        //
        // UPDATE ENEMY BULLETS (and check collision with player)
        //
        for (int i = 0; i < MAX_ENEMY_BULLETS; i++) {
            if (enemyBullets[i].active) {

                updateEnemyBullet(&enemyBullets[i]);

                if (checkPlayerHit(&player, &enemyBullets[i])) {

                    // PLAYER DIED → restart everything
                    showDeathScreen();

                    player = initPlayer();
                    for (int i = 0; i < MAX_BULLET; i++) bullets[i] = initBullet();
                    for (int i = 0; i < MAX_ENEMY_BULLETS; i++) enemyBullets[i] = initEnemyBullet();
                    initInvaders(invaders);

                    break;
                }
            }
        }

        //
        // WIN CONDITION
        //
        if (allInvadersDead(invaders)) {

            showDeathScreen();

            // Reset entire game
            player = initPlayer();
            for (int i = 0; i < MAX_BULLET; i++) bullets[i] = initBullet();
            for (int i = 0; i < MAX_ENEMY_BULLETS; i++) enemyBullets[i] = initEnemyBullet();
            initInvaders(invaders);
        }

        usleep(16000); // 60 FPS timing
    }

    return 0;
}
