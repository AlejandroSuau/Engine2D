#include "FireOpenGL.hpp"

#include <stdexcept>
#include <memory>
#include <string>

int main(int argc, char** argv) {
    FireOpenGL fire_opengl(SCREEN_WIDTH, SCREEN_HEIGHT);
    fire_opengl.Run();

    return 0;
}