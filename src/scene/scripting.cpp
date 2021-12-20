

#include "scripting.h"

std::unique_ptr<Camera> Scripting::createScriptedCamera(glm::vec3 pos, glm::vec3 rot) {

    // todo temp disabled for quicker debugging
    std::vector<MOVE> keyframetmp = {{0.0, 50.0, glm::vec3(8.48,3.95,3.45), glm::vec3(8.54,2.33,3.11)}};
    // std::vector<MOVE> keyframe0 = {{0.0, 50.0, glm::vec3(28.9, 5.42, 6.89), glm::vec3(2.28, 0.08, -1.12)}};
    // std::vector<MOVE> keyframe1 = {{0.0, 7000.0, glm::vec3(29.76, 5.42, -0.18), glm::vec3(8.54,0.09,-1.12)}};
    // std::vector<MOVE> keyframe2 = {{0.0, 7000.0, glm::vec3(17.62,12.75,0.54), glm::vec3(8.54,6.34,-1.12)}};
    // std::vector<MOVE> keyframe3 = {{0.0, 7000.0, glm::vec3(8.48,3.95,3.45), glm::vec3(8.54,3.16,-1.69)}};
    // std::vector<MOVE> keyframe4 = {{0.0, 7000.0, glm::vec3(-10.08,3.95,-2.78), glm::vec3(-11.44,3.16,-1.69)}};
    // std::vector<MOVE> keyframe5 = {{0.0, 21000.0, glm::vec3(-13.26,20.09,6.43), glm::vec3(-9.88,13.87,22.26)}};
    // std::vector<MOVE> keyframe6 = {{0.0, 7000.0, glm::vec3(-8.99,24.69,-6.15), glm::vec3(4.22,21.46,0.49)}};
    // std::vector<MOVE> keyframe7 = {{0.0, 30000.0, glm::vec3(-8.99,1.50,-6.15), glm::vec3(-0.12,-1.02,1.36)}};
    // std::vector<MOVE> keyframe8 = {{0.0, 7000.0, glm::vec3(-8.99,1.50,-6.15), glm::vec3(-20.59,-1.02,1.36)}};
    // std::vector<MOVE> keyframe9 = {{0.0, 14000.0, glm::vec3(0.49,1.50,-6.32), glm::vec3(-20.59,-1.02,1.36)}};
    // std::vector<MOVE> keyframe10 = {{0.0, 14000.0, glm::vec3(3.31,0.59,-2.34), glm::vec3(1.66,-1.02,-22.81)}};
    // std::vector<MOVE> keyframe11 = {{0.0, 7000.0, glm::vec3(3.31,0.59,-2.34), glm::vec3(-26.53,-0.06,-23.27)}};
    // std::vector<MOVE> keyframe12 = {{0.0, 7000.0, glm::vec3(7.08,0.8,-0.14), glm::vec3(-91.94,17.75,-24.11)}};
    // std::vector<MOVE> keyframe13 = {{0.0, 7000.0, glm::vec3(7.76,0.67,5.26), glm::vec3(-91.94,58.21,-22.98)}};

    auto camera = std::make_unique<Camera>(60.0f, 1.0f, 0.1f, 100.0f);
    camera->position = pos;
    camera->back = rot;
    camera->addKeyframe(&keyframetmp);
    // camera->addKeyframe(&keyframe0);
    // camera->addKeyframe(&keyframe1);
    // camera->addKeyframe(&keyframe2);
    // camera->addKeyframe(&keyframe3);
    // camera->addKeyframe(&keyframe4);
    // camera->addKeyframe(&keyframe5);
    // camera->addKeyframe(&keyframe6);
    // camera->addKeyframe(&keyframe7);
    // camera->addKeyframe(&keyframe8);
    // camera->addKeyframe(&keyframe9);
    // camera->addKeyframe(&keyframe10);
    // camera->addKeyframe(&keyframe11);
    // camera->addKeyframe(&keyframe12);
    // camera->addKeyframe(&keyframe13);

    return camera;
}






