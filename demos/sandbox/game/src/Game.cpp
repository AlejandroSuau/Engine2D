#include "Game.hpp"

Game::Game(Engine2D::Engine& engine) 
    : engine_(engine)
    , texture_manager_(*engine_.SDLRenderer(), asset_locator_)
    , font_manager_(asset_locator_) {}

void Game::Start() {
    engine_.SetCursorDisplay(false);
    
    font_text_ = font_manager_.LoadFont("fonts/atari-full.ttf", 10, "attari-full-10");
    font_title_ = font_manager_.LoadFont("fonts/atari-full.ttf", 20, "attari-full-20");

    texture_hand_default_ = texture_manager_.LoadTexture("images/cursor_light/hand_default.png", "hand_default");
    texture_hand_click_ = texture_manager_.LoadTexture("images/cursor_light/hand_click.png", "hand_click");
    texture_hand_drag_ = texture_manager_.LoadTexture("images/cursor_light/hand_drag.png", "hand_drag");

    key_texture_display_[0].texture = texture_manager_.LoadTexture("images/keys_light/C.png", "keys-C");
    key_texture_display_[1].texture = texture_manager_.LoadTexture("images/keys_light/S.png", "keys-S");
    key_texture_display_[2].texture = texture_manager_.LoadTexture("images/keys_light/R.png", "keys-R");
    key_texture_display_[3].texture = texture_manager_.LoadTexture("images/keys_light/W.png", "keys-W");
    key_texture_display_[4].texture = texture_manager_.LoadTexture("images/keys_light/D.png", "keys-D");
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

    // Render title
    renderer.RenderText(*font_title_, "<Pathfinder>", {41, 128, 185, 255}, {400, 15}, true);  
    static const float key_dimensions = 30.f;
    for (const auto& key_texture : key_texture_display_) {
        renderer.RenderTexture(
            key_texture.texture, 
            {0, 0, 19, 21},
            {key_texture.rect_dest.x, key_texture.rect_dest.y, key_dimensions, key_dimensions});
        
        const Vec2<int> text_dest {
            static_cast<int>(key_texture.rect_dest.x + key_dimensions),
            static_cast<int>(key_texture.rect_dest.y + 10.f)};
        renderer.RenderText(*font_text_, key_texture.message, {60, 70, 80, 230}, text_dest, false);      
    }

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

    // Render Grid
    renderer.SetRenderingColor({200, 210, 220, 255});
    for (const auto& c : cells) {
        renderer.RenderRect(
            {c.top_left_.x, c.top_left_.y, cell_dimensions, cell_dimensions}
        );
    }

    const auto costs = pathfinder_.GCosts();
    if (show_pathfinder_costs_ && pathfinder_.GetStepCount() > 0) {
        for (const auto& c : cells) {
            const auto cost = costs[c.index_];
            if (cost != Engine2D::Pathfinding::kINF) {
                renderer.RenderText(*font_text_, std::to_string(cost), {100, 200, 100, 255}, static_cast<Vec2<int>>(c.center_));
            }
        } 
    }

    // Render cursor
    static const float cursor_dimensions = 30.f;
    switch(user_action_) {
        case EMouseUserAction::MOVING_START_NODE:
        case EMouseUserAction::MOVING_GOAL_NODE:
            renderer.RenderTexture(
                texture_hand_drag_,
                {0, 0, 51, 51},
                {cursor_coords_.x - 5.f, cursor_coords_.y, cursor_dimensions, cursor_dimensions});
        break;
        case EMouseUserAction::DRAWING_BLOCKS:
        case EMouseUserAction::UNDRAWING_BLOCKS:
            renderer.RenderTexture(
                texture_hand_click_,
                {0, 0, 48, 45},
                {cursor_coords_.x, cursor_coords_.y, cursor_dimensions, cursor_dimensions});
        break;
        default:
            renderer.RenderTexture(
                texture_hand_default_, 
                {0, 0, 51, 63},
                {cursor_coords_.x - 5.f, cursor_coords_.y, cursor_dimensions, cursor_dimensions});
        break;
    }
}

const Engine2D::Grid& Game::GetGrid() const {
    return grid_;
}

void Game::HandleEvent(const SDL_Event& event) {
    const bool is_mouse_down = (event.type == SDL_MOUSEBUTTONDOWN);
    const bool is_mouse_up = (event.type == SDL_MOUSEBUTTONUP);
    const bool is_mouse_motion = (event.type == SDL_MOUSEMOTION);
    if (is_mouse_down || is_mouse_up || is_mouse_motion) {
        cursor_coords_.x = static_cast<float>(event.button.x);
        cursor_coords_.y = static_cast<float>(event.button.y);
        if (!grid_.AreCoordsInsideBoundaries(cursor_coords_)) return;

        const auto colrow = grid_.CoordsToColRow(cursor_coords_);
        cursor_colrow_index_ = grid_.ColRowToIndex(colrow);

        if (is_mouse_down) {
            OnMouseDown();
        } else if (is_mouse_motion) {
            OnMouseMotion();
        } else if (is_mouse_up) {
            user_action_ = EMouseUserAction::NONE;
        }
    }

    if (event.type == SDL_KEYUP) {
        OnKeyRelease(event.key.keysym.scancode);
    }
}

void Game::OnMouseDown() {
    if (cursor_colrow_index_ == pathfinder_.GetStartIndex()) {
        user_action_ = EMouseUserAction::MOVING_START_NODE;
    } else if (cursor_colrow_index_ == pathfinder_.GetGoalIndex()) {
        user_action_ = EMouseUserAction::MOVING_GOAL_NODE;
    } else if (grid_.IsWalkable(cursor_colrow_index_)) {
        user_action_ = EMouseUserAction::DRAWING_BLOCKS;
        grid_.SetIsWalkable(cursor_colrow_index_, false);
    } else {
        user_action_ = EMouseUserAction::UNDRAWING_BLOCKS;
        grid_.SetIsWalkable(cursor_colrow_index_, true);
    }
}

void Game::OnMouseMotion() {
    const auto is_walkable = grid_.IsWalkable(cursor_colrow_index_);
                                   
    switch(user_action_) {
        case EMouseUserAction::MOVING_START_NODE:
            if (!is_walkable || cursor_colrow_index_ == pathfinder_.GetStartIndex())
                return;
            
            pathfinder_.SetStartIndex(cursor_colrow_index_);
        break;
        case EMouseUserAction::MOVING_GOAL_NODE:
            if (!is_walkable || cursor_colrow_index_ == pathfinder_.GetGoalIndex())
                return;

            pathfinder_.SetGoalIndex(cursor_colrow_index_);
        break;
        case EMouseUserAction::UNDRAWING_BLOCKS:
            grid_.SetIsWalkable(cursor_colrow_index_, true);
        break;
        case EMouseUserAction::DRAWING_BLOCKS:
            grid_.SetIsWalkable(cursor_colrow_index_, false);
        break;
    }
}

void Game::OnKeyRelease(SDL_Scancode key_scancode) {
    switch(key_scancode) {
        case SDL_SCANCODE_C:
            pathfinder_.Reset();
            pathfinder_timer_.Start();
        break;
        case SDL_SCANCODE_S:
            pathfinder_.Step();
        break;
        case SDL_SCANCODE_R:
            pathfinder_.Reset();
            pathfinder_timer_.Reset(false);
        break;
        case SDL_SCANCODE_W:
            for (auto& c : grid_.Cells()) { c.is_walkable_ = true; }
        break;
        case SDL_SCANCODE_D:
            show_pathfinder_costs_ = !show_pathfinder_costs_;
        break;
    }
}

std::string Game::GetWindowTitle() { return "Pathfinder Demo"; }
int Game::GetScreenWidth() { return 800; }
int Game::GetScreenHeight() { return 800; }