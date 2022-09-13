#include "dlib.h"

/* Procedure definitions */
void initGame(void);
void updateGame(void);
int main(void);

/* Game Data */
rectangle player;

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
    player.mem = 0x1234;
    drawRect(&player);
    
    stopStopwatch();
}

void updateGame() {
    byte gamepad;
    
    gamepad=readGamepad(0);
    if(gamepad & BUTTON_LEFT && player.x>0) {
        moveLeft(&player);
    }
    else if(gamepad & BUTTON_RIGHT && player.x+player.width<128) {
        moveRight(&player);
    }
}

int main(void){
    // Init game
    initGame();
    
    consoleLogStr("Game loop\n");
    consoleLogHex(0xff);
    // Game loop
    while(1) {
        //waitFrames(50);
        
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
