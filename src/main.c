#include "display.h"
#include "vector.h"
#include <stdio.h>
#include <SDL2/SDL.h>

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

bool isPacmanMode = true; // Flag to indicate if pacman mode is active
bool isRunning = false;

int widthWindow = 400;
int heightWindow = 300;

Uint32 timePast;
Uint32 timeNew = 0;
double timeDiff;

typedef struct {
    Vector2D pos;
    Vector2D vel;
    int width;
    int height;
    uint32_t color;
} Square;

Square rectangle = {{0, 0}, {0, 0}, 50, 50, 0xFFFF0000};


void setup(){
    isRunning = initDisplay(widthWindow, heightWindow);
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
            switch(event.key.keysym.sym){ //which key was pressed problems 
                case SDLK_ESCAPE:
                    isRunning = false;
                    break;
                /*case SDLK_w://if simultanious keys are pressed
                    rectangle.vel.y = -100; // Move up
                    break;
                case SDLK_s:
                    rectangle.vel.y = 100; // Move down
                    break;
                case SDLK_a:
                    rectangle.vel.x = -100; // Move left
                    break;
                case SDLK_d:
                    rectangle.vel.x = 100; // Move right
                    break;
                default:
                    break;
            }
        }else if(event.type == SDL_KEYUP){// key released
            switch(event.key.keysym.sym){ //which key was released
                case SDLK_w:
                case SDLK_s:
                    rectangle.vel.y = 0; // Stop vertical movement
                    break;
                case SDLK_a:
                case SDLK_d:
                    rectangle.vel.x = 0; // Stop horizontal movement
                    break;*/
                default:
                    break;
            }
        }
    }
    // Handle continuous key presses for movement
    rectangle.vel.x = 0; // Reset horizontal velocity
    rectangle.vel.y = 0; // Reset vertical velocity
    const Uint8* keyboardState = SDL_GetKeyboardState(NULL); // Get the current state of the keyboard
    if(keyboardState[SDL_SCANCODE_W]){
        rectangle.vel.y += -100; // Move up
    }
    if(keyboardState[SDL_SCANCODE_S]){
        rectangle.vel.y += 100; // Move down
    }
    if(keyboardState[SDL_SCANCODE_A]){
        rectangle.vel.x += -100; // Move left
    }
    if(keyboardState[SDL_SCANCODE_D]){
        rectangle.vel.x += 100; // Move right
    }
}

void drawRectanglePM(){
    drawRectangle(rectangle.pos.x, rectangle.pos.y, rectangle.width, rectangle.height, rectangle.color);// Draw the rectangle at its current position
    if(rectangle.pos.x + rectangle.width >= widthWindow){// if the rectangle is partially off the right edge of the window
        drawRectangle(rectangle.pos.x - widthWindow, rectangle.pos.y, rectangle.width, rectangle.height, rectangle.color);
    }
    if(rectangle.pos.y + rectangle.height >= heightWindow){// if the rectangle is partially off the bottom edge of the window
        drawRectangle(rectangle.pos.x, rectangle.pos.y - heightWindow, rectangle.width, rectangle.height, rectangle.color);
    }
    if(rectangle.pos.x < 0){// if the rectangle is partially off the left edge of the window
        drawRectangle(rectangle.pos.x + widthWindow, rectangle.pos.y, rectangle.width, rectangle.height, rectangle.color);
    }
    if(rectangle.pos.y < 0){// if the rectangle is partially off the top edge of the window
        drawRectangle(rectangle.pos.x, rectangle.pos.y + heightWindow, rectangle.width, rectangle.height, rectangle.color);
    }
    if(rectangle.pos.x + rectangle.width >= widthWindow && rectangle.pos.y + rectangle.height >= heightWindow){// if the rectangle is partially off the right and bottom edges of the window
        drawRectangle(rectangle.pos.x - widthWindow, rectangle.pos.y - heightWindow, rectangle.width, rectangle.height, rectangle.color);
    }
    if(rectangle.pos.x < 0 && rectangle.pos.y < 0){// if the rectangle is partially off the left and top edges of the window
        drawRectangle(rectangle.pos.x + widthWindow, rectangle.pos.y + heightWindow, rectangle.width, rectangle.height, rectangle.color);
    }
    if(rectangle.pos.x < 0 && rectangle.pos.y + rectangle.height >= heightWindow){// if the rectangle is partially off the left and bottom edges of the window
        drawRectangle(rectangle.pos.x + widthWindow, rectangle.pos.y - heightWindow, rectangle.width, rectangle.height, rectangle.color);
    }
    if (rectangle.pos.y < 0 && rectangle.pos.x + rectangle.width >= widthWindow)// if the rectangle is partially off the top and right edges of the window
    {
        drawRectangle(rectangle.pos.x - widthWindow, rectangle.pos.y + heightWindow, rectangle.width, rectangle.height, rectangle.color);
    }
    
}

void render(){
    clearDisplay(0xFF000000); // Clear the display with black color
    if(isPacmanMode)
        drawRectanglePM(); // Draw the rectangle in pacman mode
    else
        drawRectangle(rectangle.pos.x, rectangle.pos.y, rectangle.width, rectangle.height, rectangle.color); // Draw the current position in pacman mode
    if(!updateDisplay()){
        printf("Failed to update display\n");
        isRunning = false;
    }
}

void updatePM(){
    timePast = timeNew;
    timeNew = SDL_GetTicks();// Get the number of milliseconds since the SDL library initialization    
    timeDiff = (timeNew - timePast)/1000.0; // Convert to seconds
    double distanceX = rectangle.vel.x * timeDiff; // Calculate the distance to move based on speed and time difference
    double distanceY = rectangle.vel.y * timeDiff; // Calculate the distance to move based on speed and time difference
    Vector2D movement = init(distanceX, distanceY); // Create a vector for the movement
    rectangle.pos = sum(rectangle.pos, movement); // Update the position of the rectangle by adding the movement vector to the current position
    if(rectangle.pos.x >= widthWindow){ // If the rectangle has moved off the right edge of the window
        rectangle.pos.x -= widthWindow;
    }
    else if(rectangle.pos.x + rectangle.width <= 0){// If the rectangle has moved off the left edge of the window
        rectangle.pos.x += widthWindow;
    }

    if(rectangle.pos.y >= heightWindow){// If the rectangle has moved off the bottom edge of the window
        rectangle.pos.y -= heightWindow;
    }
    else if(rectangle.pos.y + rectangle.height <= 0){// If the rectangle has moved off the top edge of the window
        rectangle.pos.y += heightWindow;
    }
}

void updateC(){
    timePast = timeNew;
    timeNew = SDL_GetTicks();// Get the number of milliseconds since the SDL library initialization    
    timeDiff = (timeNew - timePast)/1000.0; // Convert to seconds
    double distanceX = rectangle.vel.x * timeDiff; // Calculate the distance to move based on speed and time difference
    double distanceY = rectangle.vel.y * timeDiff; // Calculate the distance to move based on speed and time difference
    Vector2D movement = init(distanceX, distanceY); // Create a vector for the movement
    rectangle.pos = sum(rectangle.pos, movement); // Update the position of the rectangle by adding the movement vector to the current position

    if(rectangle.pos.x + rectangle.width >= widthWindow){// If the rectangle has moved off the right edge of the window
        rectangle.vel.x = 0;
        rectangle.pos.x = widthWindow - rectangle.width; // Keep the rectangle within the window bounds
    }
    if(rectangle.pos.x < 0){// If the rectangle has moved off the left edge of the window
        rectangle.vel.x = 0;
        rectangle.pos.x = 0; // Keep the rectangle within the window bounds
    }

    if(rectangle.pos.y + rectangle.height >= heightWindow){// If the rectangle has moved off the bottom edge of the window
        rectangle.vel.y = 0;
        rectangle.pos.y = heightWindow - rectangle.height; // Keep the rectangle within the window bounds
    }
    if(rectangle.pos.y < 0){// If the rectangle has moved off the top edge of the window
        rectangle.vel.y = 0;
        rectangle.pos.y = 0; // Keep the rectangle within the window bounds
    }
}

int main(int argc, char* argv[]){
    setup();
    while(isRunning){
        processInput();
        if(isPacmanMode)
            updatePM();
        else
            updateC();
        render();
        SDL_Delay(16);//delay for 16 milliseconds to limit the frame rate to ~60 FPS
    }
    freeDisplay();
    return 0;
}
