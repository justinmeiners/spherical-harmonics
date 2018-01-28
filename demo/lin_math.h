#ifndef LIN_MATH_H
#define LIN_MATH_H

#include "math.h"

typedef struct
{
    float x, y;
} Vec2;

typedef struct
{
    float x, y, z;
} Vec3;

typedef struct
{
    float m[16];
} Mat4;

Vec2 vec2_create(float x, float y);

Vec3 vec3_create(float x, float y, float z);
Vec3 vec3_add(Vec3 a, Vec3 b);
Vec3 vec3_sub(Vec3 a, Vec3 b);
Vec3 vec3_scale(Vec3 a, float c);
float vec3_dot(Vec3 a, Vec3 b);
Vec3 vec3_cross(Vec3 a, Vec3 b);
float vec3_norm(Vec3 a);
Vec3 vec3_unit(Vec3 a);
Vec3 vec3_negate(Vec3 a);
Vec3 vec3_abs(Vec3 a);
int vec3_eq(Vec3 a, Vec3 b);

#define mat4_get(Mat, x, y) ((Mat)->m[((x) * 4) + (y)])
#define mat4_set(Mat, x, y, val) ((Mat)->m[((x) * 4) + (y)] = (val))
void mat4_identity(Mat4* m);
void mat4_copy(Mat4* dest, const Mat4* src);
void mat4_mult(const Mat4* a, const Mat4* b, Mat4* ab);
Vec3 mat4_mult_vec3(Mat4* m, Vec3 v);
Mat4 mat4_create_identity();
Mat4 mat4_create_frustum(float left, float right, float bottom, float top, float near, float far);
Mat4 mat4_create_perspective(float fov, float aspect, float near, float far);
Mat4 mat4_create_translate(Vec3 v);
Mat4 mat4_create_rotate(float angle, Vec3 axis);
Mat4 mat4_create_look(Vec3 eye, Vec3 target, Vec3 up);
void mat4_print(const Mat4* m);

#endif

