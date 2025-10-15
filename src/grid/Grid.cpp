#include "grid/Grid.hpp"

#include <assert.h>
#include <algorithm>

namespace Engine2D {

const Grid::NeighboursCrossColRow_t Grid::kNeighbourOffsetCross {{
    {0, -1}, // TOP
    {1,  0}, // RIGHT
    {0,  1}, // BOTTOM
    {-1, 0}, // LEFT
}};

// Mandatory. First 4 are diagonals
// Starprovider relies on this for diagonal base costs.
// Reason: You don't have to ask if this pos is a diagonal. 4 first are.
const Grid::NeighboursStarColRow_t Grid::kNeighboursOffsetStar {{
    {-1, -1}, // TOP-LEFT
    { 1, -1}, // TOP-RIGHT
    { 1,  1}, // BOTTOM-RIGHT
    {-1,  1}, // BOTTOM-LEFT
    { 0, -1}, // TOP
    { 1,  0}, // RIGHT
    { 0,  1}, // BOTTOM
    {-1,  0}, // LEFT
}};

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
    , half_cell_(static_cast<float>(cell_dimensions_) / 2.f)
    , row_count_int_(static_cast<int>(row_count))
    , col_count_int_(static_cast<int>(col_count)) {
    Init();
}

void Grid::Init() {
    const int default_cell_cost = 1;
    std::size_t i = 0;
    Coords_t pos = origin_;
    cells_.reserve(cell_count_);
    for (int row = 0; row < row_count_int_; ++row) {
        pos.x = origin_.x;
        for (int col = 0; col < row_count_int_; ++col) {
            const auto center_pos = pos + Coords_t{half_cell_, half_cell_};
            cells_.push_back(Grid::Cell{i, pos, center_pos, ColRow_t{col, row}, default_cell_cost, true});
            pos.x += cell_dimensions_;
            ++i;
        }
        pos.y += cell_dimensions_;
    }
}

Grid::NeighboursCrossCell_t Grid::GetNeighboursCross(ColRow_t from) const {
    NeighboursCrossCell_t result {};
    std::size_t i = 0;
    for (const auto& colrow_offset : kNeighbourOffsetCross) {
        const auto colrow = from + colrow_offset;
        auto* cell = GetCell(colrow);
        result[i] = (cell && cell->is_walkable_) ? std::optional<Cell>(*cell) : std::nullopt;
        ++i;
    }

    return result;
}

Grid::NeighboursStarCell_t Grid::GetNeighboursStar(ColRow_t from) const {
    NeighboursStarCell_t result {};
    std::size_t i = 0;
    for (const auto& colrow_offset : kNeighboursOffsetStar) {
        const auto colrow = from + colrow_offset;
        auto* cell = GetCell(colrow);
        result[i] = (cell && cell->is_walkable_) ? std::optional<Cell>(*cell) : std::nullopt;
        ++i;
    }

    return result;
}

std::size_t Grid::CoordsToIndex(Coords_t coords) const {
    const auto colrow = CoordsToColRow(coords);
    return ColRowToIndex(colrow);
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
    return {static_cast<int>(index % col_count_),
            static_cast<int>(index / col_count_)};
}

ColRow_t Grid::CoordsToColRow(Coords_t coords) const {
    return {static_cast<int>(coords.x - origin_.x) / static_cast<int>(cell_dimensions_),
            static_cast<int>(coords.y - origin_.y) / static_cast<int>(cell_dimensions_)};
}

void Grid::ClampColRowIntoMapDimensions(ColRow_t& colrow) const {
    colrow.x = std::clamp(colrow.x, 0, col_count_int_ - 1);
    colrow.y = std::clamp(colrow.y, 0, row_count_int_ - 1);
}

bool Grid::IsWalkable(std::size_t index) const {
    assert(index < cell_count_ && "Cell index above cells size");
    return (cells_[index].is_walkable_);
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

bool Grid::SetIsWalkable(std::size_t index, bool is_walkable) {
    assert(index < cell_count_ && "Cell index above cells size");
    return cells_[index].is_walkable_ = is_walkable;
}

std::vector<Grid::Cell>& Grid::Cells() {
    return cells_;
}

const std::vector<Grid::Cell>& Grid::Cells() const {
    return cells_;
}

Grid::Cell* Grid::GetCell(Coords_t coords) noexcept {
    return GetCellImpl(*this, coords);
}

Grid::Cell* Grid::GetCell(ColRow_t colrow) noexcept {
    return GetCellImpl(*this, colrow);
}

Grid::Cell* Grid::GetCell(std::size_t index) noexcept {
    return GetCellImpl(*this, index);
}

const Grid::Cell* Grid::GetCell(Coords_t coords) const noexcept {
    return GetCellImpl(*this, coords);
}

const Grid::Cell* Grid::GetCell(ColRow_t colrow) const noexcept {
    return GetCellImpl(*this, colrow);
}

const Grid::Cell* Grid::GetCell(std::size_t index) const noexcept {
    return GetCellImpl(*this, index);
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
