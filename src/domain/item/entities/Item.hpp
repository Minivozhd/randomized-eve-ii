#pragma once

#include "domain/game/entities/Level.hpp"
#include <algorithm>
#include <cstdint>
#include <map>
#include <string>
#include <vector>

namespace domain::item {

#define ITEM_COST_START_ADDRESS 0x7a040

typedef enum {
    COMMON = 1,
    RARE = 2,
    LEGENDARY = 3,
    IMPOSSIBLE = 4,
} pe2_item_rarity_t;

typedef enum {
    HP_RECOVERY = 1,
    MP_RECOVERY = 2,
    AMMO = 3,
    MISC = 4,
    UNIQUE = 5,
} pe2_item_type_t;

typedef enum {
    CONSUMABLE,
    ARMOR,
    ATTACHMENT,
    WEAPON,
    ARTIFACT,
    MAGIC,
} pe2_item_class_t;

typedef enum {
    PICKUP_DEFAULT,
    PICKUP_SECRET,
    REWARD,
} pe2_item_pickup_t;

typedef enum {
    NONE,
    P08,
    M4A1,
    MP5AP5,
    PISTOL,
    SHOTGUN,
    RIFLE,
    GRENADE,
    MAGNUM,
} pe2_item_tag_t;

typedef struct item {
    uint8_t id;
    pe2_item_type_t type = MISC;
    pe2_item_rarity_t rarity = COMMON;
    pe2_item_class_t item_class = CONSUMABLE;
    pe2_item_tag_t main_tag = NONE;
    pe2_item_tag_t additional_tag = NONE;
    size_t cost = 0;
    size_t value = 0;
} pe2_item_t;

typedef enum {
    NRB,
    SN,
} pe2_shop_type_t;

typedef struct shop {
    pe2_shop_type_t type;
    uint8_t weapon;
    uint8_t ammo = 0;
    uint8_t armor;
    uint8_t item;
    std::vector<std::string> location{};
    std::vector<size_t> offset{};
    bool has_charger = false;
    std::vector<std::vector<bool>> has_gap{};
} pe2_shop_t;

extern std::map<uint8_t, pe2_item_t> items_map;
extern std::map<uint8_t, pe2_item_t> elements_map;
extern std::map<pe2_item_type_t, std::map<pe2_item_rarity_t, std::vector<uint8_t>>> item_map_by_type;
extern std::map<pe2_item_tag_t, std::map<pe2_item_rarity_t, std::vector<uint8_t>>> weapon_ammo_allowance_map;
extern std::map<pe2_item_tag_t, std::map<pe2_item_rarity_t, std::vector<uint8_t>>>
    weapon_attachment_allowance_map;
extern std::map<pe2_item_type_t, double> item_types_probability_map;
extern std::map<pe2_item_rarity_t, double> item_rarity_probability_map;
extern std::map<pe2_item_rarity_t, double> secret_item_rarity_probability_map;
extern std::map<pe2_item_rarity_t, double> reward_item_rarity_probability_map;
extern std::vector<uint8_t> weapons_ids;
extern std::vector<std::pair<size_t, pe2_item_pickup_t>> acropolis_pickup_items_map;
extern std::vector<std::pair<size_t, pe2_item_pickup_t>> acropolis_shooting_range_rewards_map;
extern std::vector<std::pair<size_t, pe2_item_pickup_t>> dryfield_motel_room_6_box;
extern std::vector<std::pair<size_t, pe2_item_pickup_t>> dryfield_day_items_pickup_dict;
extern std::vector<std::pair<size_t, pe2_item_pickup_t>> dryfield_night_items_pickup_dict;
extern std::vector<std::pair<size_t, pe2_item_pickup_t>> dryfield_night_return_items_pickup_dict;
extern std::vector<std::pair<size_t, pe2_item_pickup_t>> mines_and_shelter_sterilization_room_box;
extern std::vector<std::pair<size_t, pe2_item_pickup_t>> mines_and_shelter_pickup_items_dict;
extern std::vector<std::pair<size_t, pe2_item_pickup_t>> neo_ark_and_shelter_pickup_items_dict;
extern std::vector<uint8_t> starting_unique_items;
extern std::vector<uint8_t> starting_rare_items_id;
extern std::vector<uint8_t> starting_armors;
extern std::vector<uint8_t> all_armors;
extern std::vector<uint8_t> starting_weapons;
extern std::vector<uint8_t> all_weapons;
extern std::vector<uint8_t> all_weapons_and_attachments;
extern std::vector<uint8_t> all_misc_items;
extern std::vector<uint8_t> all_ammo;
extern std::map<pe2_item_rarity_t, std::vector<uint8_t>> magic;
extern std::vector<uint8_t> all_magic;
extern std::vector<uint8_t> starting_magic;
extern std::vector<pe2_shop_t> shops_vector;
extern std::map<size_t, size_t> new_costs;
extern std::map<game::pe2_level_name_t, std::vector<size_t>> ammo_boxes;

} // namespace domain::item
