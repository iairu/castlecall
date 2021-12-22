#include "hierarchical_tree.h"
#include "paths.h"

#include <shaders/phong_vert_glsl.h>
#include <shaders/phong_frag_glsl.h>

std::unique_ptr<ppgso::Mesh> Trunk::mesh;
std::unique_ptr<ppgso::Texture> Trunk::texture;
std::unique_ptr<ppgso::Shader> Trunk::shader;
std::unique_ptr<ppgso::Mesh> Leaves::mesh;
std::unique_ptr<ppgso::Texture> Leaves::texture;
std::unique_ptr<ppgso::Shader> Leaves::shader;
std::unique_ptr<ppgso::Mesh> Ground::mesh;
std::unique_ptr<ppgso::Texture> Ground::texture;
std::unique_ptr<ppgso::Shader> Ground::shader;

Trunk::Trunk() {
    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP(TEXTURE_PATH "nature.bmp"));
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>(OBJ_PATH "tree_trunk.obj");
}
Leaves::Leaves() {
    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP(TEXTURE_PATH "nature.bmp"));
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>(OBJ_PATH "tree_leaves.obj");
}
Ground::Ground() {
    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP(TEXTURE_PATH "nature.bmp"));
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>(OBJ_PATH "tree_ground.obj");
}

void Trunk::render(Scene &scene, std::unique_ptr<ppgso::Shader> altShader) {
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
void Leaves::render(Scene &scene, std::unique_ptr<ppgso::Shader> altShader) {
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
void Ground::render(Scene &scene, std::unique_ptr<ppgso::Shader> altShader) {
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

bool Trunk::parent_update(glm::mat4 parentModelMatrix, glm::mat4 parentParentModelMatrix) {
    generateModelMatrix();
    glm::mat4 ownModelMatrix{1};
    ownModelMatrix *= modelMatrix;
    modelMatrix = parentParentModelMatrix;
    modelMatrix *= parentModelMatrix;
    modelMatrix *= ownModelMatrix;
    return true;
}
bool Leaves::parent_update(glm::mat4 parentModelMatrix, glm::mat4 parentParentModelMatrix) {
    generateModelMatrix();
    glm::mat4 ownModelMatrix{1};
    ownModelMatrix *= modelMatrix;
    modelMatrix = parentParentModelMatrix;
    modelMatrix *= parentModelMatrix;
    modelMatrix *= ownModelMatrix;
    return true;
}
bool Ground::parent_update(glm::mat4 parentModelMatrix) {
    generateModelMatrix();
    glm::mat4 ownModelMatrix{1};
    ownModelMatrix *= modelMatrix;
    modelMatrix = parentModelMatrix;
    modelMatrix *= ownModelMatrix;
    return true;
}


Tree::Tree() {
    trunk = std::make_unique<Trunk>();
    leaves = std::make_unique<Leaves>();
    leaves2 = std::make_unique<Leaves>();
    // todo assign transformations to trunk
    // todo assign transformations to leaves
    leaves2->position = {-0.67,0,0};
    leaves2->rotation = {0,0,3.14};
}

bool Tree::parent_update(glm::mat4 parentModelMatrix) {
    generateModelMatrix();

    // Update hierarchy
    trunk->parent_update(modelMatrix, parentModelMatrix);
    leaves->parent_update(modelMatrix, parentModelMatrix);
    leaves2->parent_update(modelMatrix, parentModelMatrix);

    return true;
}

void Tree::render(Scene &scene, std::unique_ptr<ppgso::Shader> altShader) {
    /*if (scene.keyboard[GLFW_KEY_I]) {
        leaves2->position.y -= 0.01;
        printf(" %.2f \n", leaves2->position.y);
    } else if (scene.keyboard[GLFW_KEY_K]) {
        leaves2->position.y += 0.01;
        printf(" %.2f \n", leaves2->position.y);
    }*/

    // Render hierarchy
    trunk->render(scene, move(altShader));
    leaves->render(scene, move(altShader));
    leaves2->render(scene, move(altShader));
}

HierarchicalTree::HierarchicalTree() {
    tree = std::make_unique<Tree>();
    ground = std::make_unique<Ground>();
    leafParticles = std::make_unique<LeafParticleSystem>();
    tree->position = {0,0,0};
    ground->position = {1,0,0};
    // leafParticles->n = 5; // change particle system properties firrt if needed
    leafParticles->generateParticles(); // initialize the particle system
}

bool HierarchicalTree::update(Scene &scene, float dt) {
    generateModelMatrix();

    // Update hierarchy
    tree->parent_update(modelMatrix);
    // ground->parent_update(modelMatrix);
    leafParticles->parent_update(modelMatrix, scene, dt);

    return true;
}

void HierarchicalTree::render(Scene &scene, std::unique_ptr<ppgso::Shader> altShader) {
    // if (scene.keyboard[GLFW_KEY_U]) {
    //     tree->position.y -= 0.05;
    //     printf(" %.2f \n", tree->position.y);
    // } else if (scene.keyboard[GLFW_KEY_J]) {
    //     tree->position.y += 0.05;
    //     printf(" %.2f \n", tree->position.y);
    // }
    // if (scene.keyboard[GLFW_KEY_O]) {
    //     tree->rotation.z -= 0.01;
    //     printf(" %.2f ", tree->rotation.z);
    // } else if (scene.keyboard[GLFW_KEY_L]) {
    //     tree->rotation.z += 0.01;
    //     printf(" %.2f ", tree->rotation.z);
    // }

    // if (scene.keyboard[GLFW_KEY_0]) {
    //     position.x -= 0.01;
    //     printf(" %.2f ", rotation.z);
    // }
    // Render hierarchy
    tree->render(scene, move(altShader));
    // ground->render(scene);
    leafParticles->render(scene, move(altShader));
}