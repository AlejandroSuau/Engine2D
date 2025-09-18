#include "game/pacman/IChaseStrategy.hpp"

#include <SDL2/SDL.h>

#include "grid/Grid.hpp"
#include "grid/GridMoverUtils.hpp"

#include "game/pacman/Game.hpp"

/** Blinky */
Engine2D::Coords_t ChaseStrategyBlinky::ChasingTargetPosition(const Game& game_context) {
    const auto& grid = game_context.GetGrid();
    const auto player_cell_colrow = grid.CoordsToColRow(game_context.GetPlayer().pos_);
    return (grid.ColRowToCoords(player_cell_colrow));
}

/** Pinky */
Engine2D::Coords_t ChaseStrategyPinky::ChasingTargetPosition(const Game& game_context) {
    const auto& player = game_context.GetPlayer();
    const auto& grid = game_context.GetGrid();
    const auto player_dir = player.mover.Direction();
    const auto player_dir_vector = static_cast<Vec2<int>>(Engine2D::FromDirectionToVector(player_dir));
    
    auto player_cell_colrow = grid.CoordsToColRow(player.pos_);
    auto target_colrow = player_cell_colrow + Engine2D::ColRow_t {
        4 * player_dir_vector.x, 4 * player_dir_vector.y
    };

    // Here is due to the bug (if going up - 4 left)
    if (player_dir == Engine2D::EDirection::UP) {
        target_colrow.x -= 4;
    }
    
    grid.ClampColRowIntoMapDimensions(target_colrow);

    return grid.ColRowToCenterCoords(target_colrow);
}

/** Inky */
// Rules:
// 1. Imaginary point 2 cells through player's direction.
// 2. Then: Target = (2 * Imaginary Point) - Blinky's position)
Engine2D::Coords_t ChaseStrategyInky::ChasingTargetPosition(const Game& game_context) {
    /*auto& pathfinder = game.GetPathfinder();
    const auto& game_map = game.GetMap();
    const auto player_position = game.GetPlayer().GetCenterPosition();
    const auto row_col_player = game_map.FromCoordsToColRow(player_position);
    const auto player_direction = game.GetPlayer().GetDirectionVector();
    
    // Imaginary row col 2 cells through player's direction.
    static const int kCellsDistance = 2;
    Vec2<int> row_col_imaginary {
        row_col_player.x + (player_direction.x * kCellsDistance),
        row_col_player.y + (player_direction.y * kCellsDistance)
    };
    
    // Blinky position doubles that imaginary point diff.
    auto ghost_blinky = game.GetGhost("Blinky");
    const auto blinky_position = ghost_blinky->get().GetCenterPosition();
    const auto col_row_blinky = game_map.FromCoordsToColRow(blinky_position);
    Vec2<int> col_row_target {
        2 * row_col_imaginary.x - col_row_blinky.x,
        2 * row_col_imaginary.y - col_row_blinky.y};
    game_map.ClampColRowIntoMapDimensions(col_row_target);

    return pathfinder.FindPath(col_row_ghost, col_row_target);*/
}

/** Clyde */
Engine2D::Coords_t ChaseStrategyClyde::ChasingTargetPosition(const Game& game_context) {
    /*const auto& game_map = game.GetMap();
    const auto player_position = game.GetPlayer().GetCenterPosition();
    const auto col_row_player = game_map.FromCoordsToColRow(player_position);
    
    clyde position

    static const double kLimitDistance = 8.0;
    const auto distance = std::hypot(
        col_row_ghost.y - col_row_player.y,
        col_row_ghost.x - col_row_player.x);
    
    Vec2<int> col_row_target;
    if (distance < kLimitDistance) {
        col_row_target.y = static_cast<int>(game_map.GetRowsCount()) - 1;
    } else {
        col_row_target = col_row_player;
    }

    return pathfinder.FindPath(col_row_ghost, col_row_target);*/
}
