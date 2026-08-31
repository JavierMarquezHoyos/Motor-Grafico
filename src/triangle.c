#include "triangle.h"

Vector3D triangleNormal(Triangle triangle) {
    Vector3D vec01 = rest3D(triangle.points[1], triangle.points[0]);
    Vector3D vec02 = rest3D(triangle.points[2], triangle.points[0]);
    Vector3D normal = crossProduct3D(vec01, vec02);
    return normalize3D(normal);
}
    
Vector3D triangleNormal2(Vector3D p0, Vector3D p1, Vector3D p2) {
    Vector3D vec01 = rest3D(p1, p0);
    Vector3D vec02 = rest3D(p2, p0);
    Vector3D normal = crossProduct3D(vec01, vec02);
    return normalize3D(normal);
}