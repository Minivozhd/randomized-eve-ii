#pragma once
#include "domain/enemy/entities/Enemy.hpp"
#include <cstddef>
#include <map>
// The production count algorithm with its two random draws supplied explicitly.
// Keeping this pure lets the local loader matrix enumerate every integer value
// partition and all three share branches, without sampling platform RNGs.
namespace domain::enemy {
inline std::map<uint8_t, size_t> calculateEnemyCounts(const std::map<uint32_t, uint8_t> &enemy_slots,
                                                      size_t &pool_size, size_t value, double prob) {
    // Reserve space for dummy monster if it will be added
    // This is needed because dummy monster descriptor is added separately
    // but still consumes a slot in the pool
    for (const auto &entry : enemy_slots) {
        if (entry.second == DUMMY_MONSTER_ID) {
            if (pool_size > 0) {
                pool_size--;
            }
            break;
        }
    }

    // Let's find out how many real enemy types do we have
    std::vector<std::pair<uint8_t, double>> enemy_types{};
    // domain::encounter::pe2_encounter_type_t encounter_type = ENCOUNTER_TYPE_MONO;
    for (auto &&entry : enemy_slots) {
        if (entry.second != NO_ENEMY_ID && entry.second != DUMMY_MONSTER_ID) {
            enemy_types.push_back({entry.second, 1.0});
        }
    }
    if (enemy_types.size() > 1) {
        // If we have several enemy types let's choose encounter type randomly
        if (prob < 0.33) {
            // encounter_type = ENCOUNTER_TYPE_EVEN;
            enemy_types[0].second = 0.5;
            enemy_types[1].second = 0.5;
        } else if (prob < 0.67) {
            // encounter_type = ENCOUNTER_TYPE_FIRST_PREVAILS;
            enemy_types[0].second = 0.33;
            enemy_types[1].second = 0.67;
        } else {
            // encounter_type = ENCOUNTER_TYPE_SECOND_PREVAILS;
            enemy_types[0].second = 0.66;
            enemy_types[1].second = 0.34;
        }
    }
    std::map<uint8_t, size_t> enemy_count_map{};
    size_t mandatory_encounter_size = 3;
    bool is_first_enemy_type = true;
    bool already_have_res_enemy = false;
    for (auto &&entry : enemy_types) {
        size_t value_partition = value * entry.second;
        enemy_count_map[entry.first] = 0;
        pe2_enemy_t current_enemy = enemies_map.at(entry.first);
        size_t current_enemy_type_pool = 12 * entry.second;
        if (enemy_types.size() == 1) {
            mandatory_encounter_size = 2;
        } else {
            if (is_first_enemy_type) {
                if (current_enemy.res_type == ENEMY_RES_TYPE_NEEDS_ADDITIONAL_RESOURCES) {
                    mandatory_encounter_size = 2;
                    already_have_res_enemy = true;
                } else {
                    mandatory_encounter_size = 1;
                }
            } else {
                if (current_enemy.res_type == ENEMY_RES_TYPE_NEEDS_ADDITIONAL_RESOURCES &&
                    !already_have_res_enemy) {
                    mandatory_encounter_size = 2;
                } else if (current_enemy.res_type == ENEMY_RES_TYPE_NEEDS_ADDITIONAL_RESOURCES) {
                    mandatory_encounter_size = 1;
                } else if (already_have_res_enemy) {
                    mandatory_encounter_size = 1;
                } else {
                    mandatory_encounter_size = 2;
                }
            }
        }
        while (value_partition > 0 && current_enemy_type_pool > 0) {
            enemy_count_map[entry.first]++;
            switch (current_enemy.size_type) {
            case ENEMY_SIZE_SMALL:
                current_enemy_type_pool = current_enemy_type_pool > 1 ? current_enemy_type_pool - 1 : 0;
                break;
            case ENEMY_SIZE_SMALLER:
                current_enemy_type_pool = current_enemy_type_pool > 2 ? current_enemy_type_pool - 2 : 0;
                break;
            case ENEMY_SIZE_NORMAL:
                current_enemy_type_pool = current_enemy_type_pool > 3 ? current_enemy_type_pool - 3 : 0;
                break;
            default:
                current_enemy_type_pool--;
            }
            if (value_partition > current_enemy.value) {
                value_partition = value_partition - current_enemy.value;
            } else {
                value_partition = 0;
            }
            if (mandatory_encounter_size > 0) {
                mandatory_encounter_size--;
            } else {
                if (pool_size > 0) {
                    pool_size--;
                }
                // we don't have enough space
            }
            if (mandatory_encounter_size <= 0 && pool_size <= 0) {
                break;
            }
        }
        if (is_first_enemy_type) {
            is_first_enemy_type = false;
        }
    }

    return enemy_count_map;
}
} // namespace domain::enemy
