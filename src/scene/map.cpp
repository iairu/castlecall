
#include <algorithm>
#include <sstream>
#include <iostream>
#include <fstream>

#include "map.h"
#include "obj/shroom.h"
#include "obj/ground_wild.h"
#include "obj/ground_castle.h"
#include "obj/rock.h"
#include "obj/water.h"
#include "obj/bridge.h"
#include "obj/menu_logo.h"
#include "obj/menu_txt.h"
#include "obj/skybox.h"
#include "obj/hall.h"
#include "obj/high_tower.h"
#include "obj/stairs.h"
#include "obj/gate.h"
#include "obj/wall_gate.h"
#include "obj/wall_rest.h"

#include "obj/hierarchical_tree.h"

std::unique_ptr<Object> Map::getObj(ITMTYPE tgt_obj) {
    switch (tgt_obj) {
        case SKYBOX:
            return std::make_unique<Skybox>();
            break;
        case MENU_LOGO:
            return std::make_unique<MenuLogo>();
            break;
        case MENU_TXT:
            return std::make_unique<MenuTXT>();
            break;
        case GROUND_WILD:
            return std::make_unique<GroundWild>();
            break;
        case GROUND_CASTLE:
            return std::make_unique<GroundCastle>();
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
        case WALL_REST:
            return std::make_unique<WallRest>();
            break;
        case HALL:
            return std::make_unique<Hall>();
            break;
        case HIGH_TOWER:
            return std::make_unique<HighTower>();
            break;
        case STAIRS:
            return std::make_unique<Stairs>();
            break;
        case GATE:
            return std::make_unique<Gate>();
            break;
        case WALL_GATE:
            return std::make_unique<WallGate>();
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

	
	f.open("res/map.txt", std::fstream::in);
	
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

void Map::placeObj(MAPITEM item, Scene *scene) {
    std::unique_ptr<Camera> camera;
    switch (item.object) {
        case CAMERA:
            if(scene->camera == NULL) {
                camera = Scripting::createScriptedCamera(item.pos, item.rotation);
                scene->camera = move(camera);
            }
            else {
                scene->camera->position = item.pos;
                scene->camera->back = item.rotation;
            }
        default:
            break;
    }
} // TODO: add support for nature generator & characters

void Map::placeItems(unsigned int scene_id, Scene *scene) {
    if(this->scenes.empty() || this->scenes.size() < scene_id) {
        std::cout << "invalid scene id: " << scene_id << std::endl;
        return;
    }

    std::vector<MAPITEM> * map = &this->scenes.at(scene_id - 1).map;

    for ( auto mapitem : *map) {
        auto obj = Map::getObj(mapitem.object);
        if (obj == NULL) {
            Map::placeObj(mapitem, scene);
            continue;
        }
        obj->scale = mapitem.scale;
        obj->position = mapitem.pos;
        obj->rotation = mapitem.rotation;
        scene->objects.push_back(move(obj));
    }

    auto htree = std::make_unique<HierarchicalTree>();
    htree->position = {0, -0.75, 0}; // second one moves up-down
    htree->scale = {0.25, 0.25, 0.25}; // scale to 1/4 size
    htree->rotation = {0, 0, 0}; // last one rotates around up-down axis ( in radians )
    scene->objects.push_back(move(htree));

    auto htree2 = std::make_unique<HierarchicalTree>();
    htree2->position = {2, -0.75, 0}; // second one moves up-down
    htree2->scale = {0.25, 0.25, 0.25}; // scale to 1/4 size
    htree2->rotation = {0, 0, 3.14}; // last one rotates around up-down axis ( in radians -> 3.14 = 180* rotation )
    scene->objects.push_back(move(htree2));
}
