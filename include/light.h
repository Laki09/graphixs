//
//  light.h
//  graphixs
//
//  Created by Lars Höll on 08.09.25.
//
#pragma once
#ifndef LIGHT_H
#define LIGHT_H

#include "../lib/cglm/cglm.h"

typedef struct {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
} Light;

Light* create_light(vec3 ambient, vec3 diffuse, vec3 specular);
Light* create_light_simple(vec3 color, float ambient, float diffuse, float specular);

#endif
