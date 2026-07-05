#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct {
    float x;
    float y;
} Vector2D;

Vector2D init(float x1, float y1);
Vector2D sum(Vector2D v1, Vector2D v2);
Vector2D mult(Vector2D v, float scalar);
float getX(Vector2D v);
float getY(Vector2D v);