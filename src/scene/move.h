#ifndef _MOVE_S_H
#define _MOVE_S_H

#include <glm/glm.hpp>

typedef struct _MOVE {
    double start; // time to start since last action finish
    double duration; // remaining time
    glm::vec3 target_pos;
    glm::vec3 target_rot;
} MOVE, *PMOVE;

#endif
