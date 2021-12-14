#ifndef _HTREE_H
#define _HTREE_H


#include <memory>

#include <ppgso/ppgso.h>

#include "../scene.h"
#include "../object.h"

class Trunk final : private Object {
    private:
        // Static resources (Shared between instances)
        static std::unique_ptr<ppgso::Mesh> mesh;
        static std::unique_ptr<ppgso::Shader> shader;
        static std::unique_ptr<ppgso::Texture> texture;

    public:
        Trunk();

        void render(Scene &scene) override;
        bool update(Scene &scene, float dt) override {
            generateModelMatrix();
            return true;
        }
        void onClick(Scene &scene) override {}
};

class Leaves final : private Object {
    private:
        // Static resources (Shared between instances)
        static std::unique_ptr<ppgso::Mesh> mesh;
        static std::unique_ptr<ppgso::Shader> shader;
        static std::unique_ptr<ppgso::Texture> texture;

    public:
        Leaves();

        void render(Scene &scene) override;
        bool update(Scene &scene, float dt) override {
            generateModelMatrix();
            return true;
        }
        void onClick(Scene &scene) override {}
};

class Ground final : private Object {
    private:
        // Static resources (Shared between instances)
        static std::unique_ptr<ppgso::Mesh> mesh;
        static std::unique_ptr<ppgso::Shader> shader;
        static std::unique_ptr<ppgso::Texture> texture;

    public:
        Ground();

        void render(Scene &scene) override;
        bool update(Scene &scene, float dt) override {
            generateModelMatrix();
            return true;
        }
        void onClick(Scene &scene) override {}
};

class Tree final : private Object {
    private:
        // Hierarchy of objects
        std::unique_ptr<Trunk> trunk;
        std::unique_ptr<Leaves> leaves;

    public:
        Tree();

        void render(Scene &scene) override;
        bool update(Scene &scene, float dt) override;
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
