//
//  main.c
//  graphixs
//
//  Created by Lars Höll on 19.08.25.
//
#include <stdio.h>
#include <OpenGL/gl3.h>
#include <OpenGL/glext.h>
#include "../lib/include/GLFW/glfw3.h"
#include "../lib/cglm/cglm.h"
#include <math.h>

#include "../include/stb_image.h"
#include "../include/shader.h"
#include "../include/mesh_gen.h"
#include "../include/material.h"
#include "../include/light.h"

#define WINDOW_WIDTH 1080.0f
#define WINDOW_HEIGHT 720.0f

float deltaTime = 0.0f; // Time between current frame and last frame
float lastFrame = 0.0f;

vec3 cameraPosition = {0.0f, 50.0f, 0.0f};
vec3 cameraFront = {0.0f, 0.0f, -1.0f};
vec3 cameraUp = {0.0f, 1.0f, 0.0f};
vec3 lightPosition = {500.0f, 300.0f, -500.0f};

// cube vertices with texture coordinates
float cubeVertices[] = {
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
    0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
    0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
    0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
    -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
    0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
    0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
    0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
    -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
    -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
    -0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
    -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
    0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
    0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
    0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
    0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
    0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
    0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
    0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
    0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
    0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
    -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
    0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
    0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
    0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
    -0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
    -0.5f, 0.5f, -0.5f, 0.0f, 1.0f
};

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    
    const float cameraSpeed = 100.0f * deltaTime;
    const float verticalSpeed = 50.0f * deltaTime;
    
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        vec3 temp;
        glm_vec3_scale(cameraFront, cameraSpeed, temp);
        glm_vec3_add(cameraPosition, temp, cameraPosition);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        vec3 temp;
        glm_vec3_scale(cameraFront, cameraSpeed, temp);
        glm_vec3_sub(cameraPosition, temp, cameraPosition);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        vec3 temp;
        glm_vec3_cross(cameraFront, cameraUp, temp);
        glm_vec3_normalize(temp);
        glm_vec3_scale(temp, cameraSpeed, temp);
        glm_vec3_sub(cameraPosition, temp, cameraPosition);
    }
    
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        vec3 temp;
        glm_vec3_cross(cameraFront, cameraUp, temp);
        glm_vec3_normalize(temp);
        glm_vec3_scale(temp, cameraSpeed, temp);
        glm_vec3_add(cameraPosition, temp, cameraPosition);
    }
    
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        vec3 temp;
        glm_vec3_scale(cameraUp, verticalSpeed, temp);
        glm_vec3_add(cameraPosition, temp, cameraPosition);
    }
    
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        vec3 temp;
        glm_vec3_scale(cameraUp, verticalSpeed, temp);
        glm_vec3_sub(cameraPosition, temp, cameraPosition);
    }
}

float pitch = 0.0f;
float yaw = -90.0f;
float lastX = WINDOW_WIDTH / 2, lastY = WINDOW_HEIGHT / 2;
int mouseMoved = 0;

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    
    if (!mouseMoved) {
        lastX = xpos;
        lastY = ypos;
        mouseMoved = 1;
    }
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed: y ranges bottom to top
    lastX = xpos;
    lastY = ypos;
    
    const float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;
    
    yaw += xoffset;
    pitch += yoffset;
    
    if(pitch > 89.0f)
    pitch = 89.0f;
    if(pitch < -89.0f)
    pitch = -89.0f;
    
    vec3 direction = {
        cos(glm_rad(yaw)) * cos(glm_rad(pitch)),
        sin(glm_rad(pitch)),
        sin(glm_rad(yaw)) * cos(glm_rad(pitch))};
    glm_vec3_normalize(direction);
    glm_vec3_copy(direction, cameraFront);
}

int main(int argc, const char * argv[]) {
    
    if (argc != 3) {
        fprintf(stderr, "Error: please give to paths to (1) vertex shader source code\n"
                "                               (2) fragment shader source code\n");
        return 1;
    }
    
    
    if (!glfwInit()) {
        fprintf(stderr, "GLFW could not be initialised.\n");
        return 1;
    }
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    
    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "New Window", NULL, NULL);
    
    if (!window) {
        fprintf(stderr, "Error: Failed to create window.\n");
        goto cleanup;
    }
    
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    
    glfwMakeContextCurrent(window);
    
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    
    glfwSetCursorPosCallback(window, mouse_callback);
    
//    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
//    glDisable(GL_CULL_FACE);
    
    
    int err = 0;
    unsigned int shaderProgram = createShaderFromSource(argv[1], argv[2], &err);
    if (err) {
        fprintf(stderr, "Error: Failed to make shaderProgram.\n");
        goto cleanup;
    }
    err = 0;
    unsigned int lightShaderProgram = createShaderFromSource(
        "/Users/larsholl/Desktop/XCode/Projects/graphixs/graphixs/data/shaders/lightVertShader.vs",
        "/Users/larsholl/Desktop/XCode/Projects/graphixs/graphixs/data/shaders/lightFragShader.fs",
        &err);
    if (err) {
        fprintf(stderr, "Error: Failed to make lightShaderProgram.\n");
        glDeleteProgram(shaderProgram);
        goto cleanup;
    }
    
    // mesh generation
    int mesh_width = 2000;
    int mesh_height = 2000;
    const size_t vertexCount = (size_t)mesh_width * (size_t)mesh_height;
    const size_t indexCount  = (size_t)(mesh_width - 1) * (size_t)(mesh_height - 1) * 6;
    float* vertices;
    if (!(vertices = malloc(sizeof(float) * vertexCount * 3))) {
        fprintf(stderr, "Error: Failed to allocate enough memory for mesh generation.\n");
        goto cleanup;
    }
    unsigned int* indices;
    if (!(indices = malloc(sizeof(int) * indexCount))) {
        fprintf(stderr, "Error: Failed to allocate enough memory for mesh generation.\n");
        free(vertices);
        goto cleanup;
    }
    float* normals;
    if (!(normals = malloc(sizeof(float) * vertexCount * 3))) {
        fprintf(stderr, "Error: Failed to allocate enough memory for mesh generation.\n");
        free(vertices);
        free(indices);
        goto cleanup;
    }
    int meshSize;
    
    generate_plane_mesh(mesh_width, mesh_height, vertices, indices, &meshSize);
    build_normals_central_diff(mesh_width, mesh_height, vertices, normals);
//    for (int i = 0; i < vertexCount; i++) {
//        printf("[%f, %f, %f]\n", vertices[i * 3], vertices[i*3 + 1], vertices[i*3+2]);
//    }
//    for (int i = 0; i < indexCount; i += 3) {
//        printf("[%d, %d, %d]\n", indices[i], indices[i + 1], indices[i + 2]);
//    }
    
    GLuint VBO, VAO, EBO, NBO, lightVAO, cubeVBO;
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &NBO);
    glGenBuffers(1, &cubeVBO);
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &EBO);
    glGenVertexArrays(1, &lightVAO);
    
    glBindVertexArray(VAO);
    
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertexCount * 3 * sizeof(float), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, NBO);
    glBufferData(GL_ARRAY_BUFFER, vertexCount * 3 * sizeof(float), normals, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(unsigned int), indices, GL_STATIC_DRAW);
    
    glBindVertexArray(lightVAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, 36 * 5 *sizeof(float), cubeVertices, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    
    glUseProgram(shaderProgram);
    shaderProgramSetInt(shaderProgram, "texture2", 1);
    
    
    mat4 proj;
    glm_perspective(glm_rad(45.0f), WINDOW_WIDTH/WINDOW_HEIGHT, 0.1f, 10000.0f, proj);
    
    
    
    unsigned int modelLoc = glGetUniformLocation(shaderProgram, "model");
    unsigned int projLoc = glGetUniformLocation(shaderProgram, "projection");
    unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
    
    glUseProgram(lightShaderProgram);
    GLint lightModelLoc = glGetUniformLocation(lightShaderProgram, "model");
    GLint lightViewLoc  = glGetUniformLocation(lightShaderProgram, "view");
    GLint lightProjLoc  = glGetUniformLocation(lightShaderProgram, "projection");

    //Lighting
    vec3 color = {1.0f, 1.0f, 1.0f};
    Material* m = create_material_simple(color, 0.1f, 0.7f, 0.5f, 64.0f);
    vec3 lightColor = {1.0f, 1.0f, 1.0f};
    

    while (!glfwWindowShouldClose(window)) {
        
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        
        static double lastTime = 0.0;
        static int nbFrames = 0;

        double currentTime = glfwGetTime();
        nbFrames++;

        // Nur jede Sekunde aktualisieren
        if (currentTime - lastTime >= 1.0) {
            char title[256];
            snprintf(title, sizeof(title), "Graphixs - FPS: %d", nbFrames);
            glfwSetWindowTitle(window, title);

            nbFrames = 0;
            lastTime += 1.0;
        }
        
        // lightColor[0] = 1.0f;
        lightColor[0] = sin(glfwGetTime() * 2.0f);
        //lightColor[1] = 1.0f;
        lightColor[1] = sin(glfwGetTime() * 0.7f);
        //lightColor[2] = 1.0f;
        lightColor[2] = sin(glfwGetTime() * 1.5f);
        Light* l = create_light_simple(lightColor, 1.0f, 1.0f, 1.0f);
        processInput(window);
        // render
        // ------
        
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glEnable(GL_DEPTH_TEST);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        mat4 view;
        vec3 cameraDirection;
        glm_vec3_add(cameraPosition, cameraFront, cameraDirection);
        glm_lookat(cameraPosition, cameraDirection, cameraUp, view);

        // draw our first triangle
        glUseProgram(shaderProgram);
        
        
        
        glBindVertexArray(VAO);

        
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, (const GLfloat*)proj);
     
        
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, (const GLfloat*)view);
        
        mat4 model;
        glm_mat4_identity(model);
        vec3 scale = {0.5f, 0.5f, 0.5f};
        glm_scale(model, scale);
        vec3 rotationAxis = {1.0f, 0.0f, 0.0f};
        glm_rotate(model, glm_rad(-90.0f), rotationAxis);
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, (const GLfloat*)model);
        shaderProgramSet3Float(shaderProgram, "objectColor", 1.0f, 1.0f, 1.0f);
        shaderProgramSet3Float(shaderProgram, "lightColor", 1.0f, 1.0f, 1.0f);
        shaderProgramSet3Float(shaderProgram, "lightPos", lightPosition[0], lightPosition[1], lightPosition[2]);
        shaderProgramSet3Float(shaderProgram, "viewPos", cameraPosition[0], cameraPosition[1], cameraPosition[2]);
        shaderProgramSetMaterial(shaderProgram, "", m);
        shaderProgramSetLight(shaderProgram, "", lightPosition, l);
        
        glDrawElements(GL_TRIANGLES, meshSize, GL_UNSIGNED_INT, 0);
        
        glUseProgram(lightShaderProgram);
        glBindVertexArray(lightVAO);
        
        glUniformMatrix4fv(lightProjLoc, 1, GL_FALSE, (const GLfloat*)proj);
        glUniformMatrix4fv(lightViewLoc, 1, GL_FALSE, (const GLfloat*)view);

        glm_mat4_identity(model);
        glm_translate(model, lightPosition);
        scale[0] = 10.0f; scale[1] = 10.0f; scale[2]= 10.0f;
        glm_scale(model, scale);
        glUniformMatrix4fv(lightModelLoc, 1, GL_FALSE, (const GLfloat*)model);
        
        shaderProgramSet3Float(lightShaderProgram, "color", lightColor[0], lightColor[1], lightColor[2]);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        
        
        

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
        free(l);
    }
    
    glDeleteVertexArrays(1, &VAO);
    glDeleteVertexArrays(1, &lightVAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteBuffers(1, &cubeVBO);
    glDeleteProgram(shaderProgram);
    glDeleteProgram(lightShaderProgram);
    
    free(vertices);
    free(indices);
    free(normals);
    free(m);
    glfwDestroyWindow(window);
    
    
    
cleanup:
    glfwTerminate();
    return 0;
}


