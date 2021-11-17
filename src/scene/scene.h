#ifndef _CASTLECALL_SCENE_H
#define _CASTLECALL_SCENE_H

#include <memory>
#include <map>
#include <list>

#include "object.h"

class Scene {
    public:
        void update(float time); // mass update all objects
        void render(); // mass render all objects

        // Camera
        // std::unique_ptr<Camera> camera;

        // List of all objects to be rendered in this scene
        std::list<std::unique_ptr<Object>> objects;

        // Keyboard state (map)

        // Cursor state (struct)
};

#endif