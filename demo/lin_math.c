
#include <memory.h>
#include <stdio.h>
#include "lin_math.h"


Vec2 vec2_create(float x, float y)
{
    Vec2 r;
    r.x = x;
    r.y = y;
    return r;
}


Vec3 vec3_create(float x, float y, float z)
{
    Vec3 r;
    r.x = x;
    r.y = y;
    r.z = z;
    return r;
}

Vec3 vec3_add(Vec3 a, Vec3 b)
{
    a.x += b.x;
    a.y += b.y;
    a.z += b.z;
    return a;
}

Vec3 vec3_sub(Vec3 a, Vec3 b)
{
    a.x -= b.x;
    a.y -= b.y;
    a.z -= b.z;
    return a;
}

Vec3 vec3_scale(Vec3 a, float c)
{
    a.x *= c;
    a.y *= c;
    a.z *= c;
    return a;
}

float vec3_dot(Vec3 a, Vec3 b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

Vec3 vec3_cross(Vec3 a, Vec3 b)
{
    Vec3 r;
    r.x = a.y * b.z - a.z * b.y;
    r.y = a.z * b.x - a.x * b.z;
    r.z = a.x * b.y - a.y * b.x;
    return r;
}

float vec3_norm(Vec3 a)
{
    return sqrtf(a.x * a.x + a.y * a.y + a.z * a.z);
}

Vec3 vec3_unit(Vec3 a)
{
    float c = 1.0f / vec3_norm(a);
    a.x = a.x * c;
    a.y = a.y * c;
    a.z = a.z * c;
    return a;
}

Vec3 vec3_negate(Vec3 a)
{
    a.x = -a.x;
    a.y = -a.y;
    a.z = -a.z;
    return a;
}

Vec3 vec3_abs(Vec3 a)
{
    a.x = fabs(a.x);
    a.y = fabs(a.y);
    a.z = fabs(a.z);
    return a;
}

int vec3_eq(Vec3 a, Vec3 b)
{
    return a.x == b.x && a.y == b.y && a.z == b.z;
}


void mat4_identity(Mat4* m)
{
    memset(m->m, 0, sizeof(m->m));
    m->m[0] = 1.0f;
    m->m[5] = 1.0f;
    m->m[10] = 1.0f;
    m->m[15] = 1.0f;
}

void mat4_copy(Mat4* dest, const Mat4* src)
{
    memcpy(dest->m, src->m, sizeof(src->m));
} 

Vec3 mat4_mult_vec3(Mat4* m, Vec3 v)
{
    Vec3 r;
    r.x = (v.x * m->m[0]) + (v.y * m->m[4]) + (v.z * m->m[8]) + m->m[12];
    r.y = (v.x * m->m[1]) + (v.y * m->m[5]) + (v.z * m->m[9]) + m->m[13];
    r.z = (v.x * m->m[2]) + (v.y * m->m[6]) + (v.z * m->m[10]) + m->m[14];
    return r;
}

void mat4_mult(const Mat4* a, const Mat4* b, Mat4* ab)
{
    Mat4 temp;

    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            float n = 0.0f;
            for (int k = 0; k < 4; ++k)
                n += mat4_get(b, i, k) * mat4_get(a, k, j);

            mat4_set(&temp, i, j, n);
        }
    }

    mat4_copy(ab, &temp);
}

Mat4 mat4_create_identity()
{
    Mat4 m;
    mat4_identity(&m);
    return m;
}

Mat4 mat4_create_frustum(float left, float right, float bottom, float top, float near, float far)
{
    Mat4 m = mat4_create_identity();
    
    float invWidth = 1.0f / (right - left);
    float invHeight = 1.0f / (top - bottom);
    float invDepth = 1.0f / (far - near);
    float twoZNear = 2.0f * near;
    
    mat4_set(&m, 0, 0, twoZNear * invWidth);
    mat4_set(&m, 1, 1, twoZNear * invHeight);
    
    mat4_set(&m, 2, 0, (right + left) * invWidth);
    mat4_set(&m, 2, 1, (top + bottom) * invHeight);
    mat4_set(&m, 2, 2, -(far + near) * invDepth);
    mat4_set(&m, 2, 3, -1.0f); 
    mat4_set(&m, 3, 2, - twoZNear * far * invDepth);
    mat4_set(&m, 3, 3, 0.0f);
    return m;
}

Mat4 mat4_create_perspective(float fov, float aspect, float near, float far)
{
    float tang = tanf(fov * M_PI / 180.0f * 0.5f);
    float near_h = near * tang;
    float near_w = near_h * aspect;
    return mat4_create_frustum(-near_w, near_w, -near_h, near_h, near, far);
}

Mat4 mat4_create_translate(Vec3 v)
{
    Mat4 m = mat4_create_identity();
    mat4_set(&m, 3, 0, v.x);
    mat4_set(&m, 3, 1, v.y);
    mat4_set(&m, 3, 2, v.z);
    return m;
}

Mat4 mat4_create_rotate(float angle, Vec3 axis)
{
   Mat4 m = mat4_create_identity();
     
   float sinAngle = sinf(angle);
   float cosAngle = cosf(angle);

   float x = axis.x;
   float y = axis.y;
   float z = axis.z;
   float xx = x * x;
   float yy = y * y;
   float zz = z * z;
   float xy = x * y;
   float yz = y * z;
   float zx = z * x;
   float xs = x * sinAngle;
   float ys = y * sinAngle;
   float zs = z * sinAngle;
   float oneMinusCos = 1.0f - cosAngle;

   mat4_set(&m, 0, 0, (oneMinusCos * xx) + cosAngle);
   mat4_set(&m, 0, 1, (oneMinusCos * xy) - zs);
   mat4_set(&m, 0, 2, (oneMinusCos * zx) + ys);

   mat4_set(&m, 1, 0, (oneMinusCos * xy) + zs);
   mat4_set(&m, 1, 1, (oneMinusCos * yy) + cosAngle);
   mat4_set(&m, 1, 2, (oneMinusCos * yz) - xs);

   mat4_set(&m, 2, 0, (oneMinusCos * zx) - ys);
   mat4_set(&m, 2, 1, (oneMinusCos * yz) + xs);
   mat4_set(&m, 2, 2, (oneMinusCos * zz) + cosAngle);  
   return m;
}

Mat4 mat4_create_look(Vec3 eye, Vec3 target, Vec3 up)
{
    Vec3 forward, side, u;
    Mat4 m = mat4_create_identity();
    
    forward = vec3_sub(target, eye);
    forward = vec3_unit(forward);
    side = vec3_cross(forward, up);
    side = vec3_unit(side);
    
    u = vec3_cross(side, forward);
    
    mat4_set(&m, 0, 0, side.x);
    mat4_set(&m, 1, 0, side.y);
    mat4_set(&m, 2, 0, side.z);
    
    mat4_set(&m, 0, 1, u.x);
    mat4_set(&m, 1, 1, u.y);
    mat4_set(&m, 2, 1, u.z);
    
    mat4_set(&m, 0, 2, -forward.x);
    mat4_set(&m, 1, 2, -forward.y);
    mat4_set(&m, 2, 2, -forward.z);
    
    Mat4 translate = mat4_create_translate(vec3_negate(eye)); 
    mat4_mult(&m, &translate, &m);
    return m;
}

void mat4_print(const Mat4* m)
{
    printf("[");

    for (int i = 0; i < 4; ++i)
    {
        printf("(");
        for (int j = 0; j < 4; ++j)
        {
            printf("%f, ", m->m[i + j * 4]);
        }
        printf(")");
    }

    printf("]\n");
}



