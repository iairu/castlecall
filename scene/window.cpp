#include <iostream>
#include <map>
#include <list>

#include "scene.h"
#include "../objects/demo.h" // objects are imported here

// (later this will be moved into scenewindow.h and scenewindow.cpp similar to scene.h and scene.cpp)

// class SceneWindow : public ppgso::Window { // <-- make this work as an actual window
class Window {
private:
    Scene scene;

    void initScene() {
        // scene.objects.clear();

        // Create a camera

        // Add skybox

        // Add enemies to the scene

        // Add a player to the scene
    }

public:
    // SceneWindow() : Window{"CastleCall", 1280, 720} { // <-- then make this work
    Window() {
        // // Initialize OpenGL state
        // // Enable Z-buffer
        // glEnable(GL_DEPTH_TEST); // <-- make opengl work
        // glDepthFunc(GL_LEQUAL);

        // // Enable polygon culling
        // glEnable(GL_CULL_FACE);
        // glFrontFace(GL_CCW);
        // glCullFace(GL_BACK);

        initScene();
    }

    // void onIdle() override {
    void onIdle()  {
        // // Track time
        // static auto time = (float) glfwGetTime();

        // // Compute time delta
        // float dt = (float) glfwGetTime() - time;

        // // Set white background
        // glClearColor(1, 1, 1, 0);

        // // Clear depth and color buffers
        // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // // Update and render all objects
        // scene.update(dt);
        // scene.render();
    }
};