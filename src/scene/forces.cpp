#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/transform.hpp>

#include <shaders/color_frag_glsl.h>
#include <shaders/color_vert_glsl.h>

#include "forces.h"
#include "scene.h"
#include "obj/paths.h"
#include "ppgso.h"

ForceObject::ForceObject() {
}

// float angle(glm::vec3 a, glm::vec3 b){
//     glm::vec3 da = glm::normalize(a);
//     glm::vec3 db = glm::normalize(b);
//     return glm::acos(glm::dot(da, db));
// }

bool ForceObject::update(Scene &scene, float dt) {
    if (!collided) {
        // Increase speed by gravity
        // the speed could in theory become fast enough to completely skip the height/width of the object
        speed += gravity * mass * dt;
        if (speed.x > maxGravity.x) speed.x = maxGravity.x * mass * dt;
        if (speed.y > maxGravity.y) speed.y = maxGravity.y * mass * dt;
        if (speed.z > maxGravity.z) speed.z = maxGravity.z * mass * dt;

        // Decrease position by speed
        lastPos = position;
        position -= speed;

        // Simulate a wind gust using oscillation every-so-often
        windTimeoutCounter += dt / 500.0f;
        if (windTimeoutCounter >= windEvery) {
            windTimeoutCounter = 0.0f;
            // Adjust position by wind strength in the given direction
            // Create a simple wind fluctuation using sine function
            windFluctuationSum += windFluctuation;
            float windFluctuationMax = 2 * 3.14f;
            if (windFluctuationSum.x > windFluctuationMax) windFluctuationSum.x -= windFluctuationMax;
            if (windFluctuationSum.y > windFluctuationMax) windFluctuationSum.y -= windFluctuationMax;
            if (windFluctuationSum.z > windFluctuationMax) windFluctuationSum.z -= windFluctuationMax;
            position.x += windStrength * (float) sin(windFluctuationSum.x);
            position.y += windStrength * (float) sin(windFluctuationSum.y);
            position.z += windStrength * (float) sin(windFluctuationSum.z);
        }
    }

    // std::cout << speed.y << " " << position.y << std::endl;

    // Check collision with all collisionboxes in scene
    collided = false;
    auto i = std::begin(scene.collisionboxes);
    while (i != std::end(scene.collisionboxes)) {
        auto box = i->get();

        // Get all corners of the collision box
        float cornerXmin = box->position.x - box->scale.x;
        float cornerXmax = box->position.x + box->scale.x;
        float cornerYmin = box->position.y - box->scale.y;
        float cornerYmax = box->position.y + box->scale.y;
        float cornerZmin = box->position.z - box->scale.z;
        float cornerZmax = box->position.z + box->scale.z;

        // Check if the ForceObject has entered the collision box
        // if the box has been entered from the given side, subtract the last movement
        // each side is scale/2 away from the center (position) of the object
        // note: if the scene has dynamically moving objects with collision boxes underneath then we don't want to stop the gravity
        if (position.x + (scale.x * 0.5f) >= cornerXmin && position.x - (scale.x * 0.5f) <= cornerXmax &&
            position.y + (scale.y * 0.5f) >= cornerYmin && position.y - (scale.y * 0.5f) <= cornerYmax &&
            position.z + (scale.z * 0.5f) >= cornerZmin && position.z - (scale.z * 0.5f) <= cornerZmax) {

            // // Approximate the position where the object should stop "slightly" above the surface
            // // todo this could be done better by finding a cross-point on the deltaPos vector
            // float tolerance = 0.02f; // inner tolerance that will make the collision still valid, but will apply a correction
            // while ((position.x >= cornerXmin + tolerance && position.x <= cornerXmax - tolerance &&
            //     position.y >= cornerYmin + tolerance && position.y <= cornerYmax - tolerance &&
            //     position.z >= cornerZmin + tolerance && position.z <= cornerZmax - tolerance)) {
            //     position -= dPos * 0.5f;
            // }
            position = lastPos;
            collided = true;
            speed = initSpeed;
        }

        i++;
    }

    return true;
}

std::unique_ptr<ppgso::Shader> CollisionBox::shader;
std::unique_ptr<ppgso::Mesh> CollisionBox::mesh;

CollisionBox::CollisionBox() {
}

bool CollisionBox::update(Scene &scene, float dt) {
    // todo In case the collision boxes are dynamic this may be used

    // if (scene.keyboard[GLFW_KEY_B])
    //     position.y -= 0.1f;
    // if (scene.keyboard[GLFW_KEY_V]) {
    //     scale.x += 0.1f;
    //     scale.z += 0.1f;
    // }
    // if (scene.keyboard[GLFW_KEY_N]) {
    //     std::cout << "posy:" << position.y << "scalex:" << scale.x << "scalez:" << scale.z << std::endl;
    // }
    return true;
}

void CollisionBox::render(Scene &scene) {
    generateModelMatrix();
    if (!shader) shader = std::make_unique<ppgso::Shader>(color_vert_glsl, color_frag_glsl);
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>(OBJ_PATH "cube.obj"); // cube.obj has scale set to 1,1,1 to match
    shader->use();
    shader->setUniform("OverallColor", glm::vec3{1.0f, 0.0f, 0.0f});
    shader->setUniform("ProjectionMatrix", scene.camera->projectionMatrix);
    shader->setUniform("ViewMatrix", scene.camera->viewMatrix);
    shader->setUniform("ModelMatrix", modelMatrix);
    mesh->render();
    return;
}