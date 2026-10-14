#include "domain/game/entities/Stage.hpp"

namespace domain::game {

std::string getNameFromStage(pe2_stage_name_t stage_name) {
    std::string result;
    switch (stage_name) {
    case STAGE0:
        result = __STAGE0_NAME_STR;
        break;
    case STAGE1:
        result = __STAGE1_NAME_STR;
        break;
    case STAGE2:
        result = __STAGE2_NAME_STR;
        break;
    case STAGE3:
        result = __STAGE3_NAME_STR;
        break;
    case STAGE4:
        result = __STAGE4_NAME_STR;
        break;
    case STAGE5:
        result = __STAGE5_NAME_STR;
        break;
    case SLUS:
        result = __SLUS_NAME_STR;
        break;
    default:
        throw std::invalid_argument("Invalid stage name");
    }
    return result;
}

} // namespace domain::game
