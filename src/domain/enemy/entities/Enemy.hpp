#pragma once

#include "domain/enemy/entities/EnemyTypes.hpp"
#include <bitset>
#include <cstdint>
#include <map>
#include <string>
#include <vector>

// TODO: failed stalker
// TODO: bass
// TODO: diver in every room
// TODO: all bosses

#define ENEMY_RECORD_DESCRIPTOR_SIZE 0x10   // 16 bytes structure
#define ENEMY_RESOURCE_DESCRIPTOR_SIZE 0x0c // 12 bytes structure
// Delimiter for one encounter record desciptors list
#define ENEMY_RECORD_DESCRIPTOR_LIST_DELIMITER                                                               \
    std::vector<char>{(char)0xff, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00,                \
                      (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00,                \
                      (char)0x00, (char)0x00, (char)0x00, (char)0x00}
// Delimiter for one encounter resource desciptors list
#define ENEMY_RESOURCE_DESCRIPTOR_LIST_DELIMITER                                                             \
    std::vector<char>{(char)0xff, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00,                \
                      (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00}
// Resource desciptor for dummy enemy containing additional code
#define ENEMY_DUMMY_DATA                                                                                     \
    std::vector<char>{(char)0x09, (char)0x00, (char)0xe6, (char)0x00, (char)0x02, (char)0x00,                \
                      (char)0x00, (char)0x00, (char)0x00, (char)0x94, (char)0x16, (char)0x80}

#define ENEMY_NO_SLOT 0x0
#define ENEMY_SLOT_1_ADDRESS 0x80131e20
#define ENEMY_SLOT_2_ADDRESS 0x80149e20
#define ENEMY_SLOT_3_ADDRESS 0x80161e20

#define ENEMY_HP_MULTIPLICATOR_ADDRESS 0x80790
#define ENEMY_DMG_MULTIPLICATOR_ADDRESS 0x806fa

/*
    ENEMY SIZE TYPE

    The game has some hardware restrictions in terms of rendering objects.
    (Maybe polygon limit or smth). Specific metrics and their values are yet
    unknown, however, there are some heuristic numbers:

    not more than 4-5 for normal enemies (like strangers/chasers/brain
    stingers) and about 13 for small enemies (one-time explosion of more
    than 13 sucklers can lead to crashes).
*/

typedef enum {
    ENEMY_SIZE_SMALL,   // e.g. sucklers, bats, rats...
    ENEMY_SIZE_SMALLER, // e.g. failed ANMCs
    ENEMY_SIZE_NORMAL,  // e.g. strangers, chasers, brain stingers
    ENEMY_SIZE_GIANT,   // bosses? recently unused
} pe2_enemy_size_t;

/*
    ENEMY RESOURCE TYPE

    Every enemy encounter in the game triggers the game to load the enemy file
    into the RAM at certain slot. The file contains code, model (along with its
    UVs), rig info, animations and may be some other stuff. Some enemies though
    require additional code (special fxs, like golem's blade fx), that was for
    some reason moved to the room code and really room specific code (for cloak
    golems that is their spawn points, which are obviously room specific, and
    for brain stingers it is their routing point of not agro walking).

    We moved additional resources into a dummy enemy file (that we made from
    a third slot file for the butterfly; read about file slots in ENEMY struct
    description) and modified original code so that enemy code knows where it
    is now. That enables usage of the enemies that CAN'T be in the original
    game in this room, but REQUIRE additional resources (golems and brain
    stingers), however we spoil butterfly slot 3 file making it unusuble for
    its original purposes and occupy one more slot for every enemy that
    requires additional resources.
*/

typedef enum {
    ENEMY_RES_TYPE_NORMAL,                     // basically every other enemy
    ENEMY_RES_TYPE_NEEDS_ADDITIONAL_RESOURCES, // all golems and brain stineger
} pe2_enemy_resource_t;

/*
    ENEMY MOVEMENT TYPE

    This affects the zones chozen for spawning (e.g. flying enemies can spawn
    over chasms, but land walking enemies can't).
*/

typedef enum {
    ENEMY_MOVE_GROUND, // enemies that walk on the ground
    ENEMY_MOVE_AERIAL, // flying enemies
    ENEMY_MOVE_WATER,  // only divers
} pe2_enemy_movement_t;

/*
    ENEMY AMBUSH TYPE

    Some enemy encounter records in this game have additional parameters for an
    enemy instance it spawns during the encounter. For some enemies these
    parameters allow enemies to have special ambushes. This enum just clarifies
    if an enemy has ability to ambush and it is supported by the randomizer.
*/

typedef enum {
    ENEMY_NO_AMBUSH,     // default value
    ENEMY_AERIAL_AMBUSH, // spawns enemies way higher
    ENEMY_GROUND_AMBUSH, // spawns enemies underground
} pe2_enemy_ambush_t;

/*
    ENEMY TEXTURE SIZE

    See ENEMY RECORD DESCRIPTOR - TEXTURE_VRAM_OFFSET field.
*/

typedef enum {
    ENEMY_TEX_NORMAL,
    ENEMY_TEX_BIG,
} pe2_enemy_texture_size_t;

/*
    ENEMY FILE SIZE

    See ENEMY RESOURCE DESCRIPTOR - SLOT field.
*/

typedef enum {
    ENEMY_FILE_1_SLOT = 1,
    ENEMY_FILE_2_SLOTS = 2,
    ENEMY_FILE_3_SLOTS = 3,
    ENEMY_FILE_4_SLOTS = 4,
} pe2_enemy_file_size_t;

/*
    ENEMY RECORD DESCRIPTOR

    Enemy record structure. This is a small 16-bytes structure that describes a
    specific enemy in specific encounter. It contains all the info needed for
    initing the specific enemy instance during the beginning of an encounter.
    In order to work, the list of the relevant encounters must be loaded to RAM
    BEFORE starting initing the enemies. This process starts sometime after
    loading the room file, so on entering the room AND when the encounter is
    active (or for golems, until their corpse dissapears too). Than it loads
    BOTH the enemy record structure by the address mentioned in the room file
    (usually the record is stored in the level file which this room is related
    too) AND the enemy resources structure (see ENEMY RESOURCE).

    Every encounter can have an arbitrary amount of record descriptors written
    consequently as an array wiht a special delimeter: 0xFF byte with 15
    consequent zeroes.

    So in order to change an enemy in an encounter we have to change this
    record along with ENEMY RESOURCE struct and write them to corresponding
    files.

    Let's see what this structure consists of:

    - ID - 1byte enemy record ID of an enemy. Although it is universal
    through the game it doesn't necessarily needs to be so. It is used ONLY
    for bonding the enemy record structure of the encounter with and
    corresponding resource structure, so there are no really any global IDs
    for enemies in PE2.
    - PARAM1 and PARAM2 - first and second enemy instance params. Enemy type
    specific param. This parameter's value effect differs from enemy to enemy
    so much: from defining an ambushing behavior to walking speed and
    aggressiveness.
    - PARAM3 - to be defined. Maybe just filler.
    - SPAWNING_POINT_X, SPAWNING_POINT_Y, SPAWNING_POINT_Z - spawning point
    coordinate components for an enemy instance in 3d space. Each component is
    a 2byte int.
    - ANGLE - spawning point orientation. a 2byte int. 0x0000, 0x4000, 0x8000
    and 0xc000 are sides of the world. (Don't remeber what is what here -_-)
    - TEXTURE_VRAM_OFFSET - is needed for combined encounters, because
    initially texture is loaded to the same place in VRAM, which leads to
    overwritting previous enemies texture by the new one. And while it is an
    optimal usage of VRAM when current enemy is spawned in the separate
    encounter and want to rewrite the texture of an enemy from prvious
    encounter, it is definetely should be changed if this enemy is loaded in
    the same encounter with another enemy of the different type.

    default value is usually: 0x00020000
    for second enemy it is usually: 0x02040200 or 0x04060400 if texture is
    bigger and it still overlaps the first one (e.g. golem texture).
*/

typedef struct pe2enrecd {
    uint8_t id = NO_ENEMY_ID;
    uint8_t param1 = 0x0;
    uint8_t param2 = 0x0;
    // 1 more byte. Never seen that used... Let it be always zero
    uint16_t spawning_point_x = 0x0;
    uint16_t spawning_point_y = 0x0;
    uint16_t spawning_point_z = 0x0;
    uint16_t angle = 0x0;
    uint32_t texture_vram_offset = 0x00020000;
} pe2_enemy_record_desc_t;

/*
    ENEMY RESOURCE DESCRIPTOR

    Enemy recources structure. This is a small 12-bytes structure that
    describes what resources should be loaded to RAM and where. Also it
    associates these resources with ENEMY RECORD DESCRIPTOR by defining ID.

    This structure is always stored in a room file and consists of as many
    descriptors as there are enemy TYPES in this encounter. So usually it has
    only one descriptor per encounter.

    - ID - is a specific ID to bound enemy record descriptor to this struct. It
    can be any value, but it needs to be equal to the ID specified in encounter
    recorder. However, we are trying to follow the game's way to give the same
    universal IDs to every enemy of the same type.
    - FILE_ID - is an ID of the file loaded to RAM for this encounter.
    - SLOT_VAL - Memory slot for this enemy file to get loaded in. This game
    handles multiple enemy types in such a smart way. Of course every enemy has
    its own logic, which has different functions, values and so on, which is
    stored in a executable layer at the beginning of the enemy file, loaded
    only when you enter the room with this enemy active, instead of being
    present in memory all the time like in some other games. Due to the fact
    that PS1 RAM has absolute addressing, every executable layer has to have
    different jump addresses and call function addresses for not onlt every
    enemy, but also for every enemy that is loaded to a different place in RAM.
    And that is of course the case when you have multiple types of enemies
    loaded at the same time, because obviously they can't share the same spot
    in RAM. Moreover, all nps are considered to be 'enemies' too in this game
    and therefore they are also part of encounter. And that's not all. It also
    determines where the instance of an enemy will be placed in dynamic area
    too. In some rooms this dynamic area is used for other purposes rendering
    water, rendering active fountain/escalator cut scene in akropolis,
    rendering friendly npcs like Flynt/Kyle and so on, restricting this
    instance loading. In this case if monster instance loading is forced the
    game hard crashes. That's why manipulating these slots is crutial. There
    are 4 slots available, however some enemies/npcs/specific code may occupy
    more than one slot, e.g. Brahman occupies all 4 slots.

    first slot is   0x131e20
    second slot is  0x149e20
    third slot is   0x161e20
*/

typedef struct pe2enresd {
    uint8_t id;
    // 1 more empty byte
    uint8_t file_id;
    // 1 more empty byte
    uint16_t slot_val;
    // 2 more empty bytes
    uint32_t file_table_addr;

} pe2_enemy_resource_desc_t;

/*
    ENEMY

    Main enemy entity structure for randomizer usage.

    <TO BE WRITTEN>
*/

typedef struct pe2en {
    uint8_t id;
    uint8_t file_id;
    size_t value;
    uint32_t radius;
    uint8_t ambush_value = 0;
    pe2_enemy_size_t size_type = ENEMY_SIZE_NORMAL;
    pe2_enemy_resource_t res_type = ENEMY_RES_TYPE_NORMAL;
    pe2_enemy_movement_t move_type = ENEMY_MOVE_GROUND;
    pe2_enemy_ambush_t ambush_type = ENEMY_NO_AMBUSH;
    pe2_enemy_file_size_t file_size = ENEMY_FILE_1_SLOT;
    pe2_enemy_texture_size_t texture_size = ENEMY_TEX_NORMAL;
    std::map<uint32_t, std::pair<uint16_t, uint32_t>> available_files{};
    std::vector<std::pair<float, uint8_t>> first_param{};
    std::vector<std::pair<float, uint8_t>> second_param{};
} pe2_enemy_t;

/*
    ENEMIES MAP

    Every supported for randomization enemy struct mapped to its id.
*/

const extern std::map<uint8_t, pe2_enemy_t> enemies_map;

const extern std::map<uint8_t, pe2_enemy_t> one_slot_cloak_golems_map;

/*
    ENEMIES IDS BY SLOT MAP
*/

extern std::map<uint32_t, std::vector<uint8_t>> enemy_ids_by_slot_map;

/*
    ENEMIES ALL SUPPORTED ENEMIES IDS VECTOR
*/

extern std::vector<uint8_t> enemy_ids_vector;

/*
    ENEMIES BY SIZE MAP
*/

const extern std::map<pe2_enemy_size_t, std::vector<uint8_t>> common_enemies_by_size_map;

extern std::vector<uint8_t> special_enemies;

const extern std::map<pe2_enemy_file_size_t, std::vector<uint8_t>> enemies_by_file_size;

std::vector<char> recordDescriptorToBytesVector(pe2_enemy_record_desc_t);
std::vector<char> resourceDescriptorToBytesVector(pe2_enemy_resource_desc_t);
std::string getEnemyName(uint8_t);
