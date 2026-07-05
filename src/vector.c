#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct {
    float x;
    float y;
} Vector2D;

Vector2D init(float x1, float y1){
    Vector2D v;
    v.x = x1;
    v.y = y1;
    return v;
}

float getX(Vector2D v){
    return v.x;
}

float getY(Vector2D v){
    return v.y;
}

Vector2D sum(Vector2D v1, Vector2D v2){
    Vector2D v;
    v.x = v1.x + v2.x;
    v.y = v1.y + v2.y;
    return v;
}

Vector2D mult(Vector2D v, float scalar){
    Vector2D result;
    result.x = v.x * scalar;
    result.y = v.y * scalar;
    return result;
}
