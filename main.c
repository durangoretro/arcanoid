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
    moveRight(&player);
}

int main(void){
    // Init game
    initGame();
    
    consoleLogStr("Game loop\n");
    // Game loop
    while(1) {
        waitFrames(60);
        
        // Wait VSYNC
        waitVSync();                
        // Start counting time
        startStopwatch();
        
        // Update game
        updateGame();
        
        // Stop counting time
        stopStopwatch();
        break;
    }
    
    return 0;
}
