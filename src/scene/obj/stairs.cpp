#include "stairs.h"
#include "paths.h"

#include <shaders/phong_vert_glsl.h>
#include <shaders/phong_frag_glsl.h>

std::unique_ptr<ppgso::Mesh> Stairs::mesh;
std::unique_ptr<ppgso::Texture> Stairs::texture;
std::unique_ptr<ppgso::Shader> Stairs::shader;

Stairs::Stairs() {
    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP(TEXTURE_PATH "hightower.bmp"));
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>(OBJ_PATH "stairs.obj");
}

void Stairs::render(Scene &scene, std::unique_ptr<ppgso::Shader> altShader) {
    altShader->use();

    // Set up light
    altShader->setUniform("camPos", scene.camera->position);
    altShader->setUniform("lightPos1", scene.lightPos1);
    altShader->setUniform("lightPos2", scene.lightPos2);
    altShader->setUniform("lightPos3", scene.lightPos3);
    altShader->setUniform("tintColor1", scene.tintColor1);
    altShader->setUniform("tintColor2", scene.tintColor2);
    altShader->setUniform("tintColor3", scene.tintColor3);

    altShader->setUniform("matAmbient", scene.ambientColor);
    altShader->setUniform("matDiffuse", scene.diffuseColor);
    altShader->setUniform("matSpecular", scene.specularColor);

    altShader->setUniform("diffusePower", scene.diffusePower);
    altShader->setUniform("specularPower", scene.specularPower);
    altShader->setUniform("ambient", scene.ambient);
    altShader->setUniform("specLight", scene.specLight);
    altShader->setUniformInt("specAmountPow", scene.specAmountPow);

    altShader->setUniform("ProjectionMatrix", scene.camera->projectionMatrix);
    altShader->setUniform("ViewMatrix", scene.camera->viewMatrix);
    altShader->setUniform("ModelMatrix", modelMatrix);
    altShader->setUniform("Texture", *texture);
    mesh->render();
}

