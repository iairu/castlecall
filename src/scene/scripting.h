#ifndef _SCRIPTING_H
#define _SCRIPTING_H

#include <glm/glm.hpp>
#include <ppgso/ppgso.h>

#include "camera.h"
#include "move.h"

class Scripting {
    public:
        static std::unique_ptr<Camera> createScriptedCamera(glm::vec3 pos, glm::vec3 rot);
};


#endif
