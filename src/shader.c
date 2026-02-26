//
//  shader.c
//  graphixs
//
//  Created by Lars Höll on 24.08.25.
//
#include <OpenGL/gl3.h>
#include <OpenGL/glext.h>
#include <stdio.h>


#include "../include/fileio.h"
#include "../include/material.h"
#include "../include/light.h"
#include "../lib/cglm/cglm.h"
#include "string.h"

unsigned int createShader(const char* shaderSource, GLenum type, int* err) {
    int success;
    char infoLog[512];
    
    unsigned int shader;
    shader = glCreateShader(type);
    glShaderSource(shader, 1, &shaderSource, NULL);
    glCompileShader(shader);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        
    if (!success) {
        fprintf(stderr, "Failed to compile shader.\n");
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        *err = 1;
        return 0;
    }
    *err = 0;
    return shader;
}

unsigned int createShaderProgram(GLuint vertexShader, GLuint fragmentShader, int* err) {
    int success;
    char infoLog[512];
    
    unsigned int program;
    program = glCreateProgram();
    
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);
    
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if(!success) {
        fprintf(stderr, "Failed to link shader program.\n");
        *err = 1;
        return 0;
    }
    *err = 0;
    return program;
}

unsigned int createShaderFromSource(const char* vertexSourcePath, const char* fragmentSourcePath, int* err) {
    *err = 0;
    if (vertexSourcePath == NULL) {
        fprintf(stderr, "Error: Please set a vertex source path.\n");
        *err = 1;
        return 0;
    }
    
    if (fragmentSourcePath == NULL) {
        fprintf(stderr, "Error: Please set a fragment source path.\n");
        *err = 1;
        return 0;
    }
    
    const char* vertexSource = read_file(vertexSourcePath);
    const char* fragmentSource = read_file(fragmentSourcePath);
    
    if (vertexSource == NULL || fragmentSource == NULL) {
        fprintf(stderr, "Error: Something went wrong while reading vertex or fragment shader source conde.\n");
        *err = 1;
        return 0;
    }
    
    // shader setup error variable
    // shader setup
    unsigned int vertexShader = createShader(vertexSource, GL_VERTEX_SHADER, err);
    if (*err) {
        fprintf(stderr, "Error: Failed to compile vertexShader.\n");
        *err = 1;
        return 0;
    }
    
    unsigned int fragmentShader = createShader(fragmentSource, GL_FRAGMENT_SHADER, err);
    if (*err) {
        fprintf(stderr, "Error: Failed to compile fragmentShader.\n");
        *err = 1;
        return 0;
    }
    
                   
    unsigned int shaderProgram = createShaderProgram(vertexShader, fragmentShader, err);
    if (*err) {
        fprintf(stderr, "Failed to link shaders and create shaderProgram.\n");
        *err = 1;
        return 0;
    }
    
   
    // delete shader objects
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
    return shaderProgram;
}

void shaderProgramSetInt(unsigned int shaderProgram, const char* variableName, int value) {
    int uniformLocation = glGetUniformLocation(shaderProgram, variableName);
    glUniform1i(uniformLocation, value);
}

void shaderProgramSetBool(unsigned int shaderProgram, const char* variableName, int value) {
    shaderProgramSetInt(shaderProgram, variableName, value);
}

void shaderProgramSetFloat(unsigned int shaderProgram, const char* variableName, float value) {
    int uniformLocation = glGetUniformLocation(shaderProgram, variableName);
    glUniform1f(uniformLocation, value);
}

void shaderProgramSet3Float(unsigned int shaderProgram, const char* variableName, float value0, float value1, float value2) {
    int uniformLocation = glGetUniformLocation(shaderProgram, variableName);
    glUniform3f(uniformLocation, value0, value1, value2);
}

void shaderProgramSetMaterial(unsigned int shaderProgram, const char* variableName, Material* material) {
    shaderProgramSet3Float(shaderProgram, "material.ambient", material->ambient[0], material->ambient[1], material->ambient[2]);
    shaderProgramSet3Float(shaderProgram, "material.diffuse", material->diffuse[0], material->diffuse[1], material->diffuse[2]);
    shaderProgramSet3Float(shaderProgram, "material.specular", material->specular[0], material->specular[1], material->specular[2]);
    shaderProgramSetFloat(shaderProgram, "material.shininess", material->shininess);
}

void shaderProgramSetLight(unsigned int shaderProgram, const char* variableName, vec3 lightPosition, Light* light) {
    shaderProgramSet3Float(shaderProgram, "light.position", lightPosition[0], lightPosition[1], lightPosition[2]);
    shaderProgramSet3Float(shaderProgram, "light.ambient", light->ambient[0], light->ambient[1], light->ambient[2]);
    shaderProgramSet3Float(shaderProgram, "light.diffuse", light->diffuse[0], light->diffuse[1], light->diffuse[2]);
    shaderProgramSet3Float(shaderProgram, "light.specular", light->specular[0], light->specular[1], light->specular[2]);

}

