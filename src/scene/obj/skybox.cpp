#include "skybox.h"
#include "paths.h"

#include <shaders/texture_vert_glsl.h>
#include <shaders/texture_frag_glsl.h>

std::unique_ptr<ppgso::Mesh> Skybox::mesh;
std::unique_ptr<ppgso::Texture> Skybox::texture;
std::unique_ptr<ppgso::Shader> Skybox::shader;

Skybox::Skybox() {
    if (!shader) shader = std::make_unique<ppgso::Shader>(texture_vert_glsl, texture_frag_glsl);
    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP(TEXTURE_PATH "skybox.bmp"));
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>(OBJ_PATH "skybox.obj");
}

void Skybox::render(Scene &scene, std::unique_ptr<ppgso::Shader> altShader) {

    shader->use();

    shader->setUniform("ProjectionMatrix", scene.camera->projectionMatrix);
    shader->setUniform("ViewMatrix", scene.camera->viewMatrix);
    shader->setUniform("ModelMatrix", modelMatrix);
    shader->setUniform("Texture", *texture);
    mesh->render();
}

