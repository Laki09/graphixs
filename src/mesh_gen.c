//
//  mesh_gen.c
//  graphixs
//
//  Created by Lars Höll on 02.09.25.
//

#include "../include/mesh_gen.h"
#include <math.h>
#include "../include/noise.h"
#include "../lib/cglm/cglm.h"

void generate_plane_mesh(int w, int h, float* vertDest, unsigned int* indexDest, int* meshSize) {
    if (w < 1 || h < 1) return;
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            int indexOffset = (x + w * y) * 3;
            vertDest[indexOffset] = x;
            vertDest[indexOffset + 1] = y;
            vertDest[indexOffset + 2] = (perlin2d(x, y, 0.003f, 2.0f) * 100.0f + perlin2d(x+10.5f, y+5.5f, 0.0006, 4.0f) * 1000.0f) / 2.0f;
        }
    }
    
    for (int j = 0; j < (h - 1); j++) {
        for (int i = 0; i < (w - 1); i++) {
            int indexOffset = (i + (w - 1) * j) * 6;
            indexDest[indexOffset] = i + (j * w);
            indexDest[indexOffset + 1] = i + (j * w) + 1;
            indexDest[indexOffset + 2] = i + (j * w) + 1 + w;
            indexDest[indexOffset + 3] = i + (j * w);
            indexDest[indexOffset + 4] = i + (j * w) + w;
            indexDest[indexOffset + 5] = i + (j * w) + 1 + w;
        }
    }
    *meshSize = (h - 1) * (w - 1) * 6;
}

static inline float height_at(const float* v, int w, int x, int y) {
    return v[(x + y*w)*3 + 2];
}

void build_normals_central_diff(int w, int h, const float* verts, float* norms) {
    for (int y = 0; y < h; ++y) {
        for (int x = 0; x < w; ++x) {
            int xl = x > 0   ? x-1 : x;
            int xr = x < w-1 ? x+1 : x;
            int yb = y > 0   ? y-1 : y;
            int yt = y < h-1 ? y+1 : y;

            float dzdx = height_at(verts, w, xr, y) - height_at(verts, w, xl, y);
            float dzdy = height_at(verts, w, x, yt) - height_at(verts, w, x, yb);

            // Ebene liegt im XY, Z ist Höhe → Up = +Z
            vec3 n = { -dzdx, -dzdy, 2.0f };           // 2.0 beruhigt die Steilheit (feel free to tune)
            glm_vec3_normalize(n);

            int i = (x + y*w) * 3;
            norms[i+0] = n[0];
            norms[i+1] = n[1];
            norms[i+2] = n[2];
        }
    }
}

void generate_plane_mesh_with_normals(int w, int h,
                                     float* vertDest,           // w*h*3
                                     unsigned int* indexDest,   // (w-1)*(h-1)*6
                                     float* normDest,           // w*h*3 (Output)
                                     int* meshSize)             // = Indexanzahl
{
    if (w < 2 || h < 2 || !vertDest || !indexDest || !normDest || !meshSize) {
        if (meshSize) *meshSize = 0;
        return;
    }

    // 1) Positionen + Indizes
    generate_plane_mesh(w, h, vertDest, indexDest, meshSize);

    const int vertexCount = w * h;
    const int indexCount  = *meshSize;

    // 2) Normals nullen
    for (int i = 0; i < vertexCount * 3; ++i) normDest[i] = 0.0f;

    // 3) Dreiecke durchgehen, Dreiecks-Normale addieren
    for (int t = 0; t < indexCount; t += 3) {
        unsigned int i0 = indexDest[t + 0];
        unsigned int i1 = indexDest[t + 1];
        unsigned int i2 = indexDest[t + 2];

        vec3 v0 = { vertDest[i0*3+0], vertDest[i0*3+1], vertDest[i0*3+2] };
        vec3 v1 = { vertDest[i1*3+0], vertDest[i1*3+1], vertDest[i1*3+2] };
        vec3 v2 = { vertDest[i2*3+0], vertDest[i2*3+1], vertDest[i2*3+2] };

        vec3 e1, e2, n;
        glm_vec3_sub(v1, v0, e1);       // e1 = v1 - v0
        glm_vec3_sub(v2, v0, e2);       // e2 = v2 - v0
        glm_vec3_cross(e2, e1, n);      // CCW -> cross(e1,e2)

        // auf die drei betroffenen Vertex-Normalen aufsummieren
        vec3 *d0 = (vec3*)&normDest[i0*3];
        vec3 *d1 = (vec3*)&normDest[i1*3];
        vec3 *d2 = (vec3*)&normDest[i2*3];
        glm_vec3_add(*d0, n, *d0);
        glm_vec3_add(*d1, n, *d1);
        glm_vec3_add(*d2, n, *d2);


    }

    // 4) pro Vertex normalisieren
    for (int i = 0; i < vertexCount; ++i) {
        vec3 *nv = (vec3*)&normDest[i*3];
        glm_vec3_normalize(*nv);
    }
}
