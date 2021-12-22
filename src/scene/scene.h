#ifndef _CASTLECALL_SCENE_H
#define _CASTLECALL_SCENE_H

#include <memory>
#include <map>
#include <list>

#include "object.h"
#include "forces.h"
#include "camera.h"

typedef enum _AXIS {X, Y, Z} AXIS;

typedef struct _SCENE_SWITCHER {
    unsigned int source, target; // prepinac sceny
    AXIS check; // os na porovnanie
    bool src_is_less; // znacka na ktorej strane priesecnika sa ma scena prepnut
    float pos; // pozicia na osi
} SCENE_SWITCHER, *PSCENE_SWITCHER;


class Scene {
    private:
        std::vector<SCENE_SWITCHER> switchers;

        unsigned int scene_id;
    
    public:
        void update(float time); // mass update all objects
        void render(std::unique_ptr<ppgso::Shader> altShader); // mass render all objects

        // Camera
        std::unique_ptr<Camera> camera;

        // List of all objects to be rendered in this scene
        std::list<std::unique_ptr<Object>> objects;

        // List of invisible boxes that can stop gravity for ForceObjects
        std::list<std::unique_ptr<CollisionBox>> collisionboxes;
        bool renderCollisionBoxes = false; // for debuging

        // for phong shader
        glm::vec3 lightPos1 = glm::vec3{1.0f, 3.0f, 1.0f};
        glm::vec3 lightPos2 = glm::vec3{9.16f, -0.23f, 10.74f};
        glm::vec3 lightPos3 = glm::vec3{-5.33f, 1.53f, -2.71f};

        glm::vec3 tintColor1 = glm::vec3{1.0f, 1.0f, 1.0f};
        glm::vec3 tintColor2 = glm::vec3{1.75f, 1.0f, 0.94f};
        glm::vec3 tintColor3 = glm::vec3{-0.86f, -0.23f, 0.21f};

        glm::vec3 ambientColor = glm::vec3{1.0f, 1.0f, 1.0f};
        glm::vec3 specularColor = glm::vec3{1.0f, 1.0f, 1.0f};
        glm::vec3 diffuseColor = glm::vec3{1.0f, 1.0f, 1.0f};
        
        float ambient = 0.2f;
        float diffusePower = 0.88f;
        float specularPower = 0.14f;
        float specLight = 1.54f;
        int specAmountPow = 8;

        // Animated tint color 1
        glm::vec3 tintColor1_from = glm::vec3{1.0f, 1.0f, 0.5f};
        glm::vec3 tintColor1_to = glm::vec3{0.5f, 0.5f, 1.0f};
        float tintColor1_onTime = 0.0f;
        float tintColor1_maxTime = 5000.0f;
        bool tintColor1_pong = false;

        // Keyboard state (map)
        std::map< int, int > keyboard;

        // Cursor state (struct)

        
        void registerSwitcher(unsigned int owner, glm::vec3 targets, glm::vec3 pos);

        void setID(unsigned int id) {
            this->scene_id = id;
        }

        unsigned int tgtScene() {
            return this->scene_id;
        }

        void performSwitchCheck();
};

#endif