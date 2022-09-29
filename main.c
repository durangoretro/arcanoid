#include "dlib.h"

/* Procedure definitions */
void initGame(void);
void updateGame(void);
void updateBall(void);
void updatePlayer(void);
void initBall(void);
void initPlayer(void);
void initBricks(void);
void initScore(void);
void resetBall(void);
void initDrawEvenRow(byte y, byte index);
void initDrawOddRow(byte y, byte index);
int main(void);

/* Game Data */
rectangle player;
ball myball;
rectangle bricks[34];

/* Game Procedures */

void initGame() {
    consoleLogStr("Init Game\n");
    startStopwatch();
    
    // Draw background
    fillScreen(CIAN);

	// Init and Draw score
	initScore();

    // Init and Draw bricks
    initBricks();
halt();
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

void initDrawEvenRow(byte y, byte index) {
    byte lastx, k, i;
    lastx = 0;
    k = index;
    for(i=0; i<8; i++) {
        bricks[k].x = lastx;
        lastx = lastx + 16;
        bricks[k].y = y;
        bricks[k].width = 14;
        bricks[k].height = 4;
        bricks[k].color = MYSTIC_RED;
        drawRect(&bricks[k]);
        k++;
    }
}

void initDrawOddRow(byte y, byte index) {
    byte lastx, k, i;    
    lastx = 0;
    k = index;
    // Draw first brick
    bricks[k].x = lastx;
    lastx = lastx + 8;
    bricks[k].y = y;
    bricks[k].width = 6;
    bricks[k].height = 4;
    bricks[k].color = MYSTIC_RED;
    drawRect(&bricks[k]);
    k++;
    for(i=0; i<9; i++) {
        bricks[k].x = lastx;
        lastx = lastx + 16;
        bricks[k].y = y;
        bricks[k].width = 14;
        bricks[k].height = 4;
        bricks[k].color = MYSTIC_RED;
        drawRect(&bricks[k]);
        k++;
    }
    // Draw last brick
    bricks[k].x = lastx;
    lastx = lastx + 8;
    bricks[k].y = y;
    bricks[k].width = 6;
    bricks[k].height = 4;
    bricks[k].color = MYSTIC_RED;
    consoleLogDecimal(bricks[k].width);
    drawRect(&bricks[k]);
}

void initBricks() {
	byte i, k, lasty;
	k = 0;
	lasty = 6;
	
    for(i=0; i<2; i++) {
        // Even rows
        //initDrawEvenRow(lasty, k);
        k = k + 8;
        lasty = lasty + 6;
        // Odd rows
        initDrawOddRow(lasty, k);
        k = k + 9;
        lasty = lasty + 6;
    }	
}

void initScore() {
	rectangle score;
	score.x = 0;
	score.y = 0;
	score.width = 128;
	score.height = 6;
	score.color = BLACK;
	drawRect(&score);
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
    if(myball.y==6) {
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
