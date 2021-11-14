#include <iostream>
#include <map>
#include <list>

#include "scene\window.cpp"

const unsigned int SIZE = 512;

int main() {
    // Initialize CastleCall window
    Window window;

    // Main execution loop
    while (window.pollEvents()) {}

    return EXIT_SUCCESS;
}
