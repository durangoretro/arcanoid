#include "dlib.h"
#define BRICKS_PER_ROW 8
#define BRICKS_ROWS 2

/* Procedure definitions */
void initGame(void);
void updateGame(void);
void updateBall(void);
void updatePlayer(void);
void initBall(void);
void initPlayer(void);
void initBricks(void);
void resetBall(void);
int main(void);

/* Game Data */
rectangle player;
ball myball;
rectangle bricks[BRICKS_PER_ROW*BRICKS_ROWS];

/* Game Procedures */

void initGame() {
    consoleLogStr("Init Game\n");
    startStopwatch();
    
    // Draw background
    fillScreen(CIAN);
    
    // Init and Draw bricks
    initBricks();
    
    // Init and Draw player
    initPlayer();
    
    // Init and draw ball
    initBall();
    
    stopStopwatch();
}

void initBall() {
	myball.color = GREEN;
    resetBall();
    drawBall(&myball); 
}

void resetBall() {
    myball.x = 62;
    myball.y = 60;
    myball.vx=2;
    myball.vy=-4;
}

void initPlayer() {
	player.x = 48;
    player.y = 122;
    player.width = 32;
    player.height = 4;
	player.color = RED;
    drawRect(&player);
}

void initBricks() {
	byte i, j, lastx, lasty;
	lasty = 0;
    for(j=0; j<BRICKS_ROWS; j++) {
        lastx = 0;    
        for(i=0; i<BRICKS_PER_ROW; i++) {
            bricks[i].x = lastx;
            lastx = lastx + 16;
            bricks[i].y = lasty;
            bricks[i].width = 14;
            bricks[i].height = 4;
            bricks[i].color = MYSTIC_RED;
            drawRect(&bricks[i]);
        }
        lasty = lasty + 6;
        consoleLogStr("Lastx\n");
        consoleLogDecimal(lastx);
    }
}

void updateGame() {
    //updateBall();
    updatePlayer();
}

void updatePlayer() {
    byte gamepad;
    
    // Read gamepad
    gamepad=readGamepad(0);
    // Move left
    if(gamepad & BUTTON_LEFT && player.x>0) {
        moveLeft(&player);
    }
    // MOve right
    else if(gamepad & BUTTON_RIGHT && player.x+player.width<128) {
        moveRight(&player);
    }
}

void updateBall() {
    byte old_vx;
    
    // Top collision
    if(myball.y==0) {
        myball.vy = 4;
    }
    // Bottom collision
    if(myball.y==120) {
        // Paddle collision
        if(myball.x >= player.x && myball.x<=player.x+player.width) {
            myball.vy = -4;
        }
        // Fail
        else {
            old_vx = myball.vx;
            myball.vx = 0;
            myball.vy = 0;
            waitStart();
            initBall();
            moveBall(&myball);
        }
    }
    // Right collision
    if(myball.x==128) {
        myball.vx = -2;
    }
    // Left collision
    if(myball.x==0) {
        myball.vx = 2;
    }
    
    // Update ball position
    myball.x = myball.x+myball.vx;
    myball.y = myball.y+myball.vy;
    // Move ball in screen
    moveBall(&myball);
}

int main(void){
    // Init game
    initGame();
    
    consoleLogStr("Game loop\n");
    consoleLogHex(0xff);
    // Game loop
    while(1) {
        //waitFrames(20);
        
        // Wait VSYNC
        waitVSync();                
        // Start counting time
        startStopwatch();
        
        // Update game
        updateGame();
        
        // Stop counting time
        stopStopwatch();        
    }
    
    return 0;
}
