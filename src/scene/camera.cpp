#include <glm/glm.hpp>

#include "camera.h"
#include "scene.h"


Camera::Camera(float fow, float ratio, float near, float far) {
  float fowInRad = (ppgso::PI/180.0f) * fow;

  this->locked = false;

  projectionMatrix = glm::perspective(fowInRad, ratio, near, far);
}

void Camera::update(Scene &scene, float dt) {
    if(this->locked && ! this->pause_script) {
        this->runScript(dt);
    }
    else {
        float speed = 0.01f;
        // Keyboard controls
        if(scene.keyboard[GLFW_KEY_LEFT]) {
            position.x += speed; // lavo X
            std::cout << "pos x:" << position.x << "y:" << position.y << "z:" << position.z << std::endl;
        } else if(scene.keyboard[GLFW_KEY_RIGHT]) {
            position.x -= speed; // pravo -X
            std::cout << "pos x:" << position.x << "y:" << position.y << "z:" << position.z << std::endl;
        } else if(scene.keyboard[GLFW_KEY_UP]) {
            position.z += speed; // rovno Z
            std::cout << "pos x:" << position.x << "y:" << position.y << "z:" << position.z << std::endl;
        } else if(scene.keyboard[GLFW_KEY_DOWN]) {
            position.z -= speed; // dozadu -Z
            std::cout << "pos x:" << position.x << "y:" << position.y << "z:" << position.z << std::endl;
        } else if(scene.keyboard[GLFW_KEY_KP_1]) {
            position.y += speed; // hore Y
            std::cout << "pos x:" << position.x << "y:" << position.y << "z:" << position.z << std::endl;
        } else if(scene.keyboard[GLFW_KEY_KP_3]) {
            position.y -= speed; // dole -Y
            std::cout << "pos x:" << position.x << "y:" << position.y << "z:" << position.z << std::endl;


        } else if(scene.keyboard[GLFW_KEY_Q]) {
            up.x += speed; // hojdacka (naklon lietadla)
            std::cout << "up x:" << up.x << "y:" << up.y << "z:" << up.z << std::endl;
        } else if(scene.keyboard[GLFW_KEY_W]) {
            up.x -= speed; // hojdacka (naklon lietadla)
            std::cout << "up x:" << up.x << "y:" << up.y << "z:" << up.z << std::endl;
        } else if(scene.keyboard[GLFW_KEY_E]) {
            up.z += speed;
            std::cout << "up x:" << up.x << "y:" << up.y << "z:" << up.z << std::endl;
        } else if(scene.keyboard[GLFW_KEY_R]) {
            up.z -= speed;
            std::cout << "up x:" << up.x << "y:" << up.y << "z:" << up.z << std::endl;
        } else if(scene.keyboard[GLFW_KEY_T]) {
            up.y += speed;
            std::cout << "up x:" << up.x << "y:" << up.y << "z:" << up.z << std::endl;
        } else if(scene.keyboard[GLFW_KEY_Z]) {
             up.y -= speed;
            std::cout << "up x:" << up.x << "y:" << up.y << "z:" << up.z << std::endl;

        } else if(scene.keyboard[GLFW_KEY_A]) {
            back.x += speed;
            std::cout << "back x:" << back.x << "y:" << back.y << "z:" << back.z << std::endl;
        } else if(scene.keyboard[GLFW_KEY_S]) {
            back.x -= speed;
            std::cout << "back x:" << back.x << "y:" << back.y << "z:" << back.z << std::endl;
        } else if(scene.keyboard[GLFW_KEY_D]) {
            back.z += speed;
            std::cout << "back x:" << back.x << "y:" << back.y << "z:" << back.z << std::endl;
        } else if(scene.keyboard[GLFW_KEY_F]) {
            back.z -= speed;
            std::cout << "back x:" << back.x << "y:" << back.y << "z:" << back.z << std::endl;
        } else if(scene.keyboard[GLFW_KEY_G]) {
            back.y += speed;
            std::cout << "back x:" << back.x << "y:" << back.y << "z:" << back.z << std::endl;
        } else if(scene.keyboard[GLFW_KEY_H]) {
            back.y -= speed;
            std::cout << "back x:" << back.x << "y:" << back.y << "z:" << back.z << std::endl;
        } 

        // TODO: movement control based on rotation
        
        // reset
        else if(scene.keyboard[GLFW_KEY_P]) {
            up = {0,1,0};
            position = {0,0,0};
            back = {0,0,-1};
        }
        else if(scene.keyboard[GLFW_KEY_L]) {
            std::cout << "pos x:" << position.x << "y:" << position.y << "z:" << position.z << std::endl;
            std::cout << "up x:" << up.x << "y:" << up.y << "z:" << up.z << std::endl;
            std::cout << "back x:" << back.x << "y:" << back.y << "z:" << back.z << std::endl;
        }
    }
    viewMatrix = lookAt(position, position-back, up);
}

// void Camera::setScript() {
//     for(std::vector<MOVE> keyframe : this->keyframes) {
//         for(MOVE move : keyframe) {
//             this->camera_script.push_back(move);
//         }
//     }
//     this->locked = true;
// }

// void Camera::addKeyframe(std::vector<MOVE> *keyframe) {
//     this->keyframes.push_back(*keyframe);
// }

void Camera::addKeyframe(std::vector<MOVE> *keyframe) {
    for(MOVE move : *keyframe) {
        this->camera_script.push_back(move);
    }
    this->locked = true;
}

void Camera::runScript(float dt) {
    if(this->camera_script.empty()) return;
    float remainder = 0;
    float * src = &dt;

    PMOVE current = &this->camera_script.at(0);
    do {
        float percentage = 0; // 0 - 1
        if(current->start > 0) {
            current->start -= *src;
            if(current->start < 0) {
                remainder = -current->start;
                current->start = 0;
            }
            src = &remainder;
        }
        else {
            remainder = dt;
            *src = remainder;
        }

        if(current->start == 0) {
            if(current->duration > 0) {
                percentage = dt / current->duration;
                current->duration -= *src;
                if(current->duration < 0) {
                    remainder = -current->duration;
                    percentage = 1.0; // finish transition
                }
                else {
                    remainder = 0;
                }

                glm::vec3 rotationDiff = current->target_rot - back;
                glm::vec3 positionDiff = current->target_pos - position;

                back += (rotationDiff * percentage);
                position += (positionDiff * percentage);

                // TODO: script bezi velmi rychlo a lock vyzera, ze nejde
            }
            else {
                this->camera_script.erase(this->camera_script.begin());
                if(this->camera_script.empty()) {
                    this->locked = false;
                    return;
                }
                current = &this->camera_script.at(0);
            }
        }
    } while (remainder > 0);
}

glm::vec3 Camera::cast(double u, double v) {
  // Create point in Screen coordinates
  glm::vec4 screenPosition{u,v,0.0f,1.0f};

  // Use inverse matrices to get the point in world coordinates
  auto invProjection = glm::inverse(projectionMatrix);
  auto invView = glm::inverse(viewMatrix);

  // Compute position on the camera plane
  auto planePosition = invView * invProjection * screenPosition;
  planePosition /= planePosition.w;

  // Create direction vector
  auto direction = glm::normalize(planePosition - glm::vec4{position,1.0f});
  return glm::vec3{direction};
}
