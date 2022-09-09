#include "dlib.h"

/* Procedure definitions */
void initGame(void);
int main(void);

/* Game Data */
rectangle player;

/* Game Procedures */

void initGame() {
    consoleLogStr("Init Game\n");
    startStopwatch();
    fillScreen(CIAN);
    
    player.x = 10;
    player.y = 11;
    player.width = 32;
    player.height = 8;
    player.mem = 0x1234;
    draw_rect(&player);
    consoleLogStr("\nMEM\n");
    consoleLogWord(player.mem);
    
    stopStopwatch();
}

int main(void){
    // Init game
    initGame();
    
    consoleLogStr("Game loop\n");
    // Game loop
    while(0) {
        // Wait VSYNC
        waitVSync();
        // Start counting time
        startStopwatch();
        
        // Stop counting time
        stopStopwatch();        
    }
    
    return 0;
}
