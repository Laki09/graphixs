//
//  mesh_gen.h
//  graphixs
//
//  Created by Lars Höll on 02.09.25.
//

#pragma once
#ifndef MESH_GEN_H
#define MESH_GEN_H

/*
    w and h have to be ≥ 2
    vertDest is float array with length of (w) * (h) * 3
    indexDest is int array with length of (w - 1) * (h - 1) * 6
*/
void generate_plane_mesh(int w, int h, float* vertDest, unsigned int* indexDest, int* meshSize);

void build_normals_central_diff(int w, int h, const float* verts, float* norms);

void generate_plane_mesh_with_normals(int w, int h,
                                     float* vertDest,
                                     unsigned int* indexDest,
                                     float* normDest,
                                     int* meshSize);

#endif
