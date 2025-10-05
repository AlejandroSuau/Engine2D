#include <SDL2/SDL.h>
#include <SDL_image.h>

#include "Engine2D.hpp"
#include "game/IGame.hpp"
#include "renderer/Renderer.hpp"
#include "grid/Grid.hpp"
#include "texture/TextureManager.hpp"

#include "pathfinding/Pathfinder.hpp"
#include "pathfinding/frontier/AStarFrontier.hpp"
#include "pathfinding/frontier/DijkstraFrontier.hpp"
#include "pathfinding/frontier/BFSFrontier.hpp"
#include "pathfinding/neighbour_provider/StarProvider.hpp"
#include "pathfinding/neighbour_provider/CrossProvider.hpp"
#include "pathfinding/heuristic/Manhattan.hpp"
#include "pathfinding/heuristic/Euclidean.hpp"
#include "pathfinding/heuristic/Octile.hpp"
#include "pathfinding/heuristic/Chebyshev.hpp"

class Game : public Engine2D::IGame {
public:
    Game(Engine2D::Engine& engine);

    void Start() override;
    void Update(float dt) override;
    void Render(Engine2D::Renderer& renderer) override;
    void HandleEvent(const SDL_Event& event) override;
    std::string GetWindowTitle() override;
    int GetScreenWidth() override;
    int GetScreenHeight() override;

    const Engine2D::Grid& GetGrid() const;

private:
    Engine2D::Engine& engine_;
    Engine2D::Grid grid_;
    TextureManager texture_manager_;
    SDL_Texture* loadbar_texture_;

    Engine2D::Pathfinding::Pathfinder<
        Engine2D::Grid,
        Engine2D::Pathfinding::Frontier::AStarFrontier,
        Engine2D::Pathfinding::Heuristic::Euclidean,
        Engine2D::Pathfinding::NeighbourProvider::StarProvider> pathfinder_{
        grid_,
        grid_.ColRowToIndex({0, 10}),
        grid_.ColRowToIndex({10, 5})
    };
};