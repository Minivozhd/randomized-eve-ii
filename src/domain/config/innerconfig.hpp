#pragma once

#include "domain/file/entities/PE2File.hpp"
#include "domain/item/entities/Item.hpp"
#include "domain/randomizer/entities/Strategy.hpp"
#include <algorithm>
#include <map>
#include <string>
#include <vector>

namespace domain {
typedef struct inner_config {
    struct Flags {
        bool randomize_enemies = false;
        bool randomize_items = false;
        bool randomize_shops = false;
        bool randomize_music = false;
        bool randomize_magic = false;
        bool active_time_battle = false;
        bool enhanced_bosses = false;
        bool full_ammo = false;
        bool cloak_golem_mult = false;
        bool enable_hud_coloring = false;
        bool enable_special_modes = false;
        bool enable_map_in_fight = false;
        bool additional_armors = false;
        bool magic_pickup_mode = false;
        bool increased_inv = false;
        bool no_white_screen = false;
        bool no_win_pose = false;
        bool no_turn = false;
        bool inventory_mode = false;
    } flags;
    int ammo_boxes_option = 0;
    int ammo_boxes_rando_option = 0;
    int enemy_enhancement_option = 0;
    std::vector<uint8_t> randomized_stages = {};
    std::vector<uint8_t> randomized_enemies_pool = {};
    std::vector<uint8_t> randomized_items_drop_pool = {};
    std::vector<uint8_t> randomized_items_shop_pool = {};
    std::vector<uint8_t> randomized_music_pool = {};
    std::vector<uint16_t> shop_costs = {};
    std::vector<file::pe2file_group_enum> unpacking_groups = {};
    std::vector<file::pe2file_enum> unpacking_types = {};
    uint8_t starting_spell = 0;
    uint8_t starting_spell_level = 0;
    uint8_t starting_weapon = 0;
    uint8_t starting_armor = 0;
    uint8_t dryfield_armor = 0;
    pe2_randomizer_magic_strategy_t spell_randomization_strategy = RANDO_STRATEGY_MAGIC_STARTING;
    pe2_randomizer_strategy_t weapon_randomization_strategy = RANDO_STRATEGY_STARTING;
    pe2_randomizer_strategy_t armor_randomization_strategy = RANDO_STRATEGY_STARTING;
    pe2_randomizer_strategy_t dryfield_armor_randomization_strategy = RANDO_STRATEGY_STARTING;
    float shop_sost_multiplier = 0.0f;
    float spell_cost_multiplier = 0.0f;
    std::map<item::pe2_item_type_t, double> item_classes_distribution = {
        {item::HP_RECOVERY, 0.1622}, {item::MP_RECOVERY, 0.1667}, {item::AMMO, 0.4405},
        {item::MISC, 0.1485},        {item::UNIQUE, 0.0721},
    };
    uint16_t enemy_damage = 100;
    uint16_t enemy_hp = 100;
    uint16_t bosses_damage = 100;
    uint16_t bosses_hp = 100;
    uint8_t player_model_option = 0;
    uint8_t hud_color_r = 0;
    uint8_t hud_color_g = 0;
    uint8_t hud_color_b = 0;
    size_t randomize_seed = 0;
    std::string input_file_1 = "";
    std::string input_file_2 = "";
    std::string randomization_state_string = "";
    int randomize_category = 0; // 0 - NRS and 1 - BN
    bool pe1_music = false;
    bool no_magic = false;
} inner_config_t;
} // namespace domain
