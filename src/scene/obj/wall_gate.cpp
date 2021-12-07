#include "wall_gate.h"
#include "paths.h"

#include <shaders/diffuse_vert_glsl.h>
#include <shaders/diffuse_frag_glsl.h>

std::unique_ptr<ppgso::Mesh> WallGate::mesh;
std::unique_ptr<ppgso::Texture> WallGate::texture;
std::unique_ptr<ppgso::Shader> WallGate::shader;

WallGate::WallGate() {
    if (!shader) shader = std::make_unique<ppgso::Shader>(diffuse_vert_glsl, diffuse_frag_glsl);
    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP(TEXTURE_PATH "tower_wall.bmp"));
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>(OBJ_PATH "wall_gate.obj");
}

void WallGate::render(Scene &scene) {

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

