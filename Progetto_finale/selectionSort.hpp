#pragma once

#include <vector>
#include <algorithm>
#include <cstddef>   // per std::size_t

template<typename T>
void selectionSort(std::vector<T>& v)
{
    const std::size_t n = v.size();
    if (n < 2) return;                       // 0 o 1 elementi

    for (std::size_t i = 0; i + 1 < n; ++i) {
        std::size_t min_index = i;
        for (std::size_t j = i + 1; j < n; ++j) {
            if (v[j] < v[min_index]) {
                min_index = j;
            }
        }
        if (min_index != i) {
            std::swap(v[i], v[min_index]);
        }
    }
}
