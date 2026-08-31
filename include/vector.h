#ifndef VECTOR_H
#define VECTOR_H
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

typedef struct {
    float x;
    float y;
} Vector2D;

typedef struct {
    float x;
    float y;
    float z;
} Vector3D;

// Function declarations for 2D vectors
Vector2D init(float x1, float y1);
Vector2D sum(Vector2D v1, Vector2D v2);
Vector2D rest(Vector2D v1, Vector2D v2);
Vector2D mult(Vector2D v, float scalar);
Vector2D normalize(Vector2D v);
double dotProduct2D(Vector2D v1, Vector2D v2);
double mod2D(Vector2D v);
float getX(Vector2D v);
float getY(Vector2D v);
// Function declarations for 3D vectors
Vector3D init3D(float x1, float y1, float z1);
Vector3D sum3D(Vector3D v1, Vector3D v2);
Vector3D rest3D(Vector3D v1, Vector3D v2);
Vector3D mult3D(Vector3D v, float scalar);
Vector3D normalize3D(Vector3D v);
Vector3D crossProduct3D(Vector3D v1, Vector3D v2);
double dotProduct3D(Vector3D v1, Vector3D v2);
double mod3D(Vector3D v);
float get3DX(Vector3D v);
float get3DY(Vector3D v);
float get3DZ(Vector3D v);

#endif // VECTOR_H