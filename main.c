#include "dlib.h"
#include <psv.h>
#include <system.h>
#include <glyph.h>
#include "bin/font.h"
#include "bin/title.h"

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
void check_collisions(void);
void checkBottomCols(void);
void checkTopCols(void);
void updateScore(void);
void displayTitle(void);
int main(void);

/* Game Data */
rectangle player;
ball myball;
brick bricks[34];
brick *current_brick;
char brokens;
byte paddle_speed;
long score;
long price;

/* Aux vars */
unsigned char index;

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
    myball.vx=1;
    myball.vy=-2;
}

void initPlayer() {
	player.x = 48;
    player.y = 122;
    player.width = 32;
    player.height = 4;
	player.color = GREEN;
    drawRect(&player);
}

void initDrawEvenRow(byte y, byte index) {
    byte lastx, k, i;
    lastx = 0;
    k = index;
    i=0;
    do {
        bricks[k].enabled = 1;
        bricks[k].x = lastx;
        lastx = lastx + 16;
        bricks[k].y = y;
        bricks[k].width = 14;
        bricks[k].height = 4;
        bricks[k].color = MYSTIC_RED;
        bricks[k].x2=bricks[k].x+bricks[k].width;
        bricks[k].y2=bricks[k].y+bricks[k].height;
        drawRect(&bricks[k]);
        k++;
        i++;
    } while(i!=8);
}

void initDrawOddRow(byte y, byte index) {
    byte lastx, k, i;    
    consoleLogStr("\nY\n");
    consoleLogDecimal(y);
    lastx = 0;
    k = index;
    // Draw first brick
    bricks[k].enabled = 1;
    bricks[k].x = lastx;
    lastx = lastx + 8;
    bricks[k].y = y;
    bricks[k].width = 6;
    bricks[k].height = 4;
    bricks[k].color = MYSTIC_RED;
    bricks[k].x2=bricks[k].x+bricks[k].width;
    bricks[k].y2=bricks[k].y+bricks[k].height;
    drawRect(&bricks[k]);
    k++;

    i=0;
    do {
        bricks[k].enabled = 1;
        bricks[k].x = lastx;
        lastx = lastx + 16;
        bricks[k].y = y;
        bricks[k].width = 14;
        bricks[k].height = 4;
        bricks[k].color = MYSTIC_RED;
        bricks[k].x2=bricks[k].x+bricks[k].width;
        bricks[k].y2=bricks[k].y+bricks[k].height;
        drawRect(&bricks[k]);
        k++;
        i++;
    } while(i!=7);
        
    // Draw last brick
    bricks[k].enabled = 1;
    bricks[k].x = lastx;
    bricks[k].y = y;
    bricks[k].width = 6;
    bricks[k].height = 4;
    bricks[k].color = MYSTIC_RED;
    bricks[k].x2=bricks[k].x+bricks[k].width;
    bricks[k].y2=bricks[k].y+bricks[k].height;
    consoleLogStr("\nBrick\n");
    consoleLogDecimal(bricks[k].x);
    consoleLogDecimal(bricks[k].y);
    consoleLogDecimal(bricks[k].width);
    consoleLogDecimal(bricks[k].height);
    drawRect(&bricks[k]);
}



void initBricks() {
	byte i, k, lasty;
	k = 0;
	lasty = 8;
	brokens = 0;
    i=0;
    do {
        // Even rows
        initDrawEvenRow(lasty, k);
        k = k + 8;
        lasty = lasty + 6;
        // Odd rows
        initDrawOddRow(lasty, k);
        k = k + 9;
        lasty = lasty + 6;
        i++;
    } while(i!=2);
}

void initScore() {
	rectangle scoreRect;
	scoreRect.x = 0;
	scoreRect.y = 0;
	scoreRect.width = 128;
	scoreRect.height = 8;
	scoreRect.color = PINK_FLAMINGO;
	drawRect(&scoreRect);
    consoleLogHex(0x00);
    consoleLogHex(0xff);
    consoleLogHex(0x00);
    consoleLogHex(0xff);
    printBCD(80, 0, font, BLACK, PINK_FLAMINGO, score);
    price=5;
}

void updateGame() {
    updateBall();
    updatePlayer();
    check_collisions();
}

void updatePlayer() {
    byte gamepad;
    
    // Read gamepad
    gamepad=readGamepad(0);
    // Move left
    if(gamepad & BUTTON_LEFT && player.x>0) {
        moveLeft(&player);
        paddle_speed=-1;
    }
    // Move right
    else if(gamepad & BUTTON_RIGHT && player.x+player.width<128) {
        moveRight(&player);
        paddle_speed=1;
    }
    else {        
        paddle_speed=0;
    }
}

void updateBall() {
    byte old_vx;
    
    // Top collision
    if(myball.y==8) {
        myball.vy = 2;
    }
    // Bottom collision
    if(myball.y==120) {
        // Paddle collision
        if(myball.x >= player.x && myball.x<=player.x+player.width) {
            myball.vy = -2;
            myball.vx=myball.vx+paddle_speed;
            price=5;
            if(brokens==34) {
                initBricks();
            }
        }
        // Fail
        else {
            old_vx = myball.vx;
            myball.vx = 0;
            myball.vy = 0;
            waitStart();
			score=0;
			updateScore();
            cleanBall(&myball);
            initBall();
            drawBall(&myball);
        }
    }
	// Right collision
    if(127<=myball.x) {
        myball.vx = -1;
    }
    // Left collision
    if(0xf0<=myball.x) {
        myball.vx = 1;
    }
    
    // Update ball position
    myball.x = myball.x+myball.vx;
    myball.y = myball.y+myball.vy;
    // Move ball in screen
    moveBall(&myball);
}

void check_collisions(void) {
    index=0;
    current_brick=bricks;
    do {
        checkBottomCols();
        checkTopCols();
        current_brick++;
        index++;
    } while(index!=34);
}

void checkBottomCols() {
    if(current_brick->enabled == 1 && current_brick->y2==myball.y
        && current_brick->x<myball.x && myball.x<current_brick->x2) {
        myball.vy = 2;
        current_brick->enabled = 0;
        current_brick->color=CIAN;
        drawRect(current_brick);
        brokens++;
        addBCD(&score, &price);
        updateScore();
        price+=price;
    }        
}

void checkTopCols() {
    if(current_brick->enabled == 1 && current_brick->y==myball.y
        && current_brick->x<myball.x && myball.x<current_brick->x2) {
        myball.vy = -2;
        current_brick->enabled = 0;
        current_brick->color=CIAN;
        drawRect(current_brick);
        brokens++;
        addBCD(&score, &price);
        updateScore();
        price+=price;
    }  
}

void updateScore() {
	printBCD(80, 0, font, BLACK, PINK_FLAMINGO, score);
}

void displayTitle() {
    render_image(title);
    waitStart();
}

int main(void){
    displayTitle();   
    
    // Init game
    initGame();
    
    consoleLogStr("Game loop\n");
    consoleLogHex(0xff);
    // Game loop
    while(1) {
        //waitFrames(10);
        
        // Wait VSYNC
        waitVSync();
        // Start counting time
        //startStopwatch();
        
        // Update game
        updateGame();
        
        // Stop counting time
        //stopStopwatch();        
    }
    
    return 0;
}
