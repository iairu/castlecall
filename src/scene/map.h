
#ifndef _MAP_H
#define _MAP_H

#include <iostream>
#include <map>
#include <list>
#include <memory>
#include <ppgso/ppgso.h>

#include <ppgso/ppgso.h>

#include "scene.h"
#include "object.h"

typedef enum _ITMTYPE {
    WALL,
    LIGHT,
    SKYBOX,
    GROUND_WILD,
    SHROOM,
    WATER,
    MENU_LOGO,
    MENU_TXT,
    ROCK,
    GROUND_CASTLE,
    CASTLE,
    GATEWAY,
    TOWER,
    BRIDGE,
    HALL,
    HIGH_TOWER,
    STAIRS
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

    public:
        Map();

        void placeItems(unsigned int scene_id, Scene *scene);
};

#endif
