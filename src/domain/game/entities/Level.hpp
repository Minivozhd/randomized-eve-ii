#pragma once

#include "domain/game/entities/Stage.hpp"
#include <stdexcept>
#include <string>

namespace domain::game {

#define LEVEL_INFO_FILE_LOAD_ADDRESS 0x80179950

#define __BASE_NAME_STR "00_base"
#define __AKROPOLIS_NAME_STR "01_akropolis"
#define __DRYFIELD_DAY_NAME_STR "02_dryfield_day"
#define __DRYFIELD_NIGHT_NAME_STR "03_dryfield_night"
#define __MINES_AND_SHELTER_NAME_STR "04_mines_and_shelter"
#define __NEOARK_AND_SHELTER_NAME_STR "05_neo_ark_and_shelter"

/*
    GAME LEVEL NAME ENUM

    Although the game is linear and looks seamless, it has a level separation
    within. The game basically consists of five levels, that have a specific
    file loaded when you enter certain points. Every level has its own STAGE
    file for only this level's resources (such as BGs, sounds, room's logic
    code and so on). The levels are:

    1. Acropolis and MIST garage    (loaded on start of the game)
    2. Dryfield day                 (loaded when you enter the gas station)
    3. Dryfield night               (loaded on defeating no. 9 in dryfield
                                        and also on returning during pierce
                                        quest)
    4. Mines and Shelter            (loaded on entering mesa from dryfield
                                        night level, or on returning back from
                                        pierce quest, or on going up from Eve
                                        Access tunnel, or on going up from the
                                        Neo Ark, or returning from Underground
                                        Garage).
    5. Neo Ark and Shelter          (loaded on etnering Eve Access Tunnel, Neo
                                        Ark, or going through the Underground
                                        Garage)
*/

typedef enum {
    BASE_LEVEL,
    AKROPOLIS_LEVEL,
    DRYFIELD_DAY_LEVEL,
    DRYFIELD_NIGHT_LEVEL,
    MINES_AND_SHELTER_LEVEL,
    NEOARK_AND_SHELTER_LEVEL,
} pe2_level_name_t;

/*
    Converts PE2_LEVEL_NAME_T to a string name of the level.

    @param LEVEL_NAME - enum variable of the level
*/
std::string getNameFromLevel(pe2_level_name_t);
pe2_level_name_t getLevelFromStage(pe2_stage_name_t);

} // namespace domain::game
