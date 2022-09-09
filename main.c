#include "dlib.h"


int main(void){
    fillScreen(CIAN);
    
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
