#include <stdio.h>
#include <stdlib.h>
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

Mesh loadMeshFromOBJ(char* filename)
{
    FILE* file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Error: Could not open file %s\n", filename);
        exit(EXIT_FAILURE);
    }

    // Count the number of vertices and faces in the OBJ file
    int numVertices = 0;
    int numFaces = 0;
    char line[256];
    while (fgets(line, sizeof(line), file)) {
        if (line[0] == 'v' && line[1] == ' ') {
            numVertices++;
        } else if (line[0] == 'f' && line[1] == ' ') {
            numFaces++;
        }
    }

    // Allocate memory for vertices and triangles
    Vector3D* vertices = (Vector3D*)malloc(numVertices * sizeof(Vector3D));
    mesh.numTriangles = numFaces;
    mesh.triangles = (Triangle*)malloc(mesh.numTriangles * sizeof(Triangle));

    // Reset file pointer to the beginning of the file
    if(fseek(file, 0, SEEK_SET) != 0) {
        fprintf(stderr, "Error: Could not seek to beginning of file %s\n", filename);
        exit(EXIT_FAILURE);
    }

    // Read vertices from the OBJ file
    int vertexIndex = 0;
    int faceIndex = 0;
    while (fgets(line, sizeof(line), file)) {
        if (line[0] == 'v' && line[1] == ' ') {
            sscanf(line, "v %f %f %f", &vertices[vertexIndex].x, &vertices[vertexIndex].y, &vertices[vertexIndex].z);
            vertexIndex++;
        }
        else if (line[0] == 'f' && line[1] == ' ') {
            int v1, v2, v3;
            sscanf(line, "f %d/%*d/%*d %d/%*d/%*d %d/%*d/%*d", &v1, &v2, &v3);
            mesh.triangles[faceIndex].points[0] = vertices[v1 - 1]; // OBJ indices are 1-based
            mesh.triangles[faceIndex].points[1] = vertices[v2 - 1];
            mesh.triangles[faceIndex].points[2] = vertices[v3 - 1];
            faceIndex++;
        }
    }

    free(vertices);
    fclose(file);
    return mesh;

}