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
}

void Scene::render() {
  // Simply render all objects
  for ( auto& obj : objects )
    obj->render(*this);
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

glm::vec3 Scene::calculateLightDirection(glm::vec3 object_pos) {
    if( ! this->hasLightSource ) return this->lightDirection;
    glm::vec3 result = object_pos - this->light_source;
    float div = (abs(result.x) > abs(result.y)) ? ((abs(result.x) > abs(result.z)) ? abs(result.x) : abs(result.z)) : ((abs(result.y) > abs(result.z)) ? abs(result.y) : abs(result.z));
    if ( div < 1.0f ) div = 1.0f;
    result *= -1;
    return glm::vec3(result.x / div, result.y / div, result.z / div); // return normalized light dir
}

