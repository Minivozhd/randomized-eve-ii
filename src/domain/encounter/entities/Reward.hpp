#pragma once

#include <cstdint>
#include <map>
#include <string>
#include <vector>

#include "domain/game/entities/GameMode.hpp"
#include "domain/game/entities/Level.hpp"

namespace domain::encounter {

typedef struct encrew {
    // first byte is always zero
    uint8_t encounter_id;
    uint8_t room_id;
    uint8_t stage_id;
    // vector of items
    // first byte is item id, second is amount?
    // first 4 items are basic and 5th is a medicine wheel drop
    std::vector<std::pair<uint8_t, uint8_t>> items_list{
        {0x00, 0x00}, {0x00, 0x00}, {0x00, 0x00}, {0x00, 0x00}};
    size_t value = 0;
} pe2_enc_reward_descriptor_t;

typedef struct rewseg {
    size_t starting_offset;
    size_t num;
    int mode;
} pe2_enc_rewards_segment_t;

extern std::map<game::pe2_level_name_t, std::pair<pe2_enc_rewards_segment_t, pe2_enc_rewards_segment_t>>
    rewards_segment_map_by_level;

bool compareByValue(const pe2_enc_reward_descriptor_t &a, const pe2_enc_reward_descriptor_t &b);

std::vector<char> rewardToByteVector(pe2_enc_reward_descriptor_t);

} // namespace domain::encounter
