
#ifndef _MAP_H
#define _MAP_H

#include <iostream>
#include <map>
#include <list>
#include <memory>

#include <ppgso/ppgso.h>

#include "scene.h"
#include "object.h"
#include "scripting.h"

typedef enum _ITMTYPE { // must match map_input.txt numbers
    LIGHT, // 0
    SKYBOX, // 1
    GROUND_WILD, // 2
    SHROOM, // 3
    WATER, // 4
    MENU_LOGO, // 5
    MENU_TXT, // 6
    ROCK, // 7
    GROUND_CASTLE, // 8
    GATE, // 9
    WALL_GATE, // 10
    WALL_REST, // 11
    BRIDGE, // 12
    HALL, // 13
    HIGH_TOWER, // 14
    STAIRS, // 15
    CAMERA, // 16
    NATURE_GEN, // 17
    SCENE_SWITCH // 18
} ITMTYPE;

typedef struct _MAPITEM {
    ITMTYPE object;
    glm::vec3 pos;
    glm::vec3 rotation;
    glm::vec3 scale;
    glm::vec3 dim;
} MAPITEM, *PMAPITEM;

typedef struct _SCENE_DESC {
    unsigned int scene_id;
    std::vector<MAPITEM> map;
} SCENE_DESC, *PSCENE_DESC;

class Map {
    private:
        std::vector<SCENE_DESC> scenes;

        std::unique_ptr<Object> getObj(ITMTYPE tgt_obj);

        void placeObj(MAPITEM item, Scene *scene, unsigned int id);

    public:
        Map();

        void placeItems(unsigned int scene_id, Scene *scene);
};

#endif
