#pragma once

#include "Vec2.hpp"

#include <vector>
#include <array>
#include <optional>

// glm vec2
using ColRow_t = Vec2<int>;
using Coords_t = Vec2<float>;

class Grid {
public:
    struct Cell {
        Cell(
            std::size_t index,
            Coords_t top_left,
            Coords_t center,
            ColRow_t colrow,
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

    Grid(
        const Coords_t origin,
        std::size_t col_count,
        std::size_t row_count,
        std::size_t cell_dimensions
    );

    std::size_t CoordsToIndex(Coords_t coords) const;
    std::size_t ColRowToIndex(ColRow_t colrow) const;
    Coords_t ColRowToCoords(ColRow_t colrow) const;
    Coords_t TopLeftCoordsToCenterCoords(Coords_t coords) const;
    Coords_t ColRowToCenterCoords(ColRow_t colrow) const;
    ColRow_t IndexToColRow(std::size_t index) const;
    ColRow_t CoordsToColRow(Coords_t coords) const;
    void ClampColRowIntoMapDimensions(ColRow_t& colrow) const;

    bool IsWalkable(std::size_t index) const;
    bool AreCoordsWalkable(Coords_t coords) const;
    bool AreColRowWalkable(ColRow_t colrow) const;
    bool AreColRowInsideBoundaries(ColRow_t colrow) const;
    bool AreCoordsInsideBoundaries(Coords_t coords) const;

    std::vector<Cell>& Cells();
    const std::vector<Cell>& Cells() const;

    Cell* GetCell(Coords_t coords) noexcept;
    Cell* GetCell(ColRow_t colrow) noexcept;
    Cell* GetCell(std::size_t index) noexcept;
    const Cell* GetCell(Coords_t coords) const noexcept;
    const Cell* GetCell(ColRow_t colrow) const noexcept;
    const Cell* GetCell(std::size_t index) const noexcept;

    std::size_t GetColCount() const;
    std::size_t GetRowCount() const;
    std::size_t GetCellCount() const;
    std::size_t GetCellDimensions() const;

    bool SetIsWalkable(std::size_t index, bool is_walkable);

private:
    const Coords_t origin_;
    const std::size_t col_count_;
    const std::size_t row_count_;
    const std::size_t cell_dimensions_;

    const float width_;
    const float height_;
    const std::size_t cell_count_;
    const float half_cell_;
    const int col_count_int_;
    const int row_count_int_;

    std::vector<Cell> cells_;

    void Init();
    
    template <class GridT>
    using cell_ptr_t = std::conditional_t<
        std::is_const_v<std::remove_reference_t<GridT>>,
        const Cell*,
        Cell*>;

    template<typename GridT>
    static cell_ptr_t<GridT> GetCellImpl(GridT& grid, ColRow_t colrow) {
        if (!grid.AreColRowInsideBoundaries(colrow)) {
            return nullptr;
        }
        
        const auto index = grid.ColRowToIndex(colrow);
        return &grid.cells_[index];
    }

    template<typename GridT>
    static cell_ptr_t<GridT> GetCellImpl(GridT& grid, Coords_t coords) {
        if (!grid.AreCoordsInsideBoundaries(coords)) {
            return nullptr;
        }

        const auto index = grid.CoordsToIndex(coords);
        return &grid.cells_[index];
    }
    
    template<typename GridT>
    static cell_ptr_t<GridT> GetCellImpl(GridT& grid, std::size_t index) {
        if (index >= grid.cell_count_) {
            return nullptr;
        }

        return &grid.cells_[index];
    }
};
