#include "domain/enemy/entities/Enemy.hpp"

// TODO: define file sizes for every enemy
// TOOD: update depricated

std::vector<char> recordDescriptorToBytesVector(pe2_enemy_record_desc_t desc) {
    return {
        static_cast<char>(desc.id),
        static_cast<char>(desc.param1),
        static_cast<char>(desc.param2),
        0x00,
        static_cast<char>(desc.spawning_point_x & 0xFF),
        static_cast<char>((desc.spawning_point_x >> 8) & 0xFF),
        static_cast<char>(desc.spawning_point_z & 0xFF),
        static_cast<char>((desc.spawning_point_z >> 8) & 0xFF),
        static_cast<char>(desc.spawning_point_y & 0xFF),
        static_cast<char>((desc.spawning_point_y >> 8) & 0xFF),
        static_cast<char>(desc.angle & 0xFF),
        static_cast<char>((desc.angle >> 8) & 0xFF),
        static_cast<char>(desc.texture_vram_offset & 0xFF),
        static_cast<char>((desc.texture_vram_offset >> 8) & 0xFF),
        static_cast<char>((desc.texture_vram_offset >> 16) & 0xFF),
        static_cast<char>((desc.texture_vram_offset >> 24) & 0xFF),
    };
}

std::vector<char> resourceDescriptorToBytesVector(pe2_enemy_resource_desc_t desc) {
    return {
        static_cast<char>(desc.id),
        0x00,
        static_cast<char>(desc.file_id),
        0x00,
        static_cast<char>(desc.slot_val & 0xFF),
        static_cast<char>((desc.slot_val >> 8) & 0xFF),
        0x00,
        0x00,
        static_cast<char>(desc.file_table_addr & 0xFF),
        static_cast<char>((desc.file_table_addr >> 8) & 0xFF),
        static_cast<char>((desc.file_table_addr >> 16) & 0xFF),
        static_cast<char>((desc.file_table_addr >> 24) & 0xFF),
    };
}

std::string getEnemyName(uint8_t id) {
    switch (id) {
    case CHASER_ID:
        return "Chaser";
    case BRAIN_STINGER_ID:
        return "Brain Stinger";
    case DIVER_ID:
        return "Diver";
    case STALKER_ID:
        return "Stalker";
    case RAT_ID:
        return "Rat";
    case BUTTERFLY_ID:
        return "Butterfly";
    case LESSER_STRANGER_ID:
        return "Lesser Stranger";
    case BRUTE_ID:
        return "Brute";
    case SUCKLER_ID:
        return "Suckler";
    case HORNED_STRANGER_ID:
        return "Horned Stranger";
    case MIND_SUCKLER_ID:
        return "Mind Suckler";
    case SCAVANGER_ID:
        return "Scavanger";
    case ODD_STRANGER_ID:
        return "Odd Stranger";
    case GRINNING_STRANGER_ID:
        return "Grinning Stranger";
    case PAWN_BLADE_GOLEM_ID:
        return "Pawn Blade Golem";
    case TURRET_ID:
        return "Turret";
    case KNIGHT_GOLEM_ID:
        return "Knight Golem";
    case ROOK_BLADE_GOLEM_ID:
        return "Rook Blade Golem";
    case AMOEBA_ID:
        return "Amoeba";
    case SCROPION_ID:
        return "Scorpion";
    case WHITE_WORM_ID:
        return "White Worm";
    case BAT_ID:
        return "Bat";
    case BEETLE_ID:
        return "Beetle";
    case BISHOP_GOLEM_ID:
        return "Bishop Golem";
    case BLOOD_SUCKLER_ID:
        return "Blood Suckler";
    case FAILED_SUCKLER_ID:
        return "Failed Suckler";
    case FAILED_CHASER_ID:
        return "Failed Chaser";
    case MOSSBACK_BRUTE_ID:
        return "Mossback Brute";
    case GREEN_WORM_ID:
        return "Green Worm";
    case PAWN_GUN_GOLEM_ID:
        return "Pawn Gun Golem";
    case ROOK_GUN_GOLEM_ID:
        return "Rook Gun Golem";
    case FAILED_BRUTE_ID:
        return "Failed Brute";
    case FAILED_STRANGER_ID:
        return "Failed Stranger";
    case DUMMY_MONSTER_ID:
        return "Dummy Monster (resource placeholder)";
    default:
        return "Undefine Enemy";
    }
}

/*
    ENEMIES MAP VALUES
*/

const std::map<uint8_t, pe2_enemy_t> enemies_map{
    {CHASER_ID,
     pe2_enemy_t{
         .id = CHASER_ID,
         .file_id = CHASER_ID,
         .value = 180,
         .radius = 512,
         .file_size = ENEMY_FILE_2_SLOTS,
         .available_files{{ENEMY_SLOT_1_ADDRESS, {0x3, 0x8014d8a4}}},
         /*
             initial state:
                 0 - default
                 1 - ??? T.B.D.
                 2 - sleeping
         */
         .first_param = {{0.8, 0}, {0.2, 2}},
         /*
             Second param forces crash on usage straigtforward. I think it
             might be used while ambushing or cutscenes.
         */
         //.second_type = { {0.5, 0}, {0.5, 1} }    // 0 - ???          1 - ???
     }},
    {BRAIN_STINGER_ID,
     pe2_enemy_t{
         .id = BRAIN_STINGER_ID,
         .file_id = BRAIN_STINGER_ID,
         .value = 100, // TODO: depricated value. update it.
         .radius = 512,
         .res_type = ENEMY_RES_TYPE_NEEDS_ADDITIONAL_RESOURCES,
         .available_files{
             {ENEMY_SLOT_1_ADDRESS, {0x0, 0x80148110}},
             {ENEMY_SLOT_2_ADDRESS, {0x1, 0x80160110}},
         },
     }},
    {DIVER_ID,
     pe2_enemy_t{
         .id = DIVER_ID,
         .file_id = DIVER_ID,
         .value = 150,
         .radius = 512,
         .move_type = ENEMY_MOVE_WATER,
         .available_files{
             {ENEMY_SLOT_1_ADDRESS, {0x0, 0x80147e48}},
             {ENEMY_SLOT_2_ADDRESS, {0x1, 0x8015fe48}},
         },
         /*
             in game has special params for offsetting actions i guess
             but i didn't figure out how to use it
         */
     }},
    {STALKER_ID,
     pe2_enemy_t{
         .id = STALKER_ID,
         .file_id = STALKER_ID,
         .value = 150,
         .radius = 512,
         .file_size = ENEMY_FILE_2_SLOTS,
         .available_files{
             {ENEMY_SLOT_1_ADDRESS, {0x3, 0x80151b10}},
         },
         /*
             in game has special params for spawning on wall/ceiling i guess
             but i didn't figure out how to use it
         */
     }},
    {RAT_ID,
     pe2_enemy_t{
         .id = RAT_ID,
         .file_id = RAT_ID,
         .value = 30,
         .radius = 256,
         .size_type = ENEMY_SIZE_SMALL,
         .available_files{
             {ENEMY_SLOT_1_ADDRESS, {0x0, 0x80138c80}}, {ENEMY_SLOT_2_ADDRESS, {0x1, 0x80150c80}},
             // is used for as dummy monster fo additional data
             // { 0x2, 0x80168c80 }
         },
     }},
    {BUTTERFLY_ID,
     pe2_enemy_t{
         .id = BUTTERFLY_ID,
         .file_id = RAT_ID, // share the same file with RAT
         .value = 20,
         .radius = 256,
         .size_type = ENEMY_SIZE_SMALL,
         .move_type = ENEMY_MOVE_AERIAL,
         .available_files{
             {ENEMY_SLOT_1_ADDRESS, {0x0, 0x801393c8}}, {ENEMY_SLOT_2_ADDRESS, {0x1, 0x801513c8}},
             // is used for as dummy monster fo additional data
             // { 0x2, 0x80168c80 }
         },
     }},
    {LESSER_STRANGER_ID,
     pe2_enemy_t{.id = LESSER_STRANGER_ID,
                 .file_id = LESSER_STRANGER_ID,
                 .value = 150,
                 .radius = 512,
                 .file_size = ENEMY_FILE_2_SLOTS,
                 .available_files{
                     {ENEMY_SLOT_1_ADDRESS, {0x3, 0x80155004}},
                 },
                 /*
                     initial state:
                         0x0 - walking slowly in random diraction
                         0x4 - eating corpse
                     running speed:
                         0x0X - fast & jump when aimed at (akropolis)
                         0x1X - slow (dryfield toilet)

                     param = initial state + running speed.
                     Meaning 0x00 is fast and walking and 0x14 - is slow and eating.

                     should have jump out ambush. Couldn't figure out how to use it
                     yet.
                 */
                 .first_param = {{0.55, 0x00}, {0.15, 0x04}, {0.15, 0x10}, {0.15, 0x14}}}},
    {BRUTE_ID, pe2_enemy_t{.id = BRUTE_ID,
                           .file_id = BRUTE_ID,
                           .value = 180,
                           .radius = 512,
                           .available_files{
                               {ENEMY_SLOT_1_ADDRESS, {0x0, 0x80147400}},
                               {ENEMY_SLOT_2_ADDRESS, {0x1, 0x8015f400}},
                               {ENEMY_SLOT_3_ADDRESS, {0x2, 0x80177400}},
                           },
                           /*
                               brute type:
                                   0x0 - regular brute
                                   0x1 - more aggressive (and may be more damaging like
                                           mossback, need to check that)
                           */
                           .first_param = {{0.5, 0}, {0.5, 1}}}},
    {SUCKLER_ID, pe2_enemy_t{.id = SUCKLER_ID,
                             .file_id = SUCKLER_ID,
                             .value = 30,
                             .radius = 256,
                             .size_type = ENEMY_SIZE_SMALL,
                             .available_files{
                                 {ENEMY_SLOT_1_ADDRESS, {0x0, 0x80138e98}},
                                 {ENEMY_SLOT_2_ADDRESS, {0x1, 0x80150e98}},
                                 {ENEMY_SLOT_3_ADDRESS, {0x2, 0x80168e98}},
                             },
                             /*
                                 init state:
                                     0x0 - common
                                     0x1 - resting

                                 maybe have the same ceiling state as blood sucklers?
                                 need to check...
                             */
                             .first_param = {{0.5, 0}, {0.5, 1}}}},
    {HORNED_STRANGER_ID, pe2_enemy_t{.id = HORNED_STRANGER_ID,
                                     .file_id = HORNED_STRANGER_ID,
                                     .value = 280,
                                     .radius = 512,
                                     .file_size = ENEMY_FILE_2_SLOTS,
                                     .available_files{
                                         {ENEMY_SLOT_1_ADDRESS, {0x3, 0x80158a18}},
                                     },
                                     /*
                                         init state:
                                             0x0 - slowly walking to random diraction
                                             0x4 - eating corpse (cut it out due to being buggy, appears
                                                     smaller than it should be)
                                             0x20 - lying almost dead

                                         should have jump out ambush. Couldn't figure out how to use it
                                         yet.
                                     */
                                     .first_param = {{0.99, 0x0}, {0.01, 0x20}}}},
    {MIND_SUCKLER_ID, pe2_enemy_t{.id = MIND_SUCKLER_ID,
                                  .file_id = MIND_SUCKLER_ID,
                                  .value = 45,
                                  .radius = 256,
                                  .size_type = ENEMY_SIZE_SMALL,
                                  .move_type = ENEMY_MOVE_AERIAL,
                                  .available_files{
                                      {ENEMY_SLOT_1_ADDRESS, {0x0, 0x8013be28}},
                                      {ENEMY_SLOT_2_ADDRESS, {0x1, 0x80153e28}},
                                      {ENEMY_SLOT_3_ADDRESS, {0x2, 0x8016be28}},
                                  },
                                  /*
                                      init state:
                                          0x0 - calm
                                          0x2 - a little bit more aggressive
                                          0x3 - attached to the ceiling animation

                                      I remember mind sucklers havin other initial state, but it was
                                      kinda buggy, so i cut it out. Don't remember what it was
                                      though.
                                  */
                                  .second_param = {{0.3, 0x0}, {0.4, 0x2}, {0.3, 0x3}}}},
    {SCAVANGER_ID,
     pe2_enemy_t{
         .id = SCAVANGER_ID,
         .file_id = SCAVANGER_ID,
         .value = 65,
         .radius = 256,
         .size_type = ENEMY_SIZE_SMALL,
         .available_files{
             {ENEMY_SLOT_1_ADDRESS, {0x0, 0x801445dc}},
             {ENEMY_SLOT_2_ADDRESS, {0x1, 0x8015c5dc}},
             {ENEMY_SLOT_3_ADDRESS, {0x2, 0x801745dc}},
         },
         /*
             init state:
                 0x00 - common
                 0x80 - blinded

             should have something related to that car encounter with Kyle.
         */
         .first_param = {{0.95, 0x00}, {0.05, 0x80}},
     }},
    {ODD_STRANGER_ID, pe2_enemy_t{.id = ODD_STRANGER_ID,
                                  .file_id = ODD_STRANGER_ID,
                                  .value = 120,
                                  .radius = 512,
                                  .file_size = ENEMY_FILE_2_SLOTS,
                                  .available_files{
                                      {ENEMY_SLOT_1_ADDRESS, {0x3, 0x80155ac4}},
                                  },
                                  /*
                                      initial state:
                                          0x0 - walking slowly in random diraction
                                          0x4 - eating corpse
                                      running speed:
                                          0x0X - fast & jump when aimed at
                                          0x1X - slow (dryfield motel rooms)

                                      param = initial state + running speed.
                                      Meaning 0x00 is fast and walking and 0x14 - is slow and eating.
                                  */
                                  .first_param = {{0.55, 0x00}, {0.15, 0x04}, {0.15, 0x10}, {0.15, 0x14}}}},
    {GRINNING_STRANGER_ID,
     pe2_enemy_t{
         .id = GRINNING_STRANGER_ID,
         .file_id = GRINNING_STRANGER_ID,
         .value = 120,
         .radius = 512,
         .file_size = ENEMY_FILE_1_SLOT,
         .available_files{
             {ENEMY_SLOT_1_ADDRESS, {0x0, 0x80149120}},
             {ENEMY_SLOT_3_ADDRESS, {0x2, 0x80179120}},
         },
         /*
             initial state:
                 0x0 - walking slowly in random diraction
                 0x4 - eating corpse
         */
         .first_param = {{0.8, 0x0}, {0.2, 0x4}},
     }},
    {PAWN_BLADE_GOLEM_ID,
     pe2_enemy_t{
         .id = PAWN_BLADE_GOLEM_ID,
         .file_id = PAWN_BLADE_GOLEM_ID,
         .value = 75, // TODO: depricated value. update it.
         .radius = 512,
         .res_type = ENEMY_RES_TYPE_NEEDS_ADDITIONAL_RESOURCES,
         .texture_size = ENEMY_TEX_BIG,
         .available_files{
             {ENEMY_SLOT_1_ADDRESS, {0x0, 0x80147df0}},
             {ENEMY_SLOT_2_ADDRESS, {0x1, 0x8015fdf0}},
             {ENEMY_SLOT_3_ADDRESS, {0x2, 0x80177df0}},
         },
         /*
             moving behavior:
                 0x0 - no walking pattern
                 0x1-0x6 - pattern 1 - 6
         */
         .first_param = {{0.2, 0}, {0.12, 1}, {0.12, 2}, {0.12, 3}, {0.12, 4}, {0.12, 5}, {0.2, 6}},
         /*
             initial state:
                 0x0 - almost blind
                 0x1 - active
         */
         .second_param = {{0.2, 0}, {0.8, 1}},
     }},
    {TURRET_ID,
     pe2_enemy_t{
         .id = TURRET_ID,
         .file_id = TURRET_ID,
         .value = 50,
         .radius = 256,
         .ambush_value = 0,
         .size_type = ENEMY_SIZE_SMALL,
         .move_type = ENEMY_MOVE_AERIAL,
         .available_files{
             {ENEMY_SLOT_1_ADDRESS, {0x0, 0x80135c30}},
             {ENEMY_SLOT_2_ADDRESS, {0x1, 0x8014dc30}},
         },
         /*
             turret type:
                 0x0 - fast red turret
                 0x1 - slow red turret
                 0x2 - paralyze turret
                 0x3 - confuse turret
                 0x4 - machine gun turret
         */
         .first_param = {{0.2, 0}, {0.2, 1}, {0.2, 2}, {0.2, 3}, {0.2, 4}},
     }},
    {KNIGHT_GOLEM_ID,
     pe2_enemy_t{
         .id = KNIGHT_GOLEM_ID,
         .file_id = KNIGHT_GOLEM_ID,
         .value = 350,
         .radius = 512,
         .res_type = ENEMY_RES_TYPE_NEEDS_ADDITIONAL_RESOURCES,
         .file_size = ENEMY_FILE_2_SLOTS,
         .texture_size = ENEMY_TEX_BIG,
         .available_files{
             {ENEMY_SLOT_1_ADDRESS, {0x3, 0x80154188}},
         },
     }},
    {ROOK_BLADE_GOLEM_ID,
     pe2_enemy_t{
         .id = ROOK_BLADE_GOLEM_ID,
         .file_id = ROOK_BLADE_GOLEM_ID,
         .value = 120, // TODO: depricated value. update it.
         .radius = 512,
         .res_type = ENEMY_RES_TYPE_NEEDS_ADDITIONAL_RESOURCES,
         .texture_size = ENEMY_TEX_BIG,
         .available_files{
             {ENEMY_SLOT_1_ADDRESS, {0x0, 0x80147ab8}},
             {ENEMY_SLOT_2_ADDRESS, {0x1, 0x8015fab8}},
         },
         /*
             moving behavior:
                 0x0 - no walking pattern
                 0x1-0x6 - pattern 1 - 6
         */
         .first_param = {{0.2, 0}, {0.12, 1}, {0.12, 2}, {0.12, 3}, {0.12, 4}, {0.12, 5}, {0.2, 6}},
         /*
             initial state:
                 0x0 - almost blind
                 0x1 - active
         */
         .second_param = {{0.2, 0}, {0.8, 1}},
     }},
    {AMOEBA_ID, pe2_enemy_t{.id = AMOEBA_ID,
                            .file_id = AMOEBA_ID,
                            .value = 75,
                            .radius = 256,
                            .size_type = ENEMY_SIZE_SMALL,
                            .available_files{
                                {ENEMY_SLOT_1_ADDRESS, {0x0, 0x8013647c}},
                                {ENEMY_SLOT_2_ADDRESS, {0x1, 0x8014e47c}},
                            },
                            /*
                                amoeba type:
                                    0x0 - blue amoeba
                                    0x1 - red amoeba
                            */
                            .second_param = {{0.5, 0x0}, {0.5, 0x1}}}},
    {SCROPION_ID,
     pe2_enemy_t{
         .id = SCROPION_ID,
         .file_id = SCROPION_ID,
         .value = 40,
         .radius = 256,
         .ambush_value = 1,
         .size_type = ENEMY_SIZE_SMALL,
         .ambush_type = ENEMY_GROUND_AMBUSH,
         .available_files{
             {ENEMY_SLOT_1_ADDRESS, {0x0, 0x801379a8}},
             {ENEMY_SLOT_2_ADDRESS, {0x1, 0x8014f9a8}},
             {ENEMY_SLOT_3_ADDRESS, {0x2, 0x801679a8}},
         },
         /*
             params are used for ambushing
         */
     }},
    {WHITE_WORM_ID,
     pe2_enemy_t{
         .id = WHITE_WORM_ID,
         .file_id = WHITE_WORM_ID,
         .value = 45,
         .radius = 256,
         .ambush_value = 2,
         .size_type = ENEMY_SIZE_SMALL,
         .ambush_type = ENEMY_AERIAL_AMBUSH,
         .available_files{
             {ENEMY_SLOT_1_ADDRESS, {0x0, 0x8013a8d4}},
             {ENEMY_SLOT_2_ADDRESS, {0x1, 0x801528d4}},
             {ENEMY_SLOT_3_ADDRESS, {0x2, 0x8016a8d4}},
         },
         /*
             params are used for ambushing
         */
     }},
    {BAT_ID,
     pe2_enemy_t{
         .id = BAT_ID,
         .file_id = BAT_ID,
         .value = 5,
         .radius = 256,
         .ambush_value = 1,
         .size_type = ENEMY_SIZE_SMALL,
         .move_type = ENEMY_MOVE_AERIAL,
         .ambush_type = ENEMY_AERIAL_AMBUSH,
         .available_files{
             {ENEMY_SLOT_1_ADDRESS, {0x0, 0x80139dac}},
             {ENEMY_SLOT_2_ADDRESS, {0x1, 0x80151dac}},
         },
         /*
             params are used for ambushing
         */
     }},
    {BEETLE_ID,
     pe2_enemy_t{
         .id = BEETLE_ID,
         .file_id = BEETLE_ID,
         .value = 50,
         .radius = 256,
         .size_type = ENEMY_SIZE_SMALL,
         .available_files{
             {ENEMY_SLOT_1_ADDRESS, {0x0, 0x80137d74}},
             {ENEMY_SLOT_2_ADDRESS, {0x1, 0x8014fd74}},
         },
         /*
             there is a param for spawning them on walls and for "ambush"
             used in Neo Ark after soling yellow puzzle.
         */
     }},
    {BISHOP_GOLEM_ID,
     pe2_enemy_t{
         .id = BISHOP_GOLEM_ID,
         .file_id = BISHOP_GOLEM_ID,
         .value = 400,
         .radius = 512,
         .res_type = ENEMY_RES_TYPE_NEEDS_ADDITIONAL_RESOURCES,
         .file_size = ENEMY_FILE_2_SLOTS,
         .texture_size = ENEMY_TEX_BIG,
         .available_files{
             {ENEMY_SLOT_1_ADDRESS, {0x3, 0x801540e0}},
         },
     }},
    {BLOOD_SUCKLER_ID, pe2_enemy_t{.id = BLOOD_SUCKLER_ID,
                                   .file_id = BLOOD_SUCKLER_ID,
                                   .value = 35,
                                   .radius = 256,
                                   .size_type = ENEMY_SIZE_SMALL,
                                   .available_files{
                                       {ENEMY_SLOT_1_ADDRESS, {0x0, 0x8013e500}},
                                       {ENEMY_SLOT_2_ADDRESS, {0x1, 0x80156500}},
                                       {ENEMY_SLOT_3_ADDRESS, {0x2, 0x8016e500}},
                                   },
                                   /*
                                       init state:
                                           0x0 - common
                                           0x1 - resting

                                       maybe have the same ceiling state as blood sucklers?
                                       need to check...
                                   */
                                   .first_param = {{0.5, 0}, {0.5, 1}}}},
    {FAILED_SUCKLER_ID,
     pe2_enemy_t{
         .id = FAILED_SUCKLER_ID,
         .file_id = FAILED_SUCKLER_ID,
         .value = 50,
         .radius = 256,
         .size_type = ENEMY_SIZE_SMALL,
         .available_files{
             {ENEMY_SLOT_1_ADDRESS, {0x0, 0x80137698}},
             {ENEMY_SLOT_2_ADDRESS, {0x1, 0x8014f698}},
         },
     }},
    {FAILED_CHASER_ID,
     pe2_enemy_t{
         .id = FAILED_CHASER_ID,
         .file_id = FAILED_CHASER_ID,
         .value = 80,
         .radius = 512,
         .size_type = ENEMY_SIZE_SMALLER,
         .available_files{
             {ENEMY_SLOT_1_ADDRESS, {0x0, 0x80142604}},
         },
     }},
    {MOSSBACK_BRUTE_ID, pe2_enemy_t{.id = MOSSBACK_BRUTE_ID,
                                    .file_id = MOSSBACK_BRUTE_ID,
                                    .value = 200,
                                    .radius = 512,
                                    .available_files{
                                        {ENEMY_SLOT_1_ADDRESS, {0x0, 0x80147400}},
                                        {ENEMY_SLOT_2_ADDRESS, {0x1, 0x8015f400}},
                                    },
                                    /*
                                        brute type:
                                            0x0 - regular brute
                                            0x1 - more aggressive
                                    */
                                    .first_param = {{0.5, 0}, {0.5, 1}}}},
    {GREEN_WORM_ID,
     pe2_enemy_t{
         .id = GREEN_WORM_ID,
         .file_id = GREEN_WORM_ID,
         .value = 40,
         .radius = 256,
         .ambush_value = 2,
         .size_type = ENEMY_SIZE_SMALL,
         .ambush_type = ENEMY_AERIAL_AMBUSH,
         .available_files{
             {ENEMY_SLOT_1_ADDRESS, {0x0, 0x8013a8dc}},
             {ENEMY_SLOT_2_ADDRESS, {0x1, 0x801528dc}},
         },
         /*
             params are used for ambushing
         */
     }},
    {PAWN_GUN_GOLEM_ID,
     pe2_enemy_t{
         .id = PAWN_GUN_GOLEM_ID,
         .file_id = PAWN_GUN_GOLEM_ID,
         .value = 70, // TODO: depricated value. update it.
         .radius = 512,
         .res_type = ENEMY_RES_TYPE_NEEDS_ADDITIONAL_RESOURCES,
         .texture_size = ENEMY_TEX_BIG,
         .available_files{
             {ENEMY_SLOT_1_ADDRESS, {0x0, 0x801482c0}},
             {ENEMY_SLOT_2_ADDRESS, {0x1, 0x801602c0}},
         },
         /*
             moving behavior:
                 0x0 - no walking pattern
                 0x1-0x6 - pattern 1 - 6
         */
         .first_param = {{0.2, 0}, {0.12, 1}, {0.12, 2}, {0.12, 3}, {0.12, 4}, {0.12, 5}, {0.2, 6}},
         /*
             initial state:
                 0x0 - almost blind
                 0x1 - active
         */
         .second_param = {{0.2, 0}, {0.8, 1}},
     }},
    {ROOK_GUN_GOLEM_ID,
     pe2_enemy_t{
         .id = ROOK_GUN_GOLEM_ID,
         .file_id = ROOK_GUN_GOLEM_ID,
         .value = 100, // TODO: depricated value. update it.
         .radius = 512,
         .res_type = ENEMY_RES_TYPE_NEEDS_ADDITIONAL_RESOURCES,
         .texture_size = ENEMY_TEX_BIG,
         .available_files{
             {ENEMY_SLOT_1_ADDRESS, {0x0, 0x801491f8}},
             {ENEMY_SLOT_2_ADDRESS, {0x1, 0x801611f8}},
         },
         /*
             moving behavior:
                 0x0 - no walking pattern
                 0x1-0x6 - pattern 1 - 6
         */
         .first_param = {{0.2, 0}, {0.12, 1}, {0.12, 2}, {0.12, 3}, {0.12, 4}, {0.12, 5}, {0.2, 6}},
         /*
             initial state:
                 0x0 - almost blind
                 0x1 - active
         */
         .second_param = {{0.2, 0}, {0.8, 1}},
     }},
    {FAILED_BRUTE_ID,
     pe2_enemy_t{
         .id = FAILED_BRUTE_ID,
         .file_id = FAILED_BRUTE_ID,
         .value = 90,
         .radius = 512,
         .size_type = ENEMY_SIZE_SMALLER,
         .available_files{
             {ENEMY_SLOT_1_ADDRESS, {0x0, 0x8013f5f0}},
             {ENEMY_SLOT_2_ADDRESS, {0x1, 0x801575f0}},
         },
     }},
    {FAILED_STRANGER_ID,
     pe2_enemy_t{
         .id = FAILED_STRANGER_ID,
         .file_id = FAILED_STRANGER_ID,
         .value = 90,
         .radius = 512,
         .size_type = ENEMY_SIZE_SMALLER,
         .available_files{
             {ENEMY_SLOT_2_ADDRESS, {0x1, 0x80153ec8}},
         },
     }},
};

const std::map<uint8_t, pe2_enemy_t> one_slot_cloak_golems_map{
    {KNIGHT_GOLEM_ID,
     pe2_enemy_t{
         .id = KNIGHT_GOLEM_ID,
         .file_id = KNIGHT_GOLEM_ID,
         .value = 350,
         .radius = 512,
         .res_type = ENEMY_RES_TYPE_NEEDS_ADDITIONAL_RESOURCES,
         // .file_size = ENEMY_FILE_2_SLOTS,  !!!OLD!!!
         .file_size = ENEMY_FILE_1_SLOT,
         .texture_size = ENEMY_TEX_BIG,
         .available_files{// { ENEMY_SLOT_1_ADDRESS, { 0x3, 0x80154188 } },   !!!OLD!!!
                          {ENEMY_SLOT_1_ADDRESS, {0x3, 0x80174188}}},
     }},
    {BISHOP_GOLEM_ID,
     pe2_enemy_t{
         .id = BISHOP_GOLEM_ID,
         .file_id = BISHOP_GOLEM_ID,
         .value = 400,
         .radius = 512,
         .res_type = ENEMY_RES_TYPE_NEEDS_ADDITIONAL_RESOURCES,
         // .file_size = ENEMY_FILE_2_SLOTS, !!!OLD!!!
         .file_size = ENEMY_FILE_1_SLOT,
         .texture_size = ENEMY_TEX_BIG,
         .available_files{
             // { ENEMY_SLOT_1_ADDRESS, { 0x3, 0x801540e0 } }, !!!OLD!!!
             {ENEMY_SLOT_1_ADDRESS, {0x3, 0x80149d44}},
         },
     }},
};

/*
    ENEMIES IDS BY SLOT MAP VALUES
*/

std::map<uint32_t, std::vector<uint8_t>> enemy_ids_by_slot_map{
    {ENEMY_SLOT_1_ADDRESS,
     {CHASER_ID,
      BRAIN_STINGER_ID,
      DIVER_ID,
      STALKER_ID,
      RAT_ID,
      BUTTERFLY_ID,
      LESSER_STRANGER_ID,
      BRUTE_ID,
      SUCKLER_ID,
      HORNED_STRANGER_ID,
      MIND_SUCKLER_ID,
      SCAVANGER_ID,
      ODD_STRANGER_ID,
      GRINNING_STRANGER_ID,
      PAWN_BLADE_GOLEM_ID,
      TURRET_ID,
      KNIGHT_GOLEM_ID,
      ROOK_BLADE_GOLEM_ID,
      AMOEBA_ID,
      SCROPION_ID,
      WHITE_WORM_ID,
      BAT_ID,
      BEETLE_ID,
      BISHOP_GOLEM_ID,
      BLOOD_SUCKLER_ID,
      FAILED_SUCKLER_ID,
      FAILED_CHASER_ID,
      MOSSBACK_BRUTE_ID,
      GREEN_WORM_ID,
      PAWN_GUN_GOLEM_ID,
      ROOK_GUN_GOLEM_ID,
      FAILED_BRUTE_ID}},
    {ENEMY_SLOT_2_ADDRESS,
     {BRAIN_STINGER_ID,
      DIVER_ID,
      RAT_ID,
      BUTTERFLY_ID,
      BRUTE_ID,
      SUCKLER_ID,
      MIND_SUCKLER_ID,
      SCAVANGER_ID,
      PAWN_BLADE_GOLEM_ID,
      TURRET_ID,
      ROOK_BLADE_GOLEM_ID,
      AMOEBA_ID,
      SCROPION_ID,
      WHITE_WORM_ID,
      BAT_ID,
      BEETLE_ID,
      BLOOD_SUCKLER_ID,
      FAILED_SUCKLER_ID,
      MOSSBACK_BRUTE_ID,
      GREEN_WORM_ID,
      PAWN_GUN_GOLEM_ID,
      ROOK_GUN_GOLEM_ID,
      FAILED_BRUTE_ID,
      FAILED_STRANGER_ID}},
    {ENEMY_SLOT_3_ADDRESS,
     {BRUTE_ID, SUCKLER_ID, MIND_SUCKLER_ID, SCAVANGER_ID, GRINNING_STRANGER_ID, PAWN_BLADE_GOLEM_ID,
      SCROPION_ID, WHITE_WORM_ID, BLOOD_SUCKLER_ID}}

};

/*
    ENEMIES ALL SUPPORTED ENEMIES IDS VECTOR VALUES
*/

std::vector<uint8_t> enemy_ids_vector{CHASER_ID,
                                      BRAIN_STINGER_ID,
                                      DIVER_ID,
                                      STALKER_ID,
                                      RAT_ID,
                                      BUTTERFLY_ID,
                                      LESSER_STRANGER_ID,
                                      BRUTE_ID,
                                      SUCKLER_ID,
                                      HORNED_STRANGER_ID,
                                      MIND_SUCKLER_ID,
                                      SCAVANGER_ID,
                                      ODD_STRANGER_ID,
                                      GRINNING_STRANGER_ID,
                                      PAWN_BLADE_GOLEM_ID,
                                      TURRET_ID,
                                      KNIGHT_GOLEM_ID,
                                      ROOK_BLADE_GOLEM_ID,
                                      AMOEBA_ID,
                                      SCROPION_ID,
                                      WHITE_WORM_ID,
                                      BAT_ID,
                                      BEETLE_ID,
                                      BISHOP_GOLEM_ID,
                                      BLOOD_SUCKLER_ID,
                                      FAILED_SUCKLER_ID,
                                      FAILED_CHASER_ID,
                                      MOSSBACK_BRUTE_ID,
                                      GREEN_WORM_ID,
                                      PAWN_GUN_GOLEM_ID,
                                      ROOK_GUN_GOLEM_ID,
                                      FAILED_BRUTE_ID,
                                      FAILED_STRANGER_ID};

const std::map<pe2_enemy_size_t, std::vector<uint8_t>> common_enemies_by_size_map{
    {ENEMY_SIZE_SMALL,
     {RAT_ID, BUTTERFLY_ID, SUCKLER_ID, MIND_SUCKLER_ID, SCAVANGER_ID, TURRET_ID, AMOEBA_ID, SCROPION_ID,
      WHITE_WORM_ID, BAT_ID, BEETLE_ID, BLOOD_SUCKLER_ID, FAILED_SUCKLER_ID, GREEN_WORM_ID}},
    {ENEMY_SIZE_SMALLER, {FAILED_CHASER_ID, FAILED_BRUTE_ID, FAILED_STRANGER_ID}},
    {ENEMY_SIZE_NORMAL,
     {
         CHASER_ID,
         STALKER_ID,
         LESSER_STRANGER_ID,
         BRUTE_ID,
         HORNED_STRANGER_ID,
         ODD_STRANGER_ID,
         GRINNING_STRANGER_ID,
         MOSSBACK_BRUTE_ID,
     }},
};

std::vector<uint8_t> special_enemies{BRAIN_STINGER_ID,    PAWN_BLADE_GOLEM_ID, KNIGHT_GOLEM_ID,
                                     ROOK_BLADE_GOLEM_ID, BISHOP_GOLEM_ID,     PAWN_GUN_GOLEM_ID,
                                     ROOK_GUN_GOLEM_ID};

const std::map<pe2_enemy_file_size_t, std::vector<uint8_t>> enemies_by_file_size{
    {ENEMY_FILE_1_SLOT,
     {BRAIN_STINGER_ID,
      DIVER_ID,
      RAT_ID,
      BUTTERFLY_ID,
      BRUTE_ID,
      SUCKLER_ID,
      MIND_SUCKLER_ID,
      SCAVANGER_ID,
      PAWN_BLADE_GOLEM_ID,
      TURRET_ID,
      ROOK_BLADE_GOLEM_ID,
      AMOEBA_ID,
      SCROPION_ID,
      WHITE_WORM_ID,
      BAT_ID,
      BEETLE_ID,
      BLOOD_SUCKLER_ID,
      FAILED_SUCKLER_ID,
      FAILED_CHASER_ID,
      MOSSBACK_BRUTE_ID,
      GREEN_WORM_ID,
      GREEN_WORM_ID,
      PAWN_GUN_GOLEM_ID,
      ROOK_GUN_GOLEM_ID,
      FAILED_BRUTE_ID,
      FAILED_STRANGER_ID}},
    {ENEMY_FILE_2_SLOTS,
     {CHASER_ID, STALKER_ID, LESSER_STRANGER_ID, HORNED_STRANGER_ID, ODD_STRANGER_ID, GRINNING_STRANGER_ID,
      KNIGHT_GOLEM_ID, BISHOP_GOLEM_ID}},
};