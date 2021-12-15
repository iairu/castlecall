
#include <functional>
#include <random>

#include "nature_gen.h"
#include "obj/hierarchical_tree.h"

void NatureGen::placeTrees(Scene *scene, glm::vec3 pos, glm::vec3 size) {
    static std::random_device dev;
    static std::mt19937 gen(dev());
    static std::uniform_int_distribution<> n(1,100);
    float x_off = (size.x * 2) * ((float)n(gen) / 100);
    float z_off = (size.z * 2) * ((float)n(gen) / 100);
    float x_start = pos.x + size.x, z_start = pos.z + size.z;
    // std::cout << n(gen) << std::endl;
    if(pos.x < 0) x_start = pos.x - size.x;
    if(pos.z < 0) z_start = pos.z - size.z;

    std::unique_ptr<HierarchicalTree> htree = std::make_unique<HierarchicalTree>();
    htree->position = {x_start + x_off, pos.y, z_start + z_off};
    htree->scale = {0.25, 0.25, 0.25}; // scale to 1/4 size
    htree->rotation = {0, 0, 0}; // last one rotates around up-down axis ( in radians )
    scene->objects.push_back(move(htree));
    
    if((size.x > 1.9 || size.z > 1.9 ) && ((x_off < 30 || z_off < 30) || (x_off > 70 || z_off > 70))) {
        x_off = (size.x * 2) - x_off;
        z_off = (size.z * 2) - z_off;

        htree = std::make_unique<HierarchicalTree>();
        
        htree->position = {x_start + x_off, pos.y, z_start + z_off};
        htree->scale = {0.25, 0.25, 0.25}; // scale to 1/4 size
        htree->rotation = {0, 0, 3}; // last one rotates around up-down axis ( in radians )

        scene->objects.push_back(move(htree));
    }

    // TODO: trees ignore y-pos; needs fix
    // TODO: make z-rotation random
}