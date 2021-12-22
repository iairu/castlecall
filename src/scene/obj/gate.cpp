#include "gate.h"
#include "paths.h"

#include <shaders/phong_vert_glsl.h>
#include <shaders/phong_frag_glsl.h>

std::unique_ptr<ppgso::Mesh> Gate::mesh;
std::unique_ptr<ppgso::Texture> Gate::texture;
std::unique_ptr<ppgso::Shader> Gate::shader;

Gate::Gate() {
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

void Gate::render(Scene &scene, std::unique_ptr<ppgso::Shader> altShader) {
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

