#include "dlib.h"

void initGame(void);
int main(void);

void initGame() {
    startStopwatch();
    fillScreen(CIAN);
    stopStopwatch(); 
}

int main(void){
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
