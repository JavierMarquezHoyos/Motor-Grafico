#include "camera.h"
#include <math.h>

#define M_PI 3.14159265358979323846


Camera initializeCamera() {
    Camera camera;
    camera.position = (Vector3D){0, 0, 0};
    camera.fov = 360.0f;
    camera.yaw = 0; // Initialize yaw to 0
    camera.pitch = 0; // Initialize pitch to 0
    camera.vel = 5.0f; //
    return camera;
}

Vector3D worldToScreen(Camera *camera, Vector3D worldPoint, float widthWindow, float heightWindow) {
    Vector3D screenPoint;
    
    screenPoint.x = (worldPoint.x * camera->fov) / worldPoint.z + (widthWindow / 2.0);//decimal division  high cost
    screenPoint.y = (worldPoint.y * camera->fov) / worldPoint.z + (heightWindow / 2.0);
    screenPoint.z = worldPoint.z;
    return screenPoint;
}

void updateCameraRotation(Camera *camera, float deltaYaw, float deltaPitch) {
    float sensitivity = 0.1f; // Adjust this value to control the sensitivity of the mouse movement
    
    camera->yaw += deltaYaw * sensitivity;
    camera->pitch -= deltaPitch * sensitivity;

    // Clamp the pitch to avoid flipping the camera
    if (camera->pitch > 89.0f)
        camera->pitch = 89.0f;
    if (camera->pitch < -89.0f)
        camera->pitch = -89.0f;

}

void updateCameraPosition(Camera *camera, float advance, float lateral, float deltaTime) {
    // Calculate the forward vector based on yaw and pitch
    camera->position.x += advance * camera->vel * deltaTime * sinf(camera->yaw * (M_PI / 180.0f));
    camera->position.z += advance * camera->vel * deltaTime * cosf(camera->yaw * (M_PI / 180.0f));
    camera->position.x += lateral * camera->vel * deltaTime * cosf(camera->yaw * (M_PI / 180.0f));
    camera->position.z += lateral * camera->vel * deltaTime * -sinf(camera->yaw * (M_PI / 180.0f));    
    
}