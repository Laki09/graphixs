//
//  material.h
//  graphixs
//
//  Created by Lars Höll on 08.09.25.
//
#pragma once
#ifndef MATERIAL_H
#define MATERIAL_H

#include "../lib/cglm/cglm.h"

typedef struct {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
} Material;

Material* create_material(vec3 ambient, vec3 diffuse, vec3 specular, float shininess);

Material* create_material_simple(vec3 color, float ambient, float diffuse, float specular, float shininess);

#endif
