#include "domain/game/entities/Level.hpp"

namespace domain::game {

std::string getNameFromLevel(pe2_level_name_t level_name) {
    switch (level_name) {
    case BASE_LEVEL:
        return __BASE_NAME_STR;
    case AKROPOLIS_LEVEL:
        return __AKROPOLIS_NAME_STR;
    case DRYFIELD_DAY_LEVEL:
        return __DRYFIELD_DAY_NAME_STR;
    case DRYFIELD_NIGHT_LEVEL:
        return __DRYFIELD_NIGHT_NAME_STR;
    case MINES_AND_SHELTER_LEVEL:
        return __MINES_AND_SHELTER_NAME_STR;
    case NEOARK_AND_SHELTER_LEVEL:
        return __NEOARK_AND_SHELTER_NAME_STR;
    default:
        throw std::runtime_error("Couldn't resolve level name");
    }
}

pe2_level_name_t getLevelFromStage(pe2_stage_name_t level_name) {
    switch (level_name) {
    case STAGE0:
        return BASE_LEVEL;
    case STAGE1:
        return AKROPOLIS_LEVEL;
    case STAGE2:
        return DRYFIELD_DAY_LEVEL;
    case STAGE3:
        return DRYFIELD_NIGHT_LEVEL;
    case STAGE4:
        return MINES_AND_SHELTER_LEVEL;
    case STAGE5:
        return NEOARK_AND_SHELTER_LEVEL;
    default:
        throw std::runtime_error("Couldn't cast stage name to level name");
    }
}

} // namespace domain::game
