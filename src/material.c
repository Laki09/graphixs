//
//  material.c
//  graphixs
//
//  Created by Lars Höll on 08.09.25.
//
#include "../include/material.h"
#include "../lib/cglm/cglm.h"

Material* create_material(vec3 ambient, vec3 diffuse, vec3 specular, float shininess) {
    Material* mat = malloc(sizeof(float) * 10);
    mat->ambient[0] = ambient[0];
    mat->ambient[1] = ambient[1];
    mat->ambient[2] = ambient[2];
    mat->diffuse[0] = diffuse[0];
    mat->diffuse[1] = diffuse[1];
    mat->diffuse[2] = diffuse[2];
    mat->specular[0] = specular[0];
    mat->specular[1] = specular[1];
    mat->specular[2] = specular[2];
    mat->shininess = shininess;
    return mat;
}

Material* create_material_simple(vec3 color, float ambient, float diffuse, float specular, float shininess) {
    vec3 ambientColor = {color[0] * ambient, color[1] * ambient, color[2] * ambient};
    vec3 diffuseColor = {color[0] * diffuse, color[1] * diffuse, color[2] * diffuse};
    vec3 specularColor = {color[0] * specular, color[1] * specular, color[2] * specular};
    return create_material(ambientColor, diffuseColor, specularColor, shininess);
}
