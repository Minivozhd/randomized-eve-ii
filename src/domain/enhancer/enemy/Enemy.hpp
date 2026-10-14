#pragma once

#include "domain/enemy/entities/EnemyTypes.hpp"
#include <cstdint>
#include <ctype.h>
#include <map>
#include <string>
#include <unordered_map>
#include <vector>

namespace domain {
typedef struct EnemyEnhancement {
    std::string filename;
    std::unordered_map<size_t, uint32_t> command_to_change;

    size_t hp_offset;
    std::vector<size_t> damage_offsets;

    size_t texture_offset_source_start = 0x0;
    size_t texture_offset_source_finish = 0x0;
    size_t texture_offset_target_start = 0x0;
    size_t texture_offset_target_finish = 0x0;

    size_t package_offset_source_start = 0x0;
    size_t package_offset_source_finish = 0x0;
    size_t package_offset_target_start = 0x0;
    size_t package_offset_target_finish = 0x0;
} pe2_enemy_enhancement_t;

extern std::map<uint8_t, pe2_enemy_enhancement_t> boss_enhancements;
} // namespace domain
