#ifndef MESH_H
#define MESH_H
#include "triangle.h"

typedef struct{
    int numTriangles;
    Triangle* triangles;
    uint32_t* textureBuffer;
    int textureWidth;
    int textureHeight;
} Mesh;

Mesh loadCubeMeshData(void);
Mesh loadMeshFromOBJ(char* filename);
void freeMesh(Mesh* mesh);

extern Mesh mesh;
#endif // MESH_H