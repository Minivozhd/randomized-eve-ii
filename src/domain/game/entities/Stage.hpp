#pragma once

#include <stdexcept>
#include <string>

namespace domain::game {

#define __STAGE0_NAME_STR "STAGE0"
#define __STAGE1_NAME_STR "STAGE1"
#define __STAGE2_NAME_STR "STAGE2"
#define __STAGE3_NAME_STR "STAGE3"
#define __STAGE4_NAME_STR "STAGE4"
#define __STAGE5_NAME_STR "STAGE5"
#define __SLUS_NAME_STR "SLUS"

/*
    GAME STAGE NAME ENUM

    <TO BE DEFINED>
*/

typedef enum {
    STAGE0 = 0,
    STAGE1 = 1,
    STAGE2 = 2,
    STAGE3 = 3,
    STAGE4 = 4,
    STAGE5 = 5,
    SLUS = 6,
} pe2_stage_name_t;

/*
    Converts PE2_STAGE_NAME_T to a string name of the stage.

    @param STAGE_NAME - enum variable of the stage
*/
std::string getNameFromStage(pe2_stage_name_t);

} // namespace domain::game
