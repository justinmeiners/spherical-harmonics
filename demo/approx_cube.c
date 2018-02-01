#include <stdlib.h>
#include <stdio.h>
#include "sh9.h"
#include "texture.h"

int main(int argc, const char* argv[])
{
    const char* paths[] = {"data/cube2/posx.jpg",
                           "data/cube2/negx.jpg",
                           "data/cube2/posy.jpg",
                           "data/cube2/negy.jpg",
                           "data/cube2/posz.jpg",
                           "data/cube2/negz.jpg"};

    Texture cube = cubemap_load(paths);
    ShCoeffs coeffs = integrate_cubemap(&cube);

    printf("red {");
    for (int i = 0; i < SH_COUNT; ++i)
    {
        printf("%f, ", coeffs.r[i]);
    }
    printf("}\n");

    printf("green {");
    for (int i = 0; i < SH_COUNT; ++i)
    {
        printf("%f, ", coeffs.g[i]);
    }
    printf("}\n");

    printf("blue {");
    for (int i = 0; i < SH_COUNT; ++i)
    {
        printf("%f, ", coeffs.b[i]);
    }
    printf("}\n");

    return 1;
}


