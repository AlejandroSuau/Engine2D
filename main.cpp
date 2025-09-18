#include "Engine2D.hpp"

#include <SDL2/SDL.h>

#include "Engine2D.hpp"

// Pacman Game
#include "game/pacman/Game.hpp"

int main(int argc, char** argv) {
    Engine2D::Engine eng;
    eng.Run(std::make_unique<Game>());

    return 0;
}
