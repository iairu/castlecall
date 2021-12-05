#include "ground_wild.h"

#include <shaders/diffuse_vert_glsl.h>
#include <shaders/diffuse_frag_glsl.h>

std::unique_ptr<ppgso::Mesh> GroundWild::mesh;
std::unique_ptr<ppgso::Texture> GroundWild::texture;
std::unique_ptr<ppgso::Shader> GroundWild::shader;

GroundWild::GroundWild() {
    if (!shader) shader = std::make_unique<ppgso::Shader>(diffuse_vert_glsl, diffuse_frag_glsl);
    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("../data/bmp/intro.bmp"));
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("../data/ground_wild.obj");
}

void GroundWild::render(Scene &scene) {

    shader->use();

    // Set up light
    shader->setUniform("LightDirection", scene.lightDirection);

    // use camera
    shader->setUniform("ProjectionMatrix", scene.camera->projectionMatrix);
    shader->setUniform("ViewMatrix", scene.camera->viewMatrix);

    // render mesh
    shader->setUniform("ModelMatrix", modelMatrix);
    shader->setUniform("Texture", *texture);
    mesh->render();
}

