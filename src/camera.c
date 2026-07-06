#include "camera.h"

Camera camera;


Camera initializeCamera() {
    camera.position = (Vector3D){0, 0, 0};
    camera.fov = 180.0f;
    return camera;
}

Vector2D worldToScreen(Vector3D worldPoint, float widthWindow, float heightWindow) {
    Vector2D screenPoint;

    screenPoint.x = (worldPoint.x * camera.fov) / worldPoint.z + (widthWindow / 2.0);
    screenPoint.y = (worldPoint.y * camera.fov) / worldPoint.z + (heightWindow / 2.0);

    return screenPoint;
}