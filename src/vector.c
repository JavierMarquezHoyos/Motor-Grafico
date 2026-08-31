#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "vector.h"


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

Vector2D rest(Vector2D v1, Vector2D v2){
    Vector2D v;
    v.x = v1.x - v2.x;
    v.y = v1.y - v2.y;
    return v;
}

Vector2D mult(Vector2D v, float scalar){
    Vector2D result;
    result.x = v.x * scalar;
    result.y = v.y * scalar;
    return result;
}

Vector2D normalize(Vector2D v){
    double magnitude = mod2D(v);
    if (magnitude == 0) {
        return init(0, 0); // Return a zero vector if the input vector is zero
    }
    return mult(v, 1.0 / magnitude);
}

double dotProduct2D(Vector2D v1, Vector2D v2){
    return v1.x * v2.x + v1.y * v2.y;
}

double mod2D(Vector2D v){
    return sqrt(v.x * v.x + v.y * v.y);
}

Vector3D init3D(float x1, float y1, float z1){
    Vector3D v;
    v.x = x1;
    v.y = y1;
    v.z = z1;
    return v;
}

float get3DX(Vector3D v){
    return v.x;
}

float get3DY(Vector3D v){
    return v.y;
}

float get3DZ(Vector3D v){
    return v.z;
}

Vector3D sum3D(Vector3D v1, Vector3D v2){
    Vector3D v;
    v.x = v1.x + v2.x;
    v.y = v1.y + v2.y;
    v.z = v1.z + v2.z;
    return v;
}

Vector3D rest3D(Vector3D v1, Vector3D v2){
    Vector3D v;
    v.x = v1.x - v2.x;
    v.y = v1.y - v2.y;
    v.z = v1.z - v2.z;
    return v;
}

Vector3D mult3D(Vector3D v, float scalar){
    Vector3D result;
    result.x = v.x * scalar;
    result.y = v.y * scalar;
    result.z = v.z * scalar;
    return result;
}

Vector3D normalize3D(Vector3D v){
    double magnitude = mod3D(v);
    if (magnitude == 0) {
        return init3D(0, 0, 0); // Return a zero vector if the input vector is zero
    }
    return mult3D(v, 1.0 / magnitude);
}

Vector3D crossProduct3D(Vector3D v1, Vector3D v2){
    Vector3D result;
    result.x = v1.y * v2.z - v1.z * v2.y;
    result.y = v1.z * v2.x - v1.x * v2.z;
    result.z = v1.x * v2.y - v1.y * v2.x;
    return result;
}

double dotProduct3D(Vector3D v1, Vector3D v2){
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

double mod3D(Vector3D v){
    return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}
