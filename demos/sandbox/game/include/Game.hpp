#include <SDL2/SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "Engine2D.hpp"
#include "game/IGame.hpp"
#include "renderer/Renderer.hpp"
#include "grid/GridMover.hpp"
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

#include <string>
#include <array>

enum class EMouseUserAction {
    MOVING_START_NODE,
    MOVING_GOAL_NODE,
    DRAWING_BLOCKS,
    UNDRAWING_BLOCKS,
    NONE
};

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
    Engine2D::Grid grid_{ {50.f, 80.f}, 14, 14, 50 };
    Engine2D::GridMover grid_mover_{ {40.f, 2.f} };
    Engine2D::AssetLocator asset_locator_;
    Engine2D::TextureManager texture_manager_;
    Engine2D::FontManager font_manager_;
    
    std::size_t cursor_colrow_index_{0};
    Engine2D::Coords_t cursor_coords_;

    SDL_Texture* texture_hand_default_ {nullptr};
    SDL_Texture* texture_hand_click_ {nullptr};
    SDL_Texture* texture_hand_drag_ {nullptr};
    TTF_Font* font_text_ {nullptr};
    TTF_Font* font_title_ {nullptr};
    TTF_Font* font_number_big_ {nullptr};
    TTF_Font* font_number_small_ {nullptr};

    EMouseUserAction user_action_{EMouseUserAction::NONE};
    bool show_pathfinder_costs_ {false};

    Engine2D::Timer pathfinder_timer_{0.1f};
    Engine2D::Pathfinding::Pathfinder<
        Engine2D::Grid,
        Engine2D::Pathfinding::Frontier::AStarFrontier,
        Engine2D::Pathfinding::Heuristic::Octile,
        Engine2D::Pathfinding::NeighbourProvider::StarProvider> pathfinder_{
        grid_,
        grid_.ColRowToIndex({3, 7}),
        grid_.ColRowToIndex({10, 7})
    };

    struct KeyTextureDisplay {
        std::string message;
        SDL_Texture* texture;
        SDL_FRect rect_dest;
    };
    std::array<KeyTextureDisplay, 5> key_texture_display_ {
        KeyTextureDisplay{" Find Path", nullptr, {330.f, 38.f, 40.f, 40.f}}, //C
        KeyTextureDisplay{" Single Step", nullptr, {90.f, 38.f, 40.f, 40.f}}, // S
        KeyTextureDisplay{" Reset Path", nullptr, {90.f, 3.f, 40.f, 40.f}}, // R
        KeyTextureDisplay{" Clean Blocks", nullptr, {565.f, 3.f, 40.f, 40.f}}, // W
        KeyTextureDisplay{" Display Costs", nullptr, {565.f, 38.f, 40.f, 40.f}}, // D
    };

    void OnMouseDown();
    void OnMouseMotion();
    void OnKeyRelease(SDL_Scancode key_scancode);
};