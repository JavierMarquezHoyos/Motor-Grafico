#ifndef DISPLAY_H
#define DISPLAY_H
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

extern int width;
extern int height;

extern uint32_t *colorBuffer;
extern double *zBuffer;

bool initDisplay(int width1, int height1);   // Initialize the display with the given width and height
void clearDisplay(uint32_t color); // Clear the display with the given color
void drawPixel(int x, int y, double z, uint32_t color); // Draw a pixel at (x, y) with the given color
bool updateDisplay(); // Update the display with the current color buffer
void freeDisplay(); // Turn off display and frees the color buffer
void drawRectangle(int x, int y, int width, int height, uint32_t color);
void drawLine(int x1, int y1, double z1, int x2, int y2, double z2, uint32_t color);
void drawHorizontalLine(int x1, int x2,double z1, double z2, int y, uint32_t color);
void drawFilledTriangle(int x0, int y0, double z0, int x1, int y1, double z1 , int x2, int y2, double z2, uint32_t color);

#endif // DISPLAY_H