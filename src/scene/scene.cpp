#include "scene.h"

void Scene::update(float time) {
    camera->update(*this, time);

    // Use iterator to update all objects so we can remove while iterating
    auto i = std::begin(objects);

    while (i != std::end(objects)) {
        // Update and remove from list if needed
        auto obj = i->get();
        if (!obj->update(*this, time))
            i = objects.erase(i); // NOTE: no need to call destructors as we store shared pointers in the scene
        else
            ++i;
    }


    // Use iterator to update all collision boxes so we can remove while iterating
    auto j = std::begin(collisionboxes);

    while (j != std::end(collisionboxes)) {
        // Update and remove from list if needed
        auto box = j->get();
        if (!box->update(*this, time))
            j = collisionboxes.erase(j); // NOTE: no need to call destructors as we store shared pointers in the scene
        else
            ++j;
    }

    // Animate tintColor1 between _from and _to, pingpong style
    if (!tintColor1_pong && tintColor1_onTime + time > tintColor1_maxTime) { // overflow
        tintColor1_pong = true;
    } else if (tintColor1_pong && tintColor1_onTime - time < 0.0f) { // underflow
        tintColor1_pong = false;
    } else if (!tintColor1_pong) {
        tintColor1_onTime += time; // ping
    } else {
        tintColor1_onTime -= time; // pong
    }
    float at = tintColor1_onTime / tintColor1_maxTime; // normalized onTime to <0.0f;1.0f> for interpolation
    tintColor1 = (tintColor1_to - tintColor1_from) * at + tintColor1_from; // interpolated tintcolor at given normalized time


    // // move light source
    // if(keyboard[GLFW_KEY_KP_4]) {
    //     lightPos1.x += 0.01f; // lavo X
    // }
    // else if(keyboard[GLFW_KEY_KP_6]) {
    //     lightPos1.x -= 0.01f; // pravo X
    // }
    // else if(keyboard[GLFW_KEY_KP_8]) {
    //     lightPos1.z += 0.01f; // lavo Z
    // }
    // else if(keyboard[GLFW_KEY_KP_2]) {
    //     lightPos1.z -= 0.01f; // pravo Z
    // }
    // else if(keyboard[GLFW_KEY_KP_7]) {
    //     lightPos1.y += 0.01f; // hore Y
    // }
    // else if(keyboard[GLFW_KEY_KP_9]) {
    //     lightPos1.y -= 0.01f; // dole Y
    // }
    // else if(keyboard[GLFW_KEY_KP_5]) {
    //     std::cout << "lightPos1 x:" << lightPos1.x << "y:" << lightPos1.y << "z:" << lightPos1.z << std::endl;
    // }

    // // tint light source
    // if(keyboard[GLFW_KEY_1]) {
    //     tintColor1.r += 0.01f;
    // }
    // else if(keyboard[GLFW_KEY_2]) {
    //     tintColor1.r -= 0.01f;
    // }
    // else if(keyboard[GLFW_KEY_3]) {
    //     tintColor1.g += 0.01f;
    // }
    // else if(keyboard[GLFW_KEY_4]) {
    //     tintColor1.g -= 0.01f;
    // }
    // else if(keyboard[GLFW_KEY_5]) {
    //     tintColor1.b += 0.01f;
    // }
    // else if(keyboard[GLFW_KEY_6]) {
    //     tintColor1.b -= 0.01f;
    // }
    // else if(keyboard[GLFW_KEY_7]) {
    //     std::cout << "tintColor1 r:" << tintColor1.r << " g:" << tintColor1.g <<  " b:" << tintColor1.b << std::endl;
    // }

    // // adjust light dampening
    // if(keyboard[GLFW_KEY_1]) {
    //     diffusePower += 0.03f;
    // }
    // else if(keyboard[GLFW_KEY_2]) {
    //     diffusePower -= 0.03f;
    // }
    // else if(keyboard[GLFW_KEY_3]) {
    //     specularPower += 0.03f;
    // }
    // else if(keyboard[GLFW_KEY_4]) {
    //     specularPower -= 0.03f;
    // }
    // else if(keyboard[GLFW_KEY_5]) {
    //     std::cout << "power d:" << diffusePower << " s:" << specularPower << std::endl;
    // }

    // // specular
    // if(keyboard[GLFW_KEY_6]) {
    //     specLight += 0.01f;
    // }
    // else if(keyboard[GLFW_KEY_7]) {
    //     specLight -= 0.01f;
    // }
    // else if(keyboard[GLFW_KEY_8]) {
    //     specAmountPow += 1;
    // }
    // else if(keyboard[GLFW_KEY_9]) {
    //     specAmountPow -= 1;
    // }
    // else if(keyboard[GLFW_KEY_0]) {
    //     std::cout << "sl:" << specLight << " sap:" << specAmountPow << std::endl;
    // }
}

void Scene::render() {
    // Simply render all objects
    for ( auto& obj : objects )
        obj->render(*this);

    // Don't render collision boxes
    if (!renderCollisionBoxes)
        return;

    // Simply render all collision boxes
    for ( auto& box : collisionboxes )
        box->render(*this);
}