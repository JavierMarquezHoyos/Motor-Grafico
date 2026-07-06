#ifndef MESH_H
#define MESH_H
#include "triangle.h"

typedef struct{
    int numTriangles;
    Triangle* triangles;
} Mesh;

Mesh loadCubeMeshData(void);

extern Mesh mesh;
#endif // MESH_H