#include "leaf_particles.h"
#include "paths.h"
#include <shaders/phong_vert_glsl.h>
#include <shaders/phong_frag_glsl.h>
#include <random>

std::unique_ptr<ppgso::Mesh> LeafParticle::mesh;
std::unique_ptr<ppgso::Texture> LeafParticle::texture;
std::unique_ptr<ppgso::Shader> LeafParticle::shader;

LeafParticle::LeafParticle() {
    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP(TEXTURE_PATH "nature.bmp"));
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>(OBJ_PATH "leaf.obj");
}
bool LeafParticle::parent_update(glm::mat4 parentModelMatrix, Scene &scene, float dt) {
    update(scene, dt);
    glm::mat4 ownModelMatrix{1};
    ownModelMatrix *= modelMatrix;
    modelMatrix = parentModelMatrix;
    modelMatrix *= ownModelMatrix;

    // Particle decay after collision by subtracting decayTime
    if (collided) {
        decayTime -= dt;
        if (decayTime < 0.0f)
            return false;
    }

    return true;
}
void LeafParticle::render(Scene &scene, std::unique_ptr<ppgso::Shader> altShader) {
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

LeafParticleSystem::LeafParticleSystem() {
    
}

void LeafParticleSystem::generateParticles() {
    // Generate first n particles
    for(int i = 0; i < n; i++) {
        generateParticle();
    }
}

void LeafParticleSystem::generateParticle() {
    // Generate a random x coordinate for the particle 
    float x = min_gen_radius + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (max_gen_radius - min_gen_radius)));
    // <min_gen_radius;max_gen_radius>

    // Randomly choose whether to use the negative coordinate instead
    float negate = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX)); // <0.0f;1.0f>
    if (negate > 0.5) x *= -1;

    // Randomly choose whether to use Z instead of X
    bool useZ = false;
    float negate2 = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX)); // <0.0f;1.0f>
    if (negate2 > 0.5) useZ = true;

    // Build the starting position of the particle
    glm::vec3 startPos = glm::vec3{useZ ? 0.0f : x, height, useZ ? x : 0.0f};

    // Generate a random decayTime
    float decayTime = min_gen_decayTime + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (max_gen_decayTime - min_gen_decayTime)));
    // <min_gen_decayTime;max_gen_decayTime>

    // Create the particle with generated values
    std::unique_ptr<LeafParticle> particle = std::make_unique<LeafParticle>();
    particle->position = startPos;
    particle->decayTime = decayTime;
    particle->mass = mass;

    // Save the particle for use
    particles.push_back(move(particle));
}

bool LeafParticleSystem::parent_update(glm::mat4 parentModelMatrix, Scene &scene, float dt) {
    // Use iterator to update all particles so we can remove while iterating
    auto i = std::begin(particles);
    while (i != std::end(particles)) {
        // Update and remove from list if needed
        auto obj = i->get();
        if (!obj->parent_update(parentModelMatrix, scene, dt)) {
            // Particle decayed, remove it
            i = particles.erase(i);
            // Generate a new particle after last one died
            generateParticle();
        }
        else
            ++i;
    }
    return true;
}

void LeafParticleSystem::render(Scene &scene, std::unique_ptr<ppgso::Shader> altShader) {
    // Use iterator to render all particles
    auto i = std::begin(particles);
    while (i != std::end(particles)) {
        auto obj = i->get();
        obj->render(scene, move(altShader));
        ++i;
    }
}