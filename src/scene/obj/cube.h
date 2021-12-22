#ifndef _CUBE_H
#define _CUBE_H


#include <memory>

#include <ppgso/ppgso.h>

#include "../scene.h"
#include "../object.h"
#include "../forces.h"

//#include "paths.h"

class Cube final : public ForceObject {
    private:
        // Static resources (Shared between instances)
        static std::unique_ptr<ppgso::Mesh> mesh;
        static std::unique_ptr<ppgso::Shader> shader;
        static std::unique_ptr<ppgso::Texture> texture;
        typedef ForceObject super;
    public:
        Cube();

        void render(Scene &scene) override;

        bool update(Scene &scene, float dt) override {
            super::update(scene, dt);
            generateModelMatrix();
            return true;
        }

        void onClick(Scene &scene) override {}
};

#endif
