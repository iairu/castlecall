#include "shroom.h"
#include "paths.h"

#include <shaders/phong_vert_glsl.h>
#include <shaders/phong_frag_glsl.h>

std::unique_ptr<ppgso::Mesh> Shroom::mesh;
std::unique_ptr<ppgso::Texture> Shroom::texture;
std::unique_ptr<ppgso::Shader> Shroom::shader;

Shroom::Shroom() {
    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP(TEXTURE_PATH "nature.bmp"));
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>(OBJ_PATH "shroom.obj");

    // // 0.0f keyframe added automatically for default transformation
    addKeyframe(1000.0f, glm::vec3{0.32f, 0.0f, 3.23f}, rotation, scale); // move to the bridge
    addKeyframe(2000.0f, glm::vec3{-3.04f, 0.0f, 3.23f}, rotation, scale); // move in front of the gate
    addKeyframe(3000.0f, glm::vec3{-3.04f, 0.0f, 3.23f}, rotation, scale); // wait in front of the gate for opening
    addKeyframe(4200.0f, glm::vec3{-4.26f, 0.0f, 3.23f}, rotation, scale); // move behind the gate
    addKeyframe(13000.0f, glm::vec3{-4.26f, 0.0f, 3.23f}, rotation, scale); // wait behind the gate for closing
    addKeyframe(17000.0f, glm::vec3{-13.1202f, 0.0f, 0.84f}, rotation, scale); // move to the corner by the high_tower
    addKeyframe(20000.0f, glm::vec3{-4.26f, 0.0f, 3.23f}, rotation, scale); // get back behind the gate
    addKeyframe(23200.0f, glm::vec3{-4.26f, 0.0f, 3.23f}, rotation, scale); // wait for the gate to open
    addKeyframe(26400.0f, glm::vec3{-3.04f, 0.0f, 3.23f}, rotation, scale); // move in front of the gate
    addKeyframe(27400.0f, glm::vec3{0.32f, 0.0f, 3.23f}, rotation, scale); // move to front of the bridge
    addKeyframe(40000.0f, position, rotation, scale); // move to the default position and sync next loop with gate opening loop
    loop = true; // loop the animation
}


bool Shroom::update(Scene &scene, float dt) {
    // Don't animate this object
    // generateModelMatrix();
    
    // Animate this object and after animation ends leave it be on its last position
    generateFrameModelMatrix(dt);

    // Animate this object and after animation ends delete it
    // return generateFrameModelMatrix();

    // // improvised movement controls to get positions for animation
    // // use with generateModelMatrix, don't use with generateFrameModelMatrix
    // if (scene.keyboard[GLFW_KEY_KP_4]) {
    //     position.x -= 0.01f;
    // } else if (scene.keyboard[GLFW_KEY_KP_6]) {
    //     position.x += 0.01f;
    // } else if (scene.keyboard[GLFW_KEY_KP_7]) {
    //     position.y -= 0.01f;
    // } else if (scene.keyboard[GLFW_KEY_KP_9]) {
    //     position.y += 0.01f;
    // } else if (scene.keyboard[GLFW_KEY_KP_8]) {
    //     position.z -= 0.01f;
    // } else if (scene.keyboard[GLFW_KEY_KP_2]) {
    //     position.z += 0.01f;
    // } else if (scene.keyboard[GLFW_KEY_KP_5]) {
    //     std::cout << "glm::vec3{" << position.x << "f, " << position.y << "f, " << position.z << "f}" << std::endl;
    // }
    
    return true;
}

void Shroom::render(Scene &scene, std::unique_ptr<ppgso::Shader> altShader) {
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

