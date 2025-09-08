#pragma once

#include "math/Vec2.hpp"

#include <vector>
#include <array>
#include <optional>

namespace Engine2D {

using ColRow_t = Vec2<int>;
using Coords_t = Vec2<float>;

class Grid {
public:
    struct Cell {
        Cell(
            std::size_t index,
            Coords_t top_left,
            Coords_t center,
            Vec2<std::size_t> colrow,
            bool is_walkable) 
            : index_(index)
            , top_left_(top_left)
            , center_(center)
            , colrow_(colrow)
            , is_walkable_(is_walkable) {}
        
        std::size_t index_;
        Coords_t top_left_;
        Coords_t center_;
        ColRow_t colrow_;
        bool is_walkable_;
    };

    using NeighboursCross_t = std::array<std::optional<Cell>, 4>;
    using NeighboursStar_t = std::array<std::optional<Cell>, 8>;

    Grid(
        const Coords_t origin,
        std::size_t col_count,
        std::size_t row_count,
        std::size_t cell_dimensions
    );

    NeighboursCross_t GetNeighboursCross();
    NeighboursStar_t GetNeighboursStar();

    std::size_t ColRowToIndex(ColRow_t colrow) const;
    Coords_t ColRowToCoords(ColRow_t colrow) const;
    Coords_t TopLeftCoordsToCenterCoords(Coords_t coords) const;
    ColRow_t IndexToColRow(std::size_t index) const;
    ColRow_t CoordsToColRow(Coords_t coords) const;

    bool IsWalkable(std::size_t cell_index) const;
    bool AreCoordsWalkable(Coords_t coords) const;
    bool AreColRowWalkable(ColRow_t colrow) const;
    bool AreColRowInsideBoundaries(ColRow_t colrow) const;
    bool AreCoordsInsideBoundaries(Coords_t coords) const;

    std::vector<Cell>& Cells();

    Cell& GetCell(Coords_t coords);
    Cell& GetCell(ColRow_t colrow);

    std::size_t GetColCount() const;
    std::size_t GetRowCount() const;
    std::size_t GetCellCount() const;
    std::size_t GetCellDimensions() const;

private:
    const Coords_t origin_;
    const std::size_t col_count_;
    const std::size_t row_count_;
    const std::size_t cell_dimensions_;

    const float width_;
    const float height_;
    const std::size_t cell_count_;

    std::vector<Cell> cells_;

    void Init();
};

}
