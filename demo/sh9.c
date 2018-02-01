#include "sh9.h"
#include <stdlib.h>
#include <stdio.h>
#include "texture.h"

static inline void normalize(double* dir, int n)
{
    double length_sqr = 0.0f;
    for (int i = 0; i < n; ++i)
        length_sqr += dir[i] * dir[i];

    double scale = 1.0 / sqrt(length_sqr);
    for (int i = 0; i < n; ++i)
        dir[i] *= scale;
}

static inline void uv_to_cube(double u, double v, int face, double* out_dir)
{
    switch (face)
    {
        case CUBE_FACE_RIGHT:
            out_dir[0] = 1.0f;
            out_dir[1] = v;
            out_dir[2] = -u;
            break;
        case CUBE_FACE_LEFT:
            out_dir[0] = -1.0f;
            out_dir[1] = v;
            out_dir[2] = u;
            break;
        case CUBE_FACE_TOP:
            out_dir[0] = u;
            out_dir[1] = 1.0f;
            out_dir[2] = -v; 
            break;
        case CUBE_FACE_BOTTOM:
            out_dir[0] = u;
            out_dir[1] = -1.0f;
            out_dir[2] = v;
            break;
        case CUBE_FACE_BACK: 
            out_dir[0] = u;
            out_dir[1] = v;
            out_dir[2] = 1.0f;
            break;
        case CUBE_FACE_FRONT:
            out_dir[0] = -u;
            out_dir[1] = v;
            out_dir[2] = -1.0f;
            break;
    }
}


static double surface_area(double x, double y)
{
    return atan2(x * y, sqrt(x * x + y * y + 1.0));
}



void sh_integrate_cubemap(const unsigned char** face_data,
                          unsigned int width,
                          unsigned int height,
                          unsigned int components_per_pixel,
                          ShChannel* out_channels)
{
    // zero out coeffecients for accumulation
    for (int comp = 0; comp < components_per_pixel; ++comp)
    {
        for (int s = 0; s < SH_COUNT; ++s)
            out_channels[comp].coeffs[s] = 0.0;
    }

    for (int face = 0; face < CUBE_FACE_COUNT; ++face)
    {
        for (int y = 0; y < height; ++y)
        {
            for (int x = 0; x < width; ++x)
            {
                // center each pixel
                double px = (double)x + 0.5;
                double py = (double)y + 0.5;
                // normalize into [-1, 1] range
                double u = 2.0 * (px / (double)width) - 1.0;
                double v = 2.0 * (py / (double)height) - 1.0;

                // calculate the solid angle
                double d_x = 1.0 / (double)width;
                double x0 = u - d_x;
                double y0 = v - d_x;
                double x1 = u + d_x;
                double y1 = v + d_x;
                double d_a = surface_area(x0, y0) - surface_area(x0, y1) - surface_area(x1, y0) + surface_area(x1, y1);

                // find vector on unit sphere
                double dir[3];
                uv_to_cube(u, v, face, dir);
                normalize(dir, 3);
  
                size_t pixel_start = (x + y * width) * components_per_pixel;

                for (int s = 0; s < SH_COUNT; ++s)
                {
                    double sh_val = sh_eval_9(s, dir[0], dir[1], dir[2]);

                    for (int comp = 0; comp < components_per_pixel; ++comp)
                    {
                        double col = face_data[face][pixel_start + comp] / 255.0;
                        out_channels[comp].coeffs[s] += col * sh_val * d_a;
                    }
                }
           }
        }
    }
}




