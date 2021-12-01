
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

typedef enum _ITMTYPE {TOWER, TERRAIN, HALL} ITMTYPE;

typedef struct _MAPITEM {
    ITMTYPE object;
    glm::vec3 pos;
    glm::vec3 rotation;
    glm::vec3 scale;
} MAPITEM, *PMAPITEM;

class Map {
    private:
        static MAPITEM map[];

        static std::unique_ptr<Object> getObj(ITMTYPE tgt_obj);

    public:
        static void placeItems(Scene *scene);
};

#endif
