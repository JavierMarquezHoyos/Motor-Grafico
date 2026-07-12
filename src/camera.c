#include "camera.h"

Camera camera;


Camera initializeCamera() {
    camera.position = (Vector3D){0, 0, 0};
    camera.fov = 360.0f;
    return camera;
}

Vector3D worldToScreen(Vector3D worldPoint, float widthWindow, float heightWindow) {
    Vector3D screenPoint;

    screenPoint.x = (worldPoint.x * camera.fov) / worldPoint.z + (widthWindow / 2.0);//decimal division  high cost
    screenPoint.y = (worldPoint.y * camera.fov) / worldPoint.z + (heightWindow / 2.0);
    screenPoint.z = worldPoint.z;
    return screenPoint;
}