#include <iostream>
#include <map>
#include <list>

#include <ppgso/ppgso.h>
#include "scene.h"

#include "map.h"

#include <shaders/screen_vert_glsl.h>
#include <shaders/screen_frag_glsl.h>

// #include "../objects/demo.h" // IMPORT OBJECTS HERE

// (later this will be moved into window.h and window.cpp)
// (similar to scene.h and scene.cpp)

#define SW_WIDTH 1280
#define SW_HEIGHT 720

class SceneWindow : public ppgso::Window {
private:
    Scene scene;
    Map * map;

    // framebuffer parts
    unsigned int framebuffer;
    unsigned int fb_colorbuffer;
    unsigned int fb_renderbuffer;
    std::unique_ptr<ppgso::Shader> post_shader;
    unsigned int rect_vao, rect_vbo;
    const float rect_map[4*3*2] = {
        // coordinates
        // vertex      // texture
        -1.0f,  1.0f,   0.0f, 1.0f,
        -1.0f, -1.0f,   0.0f, 0.0f,
         1.0f, -1.0f,   1.0f, 0.0f,

        -1.0f,  1.0f,   0.0f, 1.0f,
         1.0f, -1.0f,   1.0f, 0.0f,
         1.0f,  1.0f,   1.0f, 1.0f
     };

    void initScene() {
        scene.objects.clear();

        scene.camera = NULL;

        this->map = new Map;
        this->map->placeItems(1, &scene);
        this->map->placeItems(2, &scene);
        this->map->placeItems(3, &scene);

        // ADD OBJECTS HERE

            // Add skybox
            // scene.objects.push_back()

            // Add enemies to the scene
            // scene.objects.push_back()

            // Add a player to the scene
            // scene.objects.push_back()
    }

public:
    SceneWindow() : ppgso::Window{"CastleCall", SW_WIDTH, SW_HEIGHT} {


        // Shader for the post-process output
        post_shader = std::make_unique<ppgso::Shader>(screen_vert_glsl, screen_frag_glsl);

        // FRAMEBUFFER CREATION ---------------------------------

        // Create a framebuffer with attachments for post-processing
        // A post-process framebuffer consists of a color-attachment (the rendered view will go here) 
        // and unused depth+stencil attachments (for which we will use a renderbuffer, which doesn't provide any modification interfaces)
        // unsigned int framebuffer;
        glGenFramebuffers(1, &framebuffer);
        // unsigned int fb_colorbuffer;
        glGenTextures(1, &fb_colorbuffer);
        glBindTexture(GL_TEXTURE_2D, fb_colorbuffer);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SW_WIDTH, SW_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // todo later add edge texparameters so bloom won't overflow
        // unsigned int fb_renderbuffer;
        glGenRenderbuffers(1, &fb_renderbuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, fb_renderbuffer);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, SW_WIDTH, SW_HEIGHT);

        // Enable framebuffer for drawing (writing), but not reading
        // Attach the framebuffer attachments
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, framebuffer);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fb_colorbuffer, 0); // todo increase amount of color_attachments for bloom
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, fb_renderbuffer);
        // glBindFramebuffer(GL_DRAW_FRAMEBUFFER, NULL); // render directly to default framebuffer, bypass our framebuffer

        // Verify framebuffer completeness
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            std::cout << "ERROR: Framebuffer incomplete!" << std::endl;
        }

        // Vertex arrays and buffers for rect_map, which maps the final 2D post-processed output coordinates to the whole screen (it's a rectangle)
        glGenVertexArrays(1, &rect_vao);
        glGenBuffers(1, &rect_vbo);
        glBindVertexArray(rect_vao);
        glBindBuffer(GL_ARRAY_BUFFER, rect_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(rect_map), &rect_map, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

        // FRAMEBUFFER CREATION END -----------------------------

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

        // FRAMEBUFFER ---------------------------------

        // Render to our framebuffer for later post-processing
        // The render should (naturally) have depth enabled
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, framebuffer);
        glEnable(GL_DEPTH_TEST); // Enable Z-buffer
        // Set dark blue background
        // Clear depth and color buffers
        glClearColor(0.0f, 0.1f, 0.1f, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // Update and render all objects
        scene.update(dt);
        scene.render();

        // OUTPUT ---------------------------------

        // Output along with post-processing to the default framebuffer
        // The default framebuffer should not have depth enabled because
        // it only outputs a post-processed 2D texture over the whole screen
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
        glDisable(GL_DEPTH_TEST);
        // Clear color buffer (clearing depth and setting a background color is pointless in this case)
        glClear(GL_COLOR_BUFFER_BIT);

        post_shader->use();
        glBindVertexArray(rect_vao); // positioning of the final 2D post-processed image on the screen (fullscreen)
        glBindTexture(GL_TEXTURE_2D, fb_colorbuffer); // get the texture contents
        glDrawArrays(GL_TRIANGLES, 0, 6); // 6 coresponds to qmap coord amount (6 vertices => 2 triangles => one rectangle)
    }
};
