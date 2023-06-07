#include <qgraph.h>
#include <system.h>
#include <glyph.h>
#include <font.h>
#include <psv.h>
#include "bin/title.h"
#include "bin/controls.h"
#include "bin/hall.h"
#include "bin/input.h"

typedef struct{
    byte index;
    char initials[8];
    long score;    
} score_row;

#define HALL_SIZE 7
typedef struct{
    score_row scores[HALL_SIZE];
} storage;



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
void displayControls(void);
void displayHall(void);
void draw_lives(void);
void cleanBanner(void);

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
char lives;
storage saved_data;


/* Aux vars */
unsigned char index;
char build_hash[17];

/* Game Procedures */

void initGame() {
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
	score=0;
    lives=4;
    player.x = 48;
    player.y = 122;
    player.width = 32;
    player.height = 4;
	player.color = GREEN;
    drawRect(&player);
    draw_lives();
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
	char i;
    rectangle scoreRect;
    scoreRect.x = 0;
	scoreRect.y = 0;
	scoreRect.width = 128;
	scoreRect.height = 8;
	scoreRect.color = PINK_FLAMINGO;
	drawRect(&scoreRect);
    printBCD(80, 0, font, BLACK, PINK_FLAMINGO, score);
    price=5;
    
    i=0;
    do{
        saved_data.scores[i].initials[0]='A';
        saved_data.scores[i].initials[1]='A';
        saved_data.scores[i].initials[2]='A';
        saved_data.scores[i].initials[3]='A';
        saved_data.scores[i].initials[4]='A';
        saved_data.scores[i].initials[5]='A';
        saved_data.scores[i].initials[6]='A';
        saved_data.scores[i].initials[7]='\0';
        i++;
    } while(i!=HALL_SIZE);
}

void cleanBanner() {
    rectangle cleanrect;
    cleanrect.x = 0;
	cleanrect.y = 40;
	cleanrect.width = 128;
	cleanrect.height = 64;
	cleanrect.color = CIAN;
	drawRect(&cleanrect);
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
    if((gamepad & BUTTON_LEFT || readKeyboard(ROW_KEY_O) & KEY_O) && player.x>0) {
        moveLeft(&player);
        paddle_speed=-1;
    }
    // Move right
    else if((gamepad & BUTTON_RIGHT || readKeyboard(ROW_KEY_P) & KEY_P) && player.x+player.width<128) {
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
            cleanBall(&myball);
            lives--;
            price=5;
            if(lives!=0xff) {
                printStr(20, 60, font, BLACK, CIAN, "PRESS SPACE...");
                waitButton();
                cleanBanner();
                
                draw_lives();
                updateScore();
                cleanBall(&myball);
                initBall();
                drawBall(&myball);
            }
            // GAME OVER			
            else {
                printStr(20, 60, font, BLACK, CIAN, "GAME OVER");
                printStr(20, 70, font, BLACK, CIAN, "PRESS START...");
                waitStart();
                displayHall();
                cleanBanner();
                initGame();
            }            
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

void draw_lives() {
    ball tball;
    rectangle scoreRect;
    char i;
    
    scoreRect.x = 0;
	scoreRect.y = 0;
	scoreRect.width = 30;
	scoreRect.height = 8;
	scoreRect.color = PINK_FLAMINGO;
	drawRect(&scoreRect);
    if(lives!=0) {
        tball.y=3;
        tball.x=10;
        tball.color=GREEN;
        i=0;
        do {
            drawBall(&tball);
            tball.x+=6;
            i++;
        }while(i<lives);
    }
}

void displayTitle() {
    render_image(title);
    getBuildVersion(build_hash);
    printStr(0, 95, font, WHITE, BLACK, build_hash);
    waitStart();
}

void displayControls() {
    render_image(controls);
    waitStart();
}

void displayHall() {
    char i, y;
    char name[20];
    
    if(score>saved_data.scores[6].score){
        render_image(input);
        readStr(16, 58, font, WHITE, BLACK, name);
    }
    
    render_image(hall);
    i=0;
    y=52;
    do{
        printStr(10, y, font, WHITE, BLACK, saved_data.scores[0].initials);
        printBCD(70, y, font, WHITE, BLACK, saved_data.scores[0].score);
        y+=10;
        i++;
    } while(i!=HALL_SIZE);
    
    waitStart();
}

int main(void){
    displayTitle();   
    displayControls();
    
    // Init game
    initGame();
    
    // Game loop
    while(1) {
        //waitFrames(10);
        
        // Wait VSYNC
        waitVSync();
        
        // Update game
        updateGame();
        
    }
    
    return 0;
}
