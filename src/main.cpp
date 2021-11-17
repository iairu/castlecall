#include <iostream>
#include <map>
#include <list>

#include <ppgso/ppgso.h>

const unsigned int SIZE = 512;

int main() {
    // Initialize CastleCall window
    ppgso::Window window("Castle Call", 1280, 720); // Window in HD

    // Main execution loop
    while (window.pollEvents()) {}

    return EXIT_SUCCESS;
}
