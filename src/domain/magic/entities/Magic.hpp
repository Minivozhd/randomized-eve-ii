#pragma once

#include <algorithm>
#include <cstdint>
#include <vector>

namespace domain::magic {

typedef enum {
    PYROKINESIS = 0,
    COMBUSTION = 1,
    INFERNO = 2,
    NESCROSIS = 3,
    PLAZMA = 4,
    APOBIOSIS = 5,
    METABOLISM = 6,
    HEAL = 7,
    LIFEDRAIN = 8,
    ANTIBODY = 9,
    ENERGYSHOT = 10,
    ENERGYBALL = 11,
    UNDEFINED = -1,
} pe2_magic_enum;

typedef struct {
    pe2_magic_enum name;
    uint8_t level;
    uint8_t circle;
    size_t default_cost;
    uint8_t default_mana_bonus;
    size_t cost_address;
    size_t mana_bonus_address;
} pe2_magic;

extern std::vector<pe2_magic> magic_vector;
const extern std::vector<pe2_magic_enum> first_circle_magic;
const extern std::vector<pe2_magic_enum> all_cricles_magic;

} // namespace domain::magic
