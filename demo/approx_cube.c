#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "texture.h"

#define SH_COUNT 9

typedef struct
{
    float r[SH_COUNT];
    float g[SH_COUNT];
    float b[SH_COUNT];
} ShCoeffs;

static double sh9(int i, double x, double y, double z)
{
    switch (i)
    {
        case 0:
            return 0.5 * sqrt(1.0 / M_PI);
        case 1:
            return y * 0.5 * sqrt(3.0 / M_PI);
        case 2:
            return x * 0.5 * sqrt(3.0 / M_PI);
        case 3:
            return z * 0.5 * sqrt(3.0 / M_PI);
        case 4:
            return x * y * 0.5 * sqrt(15.0 / M_PI);
        case 5:
            return y * z * 0.5 * sqrt(15.0 / M_PI);
        case 6:
            return (2.0 * z*z - x*x - y*y) * 0.25 * sqrt(5.0 / M_PI);
        case 7:
            return x * z * 0.5 * sqrt(15.0 / M_PI);
        case 8:
            return (x*x - y*y) * 0.5 * sqrt(15.0 / M_PI);
        default:
            assert(0);
            return 0;
    }
}

static ShCoeffs integrate_cubemap(Texture* cubemap)
{
    ShCoeffs coeffs;

    for (int i = 0; i < SH_COUNT; ++i)
    {
        coeffs.r[i] = 0.0;
        coeffs.g[i] = 0.0;
        coeffs.b[i] = 0.0;
    }

    for (int face = 0; face < CUBE_FACE_COUNT; ++face)
    {
        size_t offset = cubemap->image_info[face].offset;

        for (int y = 0; y < cubemap->height; ++y)
        {
            for (int x = 0; x < cubemap->width; ++x)
            {
                double uv_x = ((x / (double)cubemap->width) - 0.5f) * 2.0f;
                double uv_y = ((y / (double)cubemap->height) - 0.5f) * 2.0f;

                double dir_x, dir_y, dir_z;
                switch (face)
                {
                    case CUBE_FACE_RIGHT:
                        dir_x = 1.0f;
                        dir_y = uv_y;
                        dir_z = -uv_x;
                        break;
                    case CUBE_FACE_LEFT:
                        dir_x = -1.0f;
                        dir_y = uv_y;
                        dir_z = uv_x;
                        break;
                    case CUBE_FACE_TOP:
                        dir_x = uv_x;
                        dir_y = 1.0f;
                        dir_z = -uv_y; 
                        break;
                    case CUBE_FACE_BOTTOM:
                        dir_x = uv_x;
                        dir_y = -1.0f;
                        dir_z = uv_y;
                        break;
                    case CUBE_FACE_BACK: 
                        dir_x = uv_x;
                        dir_y = uv_y;
                        dir_z = 1.0f;
                        break;
                    case CUBE_FACE_FRONT:
                        dir_x = -uv_x;
                        dir_y = uv_y;
                        dir_z = -1.0f;
                        break;
                }

                size_t pixel_start = offset + x + y * cubemap->width * 3;
                double red = cubemap->data[pixel_start] / 255.0;
                double green = cubemap->data[pixel_start + 1] / 255.0;
                double blue = cubemap->data[pixel_start + 2] / 255.0;

                double weight = 1.0 / ((double)cubemap->width * (double)cubemap->height * 6.0);

                for (int i = 0; i < SH_COUNT; ++i)
                {
                    double sh_val = sh9(i, dir_x, dir_y, dir_z);
                    coeffs.r[i] += sh_val * red * weight;
                    coeffs.g[i] += sh_val * green * weight;
                    coeffs.b[i] += sh_val * blue * weight;
                }
            }
        }
    }
    return coeffs;
}


int main(int argc, const char* argv[])
{
    const char* paths[] = {"data/cube1/posx.jpg",
                           "data/cube1/negx.jpg",
                           "data/cube1/posy.jpg",
                           "data/cube1/negy.jpg",
                           "data/cube1/posz.jpg",
                           "data/cube1/negz.jpg"};

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
        printf("%f, ", coeffs.g[i]);
    }
    printf("}\n");

    return 1;
}


