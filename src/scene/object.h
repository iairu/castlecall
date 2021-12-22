#pragma once
#include <memory>
#include <list>
#include <map>

#include <glm/glm.hpp>
#include "ppgso.h"

// Forward declare a scene
class Scene;

typedef struct _KEYFRAME {
    float onTime;
    glm::mat4 modelMatrix;
} KEYFRAME, *PKEYFRAME;

class Object {
public:
    // Define default constructors as this is an abstract class
    Object() = default;
    Object(const Object&) = default;
    Object(Object&&) = default;
    virtual ~Object() {};

    virtual bool update(Scene &scene, float dt) = 0;

    virtual void render(Scene &scene, std::unique_ptr<ppgso::Shader> altShader) = 0;

    virtual void onClick(Scene &scene) {};

    // Object properties
    glm::vec3 position{0,0,0};
    glm::vec3 rotation{0,0,0};
    glm::vec3 scale{1,1,1};
    glm::mat4 modelMatrix{1};

    // Keyframe animation available to all objects
    std::list<KEYFRAME> keyframes;
    float onTime = 0.0f;
    float maxOnTime = 0.0f;
    bool loop = false; // hint: if you don't want looped animation to be choppy at the end make sure it ends its starting position by adding an equivalent keyframe

protected:
    // Generate modelMatrix from position, rotation and scale
    void generateModelMatrix();

    // Add a keyframe to the list of this object's keyframes
    void addKeyframe(float duration, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);

    // Use this in object's update function to generate an animated frame instead of the default transformations
    bool generateFrameModelMatrix(float dt);
};

