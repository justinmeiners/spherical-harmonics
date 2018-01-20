#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "texture.h"

#define STB_IMAGE_IMPLEMENTATION
#define STBI_NO_LINEAR 1
#define STBI_NO_PSD 1
#define STBI_NO_HDR 1
#define STBI_NO_PIC 1
#define STBI_NO_PNM 1

#include "stb_image.h"


Texture texture_load(const char* path)
{
    int x, y, comp;
    unsigned char* data = stbi_load(path, &x, &y, &comp, 0);

    Texture texture;
    texture.width = x;
    texture.height = y;
    texture.data = data;
    texture.image_count = 1;
    return texture;
}

Texture cubemap_load(const char** paths)
{
    int x, y, comp;
    unsigned char* data = stbi_load(paths[0], &x, &y, &comp, 0);


    Texture texture;
    texture.width = x;
    texture.height = y;
    texture.image_count = CUBE_FACE_COUNT;

    size_t image_size = x * y * comp;
    texture.data = malloc(image_size * CUBE_FACE_COUNT);
    memcpy(texture.data, data, image_size);

    texture.image_info[0].offset = 0;
    texture.image_info[0].size = image_size;

    for (int i = 1; i < CUBE_FACE_COUNT; ++i)
    {
        texture.image_info[i].offset = image_size * i;
        texture.image_info[i].size = image_size;

        data = stbi_load(paths[i], &x, &y, &comp, 0);

        assert(x == texture.width && y == texture.height);
        memcpy(texture.data + image_size * i, data, image_size);
    }

    return texture;
}




