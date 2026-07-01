#include "display.h"
#include <stdio.h>
#include <SDL2/SDL.h>

bool isRunning = false;

void setup(){
    isRunning = initDisplay();
    if(!isRunning){
        printf("Failed to initialize display\n");
    }
}

void processInput(){
    SDL_Event event;
    while(SDL_PollEvent(&event) != 0){ //read all events from the event queue
        if(event.type == SDL_QUIT){
            isRunning = false;
        }
        else if(event.type == SDL_KEYDOWN){// key pressed
            switch(event.key.keysym.sym){ //which key was pressed
                case SDLK_ESCAPE:
                    isRunning = false;
                    break;
                default:
                    break;
            }
        }
    }
}

void render(){
    clearDisplay(0xFF000000); // Clear the display with black color
    for(int y = 0; y < 300; y++){
        for(int x = 0; x < 400; x++){
            drawPixel(x, y, 0xFFFFFFFF); // Draw a white pixel at (x, y)
        }
    }
    if(!updateDisplay()){
        printf("Failed to update display\n");
        isRunning = false;
    }
}
void update(){
    // logic here
}

int main(int argc, char* argv[]){
    setup();
    while(isRunning){
        processInput();
        update();
        render();
        SDL_Delay(16);//delay for 16 milliseconds to limit the frame rate to ~60 FPS
    }
    freeDisplay();
    return 0;
}
