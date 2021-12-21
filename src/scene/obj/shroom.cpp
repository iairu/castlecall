#include "shroom.h"
#include "paths.h"

#include <shaders/phong_vert_glsl.h>
#include <shaders/phong_frag_glsl.h>

std::unique_ptr<ppgso::Mesh> Shroom::mesh;
std::unique_ptr<ppgso::Texture> Shroom::texture;
std::unique_ptr<ppgso::Shader> Shroom::shader;

Shroom::Shroom() {
    if (!shader) shader = std::make_unique<ppgso::Shader>(phong_vert_glsl, phong_frag_glsl);
    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP(TEXTURE_PATH "nature.bmp"));
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>(OBJ_PATH "shroom.obj");
}

void Shroom::render(Scene &scene) {

    shader->use();

    // Set up light
    shader->setUniform("camPos", scene.camera->position);
    shader->setUniform("lightPos", scene.lightPos);

    shader->setUniform("matAmbient", scene.ambientColor);
    shader->setUniform("matDiffuse", scene.diffuseColor);
    shader->setUniform("matSpecular", scene.specularColor);
    shader->setUniform("tintColor", scene.tintColor);

    shader->setUniform("attenuationA", scene.attenuationA);
    shader->setUniform("attenuationB", scene.attenuationB);
    shader->setUniform("specLight", scene.specLight);
    shader->setUniformInt("specAmountPow", scene.specAmountPow);

    shader->setUniform("ProjectionMatrix", scene.camera->projectionMatrix);
    shader->setUniform("ViewMatrix", scene.camera->viewMatrix);
    shader->setUniform("ModelMatrix", modelMatrix);
    shader->setUniform("Texture", *texture);
    mesh->render();
}

