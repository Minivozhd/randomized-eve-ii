// Independent exhaustive-integer oracle for the minimized count boundaries.
#include "enemy_matrix_counts.hpp"
#include <iostream>
#include <stdexcept>
int main() {
    using namespace domain::enemy;
    size_t checks = 0;
    for (auto [id, e] : enemies_map)
        for (auto [other, f] : enemies_map) {
            if (other < id)
                continue;
            std::map<uint32_t, uint8_t> plan{{ENEMY_SLOT_1_ADDRESS, id}};
            if (other != id)
                plan[ENEMY_SLOT_2_ADDRESS] = other;
            if (e.res_type == ENEMY_RES_TYPE_NEEDS_ADDITIONAL_RESOURCES ||
                f.res_type == ENEMY_RES_TYPE_NEEDS_ADDITIONAL_RESOURCES)
                plan[ENEMY_SLOT_3_ADDRESS] = DUMMY_MONSTER_ID;
            for (size_t base : {size_t(1), size_t(37), size_t(501), size_t(1600)})
                for (double split : {0.0, 0.5, 0.9})
                    for (size_t pool : {size_t(0), size_t(1), size_t(2), size_t(5), size_t(13)}) {
                        std::set<std::map<uint8_t, size_t>> brute, minimized;
                        for (size_t value = size_t(base * 0.575); value <= size_t(base * 1.725); ++value) {
                            auto left = pool;
                            brute.insert(calculateEnemyCounts(plan, left, value, split));
                        }
                        for (auto value : matrix_tests::scaledCountValues(plan, base, split)) {
                            auto left = pool;
                            minimized.insert(calculateEnemyCounts(plan, left, value, split));
                        }
                        if (brute != minimized)
                            throw std::runtime_error("Count-threshold coverage mismatch");
                        ++checks;
                    }
        }
    std::cout << "PASS " << checks << " exhaustive integer partition oracles\n";
}
