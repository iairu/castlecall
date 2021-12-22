
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
        static std::unique_ptr<ppgso::Shader> shader;
        static std::unique_ptr<ppgso::Texture> texture;

        // Control points
        float cp_x;

        // 3D vectors define points/vertices of the shape
        std::vector<glm::vec3> vertices;

        // Texture coordinates
        std::vector<glm::vec2> texCoords;

        //std::vector<GLuint> indices;

        // Define our face using indexes to 3 vertices
        struct face {
            GLuint v0, v1, v2;
        };

        // Define our mesh as collection of faces
        std::vector<face> mesh;

        // These will hold the data and object buffers
        GLuint vao, vbo, tbo, ibo;


        glm::vec3 bezierPoint(const glm::vec3 controlPoints[4], float t) {
            // Compute 3D point on bezier curve

            std::vector<glm::vec3> p = {controlPoints[0], controlPoints[1], controlPoints[2], controlPoints[3], controlPoints[0]};
            int n = 4;
            // DeCastlejau non-recursive
            for (int i = 1; i < n; i++) {
                for (int j = 0; j < n - i; j++) {
                    p[j][0] = (1-t)*p[j][0] + t*p[j+1][0]; // based on linear bezier curve
                    p[j][1] = (1-t)*p[j][1] + t*p[j+1][1];
                    p[j][2] = (1-t)*p[j][2] + t*p[j+1][2];
                }
            }
            return p[0];
        }

    public:
        Water();
        ~Water();

        void updateControlPoints(bool init);

        void bezierPatch(glm::vec3 controlPoints[4][4], bool init);

        void buffer(bool init);
        
        void unbuffer();

        void render(Scene &scene, std::unique_ptr<ppgso::Shader> altShader) override;

        bool update(Scene &scene, float dt) override;

        void onClick(Scene &scene) override {}
};

#endif
