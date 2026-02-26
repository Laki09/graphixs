//
//  shader.h
//  graphixs
//
//  Created by Lars Höll on 24.08.25.
//
#pragma once
#ifndef SHADER_H
#define SHADER_H

#include "material.h"
#include "light.h"
#include "../lib/cglm/cglm.h"

unsigned int createShader(const char* shaderSource, GLenum type, int* err);

unsigned int createShaderProgram(GLuint vertexShader, GLuint fragmentShader, int* err);

unsigned int createShaderFromSource(const char* vertexSourcePath, const char* fragmentSourcePath, int* err);

void shaderProgramSetBool(unsigned int shaderProgram, const char* variableName, int value);

void shaderProgramSetInt(unsigned int shaderProgram, const char* variableName, int value);

void shaderProgramSetFloat(unsigned int shaderProgram, const char* variableName, float value);

void shaderProgramSet3Float(unsigned int shaderProgram, const char* variableName, float value0, float value1, float value2);

void shaderProgramSetMaterial(unsigned int shaderProgram, const char* variableName, Material* material);

void shaderProgramSetLight(unsigned int shaderProgram, const char* variableName, vec3 lightPosition, Light* light);


#endif
