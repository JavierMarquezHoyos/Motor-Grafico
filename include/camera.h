#ifndef CAMERA_H
#define CAMERA_H
#include "vector.h"

typedef struct {
    Vector3D position;
    //this will be used when I allow the camera to rotate, for now it will be fixed
    //Vector3D direction;
    //Vector3D up;
    float fov;
} Camera;

extern Camera camera;

Camera initializeCamera();
Vector2D worldToScreen(Vector3D worldPoint, float widthWindow, float heightWindow);

#endif // CAMERA_H
