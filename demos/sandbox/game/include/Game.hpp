#include <SDL2/SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "Engine2D.hpp"
#include "game/IGame.hpp"
#include "renderer/Renderer.hpp"
#include "grid/Grid.hpp"
#include "texture/TextureManager.hpp"
#include "font/FontManager.hpp"
#include "asset_locator/AssetLocator.hpp"

#include "timer/Timer.hpp"

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
#include "pathfinding/heuristic/Zero.hpp"

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
    Engine2D::AssetLocator asset_locator_;
    Engine2D::TextureManager texture_manager_;
    Engine2D::FontManager font_manager_;

    SDL_Texture* loadbar_texture_;
    TTF_Font* font_;

    Engine2D::Timer pathfinder_timer_{0.2f};
    //////////////// BFS ////////////////
    /*Engine2D::Pathfinding::Pathfinder<
        Engine2D::Grid,
        Engine2D::Pathfinding::Frontier::BFSFrontier,
        Engine2D::Pathfinding::Heuristic::ZeroHeuristic,
        Engine2D::Pathfinding::NeighbourProvider::CrossProvider> pathfinder_{
        grid_,
        grid_.ColRowToIndex({3, 10}),
        grid_.ColRowToIndex({15, 10})
    };*/

    //////////////// Dijkstra ////////////////
    /*Engine2D::Pathfinding::Pathfinder<
        Engine2D::Grid,
        Engine2D::Pathfinding::Frontier::DijkstraFrontier,
        Engine2D::Pathfinding::Heuristic::ZeroHeuristic,
        Engine2D::Pathfinding::NeighbourProvider::CrossProvider> pathfinder_{
        grid_,
        grid_.ColRowToIndex({3, 10}),
        grid_.ColRowToIndex({15, 10})
    };*/

    //////////////// A* Example - Star - Cross ////////////////
    /*Engine2D::Pathfinding::Pathfinder<
        Engine2D::Grid,
        Engine2D::Pathfinding::Frontier::AStarFrontier,
        Engine2D::Pathfinding::Heuristic::Euclidean,
        Engine2D::Pathfinding::NeighbourProvider::StarProvider> pathfinder_{
        grid_,
        grid_.ColRowToIndex({3, 10}),
        grid_.ColRowToIndex({15, 10})
    };*/
    Engine2D::Pathfinding::Pathfinder<
        Engine2D::Grid,
        Engine2D::Pathfinding::Frontier::AStarFrontier,
        Engine2D::Pathfinding::Heuristic::Euclidean,
        Engine2D::Pathfinding::NeighbourProvider::CrossProvider> pathfinder_{
        grid_,
        grid_.ColRowToIndex({3, 10}),
        grid_.ColRowToIndex({15, 10})
    };
};