#include <iostream>
#include <map>
#include <list>

#include <ppgso/ppgso.h>
#include "scene/sceneWindow.cpp"

const unsigned int SIZE = 512;

int main() {
    // Initialize CastleCall window
    SceneWindow window;

    // Main execution loop
    while (window.pollEvents()) {}

    return EXIT_SUCCESS;
}
