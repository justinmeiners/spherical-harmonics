
#include <stdio.h>
#include <stdlib.h>
#include <OpenGL/gl3.h>
#include "mesh.h"
#include "texture.h"

const char* vert_shader_source = " \
#version 100\n\
uniform mat4 u_view; \
uniform mat4 u_proj; \
attribute vec4 a_vertex; \
attribute vec3 a_normal; \
varying mediump vec3 v_normal; \
\
void main() \
{ \
v_normal = a_normal; \
gl_Position = u_proj * u_view * a_vertex; \
}";

const char* frag_shader_source = " \
#version 100\n\
mediump float sh9(mediump mat3 coef, mediump vec3 n)\n \
{\n \
    mediump float sum =  \
    coef[0][0] * 0.282095 \
    + coef[1][0] * 0.488603 * n.x \
    + coef[2][0] * 0.488603 * n.z \
    + coef[0][1] * 0.488603 * n.y \
    + coef[1][1] * 1.092548 * n.x * n.z \
    + coef[2][1] * 1.092548 * n.y * n.z \
    + coef[0][2] * 1.092548 * n.x * n.y \
    + coef[1][2] * 0.315391 * (3.0 * n.z*n.z - 1.0) \
    + coef[2][2] * 0.546274 * (n.x*n.x - n.y*n.y);  \
    return sum; \
} \n \
uniform mediump mat3 sh_r; \
uniform mediump mat3 sh_g;  \
uniform mediump mat3 sh_b;  \
uniform samplerCube u_cube; \
varying mediump vec3 v_normal;\
uniform bool use_cube; \
void main() \
{ \
if (use_cube) { \
    gl_FragColor = textureCube(u_cube, v_normal); \
} else { gl_FragColor = vec4(sh9(sh_r, v_normal), sh9(sh_g, v_normal), sh9(sh_b, v_normal), 1.0); }\
}";

const float sh9_red[] = {2.306290, -0.002908, -0.077373, 0.339365, -0.076132, 0.056290, -0.020493, -0.646626, -0.112228, };
const float sh9_green[] = {2.184796, -0.000868, -0.065629, 0.377558, -0.080363, 0.052740, -0.020638, -0.620614, -0.088419, };
const float sh9_blue[] = {2.062208, 0.022677, -0.051672, 0.432432, -0.090587, 0.048866, -0.020496, -0.607719, -0.067602, };


static GLuint compile_shader(GLint shaderType, const char* shaderSource, int debug)
{
    GLuint shader = glCreateShader(shaderType);
    const char* sources[] = { shaderSource };
    glShaderSource(shader, 1, sources, NULL);
    glCompileShader(shader);

    if (debug)
    {
        GLint logLength;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);

        if (logLength > 0)
        {
            GLchar *log = (GLchar *)malloc(logLength);
            glGetShaderInfoLog(shader, logLength, &logLength, log);
            printf("%s\n", log);
        }
    }

    return shader;
}

enum
{
    ATTRIB_VERTEX = 0,
    ATTRIB_NORMAL,
};

static void upload_mesh(Mesh* mesh)
{
    GLuint vao, vbo;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    
    glBufferData(GL_ARRAY_BUFFER, sizeof(MeshVert) * mesh->vertex_count, mesh->vertices, GL_STATIC_DRAW);
 
    glEnableVertexAttribArray(ATTRIB_VERTEX);
    glVertexAttribPointer(ATTRIB_VERTEX, 3, GL_FLOAT, GL_FALSE, sizeof(MeshVert), NULL);

    glEnableVertexAttribArray(ATTRIB_NORMAL);
    glVertexAttribPointer(ATTRIB_NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(MeshVert), (char*)NULL + sizeof(Vec3));     

    mesh->vao = vao;
    mesh->vbo = vbo;
}

static void upload_cubemap(Texture* texture)
{
    GLenum internalFormat = GL_RGB;
    GLenum format = GL_RGB;

    GLuint tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_CUBE_MAP, tex);

    for (int i = 0; i < CUBE_FACE_COUNT; ++i)
    {
        unsigned char* data = texture->data + texture->image_info[i].offset;
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, texture->width, texture->height, 0, internalFormat, GL_UNSIGNED_BYTE, data);
    }

    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    texture->pbo = tex;
}


typedef struct
{
    Vec3 eye;
    Vec3 target;
    Mat4 eye_rotation;
    int view_cube;

    Mesh demo;
    Texture cube;

    GLuint object_prog;
    GLuint object_vert;
    GLuint object_frag;
} Scene;


Scene scene;

void render_change_view()
{
    scene.view_cube = !scene.view_cube;
}

void render_init()
{
    glEnable(GL_DEPTH_TEST);

    scene.eye = vec3_create(0.0f, 1.0f, -2.0f);
    scene.target = vec3_create(0.0f, 1.0f, 0.0f);
    scene.eye_rotation = mat4_create_identity();

    scene.demo = mesh_load("data/ball.stl");
    upload_mesh(&scene.demo);

    const char* paths[] = {"data/cube1/posx.jpg",
                           "data/cube1/negx.jpg",
                           "data/cube1/posy.jpg",
                           "data/cube1/negy.jpg",
                           "data/cube1/posz.jpg",
                           "data/cube1/negz.jpg"};

    scene.cube = cubemap_load(paths);
    upload_cubemap(&scene.cube);
                             
    scene.object_vert = compile_shader(GL_VERTEX_SHADER, vert_shader_source, 1);
    scene.object_frag = compile_shader(GL_FRAGMENT_SHADER, frag_shader_source, 1);
    scene.object_prog = glCreateProgram();

    glAttachShader(scene.object_prog, scene.object_vert);
    glAttachShader(scene.object_prog, scene.object_frag);

    glBindAttribLocation(scene.object_prog, ATTRIB_VERTEX, "a_vertex");
    glBindAttribLocation(scene.object_prog, ATTRIB_NORMAL, "a_normal");
    glLinkProgram(scene.object_prog);
}


void render_frame(int mouse_dx, int mouse_dy)
{
    Mat4 x_rot = mat4_create_rotate(mouse_dx / 100.0f, vec3_create(0.0f, 1.0f, 0.0f));
    Mat4 y_rot = mat4_create_rotate(mouse_dy / 100.0f, vec3_create(1.0f, 0.0f, 0.0f));

    Mat4 combined;
    mat4_mult(&x_rot, &y_rot, &combined);
    mat4_mult(&combined, &scene.eye_rotation, &scene.eye_rotation);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    Mat4 proj = mat4_create_perspective(60.0f, 1.44f, 1.0f, 20.0f);
    Mat4 view = mat4_create_look(mat4_mult_vec3(&scene.eye_rotation, scene.eye), scene.target, vec3_create(0.0f, 1.0f, 0.0));

    glUseProgram(scene.object_prog);
    glUniform1i(glGetUniformLocation(scene.object_prog, "u_cube"), 0);
    glUniformMatrix4fv(glGetUniformLocation(scene.object_prog, "u_proj"), 1, GL_FALSE, proj.m);
    glUniformMatrix4fv(glGetUniformLocation(scene.object_prog, "u_view"), 1, GL_FALSE, view.m);
    glUniform1i(glGetUniformLocation(scene.object_prog, "use_cube"), scene.view_cube);
    glUniformMatrix3fv(glGetUniformLocation(scene.object_prog, "sh_r"), 1, GL_FALSE, sh9_red);
    glUniformMatrix3fv(glGetUniformLocation(scene.object_prog, "sh_g"), 1, GL_FALSE, sh9_green);
    glUniformMatrix3fv(glGetUniformLocation(scene.object_prog, "sh_b"), 1, GL_FALSE, sh9_blue);

    glBindTexture(GL_TEXTURE_CUBE_MAP, scene.cube.pbo); 
    glBindVertexArray(scene.demo.vao);
    glDrawArrays(GL_TRIANGLES, 0, scene.demo.vertex_count);
    
    for (GLenum err; (err = glGetError()) != GL_NO_ERROR;)
        printf("%x\n", err); 
}



