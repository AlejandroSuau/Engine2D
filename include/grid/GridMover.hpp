#pragma once

#include "utils/Types.hpp"

#include "grid/Grid.hpp"
#include "grid/GridMoverUtils.hpp"

namespace Engine2D {

class GridMover {
public:

    struct Params {
        float tile_size {1.f};
        float tiles_per_sec {10.f};
    };

    explicit GridMover(Params p);

    void Update(float dt, const Grid& grid, Coords_t& pos);
    void SetNextDirection(EDirection dir);
    void ClearQueue();
    void Stop();
    EDirection Direction() const noexcept;

private:
    Params params_;
    EDirection dir_ {EDirection::NONE};
    EDirection queued_dir_ {EDirection::NONE};
    std::optional<Coords_t> target_;
};

}
