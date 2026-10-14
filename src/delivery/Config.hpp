#pragma once

/*
    DELIVERY CONFIG MODULE

    This module defines the configuration structures and enums used by the delivery
    layer. It provides the interface between the UI layer and the domain layer,
    containing all user-configurable options for randomization including flags for
    different randomization features, enemy/item pools, cost multipliers, starting
    equipment, and various gameplay modifiers. The configuration is converted to
    domain-specific inner configuration format before being passed to domain layer
    components.
*/

#include <algorithm>
#include <cstdint>
#include <string>
#include <vector>

namespace delivery {
/*
    AMMO BOXES OPTION

    Defines how ammo boxes behave in the randomized game.

    DEFAULT - ammo boxes work as in the original game
    NOAMMOBOXES - ammo boxes are removed from the game
    ONETIME - ammo boxes can only be used once
*/
enum class AmmoBoxesOption : uint8_t {
    Default = 0,
    NoAmmoBoxes = 1,
    OneTime = 2,
};

/*
    AMMO BOXES RANDOMIZATION

    Defines the randomization strategy for ammo boxes.

    DEFAULT - no randomization of ammo boxes
    RANDOMIZEDBOXES - ammo boxes are randomized but they provide ammo only for the weapons dropped
    FULLRANDOMBOXES - ammo boxes are fully randomized
*/
enum class AmmoBoxesRandomization : uint8_t {
    Default = 0,
    RandomizedBoxes = 1,
    FullRandomBoxes = 2,
};

/*
    ENEMY ENHANCEMENT OPTION

    Defines how enemy enhancements are applied during randomization.

    DEFAULT - no enemy enhancements
    ENHANCEALLENEMIES - all enemies are enhanced
    INCLUDEENHANCED - include enhanced enemies in the pool as new enemy types
    ENHANCEDISC2ENEMIES - only enhance enemies from disc 2 for more harder difficult curve

    NOTE: NOT IMPLEMENTED!!!
*/
enum class EnemyEnhancementOption : uint8_t {
    Default = 0,
    EnhanceAllEnemies = 1,
    IncludeEnhanced = 2,
    EnhanceDisc2Enemies = 3,
};

/*
    RANDOMIZE CATEGORY

    Defines the replay mode.

    NRS - NORMAL, REPLAY, SCAVENGER
    BN - BOUNTY, NIGHTMARE
*/
enum class RandomizeCategory : uint8_t {
    NRS = 0,
    BN = 1,
};

/*
    FLAGS STRUCTURE

    Contains boolean flags for various randomization features and gameplay
    modifiers. Each flag enables or disables a specific randomization feature
    or game modification.

    RANDOMIZE_ENEMIES - enables enemy randomization
    RANDOMIZE_ITEMS - enables item randomization
    RANDOMIZE_SHOPS - enables shop item randomization
    RANDOMIZE_MUSIC - enables music randomization
    RANDOMIZE_MAGIC - enables magic/spell randomization
    ENHANCED_BOSSES - enables boss enhancements
    FULL_AMMO - enables starting with full ammo
    CLOAK_GOLEM_MULT - enables cloak golem multiple enemy type encounters
    ENABLE_HUD_COLORING - enables HUD color customization
    ENABLE_SPECIAL_MODES - enables special game replay modes (repaly, bounty, scavanger, nightmare)
    ENABLE_MAP_IN_FIGHT - enables map display during combat (laser glitch workaround)
    ADDITIONAL_ARMORS - enables additional armor options (police shirt, medical gown, collar shirt, etc.)
    MAGIC_PICKUP_MODE - enables magic pickup mode
    ACTIVE_TIME_BATTLE - enables active time battle system
    INCREASED_INV - enables increased invulnerability frames on entering any room,
                    in order to avoid getting stuck because of exploding enemies spawning
                    to close to the player (not perfect solution, but works)
    NO_WHITE_SCREEN - disables white screen flashes before entering an encounter
    NO_WIN_POSE - disables win pose animations after defeating an encounter
    NO_TURN - disables auto turning while aiming
    INVENTORY_MODE - enables special inventory mode
*/
struct Flags {
    bool randomize_enemies = false;
    bool randomize_items = false;
    bool randomize_shops = false;
    bool randomize_music = false;
    bool randomize_magic = false;
    bool enhanced_bosses = false;
    bool full_ammo = false;
    bool cloak_golem_mult = false;
    bool enable_hud_coloring = false;
    bool enable_special_modes = false;
    bool enable_map_in_fight = false;
    bool additional_armors = false;
    bool magic_pickup_mode = false;
    bool active_time_battle = false;
    bool increased_inv = false;
    bool no_white_screen = false;
    bool no_win_pose = false;
    bool no_turn = false;
    bool inventory_mode = false;
};

/*
    CONFIG STRUCTURE

    Main configuration structure containing all settings for randomization.
    This structure is passed from the UI layer to the delivery layer and
    contains all user-configurable options including flags, pools, multipliers,
    starting equipment, and file paths.

    FLAGS - structure containing boolean flags for features
    AMMO_BOXES_OPTION - option for ammo box behavior
    AMMO_BOXES_RANDO_OPTION - option for ammo box randomization
    ENEMY_ENHANCEMENT_OPTION - option for enemy enhancements (NOT IMPLEMENTED!!!)
    RANDOMIZED_STAGES - list of stage IDs to randomize (0-5)
    RANDOMIZED_ENEMIES_POOL - list of enemy IDs allowed in randomization
    RANDOMIZED_ITEMS_DROP_POOL - list of item IDs allowed for enemy drops
    RANDOMIZED_ITEMS_SHOP_POOL - list of item IDs allowed in shops
    RANDOMIZED_MUSIC_POOL - list of music track IDs allowed
    SHOP_COSTS - custom shop costs for items
    STARTING_SPELL - initial spell ID for the player
    STARTING_SPELL_LEVEL - initial level for the starting spell
    STARTING_WEAPON - initial weapon ID for the player
    STARTING_ARMOR - initial armor ID for the player
    DRYFIELD_ARMOR - armor ID for dryfield area
    SPELL_RANDOMIZATION_STRATEGY - strategy for spell randomization
    WEAPON_RANDOMIZATION_STRATEGY - strategy for weapon randomization
    ARMOR_RANDOMIZATION_STRATEGY - strategy for armor randomization
    DRYFIELD_ARMOR_RANDOMIZATION_STRATEGY - strategy for dryfield armor
    SHOP_COST_MULTIPLIER - multiplier for shop item costs
    SPELL_COST_MULTIPLIER - multiplier for spell costs
    ITEM_CLASSES_DISTRIBUTION - probability distribution for item classes
    ENEMY_DAMAGE - damage multiplier for enemies (percentage)
    ENEMY_HP - HP multiplier for enemies (percentage)
    BOSS_DAMAGE - damage multiplier for bosses (percentage)
    BOSS_HP - HP multiplier for bosses (percentage)
    PLAYER_MODEL_OPTION - option for player model selection
    HUD_COLOR_R - red component of HUD color
    HUD_COLOR_G - green component of HUD color
    HUD_COLOR_B - blue component of HUD color
    SEED - random seed for randomization
    INPUT_FILE_1 - path to first ROM binary file
    INPUT_FILE_2 - path to second ROM binary file
    RANDOMIZATION_STATE_STRING - serialized state string for randomization
    RANDOMIZE_CATEGORY - category of randomization (NRS or BN)
*/
typedef struct config {
    Flags flags{};
    AmmoBoxesOption ammo_boxes_option = AmmoBoxesOption::Default;
    AmmoBoxesRandomization ammo_boxes_rando_option = AmmoBoxesRandomization::Default;
    EnemyEnhancementOption enemy_enhancement_option = EnemyEnhancementOption::Default;
    std::vector<uint8_t> randomized_stages{0, 1, 2, 3, 4, 5};
    std::vector<uint8_t> randomized_enemies_pool{};
    std::vector<uint8_t> randomized_items_drop_pool{};
    std::vector<uint8_t> randomized_items_shop_pool{};
    std::vector<uint8_t> randomized_music_pool{};
    std::vector<uint16_t> shop_costs;
    uint8_t starting_spell = 0;
    uint8_t starting_spell_level = 0;
    uint8_t starting_weapon = 0;
    uint8_t starting_armor = 0;
    uint8_t dryfield_armor = 0;
    uint8_t spell_randomization_strategy = 0;
    uint8_t weapon_randomization_strategy = 0;
    uint8_t armor_randomization_strategy = 0;
    uint8_t dryfield_armor_randomization_strategy = 0;
    float shop_cost_multiplier = 0.0f;
    float spell_cost_multiplier = 0.0f;
    std::vector<double> item_classes_distribution = {0.16, 0.16, 0.45, 0.15, 0.08};
    uint16_t enemy_damage;
    uint16_t enemy_hp;
    uint16_t boss_damage;
    uint16_t boss_hp;
    uint8_t player_model_option;
    uint8_t hud_color_r = 0;
    uint8_t hud_color_g = 0;
    uint8_t hud_color_b = 0;
    size_t seed = 0;
    std::string input_file_1 = "";
    std::string input_file_2 = "";
    std::string randomization_state_string = "";
    RandomizeCategory randomize_category = RandomizeCategory::NRS; // 0 - NRS and 1 - BN
    bool boss_prototype = false; // Diagnostic Stranger -> Secret Passage, not the full boss pool.
    bool stranger_row = false;   // Diagnostic authored transfers; not a released boss pool.
    bool debug_weapons = false;  // Local test profile; intentionally outside release seed format.
    bool rupert_revolver = false;
    bool kyle_p229 = false;
} config_t;
} // namespace delivery
