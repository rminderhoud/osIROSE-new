#pragma once

namespace Core {
template <typename T, size_t N, size_t L>
struct array_wrapper { T& iterable; };

template <typename T, size_t N, size_t L>
constexpr auto begin(const array_wrapper<T, N, L>& w) {
    return std::begin(w.iterable) + N;
}

template <typename T, size_t N, size_t L>
constexpr auto end(const array_wrapper<T, N, L>& w) {
    return std::begin(w.iterable) + N + L;
}
}
