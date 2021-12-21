#ifndef _CASTLECALL_SCENE_H
#define _CASTLECALL_SCENE_H

#include <memory>
#include <map>
#include <list>

#include "object.h"
#include "camera.h"

class Scene {
    public:
        void update(float time); // mass update all objects
        void render(); // mass render all objects

        // Camera
        std::unique_ptr<Camera> camera;

        // List of all objects to be rendered in this scene
        std::list<std::unique_ptr<Object>> objects;

        // for diffuse, phong shaders
        glm::vec3 lightPos = glm::vec3{1.0f, 3.0f, 1.0f};
        glm::vec3 ambientColor = glm::vec3{1.0f, 1.0f, 1.0f};
        glm::vec3 specularColor = glm::vec3{1.0f, 1.0f, 1.0f};
        glm::vec3 diffuseColor = glm::vec3{1.0f, 1.0f, 1.0f};
        glm::vec3 tintColor = glm::vec3{1.0f, 1.0f, 1.0f};
        float attenuationA = 0.88f;
        float attenuationB = 0.14f;
        float specLight = 1.54f;
        int specAmountPow = 8;

        // Keyboard state (map)
        std::map< int, int > keyboard;

        // Cursor state (struct)
};

#endif