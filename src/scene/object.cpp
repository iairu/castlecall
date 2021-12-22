#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/transform.hpp>

#include "object.h"

void Object::generateModelMatrix() {
    modelMatrix =
        glm::translate(glm::mat4(1.0f), position)
        * glm::orientate4(rotation)
        * glm::scale(glm::mat4(1.0f), scale);
}


void Object::addKeyframe(float onTime, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale) {
    // Create a model matrix immediately for efficiency
    glm::mat4 keyframeModelMatrix{1};
    keyframeModelMatrix = glm::translate(glm::mat4(1.0f), position)
                        * glm::orientate4(rotation)
                        * glm::scale(glm::mat4(1.0f), scale);
    //
    // Save the correct end of animation timeline
    if (onTime > maxOnTime) maxOnTime = onTime;
    
    // Save the keyframe
    KEYFRAME keyframe = {onTime, keyframeModelMatrix};
    keyframes.push_back(keyframe);

    // todo sort keyframes if they're added out of order, because only the first found with higher time is considered
}

bool Object::generateFrameModelMatrix(float dt) {
    // Use object's transformation in the beginning
    if (onTime == 0.0f && keyframes.front().onTime > 0.0f) {
        generateModelMatrix();
        KEYFRAME starter = {0.0f, modelMatrix};
        keyframes.push_front(starter);
    }

    // Increase time for animation to continue
    bool animate = false;
    if (onTime + dt > maxOnTime) {
        if (loop) {
            // Restart the animation
            onTime += dt - maxOnTime;
            animate = true;
        }
    } else {
        // Continue the animation
        onTime += dt;
        animate = true;
    }

    if (animate) {
        // For all matching keyframes find the last and nearest next one to interpolate between
        KEYFRAME last;
        KEYFRAME next;
        for (KEYFRAME curr : keyframes) {
            next = curr;
            if (curr.onTime > onTime) break;
            last = curr;
        }
        if (last.onTime == next.onTime) {
            // There was only one keyframe or we ran out of keyframes 
            // Use the existing transformation data, alternatively an object could stop existing here
            return false;
        }
        // Interpolated time position of this frame between keyframes from time values
        float at = (onTime - last.onTime) / (next.onTime - last.onTime);

        // Do matrix interpolation between last->onTime, onTime, next->onTime
        // Save the result directly to this object's modelMatrix for use in rendering
        modelMatrix = (next.modelMatrix - last.modelMatrix) * at + last.modelMatrix;
    }
    return true;
}