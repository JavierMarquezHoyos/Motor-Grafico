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

int clippingTriangle(Triangle t, Triangle* clippedTriangles){
    Vector3D pointsOut[3];
    Vector3D pointsIn[4];
    Vector2D uVsOut[3];
    Vector2D uVsIn[4];
    int numOut = 0;
    int numIn = 0;
    for(int i = 0; i < 3; i++){
        if(t.points[i].z < 0.1f){
            pointsOut[numOut] = t.points[i];
            uVsOut[numOut] = t.uVs[i];
            numOut++;
        }else{
            pointsIn[numIn] = t.points[i];
            uVsIn[numIn] = t.uVs[i];
            numIn++;
        }
    }

    if(numIn == 3){
        // All points are on the camera, so the triangle is not visible
        clippedTriangles[0] = t;
        return 1;
    }else if(numIn == 2)
    {
        // Two points are on the camera, so we need to clip the triangle
        float aux = (0.1f -pointsIn[0].z)/(pointsOut[0].z - pointsIn[0].z);
        clippedTriangles[0].points[0] = pointsIn[0];
        clippedTriangles[0].points[1] = pointsIn[1];
        clippedTriangles[0].points[2] = init3D(pointsIn[0].x + aux * (pointsOut[0].x - pointsIn[0].x), pointsIn[0].y + aux * (pointsOut[0].y - pointsIn[0].y), 0.1f);
        
        clippedTriangles[0].uVs[0] = uVsIn[0];
        clippedTriangles[0].uVs[1] = uVsIn[1];
        clippedTriangles[0].uVs[2] = init(uVsIn[0].x + aux * (uVsOut[0].x - uVsIn[0].x), uVsIn[0].y + aux * (uVsOut[0].y - uVsIn[0].y));

        clippedTriangles[1].points[0] = pointsIn[1];
        clippedTriangles[1].points[1] = clippedTriangles[0].points[2];
        aux = (0.1f -pointsIn[1].z)/(pointsOut[0].z - pointsIn[1].z);
        clippedTriangles[1].points[2] = init3D(pointsIn[1].x + aux * (pointsOut[0].x - pointsIn[1].x), pointsIn[1].y + aux * (pointsOut[0].y - pointsIn[1].y), 0.1f);

        clippedTriangles[1].uVs[0] = uVsIn[1];
        clippedTriangles[1].uVs[1] = clippedTriangles[0].uVs[2];
        clippedTriangles[1].uVs[2] = init(uVsIn[1].x + aux * (uVsOut[0].x - uVsIn[1].x), uVsIn[1].y + aux * (uVsOut[0].y - uVsIn[1].y));
        return 2;
    }
    else if (numIn == 1)
    {
        // One point is on the camera, so we need to clip the triangle
        float aux = (0.1f -pointsIn[0].z)/(pointsOut[0].z - pointsIn[0].z);
        clippedTriangles[0].points[0] = pointsIn[0];
        clippedTriangles[0].points[1] = init3D(pointsIn[0].x + aux * (pointsOut[0].x - pointsIn[0].x), pointsIn[0].y + aux * (pointsOut[0].y - pointsIn[0].y), 0.1f);
        clippedTriangles[0].uVs[0] = uVsIn[0];
        clippedTriangles[0].uVs[1] = init(uVsIn[0].x + aux * (uVsOut[0].x - uVsIn[0].x), uVsIn[0].y + aux * (uVsOut[0].y - uVsIn[0].y));
        aux = (0.1f -pointsIn[0].z)/(pointsOut[1].z - pointsIn[0].z);
        clippedTriangles[0].points[2] = init3D(pointsIn[0].x + aux * (pointsOut[1].x - pointsIn[0].x), pointsIn[0].y + aux * (pointsOut[1].y - pointsIn[0].y), 0.1f);
        clippedTriangles[0].uVs[2] = init(uVsIn[0].x + aux * (uVsOut[1].x - uVsIn[0].x), uVsIn[0].y + aux * (uVsOut[1].y - uVsIn[0].y));

        return 1;
    }
    else
    {
        return 0; // All points are behind the camera, so the triangle is not visible   
    }
}