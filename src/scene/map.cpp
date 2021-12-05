
#include <algorithm>
#include <sstream>
#include <iostream>
#include <fstream>

#include "map.h"
#include "obj/tower.h"
#include "obj/shroom.h"
#include "obj/ground_wild.h"
#include "obj/rock.h"
#include "obj/water.h"
#include "obj/bridge.h"
#include "obj/wall.h"

std::unique_ptr<Object> Map::getObj(ITMTYPE tgt_obj) {
    switch (tgt_obj) {
        case WALL:
            return std::make_unique<Wall>();
            break;
        case GROUND_WILD:
            return std::make_unique<GroundWild>();
            break;
        case SHROOM:
            return std::make_unique<Shroom>();
            break;
        case ROCK:
            return std::make_unique<Rock>();
            break;
        case WATER:
            return std::make_unique<Water>();
            break;
        case BRIDGE:
            return std::make_unique<Bridge>();
            break;
        case TOWER:
            return std::make_unique<Tower>();
            break;
        default:
            break;
    }
    return NULL;
}

Map::Map() {
    std::fstream f;
	std::string line;
    unsigned int scene = 0, tmp;
    PSCENE_DESC tgt_scene = NULL;
    MAPITEM mapitem;

	
	f.open("../data/map.txt", std::fstream::in);
	
	if ( f.is_open() ) {
		
		while ( std::getline(f, line) ) {
			line.erase(std::remove(line.begin(), line.end(), '\r'), line.end());
			line.erase(std::remove(line.begin(), line.end(), '\n'), line.end());
			if (line.empty() || ! isdigit(line.front())) {
				if(line.front() == '@') {
					tmp = std::stoul(line.substr(1), NULL, 10);
                    if(tmp > scene) {
                        scene = tmp;
                        this->scenes.push_back({scene, {}});
                        tgt_scene = &this->scenes.back();
                    }
				}
				continue;
			}
			
            if(tgt_scene != NULL) {
                std::istringstream stm(line);

                if(stm >> tmp >> mapitem.pos.x >> mapitem.pos.y >> mapitem.pos.z >> mapitem.rotation.x >> mapitem.rotation.y >> mapitem.rotation.z >> mapitem.scale.x >> mapitem.scale.y >> mapitem.scale.z >> mapitem.dim.x >> mapitem.dim.y >> mapitem.dim.z) {
                    mapitem.object = (ITMTYPE)tmp;
                    tgt_scene->map.push_back(mapitem);
                }
                else std::cout << "error processing line" << std::endl;


            }
            
			
		}
		
		
		f.close();
	}
	else std::cout << "Failed to open map" << std::endl;
}

void Map::placeItems(unsigned int scene_id, Scene *scene) {
    if(this->scenes.empty() || this->scenes.size() < scene_id) {
        std::cout << "invalid scene id: " << scene_id << std::endl;
        return;
    }

    std::vector<MAPITEM> * map = &this->scenes.at(scene_id - 1).map;

    for ( auto mapitem : *map) {
        auto obj = Map::getObj(mapitem.object);
        if (obj == NULL) continue;
        obj->scale = mapitem.scale;
        obj->position = mapitem.pos;
        obj->rotation = mapitem.rotation;
        scene->objects.push_back(move(obj));
    }
}
