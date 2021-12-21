#include "hall.h"
#include "paths.h"

#include <shaders/phong_vert_glsl.h>
#include <shaders/phong_frag_glsl.h>

std::unique_ptr<ppgso::Mesh> Hall::mesh;
std::unique_ptr<ppgso::Texture> Hall::texture;
std::unique_ptr<ppgso::Shader> Hall::shader;

Hall::Hall() {
    if (!shader) shader = std::make_unique<ppgso::Shader>(phong_vert_glsl, phong_frag_glsl);
    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP(TEXTURE_PATH "gate_hall.bmp"));
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>(OBJ_PATH "hall.obj");
}

void Hall::render(Scene &scene) {

    shader->use();

    // Set up light
    shader->setUniform("camPos", scene.camera->position);
    shader->setUniform("lightPos1", scene.lightPos1);
    shader->setUniform("lightPos2", scene.lightPos2);
    shader->setUniform("lightPos3", scene.lightPos3);
    shader->setUniform("tintColor1", scene.tintColor1);
    shader->setUniform("tintColor2", scene.tintColor2);
    shader->setUniform("tintColor3", scene.tintColor3);

    shader->setUniform("matAmbient", scene.ambientColor);
    shader->setUniform("matDiffuse", scene.diffuseColor);
    shader->setUniform("matSpecular", scene.specularColor);

    shader->setUniform("attenuationA", scene.attenuationA);
    shader->setUniform("attenuationB", scene.attenuationB);
    shader->setUniform("ambient", scene.ambient);
    shader->setUniform("specLight", scene.specLight);
    shader->setUniformInt("specAmountPow", scene.specAmountPow);

    shader->setUniform("ProjectionMatrix", scene.camera->projectionMatrix);
    shader->setUniform("ViewMatrix", scene.camera->viewMatrix);
    shader->setUniform("ModelMatrix", modelMatrix);
    shader->setUniform("Texture", *texture);
    mesh->render();
}

