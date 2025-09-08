#include "grid/Grid.hpp"

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
    , cell_count_(col_count * row_count_) {
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
    return {0.f, 0.f};
}

Coords_t Grid::TopLeftCoordsToCenterCoords(Coords_t coords) const {
    return {0.f, 0.f};
}

ColRow_t Grid::IndexToColRow(std::size_t index) const {
    return {static_cast<int>(index / col_count_),
            static_cast<int>(index % col_count_)};
}

ColRow_t Grid::CoordsToColRow(Coords_t coords) const {
    return {static_cast<int>(coords.x - origin_.x) / static_cast<int>(cell_dimensions_),
            static_cast<int>(coords.y - origin_.y) / static_cast<int>(cell_dimensions_)};
}

bool Grid::IsWalkable(std::size_t cell_index) const {
    return false;
}

bool Grid::AreCoordsWalkable(Coords_t coords) const {
    return false;
}

bool Grid::AreColRowWalkable(ColRow_t colrow) const {
    return false;
}

bool Grid::AreColRowInsideBoundaries(ColRow_t colrow) const {
    return false;
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
