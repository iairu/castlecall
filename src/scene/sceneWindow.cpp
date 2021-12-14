#include <iostream>
#include <map>
#include <list>

#include <ppgso/ppgso.h>
#include "scene.h"

#include "map.h"

// #include "../objects/demo.h" // IMPORT OBJECTS HERE

// (later this will be moved into window.h and window.cpp)
// (similar to scene.h and scene.cpp)

class SceneWindow : public ppgso::Window {
private:
    Scene scene;

    Map * map;

    void initScene() {
        scene.objects.clear();
        
        // Create a camera
        auto camera = std::make_unique<Camera>(60.0f, 1.0f, 0.1f, 100.0f);
        camera->position.z = -2.0f;
        scene.camera = move(camera);

        this->map = new Map;
        this->map->placeItems(2, &scene);

        // ADD OBJECTS HERE

            // Add skybox
            // scene.objects.push_back()

            // Add enemies to the scene
            // scene.objects.push_back()

            // Add a player to the scene
            // scene.objects.push_back()
    }

public:
    SceneWindow() : ppgso::Window{"CastleCall", 1280, 720} {
        // glfwSetInputMode(window, GLFW_STICKY_KEYS, 1);

        // Initialize OpenGL state
        // Enable Z-buffer
        glEnable(GL_DEPTH_TEST); // <-- make opengl work
        glDepthFunc(GL_LEQUAL);

        // Enable polygon culling
        glEnable(GL_CULL_FACE);
        glFrontFace(GL_CCW);
        glCullFace(GL_BACK);

        initScene();
    }

    // Handles pressed key when the window is focused
    void onKey(int key, int scanCode, int action, int mods) {
        scene.keyboard[key] = action;
    }

    // void onIdle() override {
    void onIdle()  {
        // Track time
        static auto time = (float) glfwGetTime();

        // Compute time delta
        float dt = (float) glfwGetTime() - time;

        // Set dark blue background
        glClearColor(0.0f, 0.1f, 0.1f, 0);

        // Clear depth and color buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Update and render all objects
        scene.update(dt);
        scene.render();
    }
};
