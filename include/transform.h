#ifndef TRANSFORM_H
#define TRANSFORM_H
#include "vector.h"
#include <math.h>
#include "triangle.h"
#include "camera.h"

Vector3D rotateX(Vector3D v, float angle);
Vector3D rotateY(Vector3D v, float angle);
Vector3D rotateZ(Vector3D v, float angle);
Triangle applyTransformations(Triangle t, Vector3D movement, Camera cam, float angle);

#endif // TRANSFORM_H