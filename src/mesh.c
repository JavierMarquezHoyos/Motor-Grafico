#include "mesh.h"
//Counter-Clockwise o CCW
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

    int triangleInd[12][3] = {
        {4, 5, 6}, {4, 6, 7}, // Front face
        {1, 0, 3}, {1, 3, 2}, // Back face
        {7, 6, 2}, {7, 2, 3}, // Top face
        {0, 1, 5}, {0, 5, 4}, // Bottom face
        {5, 1, 2}, {5, 2, 6}, // Right face
        {0, 4, 7}, {0, 7, 3}   // Left face
    };

    for (int i = 0; i < mesh.numTriangles; i++) {
        for (int j = 0; j < 3; j++) {
            mesh.triangles[i].points[j] = vertices[triangleInd[i][j]];
        }
    }
    return mesh;
}