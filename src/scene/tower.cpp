#include "tower.h"

#include <shaders/diffuse_vert_glsl.h>
#include <shaders/diffuse_frag_glsl.h>


std::unique_ptr<ppgso::Mesh> Tower::mesh;
std::unique_ptr<ppgso::Texture> Tower::texture;
std::unique_ptr<ppgso::Shader> Tower::shader; // V C++ 17 to asi netreba prepisovat

Tower::Tower() {
    if (!shader) shader = std::make_unique<ppgso::Shader>(diffuse_vert_glsl, diffuse_frag_glsl);
    //if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("../data/tower.bmp"));
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("../data/tower.obj");

    scale *= 10;
}

void Tower::render(Scene &scene) {

    shader->use();

    // Set up light
    shader->setUniform("LightDirection", scene.lightDirection);

    // use camera
    shader->setUniform("ProjectionMatrix", scene.camera->projectionMatrix);
    shader->setUniform("ViewMatrix", scene.camera->viewMatrix);

    // render mesh
    shader->setUniform("ModelMatrix", modelMatrix);
    //shader->setUniform("Texture", *texture);
    mesh->render();
}


// TODO: uncomment texture lines later, update resource paths
