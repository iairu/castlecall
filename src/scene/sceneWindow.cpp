#include <iostream>
#include <map>
#include <list>

#include <ppgso/ppgso.h>
#include "scene.h"

#include "map.h"

#include <shaders/screen_bright_vert_glsl.h>
#include <shaders/screen_bright_frag_glsl.h>
#include <shaders/screen_blur_vert_glsl.h>
#include <shaders/screen_blur_frag_glsl.h>
#include <shaders/screen_pass_vert_glsl.h>
#include <shaders/screen_pass_frag_glsl.h>
#include <shaders/screen_blend_vert_glsl.h>
#include <shaders/screen_blend_frag_glsl.h>

// #include "../objects/demo.h" // IMPORT OBJECTS HERE

// (later this will be moved into window.h and window.cpp)
// (similar to scene.h and scene.cpp)

#define SW_WIDTH 1280
#define SW_HEIGHT 720

class SceneWindow : public ppgso::Window {
private:
    Scene scene;
    Map * map;

    unsigned int current_scene_id;

    // Post-processing: renderbuffer/framebuffer/colorbuffer/... parts, shaders, 2D "screen" output => rectangle (rect_...)
    unsigned int renderbuffer;
    unsigned int framebuffer_scene;
    unsigned int colorbuffer_scene[2]; // todo unused colorbuffer_scene[1]
    unsigned int framebuffer_bright;
    unsigned int colorbuffer_bright[2]; // todo unused colorbuffer_bright[1]
    unsigned int framebuffer_blur[2];
    unsigned int colorbuffer_blur[2];
    std::unique_ptr<ppgso::Shader> post_shader_pass; // just pass whatever colors already exist without changes
    std::unique_ptr<ppgso::Shader> post_shader_bright; // only export brighter areas
    std::unique_ptr<ppgso::Shader> post_shader_blur; // gaussian blur per direction
    std::unique_ptr<ppgso::Shader> post_shader_blend; // combine bright+blur passes
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
        this->current_scene_id = 1;
        /*this->map->placeItems(2, &scene);
        this->map->placeItems(3, &scene);*/

        // ADD OBJECTS HERE

            // Add skybox
            // scene.objects.push_back()

            // Add enemies to the scene
            // scene.objects.push_back()

            // Add a player to the scene
            // scene.objects.push_back()
    }

    void switchScene(unsigned int tgt_id) {
        if(tgt_id == current_scene_id) return;
        current_scene_id = tgt_id;
        scene.objects.clear();
        this->map->placeItems(current_scene_id, &scene);
    }

    // Creating framebuffers and colorbuffers
    void createFramebuffer(unsigned int * framebuffer, unsigned int renderbuffer) {
        glGenFramebuffers(1, framebuffer); // create a single framebuffer
        glBindFramebuffer(GL_FRAMEBUFFER, *framebuffer); // set framebuffer as active
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderbuffer); // set its renderbuffer
    }
    void create2D(unsigned int count, unsigned int * colorbuffers, unsigned int * framebuffers) {
        unsigned int attachments[count];
        glGenTextures(count, colorbuffers);
        for (unsigned int i = 0; i < count; i++) {
            glBindFramebuffer(GL_FRAMEBUFFER, framebuffers[i]); // set framebuffer as active for framebuffertexture2d
            glBindTexture(GL_TEXTURE_2D, colorbuffers[i]);
            // larger color depth - ideal for gamma correction, also includes an alpha channel which isn't really used right now
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, SW_WIDTH, SW_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL); 
            // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SW_WIDTH, SW_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // don't overflow post-process (shaders) over edges
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // don't overflow post-process (shaders) over edges
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, colorbuffers[i], 0);

            attachments[i] = GL_COLOR_ATTACHMENT0 + i;
        }
        // tell OpenGL that we're using 2 color buffers, not just one
        glDrawBuffers(count, attachments);
    }

    // Using framebuffers and colorbuffers
    // Help functions for often used framebuffer configuration, binding, rendering, ...
    void useFramebuffer(unsigned int framebuffer, bool depth = false, bool setbg = false) {
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer); // Bind the given framebuffer as active
        if (setbg) glClearColor(0.0f, 0.1f, 0.1f, 0); // Set dark blue background
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear depth and color buffers
        if (depth) glEnable(GL_DEPTH_TEST); else glDisable(GL_DEPTH_TEST);// Enable depth Z-buffer
    }

    void render2D(unsigned int colorbuffer, unsigned int colorbuffer2 = 0) { // todo is 0 guaranteed unused?
        glBindVertexArray(rect_vao); // positioning of the 2D post-processed image on the screen (fullscreen)
        glBindTexture(GL_TEXTURE_2D, colorbuffer); // save the color contents
        if (colorbuffer2 != 0) { // save to the second colorbuffer as well
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, colorbuffer2);
            glActiveTexture(GL_TEXTURE0);
        }
        glDrawArrays(GL_TRIANGLES, 0, 6); // 6 coresponds to qmap coord amount (6 vertices => 2 triangles => one rectangle)
        glBindVertexArray(0);
    }

public:
    SceneWindow() : ppgso::Window{"CastleCall", SW_WIDTH, SW_HEIGHT} {


        // Shaders for post-processing
        post_shader_pass = std::make_unique<ppgso::Shader>(screen_pass_vert_glsl, screen_pass_frag_glsl);
        post_shader_bright = std::make_unique<ppgso::Shader>(screen_bright_vert_glsl, screen_bright_frag_glsl);
        post_shader_blur = std::make_unique<ppgso::Shader>(screen_blur_vert_glsl, screen_blur_frag_glsl);
        post_shader_blend = std::make_unique<ppgso::Shader>(screen_blend_vert_glsl, screen_blend_frag_glsl);

        // FRAMEBUFFER CREATION ---------------------------------

        // Create a framebuffer with attachments for post-processing
        // A post-process framebuffer consists of 2 color-attachments (color and bright-areas-color) (the rendered view will go here) 
        // and unused depth+stencil attachments (for which we will use a renderbuffer, which doesn't provide any modification interfaces)
        // unsigned int fb_renderbuffer;
        glGenRenderbuffers(1, &renderbuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, SW_WIDTH, SW_HEIGHT);

        createFramebuffer(&framebuffer_scene, renderbuffer);
        create2D(2, colorbuffer_scene, &framebuffer_scene);

        createFramebuffer(&framebuffer_bright, renderbuffer);
        create2D(2, colorbuffer_bright, &framebuffer_bright);

        createFramebuffer(framebuffer_blur, renderbuffer);
        createFramebuffer(framebuffer_blur + 1, renderbuffer);
        create2D(2, colorbuffer_blur, framebuffer_blur);

        // // Verify framebuffer completeness
        // if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        //     std::cout << "ERROR: Framebuffer incomplete!" << std::endl;
        // }

        // Vertex arrays and buffers for rect_map, which maps the final 2D post-processed output coordinates 
        // to the whole screen (it's a rectangle)
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

        // FRAMEBUFFER SCENE ---------------------------

        // This is the only time we're rendering 3D to 2D (depth enabled)
        // all other framebuffers will be 2D only (with depth disabled)
        useFramebuffer(framebuffer_scene, true, true);
        // Update and render all objects
        scene.update(dt);

        switchScene(scene.tgtScene());

        scene.render();
        render2D(colorbuffer_bright[0]);

        // FRAMEBUFFER BRIGHTNESS -----------------------------

        useFramebuffer(framebuffer_bright);
        post_shader_bright->use();
        render2D(colorbuffer_scene[0]);

        // FRAMEBUFFER BLUR ---------------------------------

        bool horizontal = true;
        bool first_iteration = true;
        unsigned int amount = 10;
        post_shader_blur->use();
        for (unsigned int i = 0; i < amount; i++) {
            glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_blur[horizontal]); 
            post_shader_blur->setUniform("horizontal", horizontal ? 1.0 : 0.0);
            render2D(first_iteration ? colorbuffer_bright[0] : colorbuffer_blur[!horizontal]);
            horizontal = !horizontal;
            if (first_iteration) first_iteration = false;
        }

        // OUTPUT BLEND -----------------------------

        // colorbuffer_scene[0] contains untouched scene
        // colorbuffer_bright[0] contains brightness filter
        // colorbuffer_blur[0] contains blurred brightness filter

        // Default framebuffer
        useFramebuffer(0); 

        post_shader_blend->use();
        post_shader_blend->setUniformInt("scene", 0);
        post_shader_blend->setUniformInt("bloomBlur", 1);
        post_shader_blend->setUniform("exposure", 1.0f);
        post_shader_blend->setUniform("gamma", 1.2f);
        post_shader_blend->setUniform("bloom", 1.0f);
        render2D(colorbuffer_scene[0], colorbuffer_blur[!horizontal]);

        // uncomment to remove bloom (and comment-out the shader above)
        
        // post_shader_pass->use();
        // render2D(colorbuffer_scene[0]);

    }

    ~SceneWindow() {
        delete this->map;
    }
};
