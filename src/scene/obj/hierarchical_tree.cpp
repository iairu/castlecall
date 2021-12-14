#include "hierarchical_tree.h"
#include "paths.h"

#include <shaders/diffuse_vert_glsl.h>
#include <shaders/diffuse_frag_glsl.h>

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
    if (!shader) shader = std::make_unique<ppgso::Shader>(diffuse_vert_glsl, diffuse_frag_glsl);
    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP(TEXTURE_PATH "nature.bmp"));
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>(OBJ_PATH "tree_trunk.obj");
}
Leaves::Leaves() {
    if (!shader) shader = std::make_unique<ppgso::Shader>(diffuse_vert_glsl, diffuse_frag_glsl);
    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP(TEXTURE_PATH "nature.bmp"));
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>(OBJ_PATH "tree_leaves.obj");
}
Ground::Ground() {
    if (!shader) shader = std::make_unique<ppgso::Shader>(diffuse_vert_glsl, diffuse_frag_glsl);
    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP(TEXTURE_PATH "nature.bmp"));
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>(OBJ_PATH "tree_ground.obj");
}

// todo: these may not need a shader directly? because they will be transformed later
void Trunk::render(Scene &scene) {
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
void Leaves::render(Scene &scene) {
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
void Ground::render(Scene &scene) {
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


Tree::Tree() {
    trunk = std::make_unique<Trunk>();
    leaves = std::make_unique<Leaves>();
    // todo assign transformations to trunk
    // todo assign transformations to leaves
}

bool Tree::update(Scene &scene, float dt) {
    // Update hierarchy
    trunk->update(scene, dt);
    leaves->update(scene, dt);
    return true;
}

void Tree::render(Scene &scene) {
    // Render hierarchy
    trunk->render(scene);
    leaves->render(scene);
}

HierarchicalTree::HierarchicalTree() {
    tree = std::make_unique<Tree>();
    ground = std::make_unique<Ground>();
    // todo assign transformations to tree
    // todo assign transformations to ground
}

bool HierarchicalTree::update(Scene &scene, float dt) {
    // Update hierarchy
    tree->update(scene, dt);
    ground->update(scene, dt);
    return true;
}

void HierarchicalTree::render(Scene &scene) {
    // Render hierarchy
    tree->render(scene);
    ground->render(scene);
}

