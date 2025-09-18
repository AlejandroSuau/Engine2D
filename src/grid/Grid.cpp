#include "grid/Grid.hpp"

#include <assert.h>
#include <algorithm>

namespace Engine2D {

Grid::Grid(
    const Coords_t origin,
    std::size_t col_count,
    std::size_t row_count,
    std::size_t cell_dimensions) 
    : origin_(origin)
    , col_count_(col_count)
    , row_count_(row_count)
    , cell_dimensions_(cell_dimensions) 
    , width_(static_cast<float>(col_count_ * cell_dimensions_)) 
    , height_(static_cast<float>(row_count_ * cell_dimensions_))
    , cell_count_(col_count * row_count_)
    , half_cell_(static_cast<float>(cell_dimensions_) / 2.f) {
    Init();
}

void Grid::Init() {
    std::size_t i = 0;
    Coords_t pos = origin_;
    cells_.reserve(cell_count_);
    for (std::size_t row = 0; row < row_count_; ++row) {
        pos.x = origin_.x;
        for (std::size_t col = 0; col < col_count_; ++col) {
            const auto center_pos = pos + Coords_t{
                static_cast<float>(cell_dimensions_) / 2.f, static_cast<float>(cell_dimensions_) / 2.f};
            cells_.push_back(Grid::Cell{i, pos, center_pos, Vec2<std::size_t>{col, row}, true});
            pos.x += cell_dimensions_;
            ++i;
        }
        pos.y += cell_dimensions_;
    }
}

Grid::NeighboursCross_t Grid::GetNeighboursCross() {
    return NeighboursCross_t{};
}

Grid::NeighboursStar_t Grid::GetNeighboursStar() {
    return NeighboursStar_t{};
}

std::size_t Grid::ColRowToIndex(ColRow_t colrow) const {
    return (colrow.y * col_count_ + colrow.x);
}

Coords_t Grid::ColRowToCoords(ColRow_t colrow) const {
    const auto index = ColRowToIndex(colrow);
    return cells_[index].top_left_;
}

Coords_t Grid::TopLeftCoordsToCenterCoords(Coords_t coords) const {
    return coords + Coords_t{half_cell_, half_cell_};
}

Coords_t Grid::ColRowToCenterCoords(ColRow_t colrow) const {
    const auto cell_coords = ColRowToCoords(colrow);
    return TopLeftCoordsToCenterCoords(cell_coords);
}

ColRow_t Grid::IndexToColRow(std::size_t index) const {
    return {static_cast<int>(index / col_count_),
            static_cast<int>(index % col_count_)};
}

ColRow_t Grid::CoordsToColRow(Coords_t coords) const {
    return {static_cast<int>(coords.x - origin_.x) / static_cast<int>(cell_dimensions_),
            static_cast<int>(coords.y - origin_.y) / static_cast<int>(cell_dimensions_)};
}

void Grid::ClampColRowIntoMapDimensions(ColRow_t& colrow) const {
    colrow.x = std::clamp(colrow.x, 0, static_cast<int>(col_count_) - 1);
    colrow.y = std::clamp(colrow.y, 0, static_cast<int>(row_count_) - 1);
}

bool Grid::IsWalkable(std::size_t cell_index) const {
    assert(cell_index < cell_count_ && "Cell index above cells size");
    return (cells_[cell_index].is_walkable_);
}

bool Grid::AreCoordsWalkable(Coords_t coords) const {
    if (!AreCoordsInsideBoundaries(coords)) return false;
    
    const auto colrow = CoordsToColRow(coords);
    const auto index = ColRowToIndex(colrow);
    return cells_[index].is_walkable_;
}

bool Grid::AreColRowWalkable(ColRow_t colrow) const {
    if (!AreColRowInsideBoundaries(colrow)) return false;

    const auto index = ColRowToIndex(colrow);
    return cells_[index].is_walkable_;
}

bool Grid::AreColRowInsideBoundaries(ColRow_t colrow) const {
    return (colrow.y >= 0 && colrow.y < row_count_ &&
            colrow.x >= 0 && colrow.x < col_count_);
}

bool Grid::AreCoordsInsideBoundaries(Coords_t coords) const {
    return (coords.x >= origin_.x && coords.x < (width_ + origin_.x) &&
            coords.y >= origin_.y && coords.y < (height_ + origin_.y));
}

std::vector<Grid::Cell>& Grid::Cells() {
    return cells_;
}

Grid::Cell& Grid::GetCell(Coords_t coords) {
    return cells_[0];
}

Grid::Cell& Grid::GetCell(ColRow_t colrow) {
    return cells_[0];
}

std::size_t Grid::GetColCount() const {
    return col_count_;
}

std::size_t Grid::GetRowCount() const {
    return row_count_;
}

std::size_t Grid::GetCellCount() const {
    return cell_count_;
}

std::size_t Grid::GetCellDimensions() const {
    return cell_dimensions_;
}

}
