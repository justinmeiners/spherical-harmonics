#include <stdlib.h>
#include <stdio.h>
#include "sh9.h"
#include "texture.h"

int main(int argc, const char* argv[])
{
    const char* paths[] = {"data/cube1/posx.jpg",
                           "data/cube1/negx.jpg",
                           "data/cube1/posy.jpg",
                           "data/cube1/negy.jpg",
                           "data/cube1/posz.jpg",
                           "data/cube1/negz.jpg"};

    Texture cube = cubemap_load(paths);

    unsigned char* face_data[CUBE_FACE_COUNT];
    for (int i = 0; i < 6; ++i)
    {
        face_data[i] = cube.data + cube.image_info[i].offset;
    }

    ShChannel channels[3];
    sh_integrate_cubemap(face_data, cube.width, cube.height, 3, channels);

    char* channel_names [] = { "red", "green", "blue" };
    for (int comp = 0; comp < 3; ++comp)
    {
        printf("%s {", channel_names[comp]);
        for (int s = 0; s < SH_COUNT; ++s)
        {
            printf("%lf, ", channels[comp].coeffs[s]);

        }
        printf("}\n");
    }
    return 1;
}


