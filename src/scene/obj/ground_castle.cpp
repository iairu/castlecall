#include "ground_castle.h"
#include "paths.h"

#include <shaders/diffuse_vert_glsl.h>
#include <shaders/diffuse_frag_glsl.h>

std::unique_ptr<ppgso::Mesh> GroundCastle::mesh;
std::unique_ptr<ppgso::Texture> GroundCastle::texture;
std::unique_ptr<ppgso::Shader> GroundCastle::shader;

GroundCastle::GroundCastle() {
    if (!shader) shader = std::make_unique<ppgso::Shader>(diffuse_vert_glsl, diffuse_frag_glsl);
    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP(TEXTURE_PATH "nature.bmp"));
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>(OBJ_PATH "ground_castle.obj");
}

void GroundCastle::render(Scene &scene) {

    shader->use();

    // Set up light
    shader->setUniform("LightDirection", scene.calculateLightDirection(position));

    // use camera
    shader->setUniform("ProjectionMatrix", scene.camera->projectionMatrix);
    shader->setUniform("ViewMatrix", scene.camera->viewMatrix);

    // render mesh
    shader->setUniform("ModelMatrix", modelMatrix);
    shader->setUniform("Texture", *texture);
    mesh->render();
}

