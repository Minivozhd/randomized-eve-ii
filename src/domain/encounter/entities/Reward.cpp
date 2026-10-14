#include "domain/encounter/entities/Reward.hpp"

namespace domain::encounter {

std::map<game::pe2_level_name_t, std::pair<pe2_enc_rewards_segment_t, pe2_enc_rewards_segment_t>>
    rewards_segment_map_by_level{
        {game::AKROPOLIS_LEVEL,
         {// Normal Replay Scavanger
          pe2_enc_rewards_segment_t{.starting_offset = 0x278c,
                                    .num = 0xb,
                                    .mode = game::MODE_NORMAL | game::MODE_REPLAY | game::MODE_SCAVENGER},
          // Bounty Nightmare
          pe2_enc_rewards_segment_t{
              .starting_offset = 0x281c, .num = 0x5, .mode = game::MODE_BOUNTY | game::MODE_NIGTHMARE}}},
        {game::DRYFIELD_DAY_LEVEL,
         {// Normal Replay Scavanger
          pe2_enc_rewards_segment_t{.starting_offset = 0x2394,
                                    .num = 0xc,
                                    .mode = game::MODE_NORMAL | game::MODE_REPLAY | game::MODE_SCAVENGER},
          // Bounty Nightmare
          pe2_enc_rewards_segment_t{
              .starting_offset = 0x2430, .num = 0x7, .mode = game::MODE_BOUNTY | game::MODE_NIGTHMARE}}},
        {game::DRYFIELD_NIGHT_LEVEL,
         {// Normal Replay Scavanger
          pe2_enc_rewards_segment_t{.starting_offset = 0x3768,
                                    .num = 0x16,
                                    .mode = game::MODE_NORMAL | game::MODE_REPLAY | game::MODE_SCAVENGER},
          // Bounty Nightmare
          pe2_enc_rewards_segment_t{
              .starting_offset = 0x387c, .num = 0x8, .mode = game::MODE_BOUNTY | game::MODE_NIGTHMARE}}},
        {game::MINES_AND_SHELTER_LEVEL,
         {// Normal Replay Scavanger
          pe2_enc_rewards_segment_t{.starting_offset = 0x2208,
                                    .num = 0x2e,
                                    .mode = game::MODE_NORMAL | game::MODE_REPLAY | game::MODE_SCAVENGER},
          // Bounty Nightmare
          pe2_enc_rewards_segment_t{
              .starting_offset = 0x243c, .num = 0xc, .mode = game::MODE_BOUNTY | game::MODE_NIGTHMARE}}},
        {game::NEOARK_AND_SHELTER_LEVEL,
         {// Normal Replay Scavanger
          pe2_enc_rewards_segment_t{.starting_offset = 0x3060,
                                    .num = 0x1c,
                                    .mode = game::MODE_NORMAL | game::MODE_REPLAY | game::MODE_SCAVENGER},
          // Bounty Nightmare
          pe2_enc_rewards_segment_t{
              .starting_offset = 0x31bc, .num = 0x5, .mode = game::MODE_BOUNTY | game::MODE_NIGTHMARE}}}};

bool compareByValue(const pe2_enc_reward_descriptor_t &a, const pe2_enc_reward_descriptor_t &b) {
    return a.value > b.value;
}

std::vector<char> rewardToByteVector(pe2_enc_reward_descriptor_t desc) {
    return {
        0x00,
        static_cast<char>(desc.encounter_id),
        static_cast<char>(desc.room_id),
        static_cast<char>(desc.stage_id),
        static_cast<char>(desc.items_list[0].first),
        static_cast<char>(desc.items_list[0].second),
        static_cast<char>(desc.items_list[1].first),
        static_cast<char>(desc.items_list[1].second),
        static_cast<char>(desc.items_list[2].first),
        static_cast<char>(desc.items_list[2].second),
        static_cast<char>(desc.items_list[3].first),
        static_cast<char>(desc.items_list[3].second),
    };
}

} // namespace domain::encounter
