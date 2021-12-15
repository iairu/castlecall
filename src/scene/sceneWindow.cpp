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

    // framebuffer parts
    unsigned int framebuffer_default;
    unsigned int fb_colorbuffers_default[2];
    unsigned int fb_renderbuffer_default;
    std::unique_ptr<ppgso::Shader> post_shader_pass; // just pass whatever colors already exist without changes
    unsigned int framebuffer;
    unsigned int fb_colorbuffers[2];
    unsigned int fb_renderbuffer;
    std::unique_ptr<ppgso::Shader> post_shader_bright; // only export brighter areas
    unsigned int pingpongFBO[2];
    unsigned int pingpongBuffer[2];
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
        post_shader_pass = std::make_unique<ppgso::Shader>(screen_pass_vert_glsl, screen_pass_frag_glsl);
        post_shader_bright = std::make_unique<ppgso::Shader>(screen_bright_vert_glsl, screen_bright_frag_glsl);
        post_shader_blur = std::make_unique<ppgso::Shader>(screen_blur_vert_glsl, screen_blur_frag_glsl);
        post_shader_blend = std::make_unique<ppgso::Shader>(screen_blend_vert_glsl, screen_blend_frag_glsl);

        // FRAMEBUFFER CREATION ---------------------------------

        // Create a framebuffer with attachments for post-processing
        // A post-process framebuffer consists of 2 color-attachments (color and bright-areas-color) (the rendered view will go here) 
        // and unused depth+stencil attachments (for which we will use a renderbuffer, which doesn't provide any modification interfaces)
        // unsigned int fb_renderbuffer;
        glGenRenderbuffers(1, &fb_renderbuffer_default);
        glBindRenderbuffer(GL_RENDERBUFFER, fb_renderbuffer_default);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, SW_WIDTH, SW_HEIGHT);
        // unsigned int framebuffer;
        glGenFramebuffers(1, &framebuffer_default);
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_default);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, fb_renderbuffer_default);
        // unsigned int fb_colorbuffer;
        glGenTextures(2, fb_colorbuffers);
        for (unsigned int i = 0; i < 2; i++) {
            glBindTexture(GL_TEXTURE_2D, fb_colorbuffers[i]);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, SW_WIDTH, SW_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
            // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SW_WIDTH, SW_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, fb_colorbuffers[i], 0);
        }
        // tell OpenGL that we're using 2 color buffers, not just one
        unsigned int attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
        glDrawBuffers(2, attachments);
        // Verify framebuffer completeness
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            std::cout << "ERROR: Framebuffer incomplete!" << std::endl;
        }



        glGenRenderbuffers(1, &fb_renderbuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, fb_renderbuffer);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, SW_WIDTH, SW_HEIGHT);

        glGenFramebuffers(1, &framebuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, fb_renderbuffer);

        glGenTextures(2, fb_colorbuffers_default);
        for (unsigned int i = 0; i < 2; i++) {

            glBindTexture(GL_TEXTURE_2D, fb_colorbuffers_default[i]);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, SW_WIDTH, SW_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
            // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SW_WIDTH, SW_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, fb_colorbuffers_default[i], 0);
        }
        // tell OpenGL that we're using 2 color buffers, not just one
        glDrawBuffers(2, attachments);
        // Verify framebuffer completeness
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            std::cout << "ERROR: Framebuffer incomplete!" << std::endl;
        }


        // blur
        glGenFramebuffers(2, pingpongFBO);
        glGenTextures(2, pingpongBuffer);
        for (unsigned int i = 0; i < 2; i++)
        {
            glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[i]);
            glBindTexture(GL_TEXTURE_2D, pingpongBuffer[i]);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, SW_WIDTH, SW_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
            // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SW_WIDTH, SW_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glFramebufferTexture2D(
                GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pingpongBuffer[i], 0
            );
        }

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

        // FRAMEBUFFER SCENE ---------------------------

        // Render to our framebuffer for later post-processing
        // The render should (naturally) have depth enabled
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_default);
        // Set dark blue background
        // Clear depth and color buffers
        glClearColor(0.0f, 0.1f, 0.1f, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST); // Enable Z-buffer
        // Update and render all objects
        scene.update(dt);
        scene.render();

        glBindVertexArray(rect_vao); // positioning of the 2D post-processed image on the screen (fullscreen)
        glBindTexture(GL_TEXTURE_2D, fb_colorbuffers_default[0]); // get the color contents (recycled first color buffer for final output)
        glDrawArrays(GL_TRIANGLES, 0, 6); // 6 coresponds to qmap coord amount (6 vertices => 2 triangles => one rectangle)
        glBindVertexArray(0);

        // FRAMEBUFFER BRIGHTNESS -----------------------------

        // Render to our framebuffer for later post-processing
        // The render should (naturally) have depth enabled
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
        // Set dark blue background
        // Clear depth and color buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glDisable(GL_DEPTH_TEST); // Enable Z-buffer
        // Update and render all objects

        post_shader_bright->use();
        glBindVertexArray(rect_vao); // positioning of the 2D post-processed image on the screen (fullscreen)
        glBindTexture(GL_TEXTURE_2D, fb_colorbuffers[0]); // get the color contents (recycled first color buffer for final output)
        glDrawArrays(GL_TRIANGLES, 0, 6); // 6 coresponds to qmap coord amount (6 vertices => 2 triangles => one rectangle)
        glBindVertexArray(0);

        // FRAMEBUFFER BLUR ---------------------------------

        // Output along with post-processing to the default framebuffer
        // The default framebuffer should not have depth enabled because
        // it only outputs a post-processed 2D texture over the whole screen
        bool horizontal = true;
        bool first_iteration = true;
        unsigned int amount = 10;
        post_shader_blur->use();
        for (unsigned int i = 0; i < amount; i++)
        {
            glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[horizontal]); 
            post_shader_blur->setUniform("horizontal", horizontal ? 1.0 : 0.0);
            // blur input: start with the rendered scene, save it to pingpongbuffer and then keep adding on top of pingpongbuffer
            glBindTexture(GL_TEXTURE_2D, first_iteration ? fb_colorbuffers_default[0] : pingpongBuffer[!horizontal]); 
            glBindVertexArray(rect_vao); // positioning of the 2D post-processed image on the screen (fullscreen)
            glDrawArrays(GL_TRIANGLES, 0, 6); // 6 coresponds to qmap coord amount (6 vertices => 2 triangles => one rectangle)
            glBindVertexArray(0);
            horizontal = !horizontal;
            if (first_iteration)
                first_iteration = false;
        }


        // OUTPUT BLEND -----------------------------

        // fb_colorbuffers_default[0] contains brightness filter
        // pingpongBuffer[0] contains blurred brightness filter
        // fb_colorbuffers[0] contains untouched scene

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        // Clear color buffer (setting a background color is pointless in this case)
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // post_shader_pass->use();
        // glBindVertexArray(rect_vao); // positioning of the 2D post-processed image on the screen (fullscreen)
        // glBindTexture(GL_TEXTURE_2D, pingpongBuffer[0]); // get the color contents (recycled first color buffer for final output)
        // glDrawArrays(GL_TRIANGLES, 0, 6); // 6 coresponds to qmap coord amount (6 vertices => 2 triangles => one rectangle)
        post_shader_blend->use();
        glEnable(GL_TEXTURE_2D);
        glActiveTexture(GL_TEXTURE0);
        post_shader_blend->setUniformInt("scene", 0);
        glBindTexture(GL_TEXTURE_2D, fb_colorbuffers[0]); // get the color contents (recycled first color buffer for final output)
        glActiveTexture(GL_TEXTURE1);
        post_shader_blend->setUniformInt("bloomBlur", 1);
        glBindTexture(GL_TEXTURE_2D, pingpongBuffer[0]); // get the color contents (recycled first color buffer for final output)
        post_shader_blend->setUniform("exposure", 1.0);
        post_shader_blend->setUniform("gamma", 1.2);
        post_shader_blend->setUniform("bloom", 1.0);
        glBindVertexArray(rect_vao); // positioning of the final 2D post-processed image on the screen (fullscreen)
        glDrawArrays(GL_TRIANGLES, 0, 6); // 6 coresponds to qmap coord amount (6 vertices => 2 triangles => one rectangle)
        glBindVertexArray(0);

    }
};
