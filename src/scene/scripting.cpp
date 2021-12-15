

#include "scripting.h"

std::unique_ptr<Camera> Scripting::createScriptedCamera(glm::vec3 pos, glm::vec3 rot) {

    std::vector<MOVE> script = {{0.0, 2000.0, glm::vec3(0, 0, 0), glm::vec3(0, 0, 0)}};



    auto camera = std::make_unique<Camera>(60.0f, 1.0f, 0.1f, 100.0f);
    camera->position = pos;
    camera->back = rot;
    camera->setScript(&script);

    return camera;
}






