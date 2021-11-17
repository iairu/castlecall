#include <iostream>
#include <map>
#include <list>

#include "ppgso.h"
#include "scene.h"

// #include "../objects/demo.h" // IMPORT OBJECTS HERE

// (later this will be moved into window.h and window.cpp)
// (similar to scene.h and scene.cpp)

class Window : public ppgso::Window {
private:
    Scene scene;

    void initScene() {
        scene.objects.clear();

        // Create a camera
        // scene.camera =

        // ADD OBJECTS HERE

            // Add skybox
            // scene.objects.push_back()

            // Add enemies to the scene
            // scene.objects.push_back()

            // Add a player to the scene
            // scene.objects.push_back()
    }

public:
    Window() : ppgso::Window{"CastleCall", 1280, 720} {
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

    // void onIdle() override {
    void onIdle()  {
        // Track time
        static auto time = (float) glfwGetTime();

        // Compute time delta
        float dt = (float) glfwGetTime() - time;

        // Set dark gray background
        glClearColor(0.1f, 0.1f, 0.1f, 0);

        // Clear depth and color buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Update and render all objects
        scene.update(dt);
        scene.render();
    }
};