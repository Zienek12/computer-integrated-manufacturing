#pragma once

#include <chrono>
#include <utility>

template <typename Func>
auto zmierzCzas(Func func) -> std::pair<decltype(func()), long long> {
    const std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
    const decltype(func()) wynik = func();
    const std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
    const long long duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    return std::make_pair(wynik, duration);
}
