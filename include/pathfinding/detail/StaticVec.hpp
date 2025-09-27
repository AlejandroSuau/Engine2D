#pragma once

#include <array>
#include <cstddef>

namespace Engine2D
{

template<class T, std::size_t N>
struct StaticVec {
    std::array<T, N> data{};
    std::size_t size{0};

    void push_back(const T& v) { data[size++] = v; }
    auto begin() { return data.begin(); }
    auto end() { return data.begin() + size; }
    auto cbegin() const { return data.cbegin(); }
    auto cend() const { return data.cbegin() + size; }
    std::size_t count() const { return size; }
};

}