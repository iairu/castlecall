

#include "map.h"
#include "tower.h"

#define DEFAULT_SCALE {1, 1, 1}

MAPITEM Map::map[] = {{TOWER, {-1, 1, -2}, {0, 0, .35f}, DEFAULT_SCALE}};

std::unique_ptr<Object> Map::getObj(ITMTYPE tgt_obj) {
    switch (tgt_obj) {
        case TOWER:
            return std::make_unique<Tower>();
        default:
            break;
    }
    return NULL;
}

void Map::placeItems(Scene *scene) {
    unsigned int count  = sizeof (Map::map) / sizeof( MAPITEM );

    for ( unsigned int i = 0; i < count; i++) {
        auto obj = Map::getObj(Map::map[i].object);
        obj->scale = Map::map[i].scale;
        obj->position = Map::map[i].pos;
        obj->rotation = Map::map[i].rotation;
        scene->objects.push_back(move(obj));
    }
}

// TODO: move map to external file
