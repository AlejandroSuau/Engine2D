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
    std::optional<Coords_t> FinalTarget() const;
    void SetFinalTarget(Coords_t pos);

private:
    Params params_;
    EDirection dir_ {EDirection::NONE};
    EDirection queued_dir_ {EDirection::NONE};
    std::optional<Coords_t> step_target_;
    std::optional<Coords_t> final_target_;
};

}
