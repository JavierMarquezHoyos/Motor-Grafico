#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

extern int width;
extern int height;

extern uint32_t *colorBuffer;

bool initDisplay();    // Initialize the display with the given width and height
void clearDisplay(uint32_t color); // Clear the display with the given color
void drawPixel(int x, int y, uint32_t color); // Draw a pixel at (x, y) with the given color
bool updateDisplay(); // Update the display with the current color buffer
void freeDisplay(); // Turn off display and frees the color buffer
void drawRectangle(int x, int y, int width, int height, uint32_t color);