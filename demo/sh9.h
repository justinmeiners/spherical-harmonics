
#ifndef SH_9_H
#define SH_9_H

#include <math.h>
#include <assert.h>
/*

SAMPLE SHADER

Here is a sample shader function for evaluting the spherical harmonics. The functions are the same as in this header, but in numerical form. The 9 coeffecients for each color are stored in a 3x3 matrix.

@{sample shader]

*/


#define SH_COUNT 9

typedef struct
{
    double coeffs[SH_COUNT];
} ShChannel;

static inline double sh_eval_9(int i, double x, double y, double z)
{
    switch (i)
    {
        case 0:
            return 0.5 * sqrt(1.0 / M_PI);
        case 1:
            return x * 0.5 * sqrt(3.0 / M_PI);
        case 2:
            return z * 0.5 * sqrt(3.0 / M_PI);
        case 3:
            return y * 0.5 * sqrt(3.0 / M_PI);
        case 4:
            return x * z * 0.5 * sqrt(15.0 / M_PI);
        case 5:
            return y * z * 0.5 * sqrt(15.0 / M_PI);
        case 6:
            return x * y * 0.5 * sqrt(15.0 / M_PI);
        case 7:
            return (3.0 * z*z - 1.0) * 0.25 * sqrt(5.0 / M_PI);
        case 8:
            return (x*x - y*y) * 0.25 * sqrt(15.0 / M_PI);
        default:
            assert(0);
            return 0;
    }
}


void sh_integrate_cubemap(const unsigned char** face_data,
                          unsigned int width,
                          unsigned int height,
                          unsigned int components_per_pixel,
                          ShChannel* out_channels);

#endif

