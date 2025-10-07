#include "Game.hpp"

Game::Game(Engine2D::Engine& engine) 
    : engine_(engine)
    , grid_({0.f, 0.f}, 20, 20, 40)
    , asset_locator_()
    , texture_manager_(*engine_.SDLRenderer(), asset_locator_)
    , font_manager_(asset_locator_)
    , loadbar_texture_(nullptr) {}

void Game::Start() {
    loadbar_texture_ = texture_manager_.LoadTexture("images/loadbar.png", "loadbar-1");
    font_ = font_manager_.LoadFont("fonts/atari-full.ttf", 10, "attari-full");
}

void Game::Update(float dt) {
    if (pathfinder_timer_.Update(dt)) {
        pathfinder_.Step();
    }
}

void Game::Render(Engine2D::Renderer& renderer) {    
    const auto& cells = grid_.Cells();
    const auto cell_dimensions = static_cast<float>(grid_.GetCellDimensions());

    renderer.SetRenderingColor({240, 243, 249, 255});
    renderer.RenderRectFilled({
        0.f, 0.f, static_cast<float>(GetScreenWidth()), static_cast<float>(GetScreenHeight())});

    // Render Texture
    renderer.RenderTexture(loadbar_texture_, {0, 0, 1132, 71}, {0.f, 0.f, 100.f, 20.f});

    // From
    renderer.SetRenderingColor({46, 204, 113, 255});
    const auto from = grid_.ColRowToCoords(grid_.IndexToColRow(pathfinder_.GetStartIndex()));
    renderer.RenderRectFilled(
        {static_cast<float>(from.x), static_cast<float>(from.y), cell_dimensions, cell_dimensions}
    );

    // To
    renderer.SetRenderingColor({231, 76, 60, 255});
    const auto to = grid_.ColRowToCoords(grid_.IndexToColRow(pathfinder_.GetGoalIndex()));
    renderer.RenderRectFilled(
        {static_cast<float>(to.x), static_cast<float>(to.y), cell_dimensions, cell_dimensions}
    );

    // Obstacles
    renderer.SetRenderingColor({44, 62, 80, 255});
    for (const auto c : grid_.Cells()) {
        if (c.is_walkable_) continue;

        renderer.RenderRectFilled(
            {c.top_left_.x, c.top_left_.y, cell_dimensions, cell_dimensions}
        );
    }

    // Render Path Visited
    renderer.SetRenderingColor({52, 152, 219, 90});
    const auto& path_closed = pathfinder_.Closed();
    for (const auto& c : cells) {
        if (!path_closed[c.index_]) continue;

        renderer.RenderRectFilled(
            {c.top_left_.x, c.top_left_.y, cell_dimensions, cell_dimensions}
        );
    }

    // Path
    renderer.SetRenderingColor({241, 196, 15, 215});
    const auto& path = pathfinder_.GetPath();
    for (const auto cell_index : path) {
        const auto c = cells[cell_index];
        renderer.RenderRectFilled(
            {c.top_left_.x, c.top_left_.y, cell_dimensions, cell_dimensions}
        );
    }

    // Render Grid
    const auto costs = pathfinder_.GCosts();
    renderer.SetRenderingColor({200, 210, 220, 255});
    for (const auto& c : cells) {
        renderer.RenderRect(
            {c.top_left_.x, c.top_left_.y, cell_dimensions, cell_dimensions}
        );
        const auto cost = costs[c.index_];
        if (cost != Engine2D::Pathfinding::kINF) {
            renderer.RenderText(*font_, std::to_string(cost), {100, 200, 100, 255}, static_cast<Vec2<int>>(c.center_));
        }
    }
}

const Engine2D::Grid& Game::GetGrid() const {
    return grid_;
}

void Game::HandleEvent(const SDL_Event& event) {
    if (event.type == SDL_MOUSEBUTTONUP) {
        // Add Obstacles
        const auto colrow = grid_.CoordsToColRow(
            {static_cast<float>(event.button.x), static_cast<float>(event.button.y)});
        const auto colrow_index = grid_.ColRowToIndex(colrow);

        if (colrow_index == pathfinder_.GetGoalIndex() || colrow_index == pathfinder_.GetStartIndex()) {
            return;
        }

        const auto is_walkable = grid_.IsWalkable(colrow_index);
        grid_.SetIsWalkable(colrow_index, !is_walkable);
    }

    if (event.type == SDL_KEYUP) {
        if (event.key.keysym.scancode == SDL_SCANCODE_C) {
            if (pathfinder_timer_.IsRunning()) {
                pathfinder_.Reset();
            } else {
                pathfinder_timer_.Start();
            }
        } else if (event.key.keysym.scancode == SDL_SCANCODE_SPACE) {
            pathfinder_.Step();
        } else if (event.key.keysym.scancode == SDL_SCANCODE_R) {
            pathfinder_.Reset();
            pathfinder_timer_.Reset(false);
        }
    }
}

std::string Game::GetWindowTitle() { return "Hello Alejandro"; }
int Game::GetScreenWidth() { return 800; }
int Game::GetScreenHeight() { return 800; }