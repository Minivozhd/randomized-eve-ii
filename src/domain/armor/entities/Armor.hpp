#pragma once

#include "domain/item/entities/Item.hpp"
#include <cstdint>
#include <map>
#include <string>
#include <vector>

namespace domain::armor {

inline constexpr std::uint32_t ARMOR_STATS_LIST_OFFSET = 0x7aab8u;
inline constexpr std::uint32_t ARMOR_LABELS_LIST_START_OFFSET = 0x1970u;
inline constexpr std::uint32_t ARMOR_LABELS_LIST_END_OFFSET = 0x1ddcu;
inline constexpr std::uint32_t ARMOR_LABELS_ADDRESSES_OFFSET = 0x7a33cu;

// Status effects
inline constexpr std::uint32_t NO_STATUS_EFFECTS = 0x00000000u;
inline constexpr std::uint32_t RESIST_BLIND = 0x01000000u;
inline constexpr std::uint32_t RESIST_POISON = 0x02000000u;
inline constexpr std::uint32_t MP_GENERATION = 0x04000000u;
inline constexpr std::uint32_t QUICK_FIRE = 0x08000000u;
inline constexpr std::uint32_t RESIST_IMPACT = 0x10000000u;
inline constexpr std::uint32_t RESIST_SILENCE = 0x20000000u;
inline constexpr std::uint32_t MEDICAL_INSPECTION = 0x40000000u;
inline constexpr std::uint32_t MP_RECOVERY = 0x80000000u;
inline constexpr std::uint32_t HP_RECOVERY = 0x00010000u;
inline constexpr std::uint32_t RESIST_CONFUSION = 0x00020000u;
inline constexpr std::uint32_t RESIST_PARALYSIS = 0x00040000u;
inline constexpr std::uint32_t RESIST_BERSERK = 0x00080000u;
inline constexpr std::uint32_t RESIST_ALL = 0x00100000u;
inline constexpr std::uint32_t WEAPON_BOOST = 0x00200000u;
inline constexpr std::uint32_t RESIST_DAMAGE = 0x00400000u;
inline constexpr std::uint32_t PE_BOOST = 0x00800000u;
inline constexpr std::uint32_t DISCOVERY_ITEM = 0x00000100u;
inline constexpr std::uint32_t MOTION_DETECTOR = 0x00000200u;
inline constexpr std::uint32_t HEAVY_ARMOR = 0x00000400u;
inline constexpr std::uint32_t HP_DRAIN = 0x00000800u;
inline constexpr std::uint32_t BP_BOOST = 0x00001000u;
inline constexpr std::uint32_t CRITS_UP = 0x00002000u;
inline constexpr std::uint32_t EASE = 0x00004000u;
inline constexpr std::uint32_t RAPID_MAGIC = 0x00008000u;
inline constexpr std::uint32_t TONFA_BOOST = 0x00000001u;
inline constexpr std::uint32_t MUTE = 0x00000002u;
inline constexpr std::uint32_t FIRE_BOOST = 0x00000004u;
inline constexpr std::uint32_t NECROSIS_BOOST = 0x00000010u;

// Additional effects
inline constexpr std::uint32_t WATER_BOOST = 0x00000004u;
inline constexpr std::uint32_t EARTH_BOOST = 0x00000004u;
inline constexpr std::uint32_t HEAL_DOWN = 0x00000004u;
inline constexpr std::uint32_t WEAK_ARMS = 0x00200000u;
inline constexpr std::uint32_t VULNERABILITY = 0x00400000u;
inline constexpr std::uint32_t WEAK_PE = 0x00800000u;
inline constexpr std::uint32_t BP_LOSS = 0x00001000u;
inline constexpr std::uint32_t THIRST = 0x04000000u;
inline constexpr std::uint32_t MAGNUM_BOOST = 0x00000001u;
inline constexpr std::uint32_t HAMMER_BOOST = 0x00000001u;
inline constexpr std::uint32_t M4A1_BOOST = 0x00000001u;
inline constexpr std::uint32_t FREE_ESCAPE = 0x00000001u;
inline constexpr std::uint32_t MP_DRAIN = 0x00000800u;

extern std::map<uint32_t, std::vector<uint8_t>> new_armor_patch_map;
extern std::map<uint32_t, std::vector<uint8_t>> new_armor_patch_map_slus;

typedef struct {
    item::pe2_item_rarity_t rarity;
    int16_t hp_bonus;
    uint8_t mp_bonus;
    uint8_t attachments;
    uint32_t status_effects;
    uint32_t cost;
    std::string image_file;
    std::string name;
    std::string description;
} pe2_armor_t;

extern std::map<item::pe2_item_rarity_t, std::vector<pe2_armor_t>> armors_pool;

typedef struct {
    item::pe2_item_rarity_t rarity;
    uint8_t default_hp_bonus;
    uint8_t default_mp_bonus;
    uint8_t default_attachments;
    uint32_t default_status_effects;
    std::string default_name;
    std::string default_description;
    uint32_t palette_offset;
    uint32_t texture_offset;
    uint32_t special_features_text_offset;
    uint32_t cost_offset;
} pe2_armor_slot_t;

extern std::map<uint8_t, pe2_armor_slot_t> armor_slots_map;
extern std::map<item::pe2_item_rarity_t, double> armor_prob_map;
extern std::map<size_t, std::string> special_features_map;

} // namespace domain::armor
