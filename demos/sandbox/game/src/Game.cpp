#include "Game.hpp"

Game::Game(Engine2D::IEngineAPI& engine_api) 
    : engine_api_(engine_api)
    , texture_manager_(*engine_api_.GetSDLRenderer(), asset_locator_)
    , font_manager_(asset_locator_) {}

void Game::Start() {
    engine_api_.SetCursorDisplay(false);
    
    auto& collision_world = engine_api_.GetCollisionWorld();  
    /*collision_world.AddCollider(ball_one.collider);
        Engine2D::Collider{
            1, 
            Engine2D::Circle{Vec2<float>{200.f, 0.f}, 20.f},
            1, // layer
            1 // mask
        } */
    ball_one.collider_id = collision_world.AddCollider({
        static_cast<std::uint32_t>(-1),
        Engine2D::Circle{Vec2<float>{200.f, 0.f}, 20.f},
        1, // layer
        1 // mask
    });

    ball_two.collider_id = collision_world.AddCollider({ 
        static_cast<std::uint32_t>(-1),
        Engine2D::Circle{Vec2<float>{215.f, 0.f}, 20.f},
            1, // layer
            1 // mask
    });

    collision_world.AddListener(this);
}

void Game::Update(float dt) {
    ball_one.is_colliding = false;
    ball_two.is_colliding = false;

    ball_one.center.y += ball_one.velocity * dt;
    engine_api_.GetCollisionWorld().GetCollider(ball_one.collider_id).SetCenterPosition(ball_one.center);
    
    ball_two.center.y += ball_two.velocity * dt;
    engine_api_.GetCollisionWorld().GetCollider(ball_two.collider_id).SetCenterPosition(ball_two.center);
}

void Game::Render(Engine2D::Renderer& renderer) {  

    // TODO: REMOVE - Render CIRCLE
    if (ball_one.is_colliding) {
        renderer.SetRenderingColor({255, 0, 0, 255});
    } else {
        renderer.SetRenderingColor({0, 255, 0, 255});
    }
    
    //renderer.DrawCircleOutline(ball_one.center, ball_one.radius);
    renderer.DrawCircleOutline(ball_one.center, ball_one.radius);

    //renderer.DrawCircleOutline(ball_two.center, ball_two.radius);
    renderer.DrawCircleOutline(ball_two.center, ball_two.radius);
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
    }

    if (event.type == SDL_KEYUP) {
        OnKeyRelease(event.key.keysym.scancode);
    }
}

void Game::OnMouseDown() {

}

void Game::OnMouseMotion() {

}

void Game::OnKeyRelease(SDL_Scancode key_scancode) {
    switch(key_scancode) {
        case SDL_SCANCODE_C:
            pathfinder_.Reset();
            pathfinder_timer_.Start();
        break;
        case SDL_SCANCODE_S:
            if (pathfinder_.GetStepCount() == 0) {
                pathfinder_.Reset();
            }
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

void Game::OnCollision(const Engine2D::CollisionEvent& e) {
    //const auto col_pos_1 = ball_one.collider.GetCenterPosition();
   // SDL_Log("## Ball One ## Collider Pos: (%f.2, %f.2); Radius: (%f.2)", col_pos_1.x, col_pos_1.y, ball_one.radius);
    ball_one.is_colliding = true;

    //const auto col_pos_2 = ball_two.collider.GetCenterPosition();
    //SDL_Log("## Ball Two ## Collider Pos: (%f.2, %f.2); Radius: (%f.2)", col_pos_2.x, col_pos_2.y, ball_two.radius);
    ball_two.is_colliding = true;
}