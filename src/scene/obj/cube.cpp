#include "cube.h"
#include "paths.h"

#include <shaders/color_vert_glsl.h>
#include <shaders/color_frag_glsl.h>

std::unique_ptr<ppgso::Mesh> Cube::mesh;
std::unique_ptr<ppgso::Texture> Cube::texture;
std::unique_ptr<ppgso::Shader> Cube::shader;

Cube::Cube() {
    if (!shader) shader = std::make_unique<ppgso::Shader>(color_vert_glsl, color_frag_glsl); // todo replace with lighting
    // if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP(TEXTURE_PATH "nature.bmp"));
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>(OBJ_PATH "cube.obj");
}

void Cube::render(Scene &scene) {

    shader->use();

    // Set up light
    shader->setUniform("LightDirection", scene.calculateLightDirection(position));

    // use camera
    shader->setUniform("ProjectionMatrix", scene.camera->projectionMatrix);
    shader->setUniform("ViewMatrix", scene.camera->viewMatrix);

    // render mesh
    shader->setUniform("ModelMatrix", modelMatrix);
    // shader->setUniform("Texture", *texture);
    shader->setUniform("OverallColor", glm::vec3{1.0f, 0.5f, 1.0f}); // only normalized rgb values <0.0f;1.0f> => <0;255>
    mesh->render();
}

