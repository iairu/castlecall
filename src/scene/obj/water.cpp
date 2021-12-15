#include "water.h"
#include "paths.h"

#include <shaders/texture_frag_glsl.h>
#include <shaders/texture_vert_glsl.h>

#include <iostream>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/transform.hpp>

// std::unique_ptr<ppgso::Mesh> Water::mesh;
std::unique_ptr<ppgso::Texture> Water::texture;
std::unique_ptr<ppgso::Shader> Water::shader;

Water::Water() {
    // Use texture shaders for water instead
    if (!shader) shader = std::make_unique<ppgso::Shader>(texture_vert_glsl, texture_frag_glsl);
    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP(TEXTURE_PATH "water.bmp"));
    // if (!mesh) mesh = std::make_unique<ppgso::Mesh>(OBJ_PATH "water.obj");
    
    // Initialize control points & create bezier patch (mesh) from them
    cp_x = 1.0; // Initial value of an animated point
    updateControlPoints(true);
    buffer(true);

    // Initial transformation (because predefined controlPoints are not rotated/scaled well)
    rotation.x = 1.64;
    scale.x = 40;
    scale.y = 40;
    position.y = -4.21;
}
Water::~Water() {
    unbuffer();
}

void Water::updateControlPoints(bool init) {
    // cp_x will be animated during update()
    glm::vec3 controlPoints[4][4] {
        {
            {-1,  -1, 0},
            {-.5, -1, 0},
            {+.5, -1, 0},
            {+1,  -1, 0},
        },
        {
            {-1,  -.5, 0},
            {-.5, -.5, cp_x},
            {+.5, -.5, cp_x},
            {+1,  -.5, 0},
        },
        {
            {-1,  +.5, 0},
            {-.5, +.5, cp_x},
            {+.5, +.5, cp_x},
            {+1,  +.5, 0},
        },
        {
            {-1,  +1, 0},
            {-.5, +1, 0},
            {+.5, +1, 0},
            {+1,  +1, 0},
        },
    };
    bezierPatch(controlPoints, init);
}

void Water::bezierPatch(glm::vec3 controlPoints[4][4], bool init) {
    // Generate Bezier patch points and incidences
    int PATCH_SIZE = 10;
    vertices.clear();

    std::vector<glm::vec3> vinner;  // 10x3, vertical interp.
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < PATCH_SIZE; j++)
            vinner.push_back(bezierPoint(controlPoints[i], (float)j / (float)PATCH_SIZE));

    // 10x10, horiz. interp.
    for (int i = 0; i < PATCH_SIZE; i++) {
        glm::vec3 row[4] = {vinner[i], vinner[i + PATCH_SIZE], vinner[i + 2 * PATCH_SIZE], vinner[i + 3 * PATCH_SIZE]};
        for (int j = 0; j < PATCH_SIZE; j++) {
            // Vertices
            glm::vec3 point = bezierPoint(row, (float)j / (float)PATCH_SIZE);
            vertices.push_back(point);

            if (init) {
                // Texture coordinates
                glm::vec2 uv = {(float)j / (float)PATCH_SIZE, (float)i / (float)PATCH_SIZE};
                texCoords.push_back(uv);
            }
        }
    }

    if (init) {
        // Generate indices
        for (int i = 0; i < PATCH_SIZE - 1; i++) {
            for (int j = 0; j < PATCH_SIZE - 1; j++) {
                mesh.push_back({(GLuint) 00 + (10 * i + j), (GLuint) 01 + (10 * i + j), (GLuint) 10 + (10 * i + j)});  // |_
                mesh.push_back({(GLuint) 01 + (10 * i + j), (GLuint) 11 + (10 * i + j), (GLuint) 10 + (10 * i + j)});  // ^|
            }
        }
    }
}

void Water::buffer(bool init) {
    if (init) {
        // Copy data to OpenGL
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);
    }

    // Copy positions to gpu
    if (init) {
        glGenBuffers(1, &vbo);
    }
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);

    if (init) {
        // Set vertex program inputs
        auto position_attrib = shader->getAttribLocation("Position");
        glEnableVertexAttribArray(position_attrib);
        glVertexAttribPointer(position_attrib, 3, GL_FLOAT, GL_FALSE, 0, 0);

        // Copy texture positions to gpu
        glGenBuffers(1, &tbo);
        glBindBuffer(GL_ARRAY_BUFFER, tbo);
        glBufferData(GL_ARRAY_BUFFER, texCoords.size() * sizeof(glm::vec2), texCoords.data(), GL_STATIC_DRAW);

        // Set vertex program inputs
        auto texCoord_attrib = shader->getAttribLocation("TexCoord");
        glEnableVertexAttribArray(texCoord_attrib);
        glVertexAttribPointer(texCoord_attrib, 2, GL_FLOAT, GL_FALSE, 0, 0);

        // Copy indices to gpu
        glGenBuffers(1, &ibo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.size() * sizeof(face), mesh.data(), GL_STATIC_DRAW);
    }
}

void Water::unbuffer() {
    // Delete data from OpenGL
    glDeleteBuffers(1, &ibo);
    glDeleteBuffers(1, &tbo);
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
}

bool Water::update(Scene &scene, float dt) {
    // if (scene.keyboard[GLFW_KEY_U]) {
    //     scale.x -= 0.01;
    //     scale.y -= 0.01;
    //     printf(" %.2f ", scale.x);
    // } else if (scene.keyboard[GLFW_KEY_J]) {
    //     scale.x += 0.01;
    //     scale.y += 0.01;
    //     printf(" %.2f ", scale.x);
    // }

    // if (scene.keyboard[GLFW_KEY_I]) {
    //     position.y -= 0.01;
    //     printf(" %.2f ", position.y);
    // } else if (scene.keyboard[GLFW_KEY_K]) {
    //     position.y += 0.01;
    //     printf(" %.2f ", position.y);
    // }

    // if (scene.keyboard[GLFW_KEY_O]) {
    //     rotation.x -= 0.01;
    //     printf(" %.2f ", rotation.x);
    // } else if (scene.keyboard[GLFW_KEY_L]) {
    //     rotation.x += 0.01;
    //     printf(" %.2f ", rotation.x);
    // }

    // Animate cp_x control point
    cp_x = cos(dt);
    updateControlPoints(false);
    // Update vertex buffer
    buffer(false);
    
    // Generate model matrix
    generateModelMatrix();
    return true;
}

void Water::render(Scene &scene) {

    shader->use();

    // Set up light
    shader->setUniform("LightDirection", scene.lightDirection);
    shader->setUniform("Transparency", 0.25f);

    // use camera
    shader->setUniform("ProjectionMatrix", scene.camera->projectionMatrix);
    shader->setUniform("ViewMatrix", scene.camera->viewMatrix);

    // render mesh
    shader->setUniform("ModelMatrix", modelMatrix);
    shader->setUniform("Texture", *texture);

    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, (GLsizei)(sizeof(glm::vec3) * mesh.size()), GL_UNSIGNED_INT, 0); 
}

