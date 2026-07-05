#include "display.h"
#include <SDL2/SDL.h>
#include <stdbool.h>

int width;
int height;

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
SDL_Texture *texture = NULL;

uint32_t *colorBuffer = NULL;


bool initDisplay(int width1, int height1){
    width = width1;
    height = height1;   
    if(SDL_Init(SDL_INIT_VIDEO) < 0){
        return false;
    }
    colorBuffer = (uint32_t*)malloc(width * height * sizeof(uint32_t));
    if(colorBuffer == NULL){
        return false;
    }
    window = SDL_CreateWindow("Display", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if(window == NULL){
        return false;
    }
    renderer = SDL_CreateRenderer(window, -1, 0);
    if(renderer == NULL){
        return false;
    }
    //SDL_PIXELFORMAT_ARGB8888 color format, 32 bits per pixel, 8 bits per channel (red, green, blue, alpha)
    //SDL_TEXTUREACCESS_STREAMING data can be changed frequently
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, width, height);
    if(texture == NULL){
        return false;
    }
    return true;
}

void clearDisplay(uint32_t color){
    for(int y = 0; y < height; y++){
        for(int x = 0; x < width; x++){
            colorBuffer[y * width + x] = color;
        }
    }
} 

//not used for mode pacman
void drawRectanglePacman(int xpos, int ypos, int widthRectangle, int heightRectangle, uint32_t color){
    for(int x = xpos; x < xpos + widthRectangle; x++){
        for(int y = ypos; y < ypos + heightRectangle; y++){
            drawPixel(x%width, y%height, color);
        }
    }
}

void drawRectangle(int x, int y, int width, int height, uint32_t color){
    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            drawPixel(x + j, y + i, color);
        }
    }
}


void drawPixel(int x, int y, uint32_t color){
    if(x >= 0 && x < width && y >= 0 && y < height){
        colorBuffer[y * width + x] = color;
    }   
}

bool updateDisplay(){
    //NULL means the entire texture will be updated
    //colorBuffer the raw pixel data in the format of the texture
    //width * sizeof(uint32_t) the number of bytes in a row of pixel data
    if(SDL_UpdateTexture(texture, NULL, colorBuffer, width * sizeof(uint32_t)) < 0){
        return false;
    }

    //srcrect	the source SDL_Rect structure or NULL for the entire texture
    //dstrect	the destination SDL_Rect structure or NULL for the entire rendering target; the texture will be stretched to fill the given rectangle
    if(SDL_RenderCopy(renderer, texture, NULL, NULL) < 0){
        return false;
    }
    // show the current rendering on the screen
    SDL_RenderPresent(renderer);
    return true;
}

void freeDisplay(){
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    free(colorBuffer);
} 