#include "transform.h"


#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

Triangle applyTransformations(Triangle t, Vector3D movement, Camera cam, float angle) {
    Triangle result = t;
    for (int i = 0; i < 3; i++) {
        Vector3D aux = rotateY(t.points[i], angle);
        aux = sum3D(aux, movement);
        aux = rest3D(aux, cam.position);
        aux = rotateY(aux, -cam.yaw);
        aux = rotateX(aux, -cam.pitch);
        result.points[i] = aux;
    }
    return result;
}

Vector3D rotateX(Vector3D v, float angle) {
    Vector3D result;
    float rad = angle * (M_PI / 180.0f);
    result.x = v.x;
    result.y = v.y * cos(rad) - v.z * sin(rad);
    result.z = v.y * sin(rad) + v.z * cos(rad);
    return result;
}

Vector3D rotateY(Vector3D v, float angle) {
    Vector3D result;
    float rad = angle * (M_PI / 180.0f);
    result.x = v.x * cos(rad) + v.z * sin(rad);
    result.y = v.y;
    result.z = -v.x * sin(rad) + v.z * cos(rad);
    return result;
}

Vector3D rotateZ(Vector3D v, float angle) {
    Vector3D result;
    float rad = angle * (M_PI / 180.0f);
    result.x = v.x * cos(rad) - v.y * sin(rad);
    result.y = v.x * sin(rad) + v.y * cos(rad);
    result.z = v.z;
    return result;
}