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

void drawHorizontalLine1(int x1, int x2,double z1, double z2, int y, double u1, double u2, double v1, double v2, double iZ1, double iZ2, uint32_t* textureBuffer, int texWidth, int texHeight, double lightFactor){
    int tex_x, tex_y;
    if(x1 > x2){
        int temp = x1;
        x1 = x2;
        x2 = temp;

        double dtemp = z1;
        z1 = z2;
        z2 = dtemp;

        dtemp = u1;
        u1 = u2;
        u2 = dtemp;

        dtemp = v1;
        v1 = v2;
        v2 = dtemp;

        dtemp = iZ1;
        iZ1 = iZ2;
        iZ2 = dtemp;
    }
    if (x2-x1 == 0)
    {
        // es vital que vaya en funcion de la perpectiva(z) para que no se vea distorsionada la textura
        tex_x = (int)(u1/iZ1 * (texWidth - 1));
        tex_y = (int)(v1/iZ1 * (texHeight - 1));
        if (tex_x < 0) tex_x = 0;
        if (tex_x >= texWidth) tex_x = texWidth - 1;
        if (tex_y < 0) tex_y = 0;
        if (tex_y >= texHeight) tex_y = texHeight - 1;
        int index = tex_y * texWidth + tex_x;
        uint32_t modifiedColorMesh = applyLight(textureBuffer[index],lightFactor);
        drawPixel(x1, y, z1, modifiedColorMesh);
        return;
    }
    
    double zIncrement = (z2-z1)/(x2-x1);

    double uIncrement = (u2-u1)/(x2-x1);
    double vIncrement = (v2-v1)/(x2-x1);
    double iZIncrement = (iZ2-iZ1)/(x2-x1);
    
    double z = z1;
    
    double u = u1;
    double v = v1;
    double iZ = iZ1;

    for(int x = x1; x <= x2; x++){
        // es vital que vaya en funcion de la perpectiva(z) para que no se vea distorsionada la textura
        tex_x = (int)(u/iZ * (texWidth - 1));
        tex_y = (int)(v/iZ * (texHeight - 1));
        
        if (tex_x < 0) tex_x = 0;
        if (tex_x >= texWidth) tex_x = texWidth - 1;
        if (tex_y < 0) tex_y = 0;
        if (tex_y >= texHeight) tex_y = texHeight - 1;
        
        int index = tex_y * texWidth + tex_x;
        uint32_t modifiedColorMesh = applyLight(textureBuffer[index],lightFactor);
        drawPixel(x, y, z, modifiedColorMesh);
        z += zIncrement;
        u += uIncrement;
        v += vIncrement;
        iZ += iZIncrement;
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
    // 
    double m01 = (y1 - y0) > 0 ? (float)(x1 - x0) / (float)(y1 - y0) : 0;
    double m12 = (y2 - y1) > 0 ? (float)(x2 - x1) / (float)(y2 - y1) : 0;
    double m02 = (y2 - y0) > 0 ? (float)(x2 - x0) / (float)(y2 - y0) : 0;
    //
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

void drawFilledTriangle1(Triangle t, uint32_t* textureBuffer, int texWidth, int texHeight, double lightFactor){
    for (int i = 0; i < 3; i++) {// si los puntos son flotantes, se redondean a enteros para dibujar el triángulo (si no, se distorsiona la textura con lineas horizontales)
        t.points[i].x = (int)t.points[i].x;
        t.points[i].y = (int)t.points[i].y;
    }
    int temp;
    double dtemp;
    if (t.points[0].y > t.points[1].y)
    {
        temp = t.points[0].y; t.points[0].y = t.points[1].y; t.points[1].y = temp;
        temp = t.points[0].x; t.points[0].x = t.points[1].x; t.points[1].x = temp;
        dtemp = t.points[0].z; t.points[0].z = t.points[1].z; t.points[1].z = dtemp;
        dtemp = t.uVs[0].x; t.uVs[0].x = t.uVs[1].x; t.uVs[1].x = dtemp;
        dtemp = t.uVs[0].y; t.uVs[0].y = t.uVs[1].y; t.uVs[1].y = dtemp;
    }
    if (t.points[0].y > t.points[2].y)
    {
        temp = t.points[0].y; t.points[0].y = t.points[2].y; t.points[2].y = temp;
        temp = t.points[0].x; t.points[0].x = t.points[2].x; t.points[2].x = temp;
        dtemp = t.points[0].z; t.points[0].z = t.points[2].z; t.points[2].z = dtemp;
        dtemp = t.uVs[0].x; t.uVs[0].x = t.uVs[2].x; t.uVs[2].x = dtemp;
        dtemp = t.uVs[0].y; t.uVs[0].y = t.uVs[2].y; t.uVs[2].y = dtemp;
    }
    if (t.points[1].y > t.points[2].y)
    {
        temp = t.points[1].y; t.points[1].y = t.points[2].y; t.points[2].y = temp;
        temp = t.points[1].x; t.points[1].x = t.points[2].x; t.points[2].x = temp;
        dtemp = t.points[1].z; t.points[1].z = t.points[2].z; t.points[2].z = dtemp;
        dtemp = t.uVs[1].x; t.uVs[1].x = t.uVs[2].x; t.uVs[2].x = dtemp;
        dtemp = t.uVs[1].y; t.uVs[1].y = t.uVs[2].y; t.uVs[2].y = dtemp;
    }
    // 
    double iZ0 = 1.0/t.points[0].z;// 1/z0
    double iZ1 = 1.0/t.points[1].z;   
    double iZ2 = 1.0/t.points[2].z;

    //
    double uZ0 = t.uVs[0].x / t.points[0].z;
    double uZ1 = t.uVs[1].x / t.points[1].z;
    double uZ2 = t.uVs[2].x / t.points[2].z;
    //
    double vZ0 = t.uVs[0].y / t.points[0].z;
    double vZ1 = t.uVs[1].y / t.points[1].z;
    double vZ2 = t.uVs[2].y / t.points[2].z;
    
    //Incremental interpolation of 1/z, u/z, v/z 
    double izM01 = (t.points[1].y - t.points[0].y) > 0 ? (float)(iZ1 - iZ0) / (float)(t.points[1].y - t.points[0].y) : 0;
    double izM12 = (t.points[2].y - t.points[1].y) > 0 ? (float)(iZ2 - iZ1) / (float)(t.points[2].y - t.points[1].y) : 0;
    double izM02 = (t.points[2].y - t.points[0].y) > 0 ? (float)(iZ2 - iZ0) / (float)(t.points[2].y - t.points[0].y) : 0;
    //
    double uZM01 = (t.points[1].y - t.points[0].y) > 0 ? (float)(uZ1 - uZ0) / (float)(t.points[1].y - t.points[0].y) : 0;
    double uZM12 = (t.points[2].y - t.points[1].y) > 0 ? (float)(uZ2 - uZ1) / (float)(t.points[2].y - t.points[1].y) : 0;
    double uZM02 = (t.points[2].y - t.points[0].y) > 0 ? (float)(uZ2 - uZ0) / (float)(t.points[2].y - t.points[0].y) : 0;
    //
    double vZM01 = (t.points[1].y - t.points[0].y) > 0 ? (float)(vZ1 - vZ0) / (float)(t.points[1].y - t.points[0].y) : 0;
    double vZM12 = (t.points[2].y - t.points[1].y) > 0 ? (float)(vZ2 - vZ1) / (float)(t.points[2].y - t.points[1].y) : 0;
    double vZM02 = (t.points[2].y - t.points[0].y) > 0 ? (float)(vZ2 - vZ0) / (float)(t.points[2].y - t.points[0].y) : 0;
    //

    //
    double m01 = (t.points[1].y - t.points[0].y) > 0 ? (float)(t.points[1].x - t.points[0].x) / (float)(t.points[1].y - t.points[0].y) : 0;
    double m12 = (t.points[2].y - t.points[1].y) > 0 ? (float)(t.points[2].x - t.points[1].x) / (float)(t.points[2].y - t.points[1].y) : 0;
    double m02 = (t.points[2].y - t.points[0].y) > 0 ? (float)(t.points[2].x - t.points[0].x) / (float)(t.points[2].y - t.points[0].y) : 0;
    //
    double zM01 = (t.points[1].y - t.points[0].y) > 0 ? (float)(t.points[1].z - t.points[0].z) / (float)(t.points[1].y - t.points[0].y) : 0;
    double zM12 = (t.points[2].y - t.points[1].y) > 0 ? (float)(t.points[2].z - t.points[1].z) / (float)(t.points[2].y - t.points[1].y) : 0;
    double zM02 = (t.points[2].y - t.points[0].y) > 0 ? (float)(t.points[2].z - t.points[0].z) / (float)(t.points[2].y - t.points[0].y) : 0;
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
    double xStart = t.points[0].x, xEnd = t.points[0].x;
    double zStart = t.points[0].z, zEnd = t.points[0].z;
    double uStart = uZ0, uEnd = uZ0;
    double vStart = vZ0, vEnd = vZ0;
    double iZStart = iZ0, iZEnd = iZ0;
    for (int i = t.points[0].y; i < t.points[1].y; i++)
    {
        drawHorizontalLine1((int)xStart, (int)xEnd, zStart, zEnd, i, uStart, uEnd, vStart, vEnd, iZStart, iZEnd, textureBuffer, texWidth, texHeight, lightFactor);
        xStart += m02;
        xEnd += m01;
        zStart += zM02;
        zEnd += zM01;
        uStart += uZM02;
        uEnd += uZM01;
        vStart += vZM02;
        vEnd += vZM01;
        iZStart += izM02;
        iZEnd += izM01;
    }
    xEnd = t.points[1].x;
    zEnd = t.points[1].z;
    uEnd = uZ1;
    vEnd = vZ1;
    iZEnd = iZ1;
    for (int i = t.points[1].y; i < t.points[2].y; i++){
        drawHorizontalLine1((int)xStart, (int)xEnd, zStart, zEnd, i, uStart, uEnd, vStart, vEnd, iZStart, iZEnd, textureBuffer, texWidth, texHeight, lightFactor);
        xStart += m02;
        xEnd += m12;
        zStart += zM02;
        zEnd += zM12;
        uStart += uZM02;
        uEnd += uZM12;
        vStart += vZM02;
        vEnd += vZM12;
        iZStart += izM02;
        iZEnd += izM12;
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