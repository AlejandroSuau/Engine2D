#pragma once

#include "utils/Types.hpp"

// #include "pathfinding/concept/GridGraph.hpp"
// #include "pathfinding/concept/Frontier.hpp"
// #include "pathfinding/concept/Heuristic.hpp"

// BFS - ZeroHeuristic - 4dir
// Dijkstra - ZeroHeristic - 4-8 dir
// A* - Manhattan / Octile / Chebyshev / Euclidean 

#include <vector>
#include <optional>

namespace Engine2D::Pathfinding {

struct SearchBuffers {
    std::vector<bool>                    closed;
    std::vector<DistanceCost_t>          g;     
    std::vector<std::optional<NodeId_t>> parent;
};

inline constexpr DistanceCost_t kINF = std::numeric_limits<DistanceCost_t>::max();

template<class G, class F, class H, class NeighbourProv>
class Pathfinder {
public:
    using Path = std::vector<NodeId_t>;

    Pathfinder(const G& grid, NodeId_t start, NodeId_t goal)
        : g_{grid}
        , start_{start}
        , goal_{goal} {
        Reset();
    }

    Path FindPath() {
        while (!Step()) {}
        return path_;
    }

    bool Step() {
        if (done_) return true;
        if (FrontierEmpty()) {
            done_ = true;
            return true;
        }

        auto [u, g_u, p] = FrontierPop();
        if (buf_.closed[u]) {
            return false;
        }

        buf_.closed[u] = true;
        if (p) { buf_.parent[u] = *p; }
        
        if (u == goal_) {
            done_ = true;
            BuildPath();
            return true;
        }

        for (auto [v, w] : neigh_(g_, u)) {
            if (g_u == kINF || w > (kINF - g_u)) continue;

            const DistanceCost_t g_new = g_u + w;
            if (g_new < buf_.g[v]) {
                buf_.g[v] = g_new;
                if (buf_.closed[v]) {
                    buf_.closed[v] = false;
                }

                Push(v, u);
            }
        }

        return false;
    }

    void Reset() {
        Reset(start_, goal_);
    }

    void Reset(NodeId_t new_start, NodeId_t new_goal) {
        start_ = new_start;
        goal_ = new_goal;

        path_.clear();
        done_ = false;

        const auto N = g_.GetCellCount();
        if (buf_.g.size() == N) {
            std::fill(buf_.g.begin(), buf_.g.end(), kINF);
            std::fill(buf_.closed.begin(), buf_.closed.end(), false);
            std::fill(buf_.parent.begin(), buf_.parent.end(), std::nullopt);
        } else {
            buf_.g.assign(N, kINF);
            buf_.closed.assign(N, false);
            buf_.parent.assign(N, std::nullopt);
        }

        frontier_.clear();
        buf_.g[start_] = 0;
        Push(start_, std::nullopt);
    }

    [[nodiscard]] const Path& GetPath() const noexcept { return path_; }
    [[nodiscard]] const auto& GCosts() const noexcept { return buf_.g; }
    [[nodiscard]] const auto& Parents() const noexcept { return buf_.parent; }
    [[nodiscard]] const auto& Closed() const noexcept { return buf_.closed; }
    [[nodiscard]] NodeId_t GetStartIndex() const noexcept { return start_; }
    [[nodiscard]] NodeId_t GetGoalIndex() const noexcept { return goal_; }

    void SetStartIndex(NodeId_t new_index) {start_ = new_index; }
    void SetGoalIndex(NodeId_t new_index) { goal_ = new_index; }

private:
    const G& g_;
    NodeId_t start_, goal_;
    F frontier_;
    H heur_;
    NeighbourProv neigh_;
    SearchBuffers buf_;
    Path path_;
    bool done_{false};

    void Push(NodeId_t n, std::optional<NodeId_t> parent) {
        const DistanceCost_t h = heur_(g_, n, goal_);
        frontier_.push(n, buf_.g[n], h, parent);
    }

    bool FrontierEmpty() const {
        return frontier_.empty();
    }

    auto FrontierPop() {
        return frontier_.pop();
    }

    void BuildPath() {
        path_.clear();
        for (auto u = std::optional<NodeId_t>{goal_}; u.has_value(); u = buf_.parent[*u]) {
            path_.push_back(*u);
        }
        std::reverse(path_.begin(), path_.end());
    }
};

}
