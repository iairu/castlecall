#include "gate.h"
#include "paths.h"

#include <shaders/phong_vert_glsl.h>
#include <shaders/phong_frag_glsl.h>

std::unique_ptr<ppgso::Mesh> Gate::mesh;
std::unique_ptr<ppgso::Texture> Gate::texture;
std::unique_ptr<ppgso::Shader> Gate::shader;

Gate::Gate() {
    if (!shader) shader = std::make_unique<ppgso::Shader>(phong_vert_glsl, phong_frag_glsl);
    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP(TEXTURE_PATH "gate_hall.bmp"));
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>(OBJ_PATH "gate.obj");

    // 0.0f keyframe added automatically for default transformation
    addKeyframe(3000.0f, glm::vec3{position.x, position.y - 5.0f, position.z}, rotation, scale); // open the gate
    addKeyframe(10000.0f, glm::vec3{position.x, position.y - 5.0f, position.z}, rotation, scale); // leave open for a while
    addKeyframe(13000.0f, glm::vec3{position.x, position.y, position.z}, rotation, scale); // close the gate
    addKeyframe(20000.0f, glm::vec3{position.x, position.y, position.z}, rotation, scale); // leave closed for a while
    loop = true; // loop the animation
}


bool Gate::update(Scene &scene, float dt) {
    // Don't animate this object
    // generateModelMatrix();
    
    // Animate this object and after animation ends leave it be on its last position
    generateFrameModelMatrix(dt);

    // Animate this object and after animation ends delete it
    // return generateFrameModelMatrix();
    
    return true;
}

void Gate::render(Scene &scene) {

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

    shader->setUniform("diffusePower", scene.diffusePower);
    shader->setUniform("specularPower", scene.specularPower);
    shader->setUniform("ambient", scene.ambient);
    shader->setUniform("specLight", scene.specLight);
    shader->setUniformInt("specAmountPow", scene.specAmountPow);

    shader->setUniform("ProjectionMatrix", scene.camera->projectionMatrix);
    shader->setUniform("ViewMatrix", scene.camera->viewMatrix);
    shader->setUniform("ModelMatrix", modelMatrix);
    shader->setUniform("Texture", *texture);
    mesh->render();
}

