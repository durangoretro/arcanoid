#include "dlib.h"

/* Procedure definitions */
void initGame(void);
int main(void);

/* Game Data */
unsigned short myunshort;

/* Game Procedures */

void initGame() {
    startStopwatch();
    fillScreen(CIAN);
    stopStopwatch(); 
}

int main(void){
    myunshort = 1;
    // Init game
    initGame();
    
    // Game loop
    while(1) {
        // Wait VSYNC
        waitVSync();
        // Start counting time
        startStopwatch();
        
        // Stop counting time
        stopStopwatch();        
    }
    
    return 0;
}
