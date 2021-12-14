#include <glm/glm.hpp>

#include "camera.h"
#include "scene.h"


Camera::Camera(float fow, float ratio, float near, float far) {
  float fowInRad = (ppgso::PI/180.0f) * fow;

  projectionMatrix = glm::perspective(fowInRad, ratio, near, far);
}

void Camera::update(Scene &scene, float dt) {
  viewMatrix = lookAt(position, position-back, up);

    // Keyboard controls
    if(scene.keyboard[GLFW_KEY_LEFT]) {
        position.x += 0.01; // lavo X
    } else if(scene.keyboard[GLFW_KEY_RIGHT]) {
        position.x -= 0.01; // pravo -X
    } else if(scene.keyboard[GLFW_KEY_UP]) {
        position.z += 0.01; // rovno Z
    } else if(scene.keyboard[GLFW_KEY_DOWN]) {
        position.z -= 0.01; // dozadu -Z
    } else if(scene.keyboard[GLFW_KEY_KP_1]) {
        position.y += 0.01; // hore Y
    } else if(scene.keyboard[GLFW_KEY_KP_2]) {
        position.y -= 0.01; // dole -Y

    } else if(scene.keyboard[GLFW_KEY_Q]) {
        up.x += 0.01; // hojdacka (naklon lietadla)
    } else if(scene.keyboard[GLFW_KEY_W]) {
        up.x -= 0.01; // hojdacka (naklon lietadla)
    // } else if(scene.keyboard[GLFW_KEY_E]) {
    //     up.z += 0.01;
    // } else if(scene.keyboard[GLFW_KEY_R]) {
    //     up.z -= 0.01;
    // } else if(scene.keyboard[GLFW_KEY_T]) {
    //     up.y += 0.01;
    // } else if(scene.keyboard[GLFW_KEY_Z]) {
    //     up.y -= 0.01;

    // velmi blba rotacia
    } else if(scene.keyboard[GLFW_KEY_A]) {
        back.x += 0.01;
    } else if(scene.keyboard[GLFW_KEY_S]) {
        back.x -= 0.01;
    } else if(scene.keyboard[GLFW_KEY_D]) {
        back.z += 0.01;
    } else if(scene.keyboard[GLFW_KEY_F]) {
        back.z -= 0.01;
    } else if(scene.keyboard[GLFW_KEY_G]) {
        back.y += 0.01;
    } else if(scene.keyboard[GLFW_KEY_H]) {
        back.y -= 0.01;
    } 
    
    // reset
    else if(scene.keyboard[GLFW_KEY_P]) {
        up = {0,1,0};
        position = {0,0,0};
        back = {0,0,-1};
    }
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
