#ifndef _HTREE_H
#define _HTREE_H


#include <memory>

#include <ppgso/ppgso.h>

#include "../scene.h"
#include "../object.h"

class Trunk final : public Object {
    private:
        // Static resources (Shared between instances)
        static std::unique_ptr<ppgso::Mesh> mesh;
        static std::unique_ptr<ppgso::Shader> shader;
        static std::unique_ptr<ppgso::Texture> texture;

    public:
        Trunk();

        bool parent_update(glm::mat4 parentModelMatrix, glm::mat4 parentParentModelMatrix);

        void render(Scene &scene) override;
        bool update(Scene &scene, float dt) override {return false;}
        void onClick(Scene &scene) override {}
};

class Leaves final : public Object {
    private:
        // Static resources (Shared between instances)
        static std::unique_ptr<ppgso::Mesh> mesh;
        static std::unique_ptr<ppgso::Shader> shader;
        static std::unique_ptr<ppgso::Texture> texture;

    public:
        Leaves();

        bool parent_update(glm::mat4 parentModelMatrix, glm::mat4 parentParentModelMatrix);

        void render(Scene &scene) override;
        bool update(Scene &scene, float dt) override {return false;}
        void onClick(Scene &scene) override {}
};

class Ground final : public Object {
    private:
        // Static resources (Shared between instances)
        static std::unique_ptr<ppgso::Mesh> mesh;
        static std::unique_ptr<ppgso::Shader> shader;
        static std::unique_ptr<ppgso::Texture> texture;

    public:
        Ground();

        bool parent_update(glm::mat4 parentModelMatrix);

        void render(Scene &scene) override;
        bool update(Scene &scene, float dt) override {return false;}
        void onClick(Scene &scene) override {}
};

class Tree final : public Object {
    private:
        // Hierarchy of objects
        std::unique_ptr<Trunk> trunk;
        std::unique_ptr<Leaves> leaves;
        std::unique_ptr<Leaves> leaves2;

    public:
        Tree();

        bool parent_update(glm::mat4 parentModelMatrix);

        void render(Scene &scene) override;
        bool update(Scene &scene, float dt) override {return false;}
        void onClick(Scene &scene) override {}
};

class HierarchicalTree final : public Object {
    private:
        // Hierarchy of objects
        std::unique_ptr<Tree> tree;
        std::unique_ptr<Ground> ground;

    public:
        HierarchicalTree();

        void render(Scene &scene) override;
        bool update(Scene &scene, float dt) override;
        void onClick(Scene &scene) override {}
};

#endif
