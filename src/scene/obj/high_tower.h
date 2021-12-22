
#ifndef _HIGH_TOWER_H
#define _HIGH_TOWER_H


#include <memory>

#include <ppgso/ppgso.h>

#include "../scene.h"
#include "../object.h"

class HighTower final : public Object {
    private:
        // Static resources (Shared between instances)
        static std::unique_ptr<ppgso::Mesh> mesh;
        static std::unique_ptr<ppgso::Shader> shader;
        static std::unique_ptr<ppgso::Texture> texture;

    public:
        HighTower();

        void render(Scene &scene, std::unique_ptr<ppgso::Shader> altShader) override;

        bool update(Scene &scene, float dt) override;

        void onClick(Scene &scene) override {}
};

#endif
