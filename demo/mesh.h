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

enum 
{
    CUBE_FACE_RIGHT = 0, // x+
    CUBE_FACE_LEFT,  // x-
    CUBE_FACE_TOP,   // y+
    CUBE_FACE_BOTTOM,// y-
    CUBE_FACE_BACK,  // z+
    CUBE_FACE_FRONT, // z-
    CUBE_FACE_COUNT
} CubeFace;

typedef struct
{
    int pbo;
    unsigned short width;
    unsigned short height;

    struct
    {
        size_t offset;
        size_t size;
    } image_info[32];
    unsigned int image_count;
    
    unsigned char* data;
} Texture;

Mesh mesh_load(const char* path);

Texture texture_load(const char* path);
Texture cubemap_load(const char** path);


#endif

