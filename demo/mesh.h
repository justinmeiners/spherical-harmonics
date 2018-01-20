#ifndef MESH_H
#define MESH_H

#include "lin_math.h"

typedef struct
{
    Vec3 point;
    Vec3 normal;
} MeshVert;

typedef struct
{
    unsigned int vao;
    unsigned int vbo; 
    unsigned int vertex_count;
    MeshVert* vertices;
} Mesh;

Mesh mesh_load(const char* path);


#endif
