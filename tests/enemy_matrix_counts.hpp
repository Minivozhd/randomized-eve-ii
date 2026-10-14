#pragma once
#include "domain/enemy/entities/EnemyCounts.hpp"
#include <algorithm>
#include <cmath>
#include <set>
namespace matrix_tests {
inline std::set<size_t> scaledCountValues(const std::map<uint32_t, uint8_t> &plan, size_t baseValue,
                                          double shareCase) {
    using namespace domain::enemy;
    const size_t low = size_t(baseValue * 0.575), high = size_t(baseValue * 1.725);
    std::vector<double> shares;
    size_t types =
        std::count_if(plan.begin(), plan.end(), [](auto p) { return p.second != DUMMY_MONSTER_ID; });
    if (types == 1)
        shares = {1.0};
    else if (shareCase < 0.33)
        shares = {0.5, 0.5};
    else if (shareCase < 0.67)
        shares = {0.33, 0.67};
    else
        shares = {0.66, 0.34};
    std::set<size_t> values = {low, high};
    size_t i = 0;
    for (auto [slot, id] : plan)
        if (id != DUMMY_MONSTER_ID) {
            double share = shares.at(i++);
            for (size_t n = 0; n <= 12; ++n) {
                long boundary = long(std::ceil((n * enemies_map.at(id).value + 1) / share));
                for (long delta = -2; delta <= 2; ++delta)
                    if (boundary + delta >= long(low) && boundary + delta <= long(high))
                        values.insert(boundary + delta);
            }
        }
    return values;
}
} // namespace matrix_tests
