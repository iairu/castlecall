#ifndef _CUBE_H
#define _CUBE_H


#include <memory>

#include <ppgso/ppgso.h>

#include "../scene.h"
#include "../object.h"
#include "../gravity.h"

//#include "paths.h"

class Cube final : public GravityObject {
    private:
        // Static resources (Shared between instances)
        static std::unique_ptr<ppgso::Mesh> mesh;
        static std::unique_ptr<ppgso::Shader> shader;
        static std::unique_ptr<ppgso::Texture> texture;
        typedef GravityObject super;
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
