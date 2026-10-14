#pragma once

#include "domain/encounter/entities/Reward.hpp"
#include "domain/game/entities/Level.hpp"
#include <map>
#include <vector>

typedef struct rcontext {
    std::map<domain::game::pe2_level_name_t, std::vector<domain::encounter::pe2_enc_reward_descriptor_t>>
        reward_slots{};
    std::map<domain::game::pe2_level_name_t, std::vector<domain::encounter::pe2_enc_reward_descriptor_t>>
        bounty_reward_slots{};
    uint8_t selected_weapon;
} pe2_randomizer_context;
