#ifndef _LEAFP_H
#define _LEAFP_H

#include <memory>

#include <ppgso/ppgso.h>

#include "../scene.h"
#include "../object.h"
#include "../forces.h"

class LeafParticle final : public ForceObject {
    private:
        // Static resources (Shared between instances)
        static std::unique_ptr<ppgso::Mesh> mesh;
        static std::unique_ptr<ppgso::Shader> shader;
        static std::unique_ptr<ppgso::Texture> texture;
        typedef ForceObject super;
    public:
        float decayTime; // generated alongside particles starting position by LeafParticleSystem::generateParticles();
        
        LeafParticle();

        // these particles are used within hierarchical_tree - for that reason there is parent_update instead of just update
        bool parent_update(glm::mat4 parentModelMatrix, Scene &scene, float dt);

        void render(Scene &scene, std::unique_ptr<ppgso::Shader> altShader) override;
        bool update(Scene &scene, float dt) override {
            super::update(scene, dt);
            generateModelMatrix();
            return true;
        }
        void onClick(Scene &scene) override {}
};

class LeafParticleSystem {
    private:
        std::list<std::unique_ptr<LeafParticle>> particles;
        void generateParticle();
    public:
        float min_gen_radius = 3.0f; // see generateParticle
        float max_gen_radius = 4.3f; // see generateParticle
        float min_gen_decayTime = 1000.0f; // see generateParticle
        float max_gen_decayTime = 4000.0f; // see generateParticle
        float mass = 0.3f; // lower mass => slower gravity fall
        float height = 6.0f; // see generateParticle - all start at this height and are brought down by gravity
        int n = 2; // number of particles at any given time

        LeafParticleSystem();

        void generateParticles();
        bool parent_update(glm::mat4 parentModelMatrix, Scene &scene, float dt);
        void render(Scene &scene, std::unique_ptr<ppgso::Shader> altShader);
};

#endif
