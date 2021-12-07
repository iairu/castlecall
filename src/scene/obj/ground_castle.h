
#ifndef _GROUND_CASTLE_H
#define _GROUND_CASTLE_H


#include <memory>

#include <ppgso/ppgso.h>

#include "../scene.h"
#include "../object.h"

class GroundCastle final : public Object {
    private:
        // Static resources (Shared between instances)
        static std::unique_ptr<ppgso::Mesh> mesh;
        static std::unique_ptr<ppgso::Shader> shader;
        static std::unique_ptr<ppgso::Texture> texture;

    public:
        GroundCastle();

        void render(Scene &scene) override;

        bool update(Scene &scene, float dt) override {
            generateModelMatrix();
            return true;
        }

        void onClick(Scene &scene) override {}
};

#endif
