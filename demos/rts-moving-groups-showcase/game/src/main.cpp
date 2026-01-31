#include "Engine2D.hpp"

#include "Game.hpp"

int main(int argc, char** argv) {
    Engine2D::Engine eng;
    eng.Run(std::make_unique<Game>(eng));

    return 0;
}
