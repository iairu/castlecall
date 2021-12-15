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

        glm::vec3 light_source;

        bool hasLightSource = false;

        // List of all objects to be rendered in this scene
        std::list<std::unique_ptr<Object>> objects;

        glm::vec3 lightDirection{-1.0f, 1.0f, -1.0f}; // keep this for legacy components

        // Keyboard state (map)
        std::map< int, int > keyboard;

        // Cursor state (struct)
        glm::vec3 calculateLightDirection(glm::vec3 object_pos);
};

#endif