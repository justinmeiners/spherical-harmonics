#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "mesh.h"

static void mesh_smooth(Mesh* mesh)
{
    Vec3* smooth_normals = malloc(sizeof(Vec3) * mesh->vertex_count);

    int tri_count = mesh->vertex_count / 3;
    for (int t0 = 0; t0 < tri_count; ++t0)
    {
        for (int v0 = 0; v0 < 3; ++v0)
        {
            Vec3 point1 = mesh->vertices[t0 * 3 + v0].point;
            Vec3 norm = mesh->vertices[t0 * 3 + v0].normal;
            int count = 1;

            for (int t1 = 0; t1 < tri_count; ++t1)
            {
                if (t0 == t1) continue;

                for (int v1 = 0; v1 < 3; ++v1)
                {
                    Vec3 point2 = mesh->vertices[t1 * 3 + v1].point;
                    Vec3 norm2 = mesh->vertices[t1 * 3 + v1].normal;

                    if (vec3_eq(point1, point2))
                    {
                        norm = vec3_add(norm, norm2);
                        ++count;
                    } 
                }
            }

            smooth_normals[t0 * 3 + v0] = vec3_scale(norm, 1.0f / (float)count);
        }
    }

    for (int i = 0; i < mesh->vertex_count; ++i)
    {
        mesh->vertices[i].normal = smooth_normals[i];
    }
    free(smooth_normals);
}

static Mesh mesh_load_file(FILE* file)
{   
#define HEADER_SIZE 80

    char header[HEADER_SIZE];
    fread(header, HEADER_SIZE, 1, file);

    uint32_t triangle_count;
    fread(&triangle_count, sizeof(triangle_count), 1, file);

    Mesh mesh;
    mesh.vertex_count = triangle_count * 3;
    mesh.vertices = malloc(sizeof(MeshVert) * mesh.vertex_count);

    for (int i = 0; i < triangle_count; ++i)
    {
        Vec3 normal;
        fread(&normal, sizeof(normal), 1, file);

        Vec3 vertices[3];
        fread(vertices, sizeof(vertices[0]), 3, file);

        uint16_t attribute_size;
        fread(&attribute_size, sizeof(attribute_size), 1, file);
        // skip attributes
        fseek(file, attribute_size, SEEK_CUR);

        for (int k = 0; k < 3; ++k)
        {
            mesh.vertices[i * 3 + k].point = vertices[k];
            mesh.vertices[i * 3 + k].normal = normal;
        }
    }
    mesh_smooth(&mesh);
    return mesh;
}

Mesh mesh_load(const char* path)
{
    FILE* file = fopen(path, "rb");
    return mesh_load_file(file);
}


