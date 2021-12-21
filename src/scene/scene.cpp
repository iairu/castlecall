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

    // // attenuation
    // if(keyboard[GLFW_KEY_1]) {
    //     attenuationA += 0.01f;
    // }
    // else if(keyboard[GLFW_KEY_2]) {
    //     attenuationA -= 0.01f;
    // }
    // else if(keyboard[GLFW_KEY_3]) {
    //     attenuationB += 0.01f;
    // }
    // else if(keyboard[GLFW_KEY_4]) {
    //     attenuationB -= 0.01f;
    // }
    // else if(keyboard[GLFW_KEY_5]) {
    //     std::cout << "att a:" << attenuationA << " b:" << attenuationB << std::endl;
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

/*std::vector<Object*> Scene::intersect(const glm::vec3 &position, const glm::vec3 &direction) {
  std::vector<Object*> intersected = {};
  for(auto& object : objects) {
    // Collision with sphere of size object->scale.x
    auto oc = position - object->position;
    auto radius = object->scale.x;
    auto a = glm::dot(direction, direction);
    auto b = glm::dot(oc, direction);
    auto c = glm::dot(oc, oc) - radius * radius;
    auto dis = b * b - a * c;

    if (dis > 0) {
      auto e = sqrt(dis);
      auto t = (-b - e) / a;

      if ( t > 0 ) {
        intersected.push_back(object.get());
        continue;
      }

      t = (-b + e) / a;

      if ( t > 0 ) {
        intersected.push_back(object.get());
        continue;
      }
    }
  }

  return intersected;
}*/
