#pragma once

#include "domain/file/entities/FileTypes.hpp"
#include "domain/game/entities/Stage.hpp"
#include <string>
#include <vector>

#define STAGE_NUMS 5
#define STAGE_BASE "STAGE"
#define STAGE_FORMAT ".CDF"
#define ROOM_DECOMPRESSED_FORMAT ".pe2rml.decompr"
#define DECOMPRESSED_FORMAT ".pe2lvl.decompr"
#define STAGE0_FOLDER "STAGE0/"

namespace domain::file {
typedef enum {
    LEVEL,
    ROOM_LOGIC,
    ENEMY_LOGIC,
} pe2file_enum;

typedef enum {
    LEVEL_G = 0,
    ACROPOLIS_ROOM_G = 1,
    DRYFIELD_DAY_ROOM_G = 2,
    DRYFIELD_NIGHT_ROOM_G = 3,
    MINES_AND_UPPER_SHELTER_ROOM_G = 4,
    NEO_ARK_AND_LOWER_SHELTER_ROOM_G = 5,
    ENEMY_G = 6,
} pe2file_group_enum;

// Use domain::game::pe2_stage_name_t for stage enum

typedef struct pe2file {
    std::string filename;
    bool is_segmented;
    bool is_compressed;
    int file_offset;
    int file_end_offset;
    pe2file_enum type;
    pe2file_group_enum group;
} pe2file_t;

typedef struct pe2stage {
    std::string filename;
    game::pe2_stage_name_t stage;
    std::vector<pe2file_t> inner_files;
    std::vector<char> starting_signature;
    std::vector<char> ending_signature;
    std::vector<pe2file_group_enum> group_tags;
} pe2stage_t;

extern const std::vector<pe2file_t> stage0_files_info;
extern const std::vector<pe2file_t> stage1_files_info;
extern const std::vector<pe2file_t> stage2_files_info;
extern const std::vector<pe2file_t> stage3_files_info;
extern const std::vector<pe2file_t> stage4_files_info;
extern const std::vector<pe2file_t> stage5_files_info;

extern const std::vector<pe2file_t> stage0_special_files_info;
extern const std::vector<pe2file_t> stage0_enemies_info;
extern const std::vector<pe2file_t> stage0_bosses_info;

extern const std::vector<char> start_signature_stage0;
extern const std::vector<char> end_signature_stage0;
extern const std::vector<char> start_signature_stage1;
extern const std::vector<char> end_signature_stage1;
extern const std::vector<char> start_signature_stage2;
extern const std::vector<char> end_signature_stage2;
extern const std::vector<char> start_signature_stage3;
extern const std::vector<char> end_signature_stage3;
extern const std::vector<char> start_signature_stage4;
extern const std::vector<char> end_signature_stage4;
extern const std::vector<char> start_signature_stage5;
extern const std::vector<char> end_signature_stage5;
extern const std::vector<char> start_signature_slus;
extern const std::vector<char> end_signature_slus;

extern const std::vector<pe2stage_t> stages;

std::string getStageFileName(pe2file_group_enum group);
std::string getLevelName(pe2file_group_enum group);
} // namespace domain::file
