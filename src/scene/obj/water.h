
#ifndef _WATER_H
#define _WATER_H


#include <memory>

#include <ppgso/ppgso.h>

#include "../scene.h"
#include "../object.h"

//#include "paths.h"

class Water final : public Object {
    private:
        // Static resources (Shared between instances)
        static std::unique_ptr<ppgso::Mesh> mesh;
        static std::unique_ptr<ppgso::Shader> shader;
        static std::unique_ptr<ppgso::Texture> texture;

    public:
        Water();

        void render(Scene &scene) override;

        bool update(Scene &scene, float dt) override {
            generateModelMatrix();
            return true;
        }

        void onClick(Scene &scene) override {}
};

#endif
