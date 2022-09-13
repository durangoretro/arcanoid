#include "dlib.h"

/* Procedure definitions */
void initGame(void);
void updateGame(void);
void updateBall(void);
void updatePlayer(void);
int main(void);

/* Game Data */
rectangle player;
ball myball;

/* Game Procedures */

void initGame() {
    consoleLogStr("Init Game\n");
    startStopwatch();
    
    fillScreen(CIAN);
    player.x = 48;
    player.y = 120;
    player.width = 32;
    player.height = 6;
	player.color = MYSTIC_RED;
    drawRect(&player);
    
    myball.color = GREEN;
    myball.x = 62;
    myball.y = 60;
    myball.vx=2;
    myball.vy=-4;
    drawBall(&myball);    
    
    stopStopwatch();
}

void updateGame() {
    updateBall();
    updatePlayer();
}

void updatePlayer() {
    byte gamepad;
    
    gamepad=readGamepad(0);
    if(gamepad & BUTTON_LEFT && player.x>0) {
        moveLeft(&player);
    }
    else if(gamepad & BUTTON_RIGHT && player.x+player.width<128) {
        moveRight(&player);
    }
}

void updateBall() {
    if(myball.y==0) {
        myball.vy = 4;
    }
    if(myball.y==120) {
        myball.vy = -4;
    }
    if(myball.x==128) {
        myball.vx = -2;
    }
    if(myball.x==0) {
        myball.vx = 2;
    }
    
    myball.x = myball.x+myball.vx;
    myball.y = myball.y+myball.vy;
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
