//
//  light.c
//  graphixs
//
//  Created by Lars Höll on 08.09.25.
//
#include "../include/light.h"

Light* create_light(vec3 ambient, vec3 diffuse, vec3 specular) {
    Light* light = malloc(sizeof(float) * 9);
    light->ambient[0] = ambient[0];
    light->ambient[1] = ambient[1];
    light->ambient[2] = ambient[2];
    light->diffuse[0] = diffuse[0];
    light->diffuse[1] = diffuse[1];
    light->diffuse[2] = diffuse[2];
    light->specular[0] = specular[0];
    light->specular[1] = specular[1];
    light->specular[2] = specular[2];
    return light;
}
Light* create_light_simple(vec3 color, float ambient, float diffuse, float specular) {
    vec3 ambientColor = {color[0] * ambient, color[1] * ambient, color[2] * ambient};
    vec3 diffuseColor = {color[0] * diffuse, color[1] * diffuse, color[2] * diffuse};
    vec3 specularColor = {color[0] * specular, color[1] * specular, color[2] * specular};
    return create_light(ambientColor, diffuseColor, specularColor);
}
