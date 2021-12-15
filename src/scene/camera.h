#pragma once
#include <memory>
#include <vector>

#include "ppgso.h"
#include "move.h"
#include <glm/glm.hpp>

// Forward declare a scene
class Scene;

class Camera {
    private:
        std::vector<std::vector<MOVE>> keyframes;
        std::vector<MOVE> camera_script;

        bool locked;

        void runScript(float dt);

public:
    glm::vec3 up{0,1,0};
    glm::vec3 position{0,0,0};
    glm::vec3 back{0,0,-1};

    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;

    bool pause_script;

    Camera(float fow = 45.0f, float ratio = 1.0f, float near = 0.1f, float far = 10.0f);

    void update(Scene &scene, float dt);

    void addKeyframe(std::vector<MOVE> * keyframe);
    // void setScript();

    // Get direction vector in world coordinates through camera projection plane
    glm::vec3 cast(double u, double v);
};

