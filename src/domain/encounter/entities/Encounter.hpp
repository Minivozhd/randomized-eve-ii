#pragma once

#include "domain/game/entities/Level.hpp"
#include "domain/game/entities/Room.hpp"
#include "utils/GeometryWorks/GeometryWorks.hpp"
#include <cstdint>
#include <map>
#include <string>
#include <vector>

namespace domain::encounter {

#define AKROPOLIS_ENCOUNTERS_START 0x136c
#define AKROPOLIS_ENCOUNTERS_END 0x242c
#define DRYFIELD_DAY_ENCOUNTERS_START 0x13f4
#define DRYFIELD_DAY_ENCOUNTERS_END 0x2394
#define DRYFIELD_NIGHT_ENCOUNTERS_START 0x12f8
#define DRYFIELD_NIGHT_ENCOUNTERS_END 0x3758
#define NEO_ARK_ENCOUNTERS_START 0x1580
#define NEO_ARK_ENCOUNTERS_END 0x2e40

/*
    ENCOUNTER MASK
*/
typedef enum {
    RANDOMIZED = 0,
    UNRANDOMIZED = -1,
    FORBID_SLOT_2_ENEMIES = -2,
    FORBID_SLOT_3_ENEMIES = -3,
    FORBID_ALL_SLOTS_ENC = -4,
    FORBID_MULTI_ENEMY_ENC = -5,
    FORBID_MULTI_ENEMY_ENC_AND_SLOT_3 = -6
} pe2_encounter_mask_t;

/*
    ENCOUNTER
*/
typedef struct pe2enc {
    uint8_t id;
    pe2_encounter_mask_t mask;
    size_t encounter_value;
    size_t enemy_record_descriptor_list_addr_offset;
    size_t enemy_resources_desciptors_list_offset;
    size_t force_reward_value = 0;
    bool need_to_skip_encoutners = false;
} pe2_encounter_t;

/*
    ROOM ENVIRONMENT
*/
typedef struct pe2renv {
    bool separated_zones = false;
    uint16_t ceiling_height = 0x1c00;
    std::vector<utils::geometry_works::Zone> spawn_zones{};
    std::vector<utils::geometry_works::Zone> spawn_aerial_zones{};
    std::vector<utils::geometry_works::Zone> spawn_water_zones{};
    std::vector<utils::geometry_works::Zone> dead_zones{};
} pe2_room_environment_t;

/*
    ROOM ENCOUNTERS
*/
typedef struct pe2rencs {
    uint8_t id = 0x0;
    game::pe2_room_name_t name;
    size_t encounters_offset;
    size_t encounters_end = 0x0; // Internal lists: start of last 16-byte record, inclusive.
    size_t additional_file_end_pointer;
    pe2_room_environment_t environment;
    bool forbid_ambush_encounters = false;
    std::vector<pe2_encounter_t> encounters;
    std::vector<uint8_t> allowed_additional_enemies{};
    std::vector<uint8_t> forbidden_enemies{};
    std::map<size_t, uint8_t> file_patch_map;
} pe2_room_encounters_t;

typedef struct pe2encls {
    size_t from = 0x0;
    size_t to = 0x0;
    bool internal = false;
} pe2_encounters_list_segment_t;

typedef enum {
    ENCOUNTER_TYPE_MONO,
    ENCOUNTER_TYPE_EVEN,
    ENCOUNTER_TYPE_FIRST_PREVAILS,
    ENCOUNTER_TYPE_SECOND_PREVAILS,
} pe2_encounter_type_t;

extern std::map<game::pe2_level_name_t, std::vector<pe2_room_encounters_t>> encounters_per_level_map;
extern std::map<game::pe2_level_name_t, pe2_encounters_list_segment_t> encounters_segment_per_level_map;

} // namespace domain::encounter
