#include "display.h"
#include <SDL2/SDL.h>
#include <stdbool.h>

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

int width;
int height;

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
SDL_Texture *texture = NULL;

uint32_t *colorBuffer = NULL;
double *zBuffer = NULL;


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
    zBuffer = (double*)malloc(width * height * sizeof(double));
    if(zBuffer == NULL){
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
    for(int y = 0; y < height; y++){
        for(int x = 0; x < width; x++){
            zBuffer[y * width + x] = DBL_MAX;
        }
    }
} 

//Bresenham's line algorithm
void drawLine(int x1, int y1, double z1, int x2, int y2, double z2, uint32_t color){
    z1 *= 0.998;//evitar lineas cortadas
    z2 *= 0.998;//evitar lineas cortadas
    //step 1: calculate the differences in x and y coordinates
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);

    int steps = MAX(dy,dx);
    double zIncrement = 0.0;
    if (steps != 0)
    {
        zIncrement = (z2-z1)/steps;
    }

    //step 2: determine the direction of the line
    int sx = (x1 < x2) ? 1 : -1;// x1 < x2 then goes right, else goes left
    int sy = (y1 < y2) ? 1 : -1;// y1 < y2 then goes down, else goes up
    //step 3: initialize the error term
    int err = dx - dy;
    //step 4: loop until the end point is reached
    while(true){
        drawPixel(x1, y1, z1, color);
        if(x1 == x2 && y1 == y2) break;
        //step 5: calculate the error term and update the coordinates
        int err2 = err * 2;
        if(err2 > -dy){
            err -= dy;
            x1 += sx;
        }
        if(err2 < dx){
            err += dx;
            y1 += sy;
        }
        z1 += zIncrement;
    }
}

//not used for mode pacman
void drawRectanglePacman(int xpos, int ypos, int widthRectangle, int heightRectangle, uint32_t color){
    for(int x = xpos; x < xpos + widthRectangle; x++){
        for(int y = ypos; y < ypos + heightRectangle; y++){
            drawPixel(x%width, y%height,0.0, color);
        }
    }
}

void drawRectangle(int x, int y, int width, int height, uint32_t color){
    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            drawPixel(x + j, y + i,0.0, color);
        }
    }
}


void drawPixel(int x, int y, double z, uint32_t color){
    if(x >= 0 && x < width && y >= 0 && y < height){
        if(z < zBuffer[y * width + x]){
            zBuffer[y * width + x] = z;
            colorBuffer[y * width + x] = color;
        }
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

void drawHorizontalLine(int x1, int x2,double z1, double z2, int y, uint32_t color){
    if(x1 > x2){
        int temp = x1;
        x1 = x2;
        x2 = temp;
        double dtemp = z1;
        z1 = z2;
        z2 = dtemp;
    }
    if (x2-x1 == 0)
    {
        drawPixel(x1, y,z1, color);
        return;
    }
    
    double zIncrement = (z2-z1)/(x2-x1);
    double z = z1;
    

    for(int x = x1; x <= x2; x++){
        drawPixel(x, y,z, color);
        z += zIncrement;
    }
}

//p0 < p1 < p2 (y)
void drawFilledTriangle(int x0, int y0, double z0, int x1, int y1, double z1 , int x2, int y2, double z2, uint32_t color){
    int temp;
    double dtemp;
    if (y0>y1)
    {
        temp = y0; y0 = y1; y1 = temp;
        temp = x0; x0 = x1; x1 = temp;
        dtemp = z0; z0 = z1; z1 = dtemp;
    }
    if (y0>y2)
    {
        temp = y0; y0 = y2; y2 = temp;
        temp = x0; x0 = x2; x2 = temp;
        dtemp = z0; z0 = z2; z2 = dtemp;
    }
    if (y1>y2)
    {
        temp = y1; y1 = y2; y2 = temp;
        temp = x1; x1 = x2; x2 = temp;
        dtemp = z1; z1 = z2; z2 = dtemp;
    }
    double m01 = (y1 - y0) > 0 ? (float)(x1 - x0) / (float)(y1 - y0) : 0;
    double m12 = (y2 - y1) > 0 ? (float)(x2 - x1) / (float)(y2 - y1) : 0;
    double m02 = (y2 - y0) > 0 ? (float)(x2 - x0) / (float)(y2 - y0) : 0;
    double zM01 = (y1 - y0) > 0 ? (float)(z1 - z0) / (float)(y1 - y0) : 0;
    double zM12 = (y2 - y1) > 0 ? (float)(z2 - z1) / (float)(y2 - y1) : 0;
    double zM02 = (y2 - y0) > 0 ? (float)(z2 - z0) / (float)(y2 - y0) : 0;
    /*
    //evaluación paramétrica
    // Inefficient to multiply in floating point but perfect lines, the difference is not noticeable
    // more precise than the DDA algorithm, but slower
    for (int i = y0; i < y1; i++)
    {
        int xStart = (int)(x0 + m02 * (i - y0));
        int xEnd = (int)(x0 + m01 * (i - y0));
        drawHorizontalLine(xStart, xEnd, i, color);
    }
    for (int i = y1; i < y2; i++){
        int xStart = (int)(x0 + m02 * (i - y0));
        int xEnd = (int)(x1 + m12 * (i - y1));
        drawHorizontalLine(xStart, xEnd, i, color);
    }
    */
    //(Algoritmo DDA o Incremental)
    double xStart = x0, xEnd = x0;
    double zStart = z0, zEnd = z0;
    for (int i = y0; i < y1; i++)
    {
        drawHorizontalLine((int)xStart, (int)xEnd, zStart, zEnd, i, color);
        xStart += m02;
        xEnd += m01;
        zStart += zM02;
        zEnd += zM01;
    }
    xEnd = x1;
    zEnd = z1;
    for (int i = y1; i < y2; i++){
        drawHorizontalLine((int)xStart, (int)xEnd, zStart, zEnd, i, color);
        xStart += m02;
        xEnd += m12;
        zStart += zM02;
        zEnd += zM12;
    }
}

uint32_t applyLight(uint32_t color, double intensity) {

    if (intensity < 0.2) intensity = 0.2;//if not 0.0
    if (intensity > 1.0) intensity = 1.0;//if not 1.0

    uint32_t a = (color >> 24) & 0xFF;
    uint32_t r = (color >> 16) & 0xFF;
    uint32_t g = (color >> 8) & 0xFF;
    uint32_t b = color & 0xFF;

    r = (uint32_t)(r * intensity);
    g = (uint32_t)(g * intensity);
    b = (uint32_t)(b * intensity);

    return (a << 24) | (r << 16) | (g << 8) | b;
}


void freeDisplay(){
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    free(colorBuffer);
    free(zBuffer);
} 