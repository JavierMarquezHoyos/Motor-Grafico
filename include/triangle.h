#ifndef TRIANGLE_H
#define TRIANGLE_H
#include "vector.h"

typedef struct {
    Vector3D points[3];// Array of 3 points representing the vertices of the triangle
    Vector2D uVs[3];// Array of 3 UV coordinates corresponding to the vertices of the triangle
} Triangle;

Vector3D triangleNormal(Triangle triangle);
Vector3D triangleNormal2(Vector3D p0, Vector3D p1, Vector3D p2);

#endif // TRIANGLE_H
