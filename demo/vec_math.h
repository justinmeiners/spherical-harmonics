#ifndef VEC_MATH_H
#define VEC_MATH_H

typedef struct
{
    float x, y, z;
} Vec3;

static inline Vec3 vec3_create(float x, float y, float z)
{
    Vec3 r;
    r.x = x;
    r.y = y;
    r.z = z;
    return r;
}

static inline Vec3 vec3_scale(Vec3 a, float s)
{
    a.x *= s;
    a.y *= s;
    a.z *= s;
    return a;
}

static inline Vec3 vec3_add(Vec3 a, Vec3 b)
{
    a.x += b.x;
    a.y += b.y;
    a.z += b.z;
    return a;
}

static inline Vec3 vec3_sub(Vec3 a, Vec3 b)
{
    a.x -= b.x;
    a.y -= b.y;
    a.z -= b.z;
    return a;
}

static inline float vec3_dot(Vec3 a, Vec3 b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}



#endif
