#include "mesh.h"

Mesh mesh;

Mesh loadCubeMeshData(void)
{
    mesh.numTriangles = 12;
    mesh.triangles = (Triangle*)malloc(mesh.numTriangles * sizeof(Triangle));
    //IA its very stupid to do this manually
    // Define the vertices of the cube
    Vector3D vertices[8] = {
        { -1.0f, -1.0f, -1.0f }, // 0
        {  1.0f, -1.0f, -1.0f }, // 1
        {  1.0f,  1.0f, -1.0f }, // 2
        { -1.0f,  1.0f, -1.0f }, // 3
        { -1.0f, -1.0f,  1.0f }, // 4
        {  1.0f, -1.0f,  1.0f }, // 5
        {  1.0f,  1.0f,  1.0f }, // 6
        { -1.0f,  1.0f,  1.0f }  // 7
    };

    // Define the triangles of the cube (two triangles per face)
    int triangleInd[12][3] = {
        {0, 1, 2}, {2, 3, 0}, // Back face
        {4, 5, 6}, {6, 7, 4}, // Front face
        {4, 5, 1}, {1, 0, 4}, // Bottom face
        {7, 6, 2}, {2, 3, 7}, // Top face
        {4, 7, 3}, {3, 0, 4}, // Left face
        {5, 6, 2}, {2, 1, 5}   // Right face
    };

    for (int i = 0; i < mesh.numTriangles; i++) {
        for (int j = 0; j < 3; j++) {
            mesh.triangles[i].points[j] = vertices[triangleInd[i][j]];
        }
    }
    return mesh;
}