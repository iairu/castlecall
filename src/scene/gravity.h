#ifndef _GRAVITY_H
#define _GRAVITY_H

#include <memory>

#include "object.h"
#include "ppgso.h"

// An object that has gravity properties and can collide against CollisionBoxes in scene.collisionboxes
class GravityObject : public Object {
    private:
        glm::vec3 gravity{0.0f,0.0001f,0.0f};
        glm::vec3 maxGravity{1.0f,1.0f,1.0f};
        glm::vec3 initSpeed{0.0f,0.0f,0.0f};
        glm::vec3 speed = initSpeed;
        glm::vec3 lastPos = position;
        bool collided = false;
    public:
        float mass = 0.8f;
        GravityObject();

        bool update(Scene &scene, float dt) override;
};

// Collision box - an invisible object that collisions get checked against if within scene.collisionboxes
class CollisionBox final : public Object {
    private:
        // todo Collisionbox has predefined vertices at +-1.0 x/y/z, so that they can be easily sized using the existing scale vector
        // Scale is length from object's central origin point (which is located at the position of the object) to the corner
        static std::unique_ptr<ppgso::Mesh> mesh;
        static std::unique_ptr<ppgso::Shader> shader;
    public:
        CollisionBox();
        void render(Scene &scene) override;
        bool update(Scene &scene, float dt) override;
        void onClick(Scene &scene) override {}
};

#endif
