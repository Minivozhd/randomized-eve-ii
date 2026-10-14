#pragma once

#include "domain/game/entities/Level.hpp"
#include <algorithm>
#include <cstdint>
#include <map>
#include <vector>

namespace domain::music {

/*
    AVAILABLE MUSIC TYPES
*/
typedef enum {
    MUSIC_PEACEFUL,
    MUSIC_EXPLORING,
    MUSIC_INTENSE,
    MUSIC_FIGHT,
    MUSIC_MISC,
} pe2_music_type_t;

/*
    MUSIC ENTRY STRUCT
*/
typedef struct {
    std::vector<size_t> offsets;
    pe2_music_type_t type;
    std::vector<uint8_t> forbidden_music;
} pe2_music_entry_t;

extern std::map<game::pe2_level_name_t, std::vector<pe2_music_entry_t>> music_entries_map;
extern std::map<pe2_music_type_t, std::vector<uint8_t>> music_types_map;

} // namespace domain::music
