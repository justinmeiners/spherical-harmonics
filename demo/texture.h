#ifndef TEXTURE_H
#define TEXTURE_H

#include <stdlib.h>

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

Texture texture_load(const char* path);
Texture cubemap_load(const char** path);


#endif

