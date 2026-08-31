#ifndef CAMERA_H
#define CAMERA_H
#include "vector.h"

typedef struct {
    Vector3D position;
    float vel;
    float yaw;// exe X
    float pitch; // exe Y
    //this will be used when I allow the camera to rotate, for now it will be fixed
    //Vector3D direction;
    //Vector3D up;
    float fov;
} Camera;


Camera initializeCamera();
Vector3D worldToScreen(Camera *camera, Vector3D worldPoint, float widthWindow, float heightWindow);
void updateCameraRotation(Camera *camera, float deltaYaw, float deltaPitch);
void updateCameraPosition(Camera *camera, float advance, float lateral, float deltaTime);

#endif // CAMERA_H
