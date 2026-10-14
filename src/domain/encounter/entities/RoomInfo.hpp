#pragma once

#include "utils/GeometryWorks/GeometryWorks.hpp"
#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>

typedef struct pe2rinfom {
    uint8_t id = 0x00;
    std::string name;
    std::vector<utils::geometry_works::Zone> spawn_zones;
    std::vector<utils::geometry_works::Zone> spawn_aerial_zones;
    std::vector<utils::geometry_works::Zone> spawn_water_zones;
    std::vector<utils::geometry_works::Zone> dead_zones;

    size_t encounters_offset;
    size_t encounters_end = 0x0; // Internal lists: start of last 16-byte record, inclusive.
    size_t encounters_num;
    std::vector<int> encounters_mask;
    std::vector<size_t> encounters_values;
    std::vector<size_t> encounters_pointers;
    std::vector<size_t> encounters_data_addresses;
    std::vector<uint8_t> additional_enemies;
    std::vector<uint8_t> forbidden_enemies;

    std::unordered_map<size_t, int8_t> cheat_map{};

    bool separated_zones = false;
    size_t additional_file_end_pointer = 0x0;
    uint16_t ceiling_height = 0x1c00;
} pe2rinfo_t;

extern std::vector<pe2rinfo_t> acropolis_room_infos;
extern std::vector<pe2rinfo_t> dryfield_day_room_infos;
extern std::vector<pe2rinfo_t> dryfield_night_room_infos;
extern std::vector<pe2rinfo_t> mines_and_shelter_room_infos;
extern std::vector<pe2rinfo_t> neo_ark_and_shelter_room_info;
