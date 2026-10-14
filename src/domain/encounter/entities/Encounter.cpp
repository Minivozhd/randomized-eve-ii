#include "domain/encounter/entities/Encounter.hpp"

namespace domain::encounter {

// TODO: may be map of rooms too
std::map<game::pe2_level_name_t, std::vector<pe2_room_encounters_t>> encounters_per_level_map{
    {game::AKROPOLIS_LEVEL,
     {
         pe2_room_encounters_t{.name = game::ROOM_AKROPOLIS_SQUARE,
                               .encounters_offset = 0x136c,
                               .additional_file_end_pointer = 0x700,
                               .encounters{pe2_encounter_t{
                                   .mask = UNRANDOMIZED,
                                   .encounter_value = 0,
                                   .enemy_record_descriptor_list_addr_offset = 0x8898,
                                   .enemy_resources_desciptors_list_offset = 0x8878,
                               }},
                               .file_patch_map{// Fix first encounter music
                                               {0x4d18, 0x01}}},

         pe2_room_encounters_t{
             .name = game::ROOM_AKROPOLIS_EAST_ELEVATOR_HALL,
             .encounters_offset = 0x138c,
             .additional_file_end_pointer = 0x708,
             .encounters{pe2_encounter_t{
                 .id = 1,
                 .mask = UNRANDOMIZED,
                 .encounter_value = 0,
                 .enemy_record_descriptor_list_addr_offset = 0x8898,
                 .enemy_resources_desciptors_list_offset = 0x8878,
             }},
         },

         pe2_room_encounters_t{.id = 0x03,
                               .name = game::ROOM_AKROPOLIS_PATIO,
                               .encounters_offset = 0x138c,
                               .additional_file_end_pointer = 0x710,
                               .environment =
                                   pe2_room_environment_t{
                                       .spawn_zones =
                                           std::vector<utils::geometry_works::Zone>{
                                               utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                                   utils::geometry_works::Point(0xe100, 0x0100),
                                                   utils::geometry_works::Point(0xf600, 0x0600),
                                                   utils::geometry_works::Point(0xf600, 0xfc00),
                                                   utils::geometry_works::Point(0xe200, 0xfc00),
                                               }),
                                               utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                                   utils::geometry_works::Point(0xf600, 0x0600),
                                                   utils::geometry_works::Point(0x0500, 0x0900),
                                                   utils::geometry_works::Point(0x0600, 0xfc00),
                                                   utils::geometry_works::Point(0xf600, 0xfc00),
                                               }),
                                               utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                                   utils::geometry_works::Point(0x0d00, 0x0000),
                                                   utils::geometry_works::Point(0x1900, 0xfe00),
                                                   utils::geometry_works::Point(0x1900, 0xf400),
                                                   utils::geometry_works::Point(0x0e00, 0xf400),
                                               }),
                                           },
                                   },
                               .encounters{pe2_encounter_t{
                                               .id = 1,
                                               .mask = RANDOMIZED,
                                               .encounter_value = 300,
                                               .enemy_record_descriptor_list_addr_offset = 0x74d8,
                                               .enemy_resources_desciptors_list_offset = 0x741c,
                                           },
                                           pe2_encounter_t{
                                               .id = 2,
                                               .mask = RANDOMIZED,
                                               .encounter_value = 300,
                                               .enemy_record_descriptor_list_addr_offset = 0x74e0,
                                               .enemy_resources_desciptors_list_offset = 0x7440,
                                           },
                                           pe2_encounter_t{
                                               .id = 3,
                                               .mask = RANDOMIZED,
                                               .encounter_value = 300,
                                               .enemy_record_descriptor_list_addr_offset = 0x74e8,
                                               .enemy_resources_desciptors_list_offset = 0x7464,
                                           },
                                           pe2_encounter_t{
                                               .id = 4,
                                               .mask = RANDOMIZED,
                                               .encounter_value = 300,
                                               .enemy_record_descriptor_list_addr_offset = 0x74f0,
                                               .enemy_resources_desciptors_list_offset = 0x7488,
                                           }},
                               .file_patch_map{// Disable drop down strangers cutscene
                                               {0x6ee, 0x45}}},

         pe2_room_encounters_t{
             .id = 0x04,
             .name = game::ROOM_AKROPOLIS_CAFETERIA,
             .encounters_offset = 0x154c,
             .additional_file_end_pointer = 0x718,
             .environment =
                 pe2_room_environment_t{
                     .separated_zones = true,
                     .spawn_zones =
                         std::vector<utils::geometry_works::Zone>{
                             utils::geometry_works::Zone(std::vector<utils::geometry_works::Point>{
                                 utils::geometry_works::Point(0xec00, 0x0800),
                                 utils::geometry_works::Point(0xf900, 0x0900),
                                 utils::geometry_works::Point(0xfa00, 0xf600),
                                 utils::geometry_works::Point(0xec00, 0xf600),
                             }),
                             utils::geometry_works::Zone(std::vector<utils::geometry_works::Point>{
                                 utils::geometry_works::Point(0xec00, 0xf500),
                                 utils::geometry_works::Point(0xfb00, 0xf300),
                                 utils::geometry_works::Point(0xfc00, 0xea00),
                                 utils::geometry_works::Point(0xeb00, 0xea00),
                             }),
                             utils::geometry_works::Zone(std::vector<utils::geometry_works::Point>{
                                 utils::geometry_works::Point(0x0200, 0x0700),
                                 utils::geometry_works::Point(0x0800, 0x0700),
                                 utils::geometry_works::Point(0x0800, 0xf400),
                                 utils::geometry_works::Point(0x0200, 0xf400),
                             }),
                         },
                     .dead_zones =
                         std::vector<utils::geometry_works::Zone>{
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0xeb00, 0xf400),
                                 utils::geometry_works::Point(0xf000, 0xef00),
                                 utils::geometry_works::Point(0xf600, 0xe900),
                                 utils::geometry_works::Point(0xea00, 0xe800),
                             }),
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0xea00, 0x0100),
                                 utils::geometry_works::Point(0xf200, 0xfe00),
                                 utils::geometry_works::Point(0xee00, 0xf800),
                                 utils::geometry_works::Point(0xea00, 0xf800),
                             }),
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0xf600, 0x0700),
                                 utils::geometry_works::Point(0xfa00, 0x0300),
                                 utils::geometry_works::Point(0xf500, 0xfd00),
                                 utils::geometry_works::Point(0xf100, 0x0200),
                             }),
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0xf600, 0xfd00),
                                 utils::geometry_works::Point(0xfa00, 0xf600),
                                 utils::geometry_works::Point(0xf300, 0xf200),
                                 utils::geometry_works::Point(0xf200, 0xf600),
                             }),
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0x0100, 0xfe00),
                                 utils::geometry_works::Point(0x0500, 0xfe00),
                                 utils::geometry_works::Point(0x0500, 0xf300),
                                 utils::geometry_works::Point(0x0100, 0xf300),
                             }),
                         }},
             .encounters{pe2_encounter_t{.id = 1,
                                         .mask = UNRANDOMIZED,
                                         .encounter_value = 0,
                                         .enemy_record_descriptor_list_addr_offset = 0xc814,
                                         .enemy_resources_desciptors_list_offset = 0xc74c,
                                         .force_reward_value = 200},
                         pe2_encounter_t{
                             .id = 2,
                             .mask = UNRANDOMIZED,
                             .encounter_value = 0,
                             .enemy_record_descriptor_list_addr_offset = 0xc81c,
                             .enemy_resources_desciptors_list_offset = 0xc77c,
                         },
                         pe2_encounter_t{
                             .id = 3,
                             .mask = RANDOMIZED,
                             .encounter_value = 300,
                             .enemy_record_descriptor_list_addr_offset = 0xc824,
                             .enemy_resources_desciptors_list_offset = 0xc7ac,
                         },
                         pe2_encounter_t{
                             .id = 4,
                             .mask = RANDOMIZED,
                             .encounter_value = 300,
                             .enemy_record_descriptor_list_addr_offset = 0xc82c,
                             .enemy_resources_desciptors_list_offset = 0xc7c4,
                         }},
         },

         pe2_room_encounters_t{
             .name = game::ROOM_AKROPOLIS_APPROACH,
             .encounters_offset = 0x169c,
             .additional_file_end_pointer = 0x720,
             .encounters{
                 pe2_encounter_t{
                     .mask = UNRANDOMIZED,
                     .encounter_value = 0,
                     .enemy_record_descriptor_list_addr_offset = 0x1bdd8,
                     .enemy_resources_desciptors_list_offset = 0x1bdb8,
                 },
             },
         },

         pe2_room_encounters_t{.id = 0x05,
                               .name = game::ROOM_AKROPOLIS_SECURITY_ROOM,
                               .encounters_offset = 0x16cc,
                               .additional_file_end_pointer = 0x728,
                               .encounters{
                                   pe2_encounter_t{
                                       .id = 1,
                                       .mask = UNRANDOMIZED,
                                       .encounter_value = 0,
                                       .enemy_record_descriptor_list_addr_offset = 0x6ad0,
                                       .enemy_resources_desciptors_list_offset = 0x6aa4,
                                   },
                               },
                               .file_patch_map{// Disable cutscenes with escalator
                                               {0x1c90, 0x05}}},

         pe2_room_encounters_t{
             .id = 0x07,
             .name = game::ROOM_AKROPOLIS_HALLWAY,
             .encounters_offset = 0x16fc,
             .additional_file_end_pointer = 0x730,
             .environment =
                 pe2_room_environment_t{
                     .separated_zones = true,
                     .ceiling_height = 0xc00,
                     .spawn_zones =
                         std::vector<utils::geometry_works::Zone>{
                             utils::geometry_works::Zone(
                                 utils::geometry_works::Polygon{
                                     utils::geometry_works::Point(0xfb00, 0x0300),
                                     utils::geometry_works::Point(0x0800, 0x0300),
                                     utils::geometry_works::Point(0x0800, 0xfd00),
                                     utils::geometry_works::Point(0xfb00, 0xfd00),
                                 },
                                 0xfc18),
                         },
                 },
             .encounters{pe2_encounter_t{
                             .id = 1,
                             .mask = RANDOMIZED,
                             .encounter_value = 300,
                             .enemy_record_descriptor_list_addr_offset = 0x1484,
                             .enemy_resources_desciptors_list_offset = 0x1410,
                         },
                         pe2_encounter_t{
                             .id = 2,
                             .mask = UNRANDOMIZED,
                             .encounter_value = 0,
                             .enemy_record_descriptor_list_addr_offset = 0x149c,
                             .enemy_resources_desciptors_list_offset = 0x0,
                         },
                         pe2_encounter_t{
                             .id = 3,
                             .mask = UNRANDOMIZED,
                             .encounter_value = 0,
                             .enemy_record_descriptor_list_addr_offset = 0x149c,
                             .enemy_resources_desciptors_list_offset = 0x0,
                         },
                         pe2_encounter_t{
                             .id = 4,
                             .mask = RANDOMIZED,
                             .encounter_value = 300,
                             .enemy_record_descriptor_list_addr_offset = 0x149c,
                             .enemy_resources_desciptors_list_offset = 0x1458,
                         }},
         },

         pe2_room_encounters_t{
             .id = 0x08,
             .name = game::ROOM_AKROPOLIS_FOUNTAIN,
             .encounters_offset = 0x182c,
             .additional_file_end_pointer = 0x738,
             .environment =
                 pe2_room_environment_t{
                     .spawn_zones =
                         std::vector<utils::geometry_works::Zone>{
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0xed00, 0x0a00),
                                 utils::geometry_works::Point(0xf200, 0x0a00),
                                 utils::geometry_works::Point(0xf200, 0xfc00),
                                 utils::geometry_works::Point(0xed00, 0xfc00),
                             }),
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0xee00, 0xf900),
                                 utils::geometry_works::Point(0xf200, 0xfa00),
                                 utils::geometry_works::Point(0xf900, 0xf100),
                                 utils::geometry_works::Point(0xf200, 0xf100),
                             }),
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0xf900, 0xf000),
                                 utils::geometry_works::Point(0x0a00, 0xf000),
                                 utils::geometry_works::Point(0x0900, 0xed00),
                                 utils::geometry_works::Point(0xf900, 0xed00),
                             }),
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0x0e00, 0x0d00),
                                 utils::geometry_works::Point(0x1200, 0x0900),
                                 utils::geometry_works::Point(0x1200, 0xf800),
                                 utils::geometry_works::Point(0x0d00, 0xf100),
                             }),
                         },
                     .dead_zones =
                         std::vector<utils::geometry_works::Zone>{
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0x0c00, 0x0400),
                                 utils::geometry_works::Point(0x1000, 0x0300),
                                 utils::geometry_works::Point(0x1000, 0xfb00),
                                 utils::geometry_works::Point(0x0c00, 0xfb00),
                             }),
                         },
                 },
             .encounters{pe2_encounter_t{
                             .id = 1,
                             .mask = RANDOMIZED,
                             .encounter_value = 300,
                             .enemy_record_descriptor_list_addr_offset = 0x26ec,
                             .enemy_resources_desciptors_list_offset = 0x26ac,
                         },
                         pe2_encounter_t{
                             .id = 7,
                             .mask = RANDOMIZED,
                             .encounter_value = 300,
                             .enemy_record_descriptor_list_addr_offset = 0x2714,
                             .enemy_resources_desciptors_list_offset = 0x26c4,
                         }},
         },

         pe2_room_encounters_t{.id = 0x09,
                               .name = game::ROOM_AKROPOLIS_FORKED_ROAD,
                               .encounters_offset = 0x188c,
                               .additional_file_end_pointer = 0x740,
                               .environment =
                                   pe2_room_environment_t{
                                       .spawn_zones =
                                           std::vector<utils::geometry_works::Zone>{
                                               utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                                   utils::geometry_works::Point(0xe600, 0xff00),
                                                   utils::geometry_works::Point(0xf200, 0x0100),
                                                   utils::geometry_works::Point(0xf200, 0xf400),
                                                   utils::geometry_works::Point(0xe600, 0xf400),
                                               }),
                                               utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                                   utils::geometry_works::Point(0xf200, 0x0300),
                                                   utils::geometry_works::Point(0xf200, 0x0700),
                                                   utils::geometry_works::Point(0xfd00, 0xfb00),
                                                   utils::geometry_works::Point(0xf200, 0xf400),
                                               }),
                                           },
                                   },
                               .encounters{pe2_encounter_t{
                                               .id = 1,
                                               .mask = RANDOMIZED,
                                               .encounter_value = 300,
                                               .enemy_record_descriptor_list_addr_offset = 0x5c14,
                                               .enemy_resources_desciptors_list_offset = 0x5a30,
                                           },
                                           pe2_encounter_t{
                                               .id = 2,
                                               .mask = RANDOMIZED,
                                               .encounter_value = 300,
                                               .enemy_record_descriptor_list_addr_offset = 0x5c1c,
                                               .enemy_resources_desciptors_list_offset = 0x5a48,
                                           },
                                           pe2_encounter_t{
                                               .id = 7,
                                               .mask = RANDOMIZED,
                                               .encounter_value = 500,
                                               .enemy_record_descriptor_list_addr_offset = 0x5c3c,
                                               .enemy_resources_desciptors_list_offset = 0x5a84,
                                           },
                                           pe2_encounter_t{
                                               .id = 3,
                                               .mask = RANDOMIZED,
                                               .encounter_value = 300,
                                               .enemy_record_descriptor_list_addr_offset = 0x5c24,
                                               .enemy_resources_desciptors_list_offset = 0x5a6c,
                                           },
                                           pe2_encounter_t{
                                               .id = 8,
                                               .mask = RANDOMIZED,
                                               .encounter_value = 500,
                                               .enemy_record_descriptor_list_addr_offset = 0x5c44,
                                               .enemy_resources_desciptors_list_offset = 0x5aa8,
                                           }},
                               .file_patch_map{// Disable trigger with jumping worms
                                               {0x33e, 0x45}}},

         pe2_room_encounters_t{
             .id = 0x0a,
             .name = game::ROOM_AKROPOLIS_OBSERVATORY,
             .encounters_offset = 0x1d8c,
             .additional_file_end_pointer = 0x748,
             .environment =
                 pe2_room_environment_t{
                     .spawn_zones = std::vector<utils::geometry_works::Zone>{utils::geometry_works::Zone(
                         utils::geometry_works::Polygon{
                             utils::geometry_works::Point(0xed00, 0xf300),
                             utils::geometry_works::Point(0xfa00, 0xf300),
                             utils::geometry_works::Point(0xfa00, 0xdb00),
                             utils::geometry_works::Point(0xed00, 0xdb00),
                         },
                         0xf452)}},
             .encounters{pe2_encounter_t{
                             .id = 1,
                             .mask = FORBID_ALL_SLOTS_ENC,
                             .encounter_value = 300,
                             .enemy_record_descriptor_list_addr_offset = 0x3cac,
                             .enemy_resources_desciptors_list_offset = 0x3a58,
                         },
                         pe2_encounter_t{
                             .id = 2,
                             .mask = RANDOMIZED,
                             .encounter_value = 300,
                             .enemy_record_descriptor_list_addr_offset = 0x3cb4,
                             .enemy_resources_desciptors_list_offset = 0x3a70,
                         },
                         pe2_encounter_t{
                             .id = 7,
                             .mask = RANDOMIZED,
                             .encounter_value = 500,
                             .enemy_record_descriptor_list_addr_offset = 0x3cdc,
                             .enemy_resources_desciptors_list_offset = 0x3aa0,
                         },
                         pe2_encounter_t{
                             .id = 3,
                             .mask = RANDOMIZED,
                             .encounter_value = 300,
                             .enemy_record_descriptor_list_addr_offset = 0x3cbc,
                             .enemy_resources_desciptors_list_offset = 0x3a88,
                         }},
             .forbidden_enemies{
                 0x0a // combat light can move them aut of bounds here
             },
             .file_patch_map{
                 // Disables escalator backwards animation
                 {0xe6, 0x45},
                 // Diables bat spawn
                 {0x23e, 0x45},
             }},

         pe2_room_encounters_t{
             .id = 0x0b,
             .name = game::ROOM_AKROPOLIS_PROMENADE,
             .encounters_offset = 0x1edc,
             .additional_file_end_pointer = 0x750,
             .environment =
                 pe2_room_environment_t{
                     .spawn_zones =
                         std::vector<utils::geometry_works::Zone>{
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0xf700, 0xf700),
                                 utils::geometry_works::Point(0x0000, 0xf700),
                                 utils::geometry_works::Point(0x0000, 0xe100),
                                 utils::geometry_works::Point(0xf700, 0xe400),
                             }),
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0xf700, 0x0900),
                                 utils::geometry_works::Point(0x0000, 0x0c00),
                                 utils::geometry_works::Point(0x0000, 0xf700),
                                 utils::geometry_works::Point(0xf700, 0xf600),
                             }),
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0xfc00, 0x2200),
                                 utils::geometry_works::Point(0x0000, 0x2500),
                                 utils::geometry_works::Point(0x0000, 0x0a00),
                                 utils::geometry_works::Point(0xfc00, 0x0a00),
                             })},
                 },
             .forbid_ambush_encounters = true,
             .encounters{pe2_encounter_t{
                             .id = 1,
                             .mask = FORBID_ALL_SLOTS_ENC,
                             .encounter_value = 300,
                             .enemy_record_descriptor_list_addr_offset = 0x5a68,
                             .enemy_resources_desciptors_list_offset = 0x59c4,
                         },
                         pe2_encounter_t{
                             .id = 2,
                             .mask = RANDOMIZED,
                             .encounter_value = 300,
                             .enemy_record_descriptor_list_addr_offset = 0x5a70,
                             .enemy_resources_desciptors_list_offset = 0x59dc,
                         },
                         pe2_encounter_t{
                             .id = 7,
                             .mask = RANDOMIZED,
                             .encounter_value = 500,
                             .enemy_record_descriptor_list_addr_offset = 0x5a98,
                             .enemy_resources_desciptors_list_offset = 0x5a24,
                         },
                         pe2_encounter_t{
                             .id = 3,
                             .mask = RANDOMIZED,
                             .encounter_value = 300,
                             .enemy_record_descriptor_list_addr_offset = 0x5a78,
                             .enemy_resources_desciptors_list_offset = 0x5a00,
                         },
                         pe2_encounter_t{
                             .id = 8,
                             .mask = RANDOMIZED,
                             .encounter_value = 500,
                             .enemy_record_descriptor_list_addr_offset = 0x5aa0,
                             .enemy_resources_desciptors_list_offset = 0x5a3c,
                         }},
         },

         pe2_room_encounters_t{
             .id = 0x0c,
             .name = game::ROOM_AKROPOLIS_SANCTUARY,
             .encounters_offset = 0x210c,
             .additional_file_end_pointer = 0x758,
             .encounters{
                 pe2_encounter_t{
                     .id = 1,
                     .mask = UNRANDOMIZED,
                     .encounter_value = 0,
                     .enemy_record_descriptor_list_addr_offset = 0x6a74,
                     .enemy_resources_desciptors_list_offset = 0x6a30,
                 },
                 pe2_encounter_t{
                     .id = 2,
                     .mask = UNRANDOMIZED,
                     .encounter_value = 0,
                     .enemy_record_descriptor_list_addr_offset = 0x6a7c,
                     .enemy_resources_desciptors_list_offset = 0x6a54,
                 },
             },
             .file_patch_map{
                 // Manipulates bridge raise
                 {0x88, 0x02},
                 // Manipulates bridge raise
                 {0x90, 0x03},
                 // Diables bomb cut scene
                 {0x2b0, 0x00},
                 {0x2b1, 0x00},
                 {0x2b2, 0x00},
                 {0x2b3, 0x00},
                 {0x2c8, 0x00},
                 {0x2c9, 0x00},
                 {0x2ca, 0x00},
                 {0x2cb, 0x00},
             },
         },

         pe2_room_encounters_t{
             .id = 0x0d,
             .name = game::ROOM_AKROPOLIS_ROOF_GARDEN,
             .encounters_offset = 0x216c,
             .additional_file_end_pointer = 0x760,
             .environment{.spawn_zones =
                              std::vector<utils::geometry_works::Zone>{
                                  utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                      utils::geometry_works::Point(0xe100, 0xdf00),
                                      utils::geometry_works::Point(0xee00, 0xdf00),
                                      utils::geometry_works::Point(0xee00, 0xe500),
                                      utils::geometry_works::Point(0xe100, 0xe500),
                                  }),
                                  utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                      utils::geometry_works::Point(0xe700, 0xe700),
                                      utils::geometry_works::Point(0xf400, 0xe700),
                                      utils::geometry_works::Point(0xf400, 0xf500),
                                      utils::geometry_works::Point(0xe700, 0xf500),
                                  }),
                              },
                          .dead_zones =
                              std::vector<utils::geometry_works::Zone>{
                                  utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                      utils::geometry_works::Point(0xec00, 0xf300),
                                      utils::geometry_works::Point(0xef00, 0xf300),
                                      utils::geometry_works::Point(0xef00, 0xed00),
                                      utils::geometry_works::Point(0xec00, 0xed00),
                                  })}},
             .encounters{
                 pe2_encounter_t{
                     .id = 1,
                     .mask = RANDOMIZED,
                     .encounter_value = 300,
                     .enemy_record_descriptor_list_addr_offset = 0x837c,
                     .enemy_resources_desciptors_list_offset = 0x82e4,
                 },
                 pe2_encounter_t{
                     .id = 7,
                     .mask = RANDOMIZED,
                     .encounter_value = 500,
                     .enemy_record_descriptor_list_addr_offset = 0x83ac,
                     .enemy_resources_desciptors_list_offset = 0x832c,
                 },
                 pe2_encounter_t{
                     .id = 2,
                     .mask = RANDOMIZED,
                     .encounter_value = 300,
                     .enemy_record_descriptor_list_addr_offset = 0x8384,
                     .enemy_resources_desciptors_list_offset = 0x8308,
                 },
                 pe2_encounter_t{
                     .id = 8,
                     .mask = RANDOMIZED,
                     .encounter_value = 500,
                     .enemy_record_descriptor_list_addr_offset = 0x83b4,
                     .enemy_resources_desciptors_list_offset = 0x8350,
                 },
             },
             .file_patch_map{
                 // Disables worm animation trigger
                 {0x268, 0x00},
                 {0x269, 0x00},
                 {0x26a, 0x00},
                 {0x26b, 0x00},
                 {0x27c, 0x00},
                 {0x27d, 0x00},
                 {0x27e, 0x00},
                 {0x27f, 0x00},
                 {0x28c, 0x00},
                 {0x28d, 0x00},
                 {0x28e, 0x00},
                 {0x28f, 0x00},
             },
         },

         pe2_room_encounters_t{
             .id = 0x0e,
             .name = game::ROOM_AKROPOLIS_BRIDGE,
             .encounters_offset = 0x232c,
             .additional_file_end_pointer = 0x768,
             .environment{
                 .spawn_zones = std::vector<utils::geometry_works::Zone>{utils::geometry_works::Zone(
                     utils::geometry_works::Polygon{
                         utils::geometry_works::Point(0xcd00, 0xfc00),
                         utils::geometry_works::Point(0xe600, 0xfc00),
                         utils::geometry_works::Point(0xe600, 0xf400),
                         utils::geometry_works::Point(0xcd00, 0xf400),
                     },
                     0xfc9a)},
             },
             .encounters{pe2_encounter_t{
                 .id = 1,
                 .mask = FORBID_ALL_SLOTS_ENC,
                 .encounter_value = 300,
                 .enemy_record_descriptor_list_addr_offset = 0x12a94,
                 .enemy_resources_desciptors_list_offset = 0x12a5c,
             }},
             .file_patch_map{
                 // Disables water animation
                 {0x2b94, 0x00}, {0x2b95, 0x00}, {0x2b96, 0x00}, {0x2b97, 0x00}, {0x2bd8, 0x00},
                 {0x2bd9, 0x00}, {0x2bda, 0x00}, {0x2bdb, 0x00}, {0x2c6c, 0x00}, {0x2c6d, 0x00},
                 {0x2c6e, 0x00}, {0x2c6f, 0x00}, {0x2cb0, 0x00}, {0x2cb1, 0x00}, {0x2cb2, 0x00},
                 {0x2cb3, 0x00}, {0x2d1c, 0x00}, {0x2d1d, 0x00}, {0x2d1e, 0x00}, {0x2d1f, 0x00},
             },
         },

         pe2_room_encounters_t{
             .id = 0x0f,
             .name = game::ROOM_AKROPOLIS_FIRE_ESCAPE,
             .encounters_offset = 0x23bc,
             .additional_file_end_pointer = 0x770,
             .environment{
                 .spawn_zones =
                     std::vector<utils::geometry_works::Zone>{
                         utils::geometry_works::Zone(utils::geometry_works::Polygon{
                             utils::geometry_works::Point(0x0600, 0x0b00),
                             utils::geometry_works::Point(0x0a00, 0x0b00),
                             utils::geometry_works::Point(0x0a00, 0xf900),
                             utils::geometry_works::Point(0x0600, 0xf900),
                         }),
                         utils::geometry_works::Zone(utils::geometry_works::Polygon{
                             utils::geometry_works::Point(0xfe00, 0x1100),
                             utils::geometry_works::Point(0x0900, 0x1100),
                             utils::geometry_works::Point(0x0900, 0x0c00),
                             utils::geometry_works::Point(0xfe00, 0x0c00),
                         })},
             },
             .encounters{pe2_encounter_t{
                 .id = 1,
                 .mask = RANDOMIZED,
                 .encounter_value = 150,
                 .enemy_record_descriptor_list_addr_offset = 0x5394,
                 .enemy_resources_desciptors_list_offset = 0x5374,
             }},
         },

         pe2_room_encounters_t{
             .name = game::ROOM_AKROPOLIS_HELICOPTER_PAD,
             .encounters_offset = 0x23dc,
             .additional_file_end_pointer = 0x778,
             .encounters{pe2_encounter_t{
                             .id = 1,
                             .mask = UNRANDOMIZED,
                             .encounter_value = 0,
                             .enemy_record_descriptor_list_addr_offset = 0x8c30,
                             .enemy_resources_desciptors_list_offset = 0x8be0,
                         },
                         pe2_encounter_t{
                             .id = 2,
                             .mask = UNRANDOMIZED,
                             .encounter_value = 0,
                             .enemy_record_descriptor_list_addr_offset = 0x8c38,
                             .enemy_resources_desciptors_list_offset = 0x8bf9,
                         }},
         },

         pe2_room_encounters_t{.name = game::ROOM_AKROPOLIS_WEST_ELEVATOR_HALL,
                               .encounters_offset = 0,
                               .additional_file_end_pointer = 0x780,
                               .file_patch_map{// Moves progression forward to SWAT dead body as a sprite
                                               {0x1f70, 0x04}}},

         pe2_room_encounters_t{
             .name = game::ROOM_AKROPOLIS_BALDWINS_CABINET,
             .encounters_offset = 0x243c,
             .additional_file_end_pointer = 0x788,
             .encounters{
                 pe2_encounter_t{
                     .id = 1,
                     .mask = UNRANDOMIZED,
                     .encounter_value = 0,
                     .enemy_record_descriptor_list_addr_offset = 0x0,
                     .enemy_resources_desciptors_list_offset = 0x9644,
                 },
             },
         },

         pe2_room_encounters_t{
             .name = game::ROOM_AKROPOLIS_MIST_PARKING,
             .encounters_offset = 0x246c,
             .additional_file_end_pointer = 0x790,
             .encounters{
                 pe2_encounter_t{
                     .id = 1,
                     .mask = UNRANDOMIZED,
                     .encounter_value = 0,
                     .enemy_record_descriptor_list_addr_offset = 0x0,
                     .enemy_resources_desciptors_list_offset = 0x17bfc,
                 },
                 pe2_encounter_t{
                     .id = 2,
                     .mask = UNRANDOMIZED,
                     .encounter_value = 0,
                     .enemy_record_descriptor_list_addr_offset = 0x0,
                     .enemy_resources_desciptors_list_offset = 0x17c04,
                 },
             },
         },

         pe2_room_encounters_t{
             .name = game::ROOM_AKROPOLIS_SHOOTING_GALLERY,
             .encounters_offset = 0x249c,
             .additional_file_end_pointer = 0x798,
             .encounters{
                 pe2_encounter_t{
                     .id = 1,
                     .mask = UNRANDOMIZED,
                     .encounter_value = 0,
                     .enemy_record_descriptor_list_addr_offset = 0x0,
                     .enemy_resources_desciptors_list_offset = 0x109bc,
                 },
             },
         },
     }},
    {game::DRYFIELD_DAY_LEVEL,
     {
         pe2_room_encounters_t{.id = 0x02,
                               .name = game::ROOM_DRYFIELD_DAY_MAIN_STREET,
                               .encounters_offset = 0x13f4,
                               .additional_file_end_pointer = 0xd4,
                               .environment =
                                   pe2_room_environment_t{
                                       .spawn_zones =
                                           std::vector<utils::geometry_works::Zone>{
                                               utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                                   utils::geometry_works::Point(0xe5d4, 0x224c),
                                                   utils::geometry_works::Point(0xe5d4, 0xf491),
                                                   utils::geometry_works::Point(0xfd87, 0xf491),
                                                   utils::geometry_works::Point(0xfd82, 0x224c),
                                               }),
                                               utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                                   utils::geometry_works::Point(0xe5d4, 0x224c),
                                                   utils::geometry_works::Point(0xe5d4, 0x29cc),
                                                   utils::geometry_works::Point(0xfa26, 0x29cc),
                                                   utils::geometry_works::Point(0xfa0e, 0x224c),
                                               }),
                                               utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                                   utils::geometry_works::Point(0xee1c, 0xe962),
                                                   utils::geometry_works::Point(0xee26, 0xf491),
                                                   utils::geometry_works::Point(0xfd83, 0xf491),
                                                   utils::geometry_works::Point(0xf8de, 0xeb52),
                                               }),
                                           },
                                       .dead_zones =
                                           std::vector<utils::geometry_works::Zone>{
                                               utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                                   utils::geometry_works::Point(0xea00, 0xfc05),
                                                   utils::geometry_works::Point(0xe990, 0x023e),
                                                   utils::geometry_works::Point(0xedb6, 0x034b),
                                                   utils::geometry_works::Point(0xee0c, 0xfbf3),
                                               }),
                                               utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                                   utils::geometry_works::Point(0xe9b1, 0x0f99),
                                                   utils::geometry_works::Point(0xe9f1, 0x16f7),
                                                   utils::geometry_works::Point(0xee54, 0x1694),
                                                   utils::geometry_works::Point(0xeea8, 0x0f1c),
                                               }),
                                               utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                                   utils::geometry_works::Point(0xea12, 0x1fcf),
                                                   utils::geometry_works::Point(0xe9c1, 0x2638),
                                                   utils::geometry_works::Point(0xf058, 0x25e0),
                                                   utils::geometry_works::Point(0xefe6, 0x1f4a),
                                               }),
                                           },
                                   },
                               .encounters{pe2_encounter_t{
                                               .id = 1,
                                               .mask = RANDOMIZED,
                                               .encounter_value = 350,
                                               .enemy_record_descriptor_list_addr_offset = 0x7968,
                                               .enemy_resources_desciptors_list_offset = 0x7894,
                                           },
                                           pe2_encounter_t{
                                               .id = 2,
                                               .mask = RANDOMIZED,
                                               .encounter_value = 350,
                                               .enemy_record_descriptor_list_addr_offset = 0x7970,
                                               .enemy_resources_desciptors_list_offset = 0x78b8,
                                           },
                                           pe2_encounter_t{
                                               .id = 3,
                                               .mask = RANDOMIZED,
                                               .encounter_value = 350,
                                               .enemy_record_descriptor_list_addr_offset = 0x7978,
                                               .enemy_resources_desciptors_list_offset = 0x78d0,
                                           },
                                           pe2_encounter_t{
                                               .id = 7,
                                               .mask = RANDOMIZED,
                                               .encounter_value = 350,
                                               .enemy_record_descriptor_list_addr_offset = 0x7998,
                                               .enemy_resources_desciptors_list_offset = 0x78e8,
                                           }},
                               .file_patch_map{
                                   // Disable chaser cutscene
                                   // {0xaae, 0x45},
                                   {0xb58, 0x00},
                                   {0xb59, 0x00},
                                   {0xb5a, 0x00},
                                   {0xb5b, 0x00},
                                   {0xb70, 0x00},
                                   {0xb71, 0x00},
                                   {0xb72, 0x00},
                                   {0xb73, 0x00},
                                   {0xb3c, 0xa3},
                                   {0xadc, 0x00},
                                   {0xadd, 0x00},
                                   {0xade, 0x00},
                                   {0xadf, 0x00},
                               }},
         pe2_room_encounters_t{.id = 0x03,
                               .name = game::ROOM_DRYFIELD_DAY_GENERAL_STORE,
                               .encounters_offset = 0x14f4,
                               .additional_file_end_pointer = 0xdc,
                               .environment =
                                   pe2_room_environment_t{
                                       .spawn_zones =
                                           std::vector<utils::geometry_works::Zone>{
                                               utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                                   utils::geometry_works::Point(0x0404, 0x1450),
                                                   utils::geometry_works::Point(0x0413, 0x0c80),
                                                   utils::geometry_works::Point(0x0ca2, 0x0c80),
                                                   utils::geometry_works::Point(0x0c4e, 0x1450),
                                               }),
                                               utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                                   utils::geometry_works::Point(0x0e30, 0x1450),
                                                   utils::geometry_works::Point(0x0f1d, 0x0c80),
                                                   utils::geometry_works::Point(0x23f0, 0x0c80),
                                                   utils::geometry_works::Point(0x242a, 0x1450),
                                               }),
                                               utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                                   utils::geometry_works::Point(0x23f0, 0x0c98),
                                                   utils::geometry_works::Point(0x092e, 0x0c98),
                                                   utils::geometry_works::Point(0x092e, 0x012c),
                                                   utils::geometry_works::Point(0x23f0, 0x012c),
                                               }),
                                               utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                                   utils::geometry_works::Point(0x157c, 0x1d3c),
                                                   utils::geometry_works::Point(0x157c, 0x1996),
                                                   utils::geometry_works::Point(0x0514, 0x1996),
                                                   utils::geometry_works::Point(0x0514, 0x1d3c),
                                               }),
                                               utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                                   utils::geometry_works::Point(0x157c, 0x1e14),
                                                   utils::geometry_works::Point(0x157c, 0x1996),
                                                   utils::geometry_works::Point(0x206c, 0x1996),
                                                   utils::geometry_works::Point(0x1fca, 0x1e14),
                                               }),
                                           },
                                       .dead_zones =
                                           std::vector<utils::geometry_works::Zone>{
                                               utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                                   utils::geometry_works::Point(0x0c7e, 0x0c67),
                                                   utils::geometry_works::Point(0x2086, 0x0c80),
                                                   utils::geometry_works::Point(0x20d0, 0x12c0),
                                                   utils::geometry_works::Point(0x0c4c, 0x12c0),
                                               }),
                                               utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                                   utils::geometry_works::Point(0x0d49, 0x0b25),
                                                   utils::geometry_works::Point(0x0d16, 0x04b3),
                                                   utils::geometry_works::Point(0x20df, 0x04c1),
                                                   utils::geometry_works::Point(0x20c9, 0x0b16),
                                               }),
                                               utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                                   utils::geometry_works::Point(0x209e, 0x012c),
                                                   utils::geometry_works::Point(0x20a0, 0x045b),
                                                   utils::geometry_works::Point(0x23f0, 0x047e),
                                                   utils::geometry_works::Point(0x23f0, 0x012c),
                                               }),
                                               utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                                   utils::geometry_works::Point(0x0d09, 0x011c),
                                                   utils::geometry_works::Point(0x0e12, 0x02ee),
                                                   utils::geometry_works::Point(0x12c9, 0x0276),
                                                   utils::geometry_works::Point(0x12c0, 0x011c),
                                               }),
                                           },
                                   },
                               .encounters{pe2_encounter_t{
                                   .id = 1,
                                   .mask = RANDOMIZED,
                                   .encounter_value = 350,
                                   .enemy_record_descriptor_list_addr_offset = 0x809c,
                                   .enemy_resources_desciptors_list_offset = 0x7fc8,
                               }},
                               .file_patch_map{
                                   // Disable suckler sink cutscene
                                   {0x872, 0x45},
                                   {0x910, 0x00},
                                   {0x911, 0x00},
                                   {0x912, 0x00},
                                   {0x913, 0x00},
                               }},
         pe2_room_encounters_t{
             .id = 0x05,
             .name = game::ROOM_DRYFIELD_DAY_BACK_STREET,
             .encounters_offset = 0x1554,
             .additional_file_end_pointer = 0xec,
             .environment =
                 pe2_room_environment_t{
                     .spawn_zones =
                         std::vector<utils::geometry_works::Zone>{
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0x1acf, 0x08f4),
                                 utils::geometry_works::Point(0x1acf, 0x1646),
                                 utils::geometry_works::Point(0x2d91, 0x163d),
                                 utils::geometry_works::Point(0x2d91, 0x08f4),
                             }),
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0x1acf, 0x08f4),
                                 utils::geometry_works::Point(0x0ae6, 0x10d3),
                                 utils::geometry_works::Point(0x0ae6, 0x1646),
                                 utils::geometry_works::Point(0x1acf, 0x15dc),
                             }),
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0x0ae6, 0x1646),
                                 utils::geometry_works::Point(0x0ae6, 0x0ed0),
                                 utils::geometry_works::Point(0xda1c, 0x0ed0),
                                 utils::geometry_works::Point(0xda1c, 0x1646),
                             }),
                         },
                     .dead_zones =
                         std::vector<utils::geometry_works::Zone>{
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0x1dd8, 0x07f7),
                                 utils::geometry_works::Point(0x1d9d, 0x0b12),
                                 utils::geometry_works::Point(0x22dc, 0x0af1),
                                 utils::geometry_works::Point(0x2314, 0x07f4),
                             }),
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0x2d8f, 0x133f),
                                 utils::geometry_works::Point(0x274f, 0x137d),
                                 utils::geometry_works::Point(0x270b, 0x173d),
                                 utils::geometry_works::Point(0x2d8f, 0x173d),
                             }),
                         },
                 },
             .encounters{pe2_encounter_t{
                             .id = 1,
                             .mask = RANDOMIZED,
                             .encounter_value = 350,
                             .enemy_record_descriptor_list_addr_offset = 0x3498,
                             .enemy_resources_desciptors_list_offset = 0x340c,
                         },
                         pe2_encounter_t{
                             .id = 7,
                             .mask = RANDOMIZED,
                             .encounter_value = 550,
                             .enemy_record_descriptor_list_addr_offset = 0x34c8,
                             .enemy_resources_desciptors_list_offset = 0x3424,
                         }},
         },
         pe2_room_encounters_t{
             .id = 0x06,
             .name = game::ROOM_DRYFIELD_DAY_SOUVENIR_SHOP,
             .encounters_offset = 0x15b4,
             .additional_file_end_pointer = 0xf4,
             .environment =
                 pe2_room_environment_t{
                     .spawn_zones =
                         std::vector<utils::geometry_works::Zone>{
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0x15d1, 0xf3b2),
                                 utils::geometry_works::Point(0x047a, 0xf387),
                                 utils::geometry_works::Point(0x044c, 0xfcf9),
                                 utils::geometry_works::Point(0x1482, 0xfd24),
                             }),
                         },
                     .dead_zones =
                         std::vector<utils::geometry_works::Zone>{
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0x1682, 0xfb5f),
                                 utils::geometry_works::Point(0x0e4d, 0xfb5f),
                                 utils::geometry_works::Point(0x0e15, 0xf529),
                                 utils::geometry_works::Point(0x16ea, 0xf529),
                             }),
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0x0d25, 0xf9fe),
                                 utils::geometry_works::Point(0x0665, 0xf9f2),
                                 utils::geometry_works::Point(0x0627, 0xf539),
                                 utils::geometry_works::Point(0x0e32, 0xf529),
                             }),
                         },
                 },
             .encounters{pe2_encounter_t{
                 .id = 1,
                 .mask = RANDOMIZED,
                 .encounter_value = 350,
                 .enemy_record_descriptor_list_addr_offset = 0x1fe0,
                 .enemy_resources_desciptors_list_offset = 0x1fb4,
             }},
         },
         pe2_room_encounters_t{
             .name = game::ROOM_DRYFIELD_DAY_LABORATORY_ROOM,
             .encounters_offset = 0x15f4,
             .additional_file_end_pointer = 0x104,
             .encounters{pe2_encounter_t{
                 .id = 1,
                 .mask = UNRANDOMIZED,
                 .encounter_value = 0,
                 .enemy_record_descriptor_list_addr_offset = 0x35d0,
                 .enemy_resources_desciptors_list_offset = 0,
             }},
         },
         pe2_room_encounters_t{
             .name = game::ROOM_DRYFIELD_DAY_DILAPIDATED_HOUSE,
             .encounters_offset = 0x1614,
             .additional_file_end_pointer = 0x10c,
             .encounters{pe2_encounter_t{
                 .id = 1,
                 .mask = UNRANDOMIZED,
                 .encounter_value = 0,
                 .enemy_record_descriptor_list_addr_offset = 0xc380,
                 .enemy_resources_desciptors_list_offset = 0xc354,
             }},
         },
         pe2_room_encounters_t{
             .id = 0x0b,
             .name = game::ROOM_DRYFIELD_DAY_MOTEL_ROOM_1,
             .encounters_offset = 0x1654,
             .additional_file_end_pointer = 0x11c,
             .environment =
                 pe2_room_environment_t{
                     .spawn_zones =
                         std::vector<utils::geometry_works::Zone>{
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0x0100, 0x0e00),
                                 utils::geometry_works::Point(0x0100, 0x0900),
                                 utils::geometry_works::Point(0x1280, 0x0900),
                                 utils::geometry_works::Point(0x1280, 0x0e00),
                             }),
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0x0d00, 0x0e00),
                                 utils::geometry_works::Point(0x0d00, 0x1700),
                                 utils::geometry_works::Point(0x1280, 0x1700),
                                 utils::geometry_works::Point(0x1280, 0x0e00),
                             }),
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0x0100, 0x1300),
                                 utils::geometry_works::Point(0x0100, 0x1700),
                                 utils::geometry_works::Point(0x0d00, 0x1700),
                                 utils::geometry_works::Point(0x0d00, 0x1300),
                             }),
                         },
                     .spawn_aerial_zones =
                         std::vector<utils::geometry_works::Zone>{
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0x0100, 0x0e00),
                                 utils::geometry_works::Point(0x0100, 0x0100),
                                 utils::geometry_works::Point(0x1280, 0x0100),
                                 utils::geometry_works::Point(0x1280, 0x0e00),
                             }),
                         },
                 },
             .encounters{pe2_encounter_t{
                             .id = 1,
                             .mask = UNRANDOMIZED,
                             .encounter_value = 0,
                             .enemy_record_descriptor_list_addr_offset = 0x3f24,
                             .enemy_resources_desciptors_list_offset = 0x3e30,
                         },
                         pe2_encounter_t{
                             .id = 2,
                             .mask = RANDOMIZED,
                             .encounter_value = 350,
                             .enemy_record_descriptor_list_addr_offset = 0x3f2c,
                             .enemy_resources_desciptors_list_offset = 0x3e3c,
                         },
                         pe2_encounter_t{
                             .id = 3,
                             .mask = RANDOMIZED,
                             .encounter_value = 350,
                             .enemy_record_descriptor_list_addr_offset = 0x3f34,
                             .enemy_resources_desciptors_list_offset = 0x3e60,
                         },
                         pe2_encounter_t{
                             .id = 7,
                             .mask = RANDOMIZED,
                             .encounter_value = 350,
                             .enemy_record_descriptor_list_addr_offset = 0x3f54,
                             .enemy_resources_desciptors_list_offset = 0x3e84,
                         }},
             .file_patch_map{
                 // Disable suckler cutscene
                 {0x88, 0x00},
                 {0x89, 0x00},
                 {0x8a, 0x00},
                 {0x8b, 0x00},
                 {0xc0, 0x00},
                 {0xc1, 0x00},
                 {0xc2, 0x00},
                 {0xc3, 0x00},
                 {0x120, 0x00},
                 {0x121, 0x00},
                 {0x122, 0x00},
                 {0x123, 0x00},
             }},
         pe2_room_encounters_t{
             .id = 0x0c,
             .name = game::ROOM_DRYFIELD_DAY_MOTEL_ROOM_2,
             .encounters_offset = 0x1794,
             .additional_file_end_pointer = 0x124,
             .environment =
                 pe2_room_environment_t{
                     .spawn_zones =
                         std::vector<utils::geometry_works::Zone>{
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0x1200, 0x0d00),
                                 utils::geometry_works::Point(0x1200, 0x0900),
                                 utils::geometry_works::Point(0x0200, 0x0900),
                                 utils::geometry_works::Point(0x0200, 0x0d00),
                             }),
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0x0480, 0x0f00),
                                 utils::geometry_works::Point(0x0480, 0x1500),
                                 utils::geometry_works::Point(0x0180, 0x1500),
                                 utils::geometry_works::Point(0x0180, 0x0f00),
                             }),
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0x0900, 0x1100),
                                 utils::geometry_works::Point(0x0900, 0x1500),
                                 utils::geometry_works::Point(0x1200, 0x1500),
                                 utils::geometry_works::Point(0x1200, 0x1100),
                             }),
                         },
                     .spawn_aerial_zones =
                         std::vector<utils::geometry_works::Zone>{
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0x1200, 0x0f00),
                                 utils::geometry_works::Point(0x1200, 0x0100),
                                 utils::geometry_works::Point(0x0100, 0x0100),
                                 utils::geometry_works::Point(0x0100, 0x0f00),
                             }),
                         },
                 },
             .encounters{
                 pe2_encounter_t{
                     .id = 1,
                     .mask = RANDOMIZED,
                     .encounter_value = 350,
                     .enemy_record_descriptor_list_addr_offset = 0x2e58,
                     .enemy_resources_desciptors_list_offset = 0x2df0,
                 },
                 pe2_encounter_t{
                     .id = 2,
                     .mask = RANDOMIZED,
                     .encounter_value = 350,
                     .enemy_record_descriptor_list_addr_offset = 0x2e60,
                     .enemy_resources_desciptors_list_offset = 0x2e08,
                 },
                 pe2_encounter_t{
                     .id = 7,
                     .mask = RANDOMIZED,
                     .encounter_value = 350,
                     .enemy_record_descriptor_list_addr_offset = 0x2e88,
                     .enemy_resources_desciptors_list_offset = 0x2e2c,
                 },
             }},
         pe2_room_encounters_t{.id = 0x0f,
                               .name = game::ROOM_DRYFIELD_DAY_PARKING_LOT,
                               .encounters_offset = 0x1884,
                               .additional_file_end_pointer = 0x13c,
                               .environment =
                                   pe2_room_environment_t{
                                       .spawn_zones =
                                           std::vector<utils::geometry_works::Zone>{
                                               utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                                   utils::geometry_works::Point(0x24fe, 0x0dac),
                                                   utils::geometry_works::Point(0x24fe, 0x09b0),
                                                   utils::geometry_works::Point(0x101e, 0x09b0),
                                                   utils::geometry_works::Point(0x101e, 0x0dac),
                                               }),
                                               utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                                   utils::geometry_works::Point(0x101e, 0x026e),
                                                   utils::geometry_works::Point(0x101e, 0x0dac),
                                                   utils::geometry_works::Point(0xfe52, 0x0dac),
                                                   utils::geometry_works::Point(0xfe52, 0x026b),
                                               }),
                                               utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                                   utils::geometry_works::Point(0xfe52, 0x009d),
                                                   utils::geometry_works::Point(0xf448, 0x009d),
                                                   utils::geometry_works::Point(0xf448, 0x0a28),
                                                   utils::geometry_works::Point(0xfe52, 0x0a28),
                                               }),
                                               utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                                   utils::geometry_works::Point(0xf74c, 0x009d),
                                                   utils::geometry_works::Point(0xf18c, 0x009d),
                                                   utils::geometry_works::Point(0xf18c, 0xeee4),
                                                   utils::geometry_works::Point(0xf74c, 0xeee4),
                                               }),
                                               utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                                   utils::geometry_works::Point(0xeec4, 0xfd7c),
                                                   utils::geometry_works::Point(0xeec4, 0xeee4),
                                                   utils::geometry_works::Point(0xea6e, 0xeee4),
                                                   utils::geometry_works::Point(0xea6e, 0xfed4),
                                               }),
                                           },
                                   },
                               .forbid_ambush_encounters = true,
                               .encounters{
                                   pe2_encounter_t{
                                       .id = 1,
                                       .mask = RANDOMIZED,
                                       .encounter_value = 350,
                                       .enemy_record_descriptor_list_addr_offset = 0x24c4,
                                       .enemy_resources_desciptors_list_offset = 0x2460,
                                   },
                                   pe2_encounter_t{
                                       .id = 2,
                                       .mask = RANDOMIZED,
                                       .encounter_value = 350,
                                       .enemy_record_descriptor_list_addr_offset = 0x24cc,
                                       .enemy_resources_desciptors_list_offset = 0x2478,
                                   },
                                   pe2_encounter_t{
                                       .id = 7,
                                       .mask = RANDOMIZED,
                                       .encounter_value = 550,
                                       .enemy_record_descriptor_list_addr_offset = 0x24ec,
                                       .enemy_resources_desciptors_list_offset = 0x249c,
                                   },
                               }},
         pe2_room_encounters_t{
             .id = 0x10,
             .name = game::ROOM_DRYFIELD_DAY_TOILET,
             .encounters_offset = 0x1954,
             .additional_file_end_pointer = 0x144,
             .environment =
                 pe2_room_environment_t{
                     .spawn_zones =
                         std::vector<utils::geometry_works::Zone>{
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0x02bc, 0x06ae),
                                 utils::geometry_works::Point(0xfcba, 0x06ae),
                                 utils::geometry_works::Point(0xfcba, 0xf952),
                                 utils::geometry_works::Point(0x02bc, 0xf952),
                             }),
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0xf67a, 0xf9f9),
                                 utils::geometry_works::Point(0xf67a, 0x02f0),
                                 utils::geometry_works::Point(0xfac4, 0x02f0),
                                 utils::geometry_works::Point(0xfac4, 0xf952),
                             }),
                         },
                 },
             .forbid_ambush_encounters = true,
             .encounters{
                 pe2_encounter_t{
                     .id = 1,
                     .mask = FORBID_MULTI_ENEMY_ENC_AND_SLOT_3,
                     .encounter_value = 350,
                     .enemy_record_descriptor_list_addr_offset = 0x5360,
                     .enemy_resources_desciptors_list_offset = 0x5304,
                 },
                 pe2_encounter_t{
                     .id = 2,
                     .mask = RANDOMIZED,
                     .encounter_value = 350,
                     .enemy_record_descriptor_list_addr_offset = 0x5368,
                     .enemy_resources_desciptors_list_offset = 0x5328,
                 },
                 pe2_encounter_t{
                     .id = 7,
                     .mask = RANDOMIZED,
                     .encounter_value = 550,
                     .enemy_record_descriptor_list_addr_offset = 0x5390,
                     .enemy_resources_desciptors_list_offset = 0x5340,
                 },
             },
             .forbidden_enemies{0x16, 0x27},
             .file_patch_map{
                 // Disable woman in toilet
                 {0x32f, 0x10},
                 {0x3af, 0x10},
             },
         },
         pe2_room_encounters_t{
             .id = 0x12,
             .name = game::ROOM_DRYFIELD_DAY_SALOON_GR,
             .encounters_offset = 0x1a44,
             .additional_file_end_pointer = 0x154,
             .environment =
                 pe2_room_environment_t{
                     .spawn_zones =
                         std::vector<utils::geometry_works::Zone>{
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0x0f00, 0xef00),
                                 utils::geometry_works::Point(0x0f00, 0xfa00),
                                 utils::geometry_works::Point(0x0700, 0xfa00),
                                 utils::geometry_works::Point(0x0700, 0xef00),
                             }),
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0xf600, 0xf400),
                                 utils::geometry_works::Point(0xf600, 0xf880),
                                 utils::geometry_works::Point(0x0700, 0xf880),
                                 utils::geometry_works::Point(0x0700, 0xf400),
                             }),
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0x0500, 0x1280),
                                 utils::geometry_works::Point(0x0500, 0xfa00),
                                 utils::geometry_works::Point(0x0980, 0xfa00),
                                 utils::geometry_works::Point(0x0980, 0x1280),
                             }),
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0xf700, 0xf400),
                                 utils::geometry_works::Point(0xf700, 0x0d00),
                                 utils::geometry_works::Point(0xfb00, 0x0d00),
                                 utils::geometry_works::Point(0xfb00, 0xf400),
                             }),
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0x0800, 0x1280),
                                 utils::geometry_works::Point(0x0800, 0x0a00),
                                 utils::geometry_works::Point(0xfb00, 0x0a00),
                                 utils::geometry_works::Point(0xfb00, 0x1280),
                             }),
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0x0480, 0x1800),
                                 utils::geometry_works::Point(0x0480, 0x0a00),
                                 utils::geometry_works::Point(0x0080, 0x0a00),
                                 utils::geometry_works::Point(0x0080, 0x1800),
                             }),
                         },
                     .spawn_aerial_zones =
                         std::vector<utils::geometry_works::Zone>{
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0x1280, 0x1280),
                                 utils::geometry_works::Point(0x1280, 0xee00),
                                 utils::geometry_works::Point(0xf100, 0xee00),
                                 utils::geometry_works::Point(0xf100, 0x1280),
                             }),
                         },
                 },
             .encounters{
                 pe2_encounter_t{
                     .id = 1,
                     .mask = FORBID_SLOT_3_ENEMIES,
                     .encounter_value = 350,
                     .enemy_record_descriptor_list_addr_offset = 0x4564,
                     .enemy_resources_desciptors_list_offset = 0x4520,
                 },
             },
         },
         pe2_room_encounters_t{
             .id = 0x13,
             .name = game::ROOM_DRYFIELD_DAY_GR_KITCHEN,
             .encounters_offset = 0x1b24,
             .additional_file_end_pointer = 0x15c,
             .environment =
                 pe2_room_environment_t{
                     .spawn_zones =
                         std::vector<utils::geometry_works::Zone>{
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0x0258, 0x0a8c),
                                 utils::geometry_works::Point(0x0258, 0xf830),
                                 utils::geometry_works::Point(0xfe0c, 0xf830),
                                 utils::geometry_works::Point(0xfe0c, 0x0a8c),
                             }),
                         },
                 },
             .encounters{
                 pe2_encounter_t{
                     .id = 1,
                     .mask = RANDOMIZED,
                     .encounter_value = 350,
                     .enemy_record_descriptor_list_addr_offset = 0x1f00,
                     .enemy_resources_desciptors_list_offset = 0x1ebc,
                 },
                 pe2_encounter_t{
                     .id = 2,
                     .mask = RANDOMIZED,
                     .encounter_value = 350,
                     .enemy_record_descriptor_list_addr_offset = 0x1f08,
                     .enemy_resources_desciptors_list_offset = 0x1ed4,
                 },
             },
         },
         pe2_room_encounters_t{
             .id = 0x14,
             .name = game::ROOM_DRYFIELD_DAY_WATER_TOWER,
             .encounters_offset = 0x1be4,
             .additional_file_end_pointer = 0x164,
             .environment =
                 pe2_room_environment_t{
                     .spawn_zones =
                         std::vector<utils::geometry_works::Zone>{
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0x0e42, 0x0898),
                                 utils::geometry_works::Point(0x0e42, 0xe9bc),
                                 utils::geometry_works::Point(0x1644, 0xe9bc),
                                 utils::geometry_works::Point(0x1644, 0x0898),
                             }),
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0x0c19, 0xf286),
                                 utils::geometry_works::Point(0x0c19, 0xe9bc),
                                 utils::geometry_works::Point(0xf458, 0xe9bc),
                                 utils::geometry_works::Point(0xf458, 0xf286),
                             }),
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0xe9a7, 0xf4c2),
                                 utils::geometry_works::Point(0xf286, 0xf4c2),
                                 utils::geometry_works::Point(0xf286, 0x125c),
                                 utils::geometry_works::Point(0xe9a1, 0x125c),
                             }),
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0xebb0, 0xf246),
                                 utils::geometry_works::Point(0xebb0, 0xecdc),
                                 utils::geometry_works::Point(0xf1b1, 0xecdc),
                                 utils::geometry_works::Point(0xf1b1, 0xf362),
                             }),
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0xf574, 0x1644),
                                 utils::geometry_works::Point(0xff1a, 0x1644),
                                 utils::geometry_works::Point(0xff1a, 0x0dc6),
                                 utils::geometry_works::Point(0xf574, 0x0dc6),
                             }),
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0xfa88, 0xf6e8),
                                 utils::geometry_works::Point(0xfa88, 0x09f6),
                                 utils::geometry_works::Point(0x0834, 0x0972),
                                 utils::geometry_works::Point(0x0834, 0xf60a),
                             }),
                         },
                     .dead_zones =
                         std::vector<utils::geometry_works::Zone>{
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0x0158, 0x0281),
                                 utils::geometry_works::Point(0x05fb, 0x0281),
                                 utils::geometry_works::Point(0x05fb, 0xfde5),
                                 utils::geometry_works::Point(0x019b, 0xfde5),
                             }),
                         },
                 },
             .encounters{
                 pe2_encounter_t{
                     .id = 1,
                     .mask = UNRANDOMIZED,
                     .encounter_value = 0,
                     .enemy_record_descriptor_list_addr_offset = 0x9fc4,
                     .enemy_resources_desciptors_list_offset = 0x9f3c,
                     .force_reward_value = 200,
                 },
                 pe2_encounter_t{
                     .id = 2,
                     .mask = RANDOMIZED,
                     .encounter_value = 350,
                     .enemy_record_descriptor_list_addr_offset = 0x9fcc,
                     .enemy_resources_desciptors_list_offset = 0x9f54,
                 },
                 pe2_encounter_t{
                     .id = 3,
                     .mask = RANDOMIZED,
                     .encounter_value = 350,
                     .enemy_record_descriptor_list_addr_offset = 0x9fd4,
                     .enemy_resources_desciptors_list_offset = 0x9f6c,
                 },
             },
         },
         pe2_room_encounters_t{
             .id = 0x15,
             .name = game::ROOM_DRYFIELD_DAY_WATER_TANK,
             .encounters_offset = 0x1cf4,
             .additional_file_end_pointer = 0x16c,
             .encounters{
                 pe2_encounter_t{
                     .id = 1,
                     .mask = UNRANDOMIZED,
                     .encounter_value = 0,
                     .enemy_record_descriptor_list_addr_offset = 0xb638,
                     .enemy_resources_desciptors_list_offset = 0xb60c,
                 },
                 pe2_encounter_t{
                     .id = 2,
                     .mask = UNRANDOMIZED,
                     .encounter_value = 0,
                     .enemy_record_descriptor_list_addr_offset = 0xb640,
                     .enemy_resources_desciptors_list_offset = 0xb618,
                 },
             },
         },
         pe2_room_encounters_t{
             .id = 0x16,
             .name = game::ROOM_DRYFIELD_DAY_BREEZEWAY,
             .encounters_offset = 0x1d24,
             .additional_file_end_pointer = 0x174,
             .environment =
                 pe2_room_environment_t{
                     .spawn_zones =
                         std::vector<utils::geometry_works::Zone>{
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0x36e0, 0x07f6),
                                 utils::geometry_works::Point(0x36e0, 0x0514),
                                 utils::geometry_works::Point(0x4524, 0x0542),
                                 utils::geometry_works::Point(0x4524, 0x0e74),
                             }),
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0x34b5, 0x076d),
                                 utils::geometry_works::Point(0x1900, 0x076d),
                                 utils::geometry_works::Point(0x1900, 0x0514),
                                 utils::geometry_works::Point(0x34b5, 0x0514),
                             }),
                         },
                 },
             .encounters{
                 pe2_encounter_t{
                     .id = 1,
                     .mask = RANDOMIZED,
                     .encounter_value = 350,
                     .enemy_record_descriptor_list_addr_offset = 0x6d40,
                     .enemy_resources_desciptors_list_offset = 0x6ca8,
                 },
                 pe2_encounter_t{
                     .id = 3,
                     .mask = RANDOMIZED,
                     .encounter_value = 350,
                     .enemy_record_descriptor_list_addr_offset = 0x6d50,
                     .enemy_resources_desciptors_list_offset = 0x6ccc,
                 },
             },
             .file_patch_map{
                 // Disable chaser drop down cutscene
                 {0x632, 0x45},
                 {0x2a74, 0x00},
                 {0x2a75, 0x00},
                 {0x2a76, 0x00},
                 {0x2a77, 0x00},
                 {0x2b6c, 0x00},
                 {0x2b6d, 0x00},
                 {0x2b6e, 0x00},
                 {0x2b6f, 0x00},
                 {0x2b8c, 0x00},
                 {0x2b8d, 0x00},
                 {0x2b8e, 0x00},
                 {0x2b8f, 0x00},
             },
         },
         pe2_room_encounters_t{
             .id = 0x18,
             .name = game::ROOM_DRYFIELD_DAY_GARAGE,
             .encounters_offset = 0x1dd4,
             .additional_file_end_pointer = 0x184,
             .environment =
                 pe2_room_environment_t{
                     .spawn_zones =
                         std::vector<utils::geometry_works::Zone>{
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0x24e3, 0x07f8),
                                 utils::geometry_works::Point(0x13db, 0x07f8),
                                 utils::geometry_works::Point(0x13db, 0x1c92),
                                 utils::geometry_works::Point(0x24e3, 0x1c92),
                             }),
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0x0334, 0x1d60),
                                 utils::geometry_works::Point(0x08b6, 0x1d60),
                                 utils::geometry_works::Point(0x08b6, 0x074f),
                                 utils::geometry_works::Point(0x0334, 0x074f),
                             }),
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0x0af3, 0x0816),
                                 utils::geometry_works::Point(0x106e, 0x0816),
                                 utils::geometry_works::Point(0x106e, 0x0552),
                                 utils::geometry_works::Point(0x0af3, 0x0552),
                             }),
                         },
                     .spawn_aerial_zones =
                         std::vector<utils::geometry_works::Zone>{
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0x0334, 0x1d60),
                                 utils::geometry_works::Point(0x0492, 0x0747),
                                 utils::geometry_works::Point(0x24a5, 0x07f8),
                                 utils::geometry_works::Point(0x2516, 0x1c94),
                             }),
                         },
                     .dead_zones =
                         std::vector<utils::geometry_works::Zone>{
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0x1dfd, 0x161a),
                                 utils::geometry_works::Point(0x2453, 0x161a),
                                 utils::geometry_works::Point(0x2453, 0x0f07),
                                 utils::geometry_works::Point(0x1dfd, 0x0f07),
                             }),
                         },
                 },
             .encounters{
                 pe2_encounter_t{
                     .id = 1,
                     .mask = UNRANDOMIZED,
                     .encounter_value = 350,
                     .enemy_record_descriptor_list_addr_offset = 0x2b28,
                     .enemy_resources_desciptors_list_offset = 0x2aa0,
                 },
                 pe2_encounter_t{
                     .id = 2,
                     .mask = RANDOMIZED,
                     .encounter_value = 350,
                     .enemy_record_descriptor_list_addr_offset = 0x2b30,
                     .enemy_resources_desciptors_list_offset = 0x2ab8,
                 },
                 pe2_encounter_t{
                     .id = 3,
                     .mask = FORBID_SLOT_3_ENEMIES,
                     .encounter_value = 350,
                     .enemy_record_descriptor_list_addr_offset = 0x2b38,
                     .enemy_resources_desciptors_list_offset = 0x2ad0,
                 },
             },
         },
         pe2_room_encounters_t{.id = 0x19,
                               .name = game::ROOM_DRYFIELD_DAY_DRIVEWAY,
                               .encounters_offset = 0x1e94,
                               .additional_file_end_pointer = 0x18c,
                               .environment =
                                   pe2_room_environment_t{
                                       .spawn_zones =
                                           std::vector<utils::geometry_works::Zone>{
                                               utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                                   utils::geometry_works::Point(0x1773, 0x01ba),
                                                   utils::geometry_works::Point(0x1705, 0x085b),
                                                   utils::geometry_works::Point(0xfe17, 0x085b),
                                                   utils::geometry_works::Point(0xfe17, 0x019d),
                                               }),
                                               utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                                   utils::geometry_works::Point(0xf9b8, 0x0639),
                                                   utils::geometry_works::Point(0xf1f8, 0x0635),
                                                   utils::geometry_works::Point(0xf1f8, 0xf9c1),
                                                   utils::geometry_works::Point(0xf9b8, 0xf9c1),
                                               }),
                                               utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                                   utils::geometry_works::Point(0xef82, 0xf9c0),
                                                   utils::geometry_works::Point(0xdf84, 0xf9c0),
                                                   utils::geometry_works::Point(0xdf84, 0xfaba),
                                                   utils::geometry_works::Point(0xef82, 0xfaba),
                                               }),
                                           },
                                   },
                               .encounters{
                                   pe2_encounter_t{
                                       .id = 1,
                                       .mask = RANDOMIZED,
                                       .encounter_value = 350,
                                       .enemy_record_descriptor_list_addr_offset = 0x181c,
                                       .enemy_resources_desciptors_list_offset = 0x17d8,
                                   },
                               },
                               .file_patch_map{// Disable bat cutscene near well
                                               {0x332, 0x45}}},
         pe2_room_encounters_t{
             .id = 0x1a,
             .name = game::ROOM_DRYFIELD_DAY_JUNK_YARD,
             .encounters_offset = 0x2074,
             .additional_file_end_pointer = 0x194,
             .environment =
                 pe2_room_environment_t{
                     .spawn_zones =
                         std::vector<utils::geometry_works::Zone>{
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0x56b4, 0x0faf),
                                 utils::geometry_works::Point(0x38a6, 0x0fa3),
                                 utils::geometry_works::Point(0x389c, 0x0d11),
                                 utils::geometry_works::Point(0x574a, 0x0d11),
                             }),
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0x328b, 0x104e),
                                 utils::geometry_works::Point(0x328b, 0x0143),
                                 utils::geometry_works::Point(0x120c, 0x014c),
                                 utils::geometry_works::Point(0x120c, 0x104e),
                             }),
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0x0e09, 0x0c62),
                                 utils::geometry_works::Point(0x0e09, 0x02af),
                                 utils::geometry_works::Point(0x0424, 0x02af),
                                 utils::geometry_works::Point(0x0424, 0x0c62),
                             }),
                         },
                 },
             .encounters{
                 pe2_encounter_t{
                     .id = 1,
                     .mask = UNRANDOMIZED,
                     .encounter_value = 0,
                     .enemy_record_descriptor_list_addr_offset = 0x1fa0,
                     .enemy_resources_desciptors_list_offset = 0x1f2c,
                 },
                 pe2_encounter_t{
                     .id = 2,
                     .mask = FORBID_SLOT_3_ENEMIES,
                     .encounter_value = 350,
                     .enemy_record_descriptor_list_addr_offset = 0x1fa8,
                     .enemy_resources_desciptors_list_offset = 0x1f38,
                 },
             },
         },
         pe2_room_encounters_t{
             .id = 0x1b,
             .name = game::ROOM_DRYFIELD_DAY_TRAILER_COACH,
             .encounters_offset = 0x20c4,
             .additional_file_end_pointer = 0x19c,
             .encounters{
                 pe2_encounter_t{
                     .id = 1,
                     .mask = UNRANDOMIZED,
                     .encounter_value = 0,
                     .enemy_record_descriptor_list_addr_offset = 0xa138,
                     .enemy_resources_desciptors_list_offset = 0xa118,
                 },
             },
         },
         pe2_room_encounters_t{
             .id = 0x1d,
             .name = game::ROOM_DRYFIELD_DAY_MOTEL_BALCONY,
             .encounters_offset = 0x20e4,
             .additional_file_end_pointer = 0x1ac,
             .environment =
                 pe2_room_environment_t{
                     .spawn_zones =
                         std::vector<utils::geometry_works::Zone>{
                             utils::geometry_works::Zone(
                                 utils::geometry_works::Polygon{
                                     utils::geometry_works::Point(0xe5d4, 0xedfe),
                                     utils::geometry_works::Point(0xeaaa, 0xedfe),
                                     utils::geometry_works::Point(0xeac2, 0x22be),
                                     utils::geometry_works::Point(0xe5d4, 0x22be),
                                 },
                                 0xf402),
                             utils::geometry_works::Zone(
                                 utils::geometry_works::Polygon{
                                     utils::geometry_works::Point(0xe5d5, 0x29cc),
                                     utils::geometry_works::Point(0xfdb2, 0x29cc),
                                     utils::geometry_works::Point(0xfdb2, 0x2576),
                                     utils::geometry_works::Point(0xe5d5, 0x2576),
                                 },
                                 0xf402),
                             utils::geometry_works::Zone(
                                 utils::geometry_works::Polygon{
                                     utils::geometry_works::Point(0xe384, 0x0a8c),
                                     utils::geometry_works::Point(0xe3a0, 0x0516),
                                     utils::geometry_works::Point(0xcea1, 0x0516),
                                     utils::geometry_works::Point(0xcea1, 0x0ab0),
                                 },
                                 0xf402),
                         },
                 },
             .encounters{
                 pe2_encounter_t{
                     .id = 1,
                     .mask = UNRANDOMIZED,
                     .encounter_value = 0,
                     .enemy_record_descriptor_list_addr_offset = 0x8c68,
                     .enemy_resources_desciptors_list_offset = 0x8be8,
                 },
                 pe2_encounter_t{
                     .id = 2,
                     .mask = RANDOMIZED,
                     .encounter_value = 350,
                     .enemy_record_descriptor_list_addr_offset = 0x8c70,
                     .enemy_resources_desciptors_list_offset = 0x8bf4,
                 },
                 pe2_encounter_t{
                     .id = 7,
                     .mask = RANDOMIZED,
                     .encounter_value = 550,
                     .enemy_record_descriptor_list_addr_offset = 0x8c98,
                     .enemy_resources_desciptors_list_offset = 0x8c24,
                 },
                 pe2_encounter_t{
                     .id = 3,
                     .mask = RANDOMIZED,
                     .encounter_value = 350,
                     .enemy_record_descriptor_list_addr_offset = 0x8c78,
                     .enemy_resources_desciptors_list_offset = 0x8c0c,
                 },
                 pe2_encounter_t{
                     .id = 8,
                     .mask = RANDOMIZED,
                     .encounter_value = 550,
                     .enemy_record_descriptor_list_addr_offset = 0x8ca0,
                     .enemy_resources_desciptors_list_offset = 0x8c3c,
                 },
             },
         },
         pe2_room_encounters_t{
             .id = 0x1e,
             .name = game::ROOM_DRYFIELD_DAY_MOTEL_ROOM_6,
             .encounters_offset = 0x2204,
             .additional_file_end_pointer = 0x1b4,
             .encounters{
                 pe2_encounter_t{
                     .id = 1,
                     .mask = UNRANDOMIZED,
                     .encounter_value = 0,
                     .enemy_record_descriptor_list_addr_offset = 0x91ac,
                     .enemy_resources_desciptors_list_offset = 0x9180,
                 },
                 pe2_encounter_t{
                     .id = 2,
                     .mask = UNRANDOMIZED,
                     .encounter_value = 0,
                     .enemy_record_descriptor_list_addr_offset = 0x91b4,
                     .enemy_resources_desciptors_list_offset = 0x918c,
                 },
             },
         },
         pe2_room_encounters_t{
             .id = 0x20,
             .name = game::ROOM_DRYFIELD_DAY_WATER_HOLE,
             .encounters_offset = 0x2234,
             .additional_file_end_pointer = 0x1c4,
             .environment =
                 pe2_room_environment_t{
                     .spawn_zones =
                         std::vector<utils::geometry_works::Zone>{
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0x1100, 0xff00),
                                 utils::geometry_works::Point(0x1100, 0xf900),
                                 utils::geometry_works::Point(0x2d00, 0xf900),
                                 utils::geometry_works::Point(0x2d00, 0xff00),
                             }),
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0x2900, 0xf100),
                                 utils::geometry_works::Point(0x2900, 0xf800),
                                 utils::geometry_works::Point(0x5500, 0xf800),
                                 utils::geometry_works::Point(0x5500, 0xf100),
                             }),
                         },
                 },
             .encounters{
                 pe2_encounter_t{
                     .id = 1,
                     .mask = FORBID_SLOT_3_ENEMIES,
                     .encounter_value = 350,
                     .enemy_record_descriptor_list_addr_offset = 0x5204,
                     .enemy_resources_desciptors_list_offset = 0x51e4,
                 },
             },
         },
         pe2_room_encounters_t{
             .id = 0x22,
             .name = game::ROOM_DRYFIELD_DAY_WINE_BARRELS,
             .encounters_offset = 0x22c4,
             .additional_file_end_pointer = 0x1d4,
             .environment =
                 pe2_room_environment_t{
                     .spawn_zones =
                         std::vector<utils::geometry_works::Zone>{
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0x0ed8, 0x1068),
                                 utils::geometry_works::Point(0x236e, 0x1068),
                                 utils::geometry_works::Point(0x236e, 0x012c),
                                 utils::geometry_works::Point(0x0ed8, 0x012c),
                             }),
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0x0514, 0x0ce4),
                                 utils::geometry_works::Point(0x0898, 0x0cc1),
                                 utils::geometry_works::Point(0x0898, 0x012c),
                                 utils::geometry_works::Point(0x0514, 0x012c),
                             }),
                         },
                     .spawn_aerial_zones =
                         std::vector<utils::geometry_works::Zone>{
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0x0ed8, 0x1068),
                                 utils::geometry_works::Point(0x236e, 0x1068),
                                 utils::geometry_works::Point(0x236e, 0x012c),
                                 utils::geometry_works::Point(0x0ed8, 0x012c),
                             }),
                         },
                     .dead_zones =
                         std::vector<utils::geometry_works::Zone>{
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0x233e, 0x0e10),
                                 utils::geometry_works::Point(0x233e, 0x0318),
                                 utils::geometry_works::Point(0x161e, 0x0318),
                                 utils::geometry_works::Point(0x161e, 0x0e10),
                             }),
                         },
                 },
             .encounters{
                 pe2_encounter_t{
                     .id = 1,
                     .mask = RANDOMIZED,
                     .encounter_value = 350,
                     .enemy_record_descriptor_list_addr_offset = 0x3514,
                     .enemy_resources_desciptors_list_offset = 0x34e8,
                 },
             },
             .file_patch_map{
                 // Enable snail magazine shelf
                 {0x48, 0x00},
                 {0x49, 0x00},
                 {0x4a, 0x00},
                 {0x4b, 0x00},
             }},
         pe2_room_encounters_t{
             .id = 0x26,
             .name = game::ROOM_DRYFIELD_DAY_CELLAR,
             .encounters_offset = 0x2374,
             .additional_file_end_pointer = 0x1f4,
             .environment =
                 pe2_room_environment_t{
                     .spawn_zones =
                         std::vector<utils::geometry_works::Zone>{
                             utils::geometry_works::Zone(
                                 utils::geometry_works::Polygon{
                                     utils::geometry_works::Point(0x4524, 0xfd44),
                                     utils::geometry_works::Point(0x3caa, 0xfd2e),
                                     utils::geometry_works::Point(0x3caa, 0xe447),
                                     utils::geometry_works::Point(0x4524, 0xe447),
                                 },
                                 0xfc99),
                             utils::geometry_works::Zone(
                                 utils::geometry_works::Polygon{
                                     utils::geometry_works::Point(0x393e, 0xe2f3),
                                     utils::geometry_works::Point(0xfa88, 0xe2f3),
                                     utils::geometry_works::Point(0xfa88, 0xdaa2),
                                     utils::geometry_works::Point(0x393e, 0xdaa2),
                                 },
                                 0xfc99),
                         },
                 },
             .encounters{pe2_encounter_t{
                 .id = 1,
                 .mask = UNRANDOMIZED,
                 .encounter_value = 0,
                 .enemy_record_descriptor_list_addr_offset = 0x22b0,
                 .enemy_resources_desciptors_list_offset = 0x2290,
                 .force_reward_value = 400,
             }},
         },
     }},
    {game::DRYFIELD_NIGHT_LEVEL,
     {
         pe2_room_encounters_t{
             .id = 0x01,
             .name = game::ROOM_DRYFIELD_NIGHT_GAS_STATION,
             .encounters_offset = 0x12f8,
             .additional_file_end_pointer = 0x5c,
             .environment =
                 pe2_room_environment_t{
                     .spawn_zones =
                         std::vector<utils::geometry_works::Zone>{
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0x318b, 0xfec5),
                                 utils::geometry_works::Point(0x267e, 0xfec5),
                                 utils::geometry_works::Point(0x267e, 0xeaab),
                                 utils::geometry_works::Point(0x318b, 0xeaab),
                             }),
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0x318b, 0xf20e),
                                 utils::geometry_works::Point(0x3dfe, 0xfc0b),
                                 utils::geometry_works::Point(0x3dfe, 0xfedd),
                                 utils::geometry_works::Point(0x318b, 0xfedd),
                             }),
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0x267e, 0xfa20),
                                 utils::geometry_works::Point(0x0661, 0xfa20),
                                 utils::geometry_works::Point(0x0661, 0xe9db),
                                 utils::geometry_works::Point(0x267e, 0xe9db),
                             }),
                         },
                 },
             .encounters{pe2_encounter_t{
                             .id = 1,
                             .mask = UNRANDOMIZED,
                             .encounter_value = 0,
                             .enemy_record_descriptor_list_addr_offset = 0x1306c,
                             .enemy_resources_desciptors_list_offset = 0x12fd4,
                         },
                         pe2_encounter_t{
                             .id = 2,
                             .mask = RANDOMIZED,
                             .encounter_value = 400,
                             .enemy_record_descriptor_list_addr_offset = 0x13074,
                             .enemy_resources_desciptors_list_offset = 0x12fec,
                         },
                         pe2_encounter_t{
                             .id = 7,
                             .mask = RANDOMIZED,
                             .encounter_value = 400,
                             .enemy_record_descriptor_list_addr_offset = 0x1309c,
                             .enemy_resources_desciptors_list_offset = 0x1301c,
                         },
                         pe2_encounter_t{
                             .id = 3,
                             .mask = RANDOMIZED,
                             .encounter_value = 400,
                             .enemy_record_descriptor_list_addr_offset = 0x1307c,
                             .enemy_resources_desciptors_list_offset = 0x13004,
                         },
                         pe2_encounter_t{
                             .id = 13,
                             .mask = RANDOMIZED,
                             .encounter_value = 600,
                             .enemy_record_descriptor_list_addr_offset = 0x130bc,
                             .enemy_resources_desciptors_list_offset = 0x13040,
                         }},
         },
         pe2_room_encounters_t{
             .id = 0x02,
             .name = game::ROOM_DRYFIELD_NIGHT_MAIN_STREET,
             .encounters_offset = 0x14a8,
             .additional_file_end_pointer = 0x64,
             .environment =
                 pe2_room_environment_t{
                     .spawn_zones =
                         std::vector<utils::geometry_works::Zone>{
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0xe5d4, 0x224c),
                                 utils::geometry_works::Point(0xe5d4, 0xf491),
                                 utils::geometry_works::Point(0xfd87, 0xf491),
                                 utils::geometry_works::Point(0xfd82, 0x224c),
                             }),
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0xe5d4, 0x224c),
                                 utils::geometry_works::Point(0xe5d4, 0x29cc),
                                 utils::geometry_works::Point(0xfa26, 0x29cc),
                                 utils::geometry_works::Point(0xfa0e, 0x224c),
                             }),
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0xee1c, 0xe962),
                                 utils::geometry_works::Point(0xee26, 0xf491),
                                 utils::geometry_works::Point(0xfd83, 0xf491),
                                 utils::geometry_works::Point(0xf8de, 0xeb52),
                             }),
                         },
                     .dead_zones =
                         std::vector<utils::geometry_works::Zone>{
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0xea00, 0xfc05),
                                 utils::geometry_works::Point(0xe990, 0x023e),
                                 utils::geometry_works::Point(0xedb6, 0x034b),
                                 utils::geometry_works::Point(0xee0c, 0xfbf3),
                             }),
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0xe9b1, 0x0f99),
                                 utils::geometry_works::Point(0xe9f1, 0x16f7),
                                 utils::geometry_works::Point(0xee54, 0x1694),
                                 utils::geometry_works::Point(0xeea8, 0x0f1c),
                             }),
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0xea12, 0x1fcf),
                                 utils::geometry_works::Point(0xe9c1, 0x2638),
                                 utils::geometry_works::Point(0xf058, 0x25e0),
                                 utils::geometry_works::Point(0xefe6, 0x1f4a),
                             }),
                         },
                 },
             .encounters{
                 pe2_encounter_t{
                     .id = 1,
                     .mask = RANDOMIZED,
                     .encounter_value = 400,
                     .enemy_record_descriptor_list_addr_offset = 0xb450,
                     .enemy_resources_desciptors_list_offset = 0xb3f4,
                 },
                 pe2_encounter_t{
                     .id = 2,
                     .mask = UNRANDOMIZED,
                     .encounter_value = 0,
                     .enemy_record_descriptor_list_addr_offset = 0xb458,
                     .enemy_resources_desciptors_list_offset = 0xb418,
                 },
                 pe2_encounter_t{
                     .id = 7,
                     .mask = RANDOMIZED,
                     .encounter_value = 400,
                     .enemy_record_descriptor_list_addr_offset = 0xb480,
                     .enemy_resources_desciptors_list_offset = 0xb430,
                 },
             },
         },
         pe2_room_encounters_t{.id = 0x03,
                               .name = game::ROOM_DRYFIELD_NIGHT_GENERAL_STORE,
                               .encounters_offset = 0x1578,
                               .additional_file_end_pointer = 0x6c,
                               .environment =
                                   pe2_room_environment_t{
                                       .spawn_zones =
                                           std::vector<utils::geometry_works::Zone>{
                                               utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                                   utils::geometry_works::Point(0x0404, 0x1450),
                                                   utils::geometry_works::Point(0x0413, 0x0c80),
                                                   utils::geometry_works::Point(0x0ca2, 0x0c80),
                                                   utils::geometry_works::Point(0x0c4e, 0x1450),
                                               }),
                                               utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                                   utils::geometry_works::Point(0x0e30, 0x1450),
                                                   utils::geometry_works::Point(0x0f1d, 0x0c80),
                                                   utils::geometry_works::Point(0x23f0, 0x0c80),
                                                   utils::geometry_works::Point(0x242a, 0x1450),
                                               }),
                                               utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                                   utils::geometry_works::Point(0x23f0, 0x0c98),
                                                   utils::geometry_works::Point(0x092e, 0x0c98),
                                                   utils::geometry_works::Point(0x092e, 0x012c),
                                                   utils::geometry_works::Point(0x23f0, 0x012c),
                                               }),
                                               utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                                   utils::geometry_works::Point(0x157c, 0x1d3c),
                                                   utils::geometry_works::Point(0x157c, 0x1996),
                                                   utils::geometry_works::Point(0x0514, 0x1996),
                                                   utils::geometry_works::Point(0x0514, 0x1d3c),
                                               }),
                                               utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                                   utils::geometry_works::Point(0x157c, 0x1e14),
                                                   utils::geometry_works::Point(0x157c, 0x1996),
                                                   utils::geometry_works::Point(0x206c, 0x1996),
                                                   utils::geometry_works::Point(0x1fca, 0x1e14),
                                               }),
                                           },
                                       .dead_zones =
                                           std::vector<utils::geometry_works::Zone>{
                                               utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                                   utils::geometry_works::Point(0x0c7e, 0x0c67),
                                                   utils::geometry_works::Point(0x2086, 0x0c80),
                                                   utils::geometry_works::Point(0x20d0, 0x12c0),
                                                   utils::geometry_works::Point(0x0c4c, 0x12c0),
                                               }),
                                               utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                                   utils::geometry_works::Point(0x0d49, 0x0b25),
                                                   utils::geometry_works::Point(0x0d16, 0x04b3),
                                                   utils::geometry_works::Point(0x20df, 0x04c1),
                                                   utils::geometry_works::Point(0x20c9, 0x0b16),
                                               }),
                                               utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                                   utils::geometry_works::Point(0x209e, 0x012c),
                                                   utils::geometry_works::Point(0x20a0, 0x045b),
                                                   utils::geometry_works::Point(0x23f0, 0x047e),
                                                   utils::geometry_works::Point(0x23f0, 0x012c),
                                               }),
                                               utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                                   utils::geometry_works::Point(0x0d09, 0x011c),
                                                   utils::geometry_works::Point(0x0e12, 0x02ee),
                                                   utils::geometry_works::Point(0x12c9, 0x0276),
                                                   utils::geometry_works::Point(0x12c0, 0x011c),
                                               }),
                                           },
                                   },
                               .encounters{pe2_encounter_t{
                                               .id = 1,
                                               .mask = FORBID_MULTI_ENEMY_ENC_AND_SLOT_3,
                                               .encounter_value = 400,
                                               .enemy_record_descriptor_list_addr_offset = 0x8174,
                                               .enemy_resources_desciptors_list_offset = 0x80f4,
                                           },
                                           pe2_encounter_t{
                                               .id = 2,
                                               .mask = RANDOMIZED,
                                               .encounter_value = 400,
                                               .enemy_record_descriptor_list_addr_offset = 0x817c,
                                               .enemy_resources_desciptors_list_offset = 0x810c,
                                           },
                                           pe2_encounter_t{
                                               .id = 3,
                                               .mask = RANDOMIZED,
                                               .encounter_value = 400,
                                               .enemy_record_descriptor_list_addr_offset = 0x8184,
                                               .enemy_resources_desciptors_list_offset = 0x8124,
                                           },
                                           pe2_encounter_t{
                                               .id = 21,
                                               .mask = RANDOMIZED,
                                               .encounter_value = 400,
                                               .enemy_record_descriptor_list_addr_offset = 0x8214,
                                               .enemy_resources_desciptors_list_offset = 0x8148,
                                           }}},
         pe2_room_encounters_t{
             .id = 0x05,
             .name = game::ROOM_DRYFIELD_NIGHT_BACK_STREET,
             .encounters_offset = 0x16f8,
             .additional_file_end_pointer = 0x7c,
             .environment =
                 pe2_room_environment_t{
                     .spawn_zones =
                         std::vector<utils::geometry_works::Zone>{
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0x1acf, 0x08f4),
                                 utils::geometry_works::Point(0x1acf, 0x1646),
                                 utils::geometry_works::Point(0x2d91, 0x163d),
                                 utils::geometry_works::Point(0x2d91, 0x08f4),
                             }),
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0x1acf, 0x08f4),
                                 utils::geometry_works::Point(0x0ae6, 0x10d3),
                                 utils::geometry_works::Point(0x0ae6, 0x1646),
                                 utils::geometry_works::Point(0x1acf, 0x15dc),
                             }),
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0x0ae6, 0x1646),
                                 utils::geometry_works::Point(0x0ae6, 0x0ed0),
                                 utils::geometry_works::Point(0xda1c, 0x0ed0),
                                 utils::geometry_works::Point(0xda1c, 0x1646),
                             }),
                         },
                     .dead_zones =
                         std::vector<utils::geometry_works::Zone>{
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0x1dd8, 0x07f7),
                                 utils::geometry_works::Point(0x1d9d, 0x0b12),
                                 utils::geometry_works::Point(0x22dc, 0x0af1),
                                 utils::geometry_works::Point(0x2314, 0x07f4),
                             }),
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0x2d8f, 0x133f),
                                 utils::geometry_works::Point(0x274f, 0x137d),
                                 utils::geometry_works::Point(0x270b, 0x173d),
                                 utils::geometry_works::Point(0x2d8f, 0x173d),
                             }),
                         },
                 },
             .encounters{pe2_encounter_t{
                             .id = 1,
                             .mask = FORBID_MULTI_ENEMY_ENC_AND_SLOT_3,
                             .encounter_value = 400,
                             .enemy_record_descriptor_list_addr_offset = 0x3f60,
                             .enemy_resources_desciptors_list_offset = 0x3ec8,
                         },
                         pe2_encounter_t{
                             .id = 7,
                             .mask = RANDOMIZED,
                             .encounter_value = 400,
                             .enemy_record_descriptor_list_addr_offset = 0x3f90,
                             .enemy_resources_desciptors_list_offset = 0x3f28,
                         },
                         pe2_encounter_t{
                             .id = 3,
                             .mask = RANDOMIZED,
                             .encounter_value = 400,
                             .enemy_record_descriptor_list_addr_offset = 0x3f70,
                             .enemy_resources_desciptors_list_offset = 0x3ee0,
                         },
                         pe2_encounter_t{
                             .id = 4,
                             .mask = RANDOMIZED,
                             .encounter_value = 400,
                             .enemy_record_descriptor_list_addr_offset = 0x3f78,
                             .enemy_resources_desciptors_list_offset = 0x3f04,
                         },
                         pe2_encounter_t{
                             .id = 21,
                             .mask = RANDOMIZED,
                             .encounter_value = 600,
                             .enemy_record_descriptor_list_addr_offset = 0x4000,
                             .enemy_resources_desciptors_list_offset = 0x3f40,
                         }},
         },
         pe2_room_encounters_t{
             .id = 0x06,
             .name = game::ROOM_DRYFIELD_NIGHT_SOUVENIR_SHOP,
             .encounters_offset = 0x1968,
             .additional_file_end_pointer = 0x84,
             .environment =
                 pe2_room_environment_t{
                     .spawn_zones =
                         std::vector<utils::geometry_works::Zone>{
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0x15d1, 0xf3b2),
                                 utils::geometry_works::Point(0x047a, 0xf387),
                                 utils::geometry_works::Point(0x044c, 0xfcf9),
                                 utils::geometry_works::Point(0x1482, 0xfd24),
                             }),
                         },
                     .dead_zones =
                         std::vector<utils::geometry_works::Zone>{
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0x1682, 0xfb5f),
                                 utils::geometry_works::Point(0x0e4d, 0xfb5f),
                                 utils::geometry_works::Point(0x0e15, 0xf529),
                                 utils::geometry_works::Point(0x16ea, 0xf529),
                             }),
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0x0d25, 0xf9fe),
                                 utils::geometry_works::Point(0x0665, 0xf9f2),
                                 utils::geometry_works::Point(0x0627, 0xf539),
                                 utils::geometry_works::Point(0x0e32, 0xf529),
                             }),
                         },
                 },
             .encounters{pe2_encounter_t{
                             .id = 1,
                             .mask = FORBID_MULTI_ENEMY_ENC_AND_SLOT_3,
                             .encounter_value = 400,
                             .enemy_record_descriptor_list_addr_offset = 0x2074,
                             .enemy_resources_desciptors_list_offset = 0x2018,
                         },
                         pe2_encounter_t{
                             .id = 3,
                             .mask = RANDOMIZED,
                             .encounter_value = 400,
                             .enemy_record_descriptor_list_addr_offset = 0x2084,
                             .enemy_resources_desciptors_list_offset = 0x2030,
                         },
                         pe2_encounter_t{
                             .id = 14,
                             .mask = RANDOMIZED,
                             .encounter_value = 400,
                             .enemy_record_descriptor_list_addr_offset = 0x20c4,
                             .enemy_resources_desciptors_list_offset = 0x2048,
                         }},
         },
         pe2_room_encounters_t{
             .id = 0x07,
             .name = game::ROOM_DRYFIELD_NIGHT_WAREHOUSE,
             .encounters_offset = 0x1ab8,
             .additional_file_end_pointer = 0x8c,
             .environment =
                 pe2_room_environment_t{
                     .spawn_zones =
                         std::vector<utils::geometry_works::Zone>{
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0x1400, 0xf200),
                                 utils::geometry_works::Point(0x1400, 0xfe00),
                                 utils::geometry_works::Point(0x0500, 0xfe00),
                                 utils::geometry_works::Point(0x0500, 0xf200),
                             }),
                         },
                     .dead_zones =
                         std::vector<utils::geometry_works::Zone>{
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0x1000, 0xf200),
                                 utils::geometry_works::Point(0x1000, 0xf600),
                                 utils::geometry_works::Point(0x0c00, 0xf600),
                                 utils::geometry_works::Point(0x0c00, 0xf200),
                             }),
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0x0e00, 0xfb00),
                                 utils::geometry_works::Point(0x0e00, 0xfe00),
                                 utils::geometry_works::Point(0x0900, 0xfe00),
                                 utils::geometry_works::Point(0x0900, 0xfb00),
                             }),
                         },
                 },
             .encounters{pe2_encounter_t{
                             .id = 1,
                             .mask = FORBID_MULTI_ENEMY_ENC_AND_SLOT_3,
                             .encounter_value = 400,
                             .enemy_record_descriptor_list_addr_offset = 0x25e0,
                             .enemy_resources_desciptors_list_offset = 0x2584,
                         },
                         pe2_encounter_t{
                             .id = 3,
                             .mask = RANDOMIZED,
                             .encounter_value = 400,
                             .enemy_record_descriptor_list_addr_offset = 0x25f0,
                             .enemy_resources_desciptors_list_offset = 0x259c,
                         },
                         pe2_encounter_t{
                             .id = 11,
                             .mask = RANDOMIZED,
                             .encounter_value = 400,
                             .enemy_record_descriptor_list_addr_offset = 0x2630,
                             .enemy_resources_desciptors_list_offset = 0x25b4,
                         }},
         },
         pe2_room_encounters_t{
             .id = 0x08,
             .name = game::ROOM_DRYFIELD_NIGHT_SHELTER_ROOM,
             .encounters_offset = 0x1bb8,
             .additional_file_end_pointer = 0x94,
             .encounters{
                 pe2_encounter_t{
                     .id = 1,
                     .mask = UNRANDOMIZED,
                     .encounter_value = 0,
                     .enemy_record_descriptor_list_addr_offset = 0x4320,
                     .enemy_resources_desciptors_list_offset = 0x42f4,
                 },
             },
         },
         pe2_room_encounters_t{
             .id = 0x09,
             .name = game::ROOM_DRYFIELD_NIGHT_DILAPIDATED_HOUSE,
             .encounters_offset = 0x1c08,
             .additional_file_end_pointer = 0x9c,
             .environment{
                 .spawn_zones =
                     std::vector<utils::geometry_works::Zone>{
                         utils::geometry_works::Zone(utils::geometry_works::Polygon{
                             utils::geometry_works::Point(0xf650, 0x09e3),
                             utils::geometry_works::Point(0xf650, 0xf575),
                             utils::geometry_works::Point(0x09c5, 0xf575),
                             utils::geometry_works::Point(0x09c5, 0x09e3),
                         }),
                         utils::geometry_works::Zone(utils::geometry_works::Polygon{
                             utils::geometry_works::Point(0xe985, 0x02bb),
                             utils::geometry_works::Point(0xed77, 0x02bb),
                             utils::geometry_works::Point(0xed77, 0xf574),
                             utils::geometry_works::Point(0xe985, 0xf574),
                         }),
                     },
                 .spawn_aerial_zones = std::vector<utils::geometry_works::Zone>{},
                 .spawn_water_zones = std::vector<utils::geometry_works::Zone>{},
                 .dead_zones =
                     std::vector<utils::geometry_works::Zone>{
                         utils::geometry_works::Zone(utils::geometry_works::Polygon{
                             utils::geometry_works::Point(0xf830, 0xf475),
                             utils::geometry_works::Point(0xf830, 0xf9f1),
                             utils::geometry_works::Point(0x0b8c, 0xf9f1),
                             utils::geometry_works::Point(0x0b54, 0xf473),
                         }),
                     },
             },
             .encounters{pe2_encounter_t{
                             .id = 1,
                             .mask = FORBID_MULTI_ENEMY_ENC_AND_SLOT_3,
                             .encounter_value = 400,
                             .enemy_record_descriptor_list_addr_offset = 0xc9fc,
                             .enemy_resources_desciptors_list_offset = 0xc984,
                         },
                         pe2_encounter_t{
                             .id = 2,
                             .mask = FORBID_MULTI_ENEMY_ENC_AND_SLOT_3,
                             .encounter_value = 400,
                             .enemy_record_descriptor_list_addr_offset = 0xca04,
                             .enemy_resources_desciptors_list_offset = 0xc9a8,
                         },
                         pe2_encounter_t{
                             .id = 19,
                             .mask = FORBID_MULTI_ENEMY_ENC_AND_SLOT_3,
                             .encounter_value = 400,
                             .enemy_record_descriptor_list_addr_offset = 0xca8c,
                             .enemy_resources_desciptors_list_offset = 0xc9cc,
                         }},
         },
         pe2_room_encounters_t{
             .id = 0x0b,
             .name = game::ROOM_DRYFIELD_NIGHT_MOTEL_ROOM_1,
             .encounters_offset = 0x1d58,
             .additional_file_end_pointer = 0xac,
             .environment =
                 pe2_room_environment_t{
                     .spawn_zones =
                         std::vector<utils::geometry_works::Zone>{
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0x0100, 0x0e00),
                                 utils::geometry_works::Point(0x0100, 0x0900),
                                 utils::geometry_works::Point(0x1280, 0x0900),
                                 utils::geometry_works::Point(0x1280, 0x0e00),
                             }),
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0x0d00, 0x0e00),
                                 utils::geometry_works::Point(0x0d00, 0x1700),
                                 utils::geometry_works::Point(0x1280, 0x1700),
                                 utils::geometry_works::Point(0x1280, 0x0e00),
                             }),
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0x0100, 0x1300),
                                 utils::geometry_works::Point(0x0100, 0x1700),
                                 utils::geometry_works::Point(0x0d00, 0x1700),
                                 utils::geometry_works::Point(0x0d00, 0x1300),
                             }),
                         },
                     .spawn_aerial_zones =
                         std::vector<utils::geometry_works::Zone>{
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0x0100, 0x0e00),
                                 utils::geometry_works::Point(0x0100, 0x0100),
                                 utils::geometry_works::Point(0x1280, 0x0100),
                                 utils::geometry_works::Point(0x1280, 0x0e00),
                             }),
                         },
                 },
             .encounters{pe2_encounter_t{
                             .id = 1,
                             .mask = RANDOMIZED,
                             .encounter_value = 400,
                             .enemy_record_descriptor_list_addr_offset = 0x31a4,
                             .enemy_resources_desciptors_list_offset = 0x313c,
                         },
                         pe2_encounter_t{
                             .id = 2,
                             .mask = RANDOMIZED,
                             .encounter_value = 400,
                             .enemy_record_descriptor_list_addr_offset = 0x31ac,
                             .enemy_resources_desciptors_list_offset = 0x3160,
                         },
                         pe2_encounter_t{
                             .id = 21,
                             .mask = RANDOMIZED,
                             .encounter_value = 400,
                             .enemy_record_descriptor_list_addr_offset = 0x3244,
                             .enemy_resources_desciptors_list_offset = 0x3184,
                         }}},
         pe2_room_encounters_t{
             .id = 0x0c,
             .name = game::ROOM_DRYFIELD_NIGHT_MOTEL_ROOM_2,
             .encounters_offset = 0x1e88,
             .additional_file_end_pointer = 0xb4,
             .environment =
                 pe2_room_environment_t{
                     .spawn_zones =
                         std::vector<utils::geometry_works::Zone>{
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0x1200, 0x0d00),
                                 utils::geometry_works::Point(0x1200, 0x0900),
                                 utils::geometry_works::Point(0x0200, 0x0900),
                                 utils::geometry_works::Point(0x0200, 0x0d00),
                             }),
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0x0480, 0x0f00),
                                 utils::geometry_works::Point(0x0480, 0x1500),
                                 utils::geometry_works::Point(0x0180, 0x1500),
                                 utils::geometry_works::Point(0x0180, 0x0f00),
                             }),
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0x0900, 0x1100),
                                 utils::geometry_works::Point(0x0900, 0x1500),
                                 utils::geometry_works::Point(0x1200, 0x1500),
                                 utils::geometry_works::Point(0x1200, 0x1100),
                             }),
                         },
                     .spawn_aerial_zones =
                         std::vector<utils::geometry_works::Zone>{
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0x1200, 0x0f00),
                                 utils::geometry_works::Point(0x1200, 0x0100),
                                 utils::geometry_works::Point(0x0100, 0x0100),
                                 utils::geometry_works::Point(0x0100, 0x0f00),
                             }),
                         },
                 },
             .encounters{
                 pe2_encounter_t{
                     .id = 1,
                     .mask = RANDOMIZED,
                     .encounter_value = 400,
                     .enemy_record_descriptor_list_addr_offset = 0x33f0,
                     .enemy_resources_desciptors_list_offset = 0x3380,
                 },
                 pe2_encounter_t{
                     .id = 2,
                     .mask = RANDOMIZED,
                     .encounter_value = 400,
                     .enemy_record_descriptor_list_addr_offset = 0x33f8,
                     .enemy_resources_desciptors_list_offset = 0x33a4,
                 },
                 pe2_encounter_t{
                     .id = 20,
                     .mask = RANDOMIZED,
                     .encounter_value = 400,
                     .enemy_record_descriptor_list_addr_offset = 0x3488,
                     .enemy_resources_desciptors_list_offset = 0x33bc,
                 },
             }},
         pe2_room_encounters_t{
             .id = 0x0d,
             .name = game::ROOM_DRYFIELD_NIGHT_MOTEL_ROOM_3,
             .encounters_offset = 0x1fe8,
             .additional_file_end_pointer = 0xbc,
             .environment =
                 pe2_room_environment_t{
                     .spawn_zones =
                         std::vector<utils::geometry_works::Zone>{
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0x1300, 0x0100),
                                 utils::geometry_works::Point(0x0a00, 0x0100),
                                 utils::geometry_works::Point(0x0a00, 0x1200),
                                 utils::geometry_works::Point(0x1300, 0x1200),
                             }),
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0x1300, 0x0100),
                                 utils::geometry_works::Point(0x0d00, 0x0100),
                                 utils::geometry_works::Point(0x0d00, 0x1a00),
                                 utils::geometry_works::Point(0x1300, 0x1a00),
                             }),
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0x0100, 0x1500),
                                 utils::geometry_works::Point(0x0d00, 0x1500),
                                 utils::geometry_works::Point(0x0d00, 0x1a00),
                                 utils::geometry_works::Point(0x0100, 0x1a00),
                             }),
                         },
                     .spawn_aerial_zones =
                         std::vector<utils::geometry_works::Zone>{
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0x1300, 0x0100),
                                 utils::geometry_works::Point(0x0300, 0x0100),
                                 utils::geometry_works::Point(0x0300, 0x1a00),
                                 utils::geometry_works::Point(0x1300, 0x1a00),
                             }),
                         },
                 },
             .encounters{pe2_encounter_t{
                             .id = 1,
                             .mask = RANDOMIZED,
                             .encounter_value = 400,
                             .enemy_record_descriptor_list_addr_offset = 0x376c,
                             .enemy_resources_desciptors_list_offset = 0x371c,
                         },
                         pe2_encounter_t{
                             .id = 2,
                             .mask = RANDOMIZED,
                             .encounter_value = 400,
                             .enemy_record_descriptor_list_addr_offset = 0x3774,
                             .enemy_resources_desciptors_list_offset = 0x3740,
                         }}},
         pe2_room_encounters_t{
             .id = 0x0e,
             .name = game::ROOM_DRYFIELD_NIGHT_MOTEL_ROOM_4,
             .encounters_offset = 0x20b8,
             .additional_file_end_pointer = 0xc4,
             .environment =
                 pe2_room_environment_t{
                     .spawn_zones =
                         std::vector<utils::geometry_works::Zone>{
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0x0300, 0x0200),
                                 utils::geometry_works::Point(0x0900, 0x0200),
                                 utils::geometry_works::Point(0x0900, 0x0700),
                                 utils::geometry_works::Point(0x0300, 0x0700),
                             }),
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0x0300, 0x0200),
                                 utils::geometry_works::Point(0x0f00, 0x0200),
                                 utils::geometry_works::Point(0x0f00, 0x1000),
                                 utils::geometry_works::Point(0x0300, 0x1000),
                             }),
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0x1700, 0x0200),
                                 utils::geometry_works::Point(0x0f00, 0x0200),
                                 utils::geometry_works::Point(0x0f00, 0x1000),
                                 utils::geometry_works::Point(0x1700, 0x1000),
                             }),
                         },
                     .spawn_aerial_zones =
                         std::vector<utils::geometry_works::Zone>{
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0x0100, 0x0100),
                                 utils::geometry_works::Point(0x0e00, 0x0100),
                                 utils::geometry_works::Point(0x0e00, 0x1300),
                                 utils::geometry_works::Point(0x0100, 0x1300),
                             }),
                         },
                 },
             .encounters{pe2_encounter_t{
                             .id = 1,
                             .mask = RANDOMIZED,
                             .encounter_value = 400,
                             .enemy_record_descriptor_list_addr_offset = 0x2d44,
                             .enemy_resources_desciptors_list_offset = 0x2d00,
                         },
                         pe2_encounter_t{
                             .id = 2,
                             .mask = RANDOMIZED,
                             .encounter_value = 400,
                             .enemy_record_descriptor_list_addr_offset = 0x2d4c,
                             .enemy_resources_desciptors_list_offset = 0x2d18,
                         }}},
         pe2_room_encounters_t{.id = 0x0f,
                               .name = game::ROOM_DRYFIELD_NIGHT_PARKING_LOT,
                               .encounters_offset = 0x2168,
                               .additional_file_end_pointer = 0xcc,
                               .environment =
                                   pe2_room_environment_t{
                                       .spawn_zones =
                                           std::vector<utils::geometry_works::Zone>{
                                               utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                                   utils::geometry_works::Point(0x24fe, 0x0dac),
                                                   utils::geometry_works::Point(0x24fe, 0x09b0),
                                                   utils::geometry_works::Point(0x101e, 0x09b0),
                                                   utils::geometry_works::Point(0x101e, 0x0dac),
                                               }),
                                               utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                                   utils::geometry_works::Point(0x101e, 0x026e),
                                                   utils::geometry_works::Point(0x101e, 0x0dac),
                                                   utils::geometry_works::Point(0xfe52, 0x0dac),
                                                   utils::geometry_works::Point(0xfe52, 0x026b),
                                               }),
                                               utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                                   utils::geometry_works::Point(0xfe52, 0x009d),
                                                   utils::geometry_works::Point(0xf448, 0x009d),
                                                   utils::geometry_works::Point(0xf448, 0x0a28),
                                                   utils::geometry_works::Point(0xfe52, 0x0a28),
                                               }),
                                               utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                                   utils::geometry_works::Point(0xf74c, 0x009d),
                                                   utils::geometry_works::Point(0xf18c, 0x009d),
                                                   utils::geometry_works::Point(0xf18c, 0xeee4),
                                                   utils::geometry_works::Point(0xf74c, 0xeee4),
                                               }),
                                               utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                                   utils::geometry_works::Point(0xeec4, 0xfd7c),
                                                   utils::geometry_works::Point(0xeec4, 0xeee4),
                                                   utils::geometry_works::Point(0xea6e, 0xeee4),
                                                   utils::geometry_works::Point(0xea6e, 0xfed4),
                                               }),
                                           },
                                   },
                               .encounters{
                                   pe2_encounter_t{
                                       .id = 1,
                                       .mask = RANDOMIZED,
                                       .encounter_value = 400,
                                       .enemy_record_descriptor_list_addr_offset = 0x3e88,
                                       .enemy_resources_desciptors_list_offset = 0x3de8,
                                   },
                                   pe2_encounter_t{
                                       .id = 2,
                                       .mask = RANDOMIZED,
                                       .encounter_value = 400,
                                       .enemy_record_descriptor_list_addr_offset = 0x3e90,
                                       .enemy_resources_desciptors_list_offset = 0x3e0c,
                                   },
                                   pe2_encounter_t{
                                       .id = 3,
                                       .mask = RANDOMIZED,
                                       .encounter_value = 400,
                                       .enemy_record_descriptor_list_addr_offset = 0x3e98,
                                       .enemy_resources_desciptors_list_offset = 0x3e24,
                                   },
                                   pe2_encounter_t{
                                       .id = 6,
                                       .mask = RANDOMIZED,
                                       .encounter_value = 600,
                                       .enemy_record_descriptor_list_addr_offset = 0x3eb0,
                                       .enemy_resources_desciptors_list_offset = 0x3e48,
                                   },
                                   pe2_encounter_t{
                                       .id = 20,
                                       .mask = RANDOMIZED,
                                       .encounter_value = 600,
                                       .enemy_record_descriptor_list_addr_offset = 0x3f20,
                                       .enemy_resources_desciptors_list_offset = 0x3e60,
                                   },
                               },
                               .file_patch_map{// Scavanger jump from trash cutscene
                                               {0x5ae, 0x45}}},
         pe2_room_encounters_t{
             .id = 0x10,
             .name = game::ROOM_DRYFIELD_NIGHT_TOILET,
             .encounters_offset = 0x2378,
             .additional_file_end_pointer = 0xd4,
             .environment =
                 pe2_room_environment_t{
                     .spawn_zones =
                         std::vector<utils::geometry_works::Zone>{
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0x02bc, 0x06ae),
                                 utils::geometry_works::Point(0xfcba, 0x06ae),
                                 utils::geometry_works::Point(0xfcba, 0xf952),
                                 utils::geometry_works::Point(0x02bc, 0xf952),
                             }),
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0xf67a, 0xf9f9),
                                 utils::geometry_works::Point(0xf67a, 0x02f0),
                                 utils::geometry_works::Point(0xfac4, 0x02f0),
                                 utils::geometry_works::Point(0xfac4, 0xf952),
                             }),
                         },
                 },
             .forbid_ambush_encounters = true,
             .encounters{
                 pe2_encounter_t{
                     .id = 1,
                     .mask = RANDOMIZED,
                     .encounter_value = 400,
                     .enemy_record_descriptor_list_addr_offset = 0x1d9c,
                     .enemy_resources_desciptors_list_offset = 0x1d40,
                 },
                 pe2_encounter_t{
                     .id = 3,
                     .mask = RANDOMIZED,
                     .encounter_value = 400,
                     .enemy_record_descriptor_list_addr_offset = 0x1dac,
                     .enemy_resources_desciptors_list_offset = 0x1d58,
                 },
                 pe2_encounter_t{
                     .id = 11,
                     .mask = RANDOMIZED,
                     .encounter_value = 400,
                     .enemy_record_descriptor_list_addr_offset = 0x1dec,
                     .enemy_resources_desciptors_list_offset = 0x1d7c,
                 },
             },
             .file_patch_map{// Disable suckler on ceiling
                             {0x110, 0x0}},
         },
         pe2_room_encounters_t{
             .id = 0x12,
             .name = game::ROOM_DRYFIELD_NIGHT_SALOON_GR,
             .encounters_offset = 0x2508,
             .additional_file_end_pointer = 0xe4,
             .environment =
                 pe2_room_environment_t{
                     .spawn_zones =
                         std::vector<utils::geometry_works::Zone>{
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0x0f00, 0xef00),
                                 utils::geometry_works::Point(0x0f00, 0xfa00),
                                 utils::geometry_works::Point(0x0700, 0xfa00),
                                 utils::geometry_works::Point(0x0700, 0xef00),
                             }),
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0xf600, 0xf400),
                                 utils::geometry_works::Point(0xf600, 0xf880),
                                 utils::geometry_works::Point(0x0700, 0xf880),
                                 utils::geometry_works::Point(0x0700, 0xf400),
                             }),
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0x0500, 0x1280),
                                 utils::geometry_works::Point(0x0500, 0xfa00),
                                 utils::geometry_works::Point(0x0980, 0xfa00),
                                 utils::geometry_works::Point(0x0980, 0x1280),
                             }),
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0xf700, 0xf400),
                                 utils::geometry_works::Point(0xf700, 0x0d00),
                                 utils::geometry_works::Point(0xfb00, 0x0d00),
                                 utils::geometry_works::Point(0xfb00, 0xf400),
                             }),
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0x0800, 0x1280),
                                 utils::geometry_works::Point(0x0800, 0x0a00),
                                 utils::geometry_works::Point(0xfb00, 0x0a00),
                                 utils::geometry_works::Point(0xfb00, 0x1280),
                             }),
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0x0480, 0x1800),
                                 utils::geometry_works::Point(0x0480, 0x0a00),
                                 utils::geometry_works::Point(0x0080, 0x0a00),
                                 utils::geometry_works::Point(0x0080, 0x1800),
                             }),
                         },
                     .spawn_aerial_zones =
                         std::vector<utils::geometry_works::Zone>{
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0x1280, 0x1280),
                                 utils::geometry_works::Point(0x1280, 0xee00),
                                 utils::geometry_works::Point(0xf100, 0xee00),
                                 utils::geometry_works::Point(0xf100, 0x1280),
                             }),
                         },
                 },
             .encounters{
                 pe2_encounter_t{
                     .id = 1,
                     .mask = UNRANDOMIZED,
                     .encounter_value = 0,
                     .enemy_record_descriptor_list_addr_offset = 0xb92c,
                     .enemy_resources_desciptors_list_offset = 0xb8d0,
                 },
                 pe2_encounter_t{
                     .id = 2,
                     .mask = RANDOMIZED,
                     .encounter_value = 400,
                     .enemy_record_descriptor_list_addr_offset = 0xb934,
                     .enemy_resources_desciptors_list_offset = 0xb8e8,
                 },
                 pe2_encounter_t{
                     .id = 3,
                     .mask = RANDOMIZED,
                     .encounter_value = 400,
                     .enemy_record_descriptor_list_addr_offset = 0xb93c,
                     .enemy_resources_desciptors_list_offset = 0xb900,
                 },
             },
             .file_patch_map{// Sucklers in toilet cutscene
                             {0x92a, 0x45}},
         },
         pe2_room_encounters_t{
             .id = 0x13,
             .name = game::ROOM_DRYFIELD_NIGHT_GR_KITCHEN,
             .encounters_offset = 0x25b8,
             .additional_file_end_pointer = 0xec,
             .environment =
                 pe2_room_environment_t{
                     .spawn_zones =
                         std::vector<utils::geometry_works::Zone>{
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0x0258, 0x0a8c),
                                 utils::geometry_works::Point(0x0258, 0xf830),
                                 utils::geometry_works::Point(0xfe0c, 0xf830),
                                 utils::geometry_works::Point(0xfe0c, 0x0a8c),
                             }),
                         },
                 },
             .encounters{
                 pe2_encounter_t{
                     .id = 1,
                     .mask = RANDOMIZED,
                     .encounter_value = 400,
                     .enemy_record_descriptor_list_addr_offset = 0x15d0,
                     .enemy_resources_desciptors_list_offset = 0x1550,
                 },
                 pe2_encounter_t{
                     .id = 3,
                     .mask = RANDOMIZED,
                     .encounter_value = 400,
                     .enemy_record_descriptor_list_addr_offset = 0x15e0,
                     .enemy_resources_desciptors_list_offset = 0x1574,
                 },
                 pe2_encounter_t{
                     .id = 11,
                     .mask = RANDOMIZED,
                     .encounter_value = 400,
                     .enemy_record_descriptor_list_addr_offset = 0x1620,
                     .enemy_resources_desciptors_list_offset = 0x15a4,
                 },
             },
         },
         pe2_room_encounters_t{
             .id = 0x14,
             .name = game::ROOM_DRYFIELD_NIGHT_WATER_TOWER,
             .encounters_offset = 0x2768,
             .additional_file_end_pointer = 0xf4,
             .environment =
                 pe2_room_environment_t{
                     .spawn_zones =
                         std::vector<utils::geometry_works::Zone>{
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0x0e42, 0x0898),
                                 utils::geometry_works::Point(0x0e42, 0xe9bc),
                                 utils::geometry_works::Point(0x1644, 0xe9bc),
                                 utils::geometry_works::Point(0x1644, 0x0898),
                             }),
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0x0c19, 0xf286),
                                 utils::geometry_works::Point(0x0c19, 0xe9bc),
                                 utils::geometry_works::Point(0xf458, 0xe9bc),
                                 utils::geometry_works::Point(0xf458, 0xf286),
                             }),
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0xe9a7, 0xf4c2),
                                 utils::geometry_works::Point(0xf286, 0xf4c2),
                                 utils::geometry_works::Point(0xf286, 0x125c),
                                 utils::geometry_works::Point(0xe9a1, 0x125c),
                             }),
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0xebb0, 0xf246),
                                 utils::geometry_works::Point(0xebb0, 0xecdc),
                                 utils::geometry_works::Point(0xf1b1, 0xecdc),
                                 utils::geometry_works::Point(0xf1b1, 0xf362),
                             }),
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0xf574, 0x1644),
                                 utils::geometry_works::Point(0xff1a, 0x1644),
                                 utils::geometry_works::Point(0xff1a, 0x0dc6),
                                 utils::geometry_works::Point(0xf574, 0x0dc6),
                             }),
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0xfa88, 0xf6e8),
                                 utils::geometry_works::Point(0xfa88, 0x09f6),
                                 utils::geometry_works::Point(0x0834, 0x0972),
                                 utils::geometry_works::Point(0x0834, 0xf60a),
                             }),
                         },
                     .dead_zones =
                         std::vector<utils::geometry_works::Zone>{
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0x0158, 0x0281),
                                 utils::geometry_works::Point(0x05fb, 0x0281),
                                 utils::geometry_works::Point(0x05fb, 0xfde5),
                                 utils::geometry_works::Point(0x019b, 0xfde5),
                             }),
                         },
                 },
             .encounters{
                 pe2_encounter_t{
                     .id = 1,
                     .mask = RANDOMIZED,
                     .encounter_value = 400,
                     .enemy_record_descriptor_list_addr_offset = 0x55a4,
                     .enemy_resources_desciptors_list_offset = 0x5524,
                 },
                 pe2_encounter_t{
                     .id = 2,
                     .mask = FORBID_MULTI_ENEMY_ENC,
                     .encounter_value = 400,
                     .enemy_record_descriptor_list_addr_offset = 0x55ac,
                     .enemy_resources_desciptors_list_offset = 0x5548,
                 },
                 pe2_encounter_t{
                     .id = 11,
                     .mask = FORBID_MULTI_ENEMY_ENC_AND_SLOT_3,
                     .encounter_value = 400,
                     .enemy_record_descriptor_list_addr_offset = 0x55f4,
                     .enemy_resources_desciptors_list_offset = 0x5560,
                 },
                 pe2_encounter_t{
                     .id = 21,
                     .mask = FORBID_MULTI_ENEMY_ENC_AND_SLOT_3,
                     .encounter_value = 400,
                     .enemy_record_descriptor_list_addr_offset = 0x5644,
                     .enemy_resources_desciptors_list_offset = 0x5578,
                 },
             },
         },
         pe2_room_encounters_t{
             .id = 0x15,
             .name = game::ROOM_DRYFIELD_NIGHT_WATER_TANK,
             .encounters_offset = 0x28d8,
             .additional_file_end_pointer = 0xfc,
             .encounters{
                 pe2_encounter_t{
                     .id = 1,
                     .mask = UNRANDOMIZED,
                     .encounter_value = 0,
                     .enemy_record_descriptor_list_addr_offset = 0x31ac,
                     .enemy_resources_desciptors_list_offset = 0x315c,
                 },
                 pe2_encounter_t{
                     .id = 10,
                     .mask = UNRANDOMIZED,

                     .encounter_value = 0,
                     .enemy_record_descriptor_list_addr_offset = 0x31f4,
                     .enemy_resources_desciptors_list_offset = 0x3168,
                 },
                 pe2_encounter_t{
                     .id = 11,
                     .mask = UNRANDOMIZED,
                     .encounter_value = 0,
                     .enemy_record_descriptor_list_addr_offset = 0x31fc,
                     .enemy_resources_desciptors_list_offset = 0x318c,
                 },
             },
         },
         pe2_room_encounters_t{
             .id = 0x16,
             .name = game::ROOM_DRYFIELD_NIGHT_BREEZEWAY,
             .encounters_offset = 0x2958,
             .additional_file_end_pointer = 0x104,
             .environment =
                 pe2_room_environment_t{
                     .spawn_zones =
                         std::vector<utils::geometry_works::Zone>{
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0x36e0, 0x07f6),
                                 utils::geometry_works::Point(0x36e0, 0x0514),
                                 utils::geometry_works::Point(0x4524, 0x0542),
                                 utils::geometry_works::Point(0x4524, 0x0e74),
                             }),
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0x34b5, 0x076d),
                                 utils::geometry_works::Point(0x1900, 0x076d),
                                 utils::geometry_works::Point(0x1900, 0x0514),
                                 utils::geometry_works::Point(0x34b5, 0x0514),
                             }),
                         },
                 },
             .encounters{
                 pe2_encounter_t{
                     .id = 1,
                     .mask = RANDOMIZED,
                     .encounter_value = 400,
                     .enemy_record_descriptor_list_addr_offset = 0x2e2c,
                     .enemy_resources_desciptors_list_offset = 0x2dc4,
                 },
                 pe2_encounter_t{
                     .id = 3,
                     .mask = RANDOMIZED,
                     .encounter_value = 400,
                     .enemy_record_descriptor_list_addr_offset = 0x2e3c,
                     .enemy_resources_desciptors_list_offset = 0x2ddc,
                 },
                 pe2_encounter_t{
                     .id = 21,
                     .mask = RANDOMIZED,
                     .encounter_value = 400,
                     .enemy_record_descriptor_list_addr_offset = 0x2ecc,
                     .enemy_resources_desciptors_list_offset = 0x2e00,
                 },
             },
         },
         pe2_room_encounters_t{
             .id = 0x18,
             .name = game::ROOM_DRYFIELD_NIGHT_GARAGE,
             .encounters_offset = 0x2b98,
             .additional_file_end_pointer = 0x114,
             .environment =
                 pe2_room_environment_t{
                     .spawn_zones =
                         std::vector<utils::geometry_works::Zone>{
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0x24e3, 0x07f8),
                                 utils::geometry_works::Point(0x13db, 0x07f8),
                                 utils::geometry_works::Point(0x13db, 0x1c92),
                                 utils::geometry_works::Point(0x24e3, 0x1c92),
                             }),
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0x0334, 0x1d60),
                                 utils::geometry_works::Point(0x08b6, 0x1d60),
                                 utils::geometry_works::Point(0x08b6, 0x074f),
                                 utils::geometry_works::Point(0x0334, 0x074f),
                             }),
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0x0af3, 0x0816),
                                 utils::geometry_works::Point(0x106e, 0x0816),
                                 utils::geometry_works::Point(0x106e, 0x0552),
                                 utils::geometry_works::Point(0x0af3, 0x0552),
                             }),
                         },
                     .spawn_aerial_zones =
                         std::vector<utils::geometry_works::Zone>{
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0x0334, 0x1d60),
                                 utils::geometry_works::Point(0x0492, 0x0747),
                                 utils::geometry_works::Point(0x24a5, 0x07f8),
                                 utils::geometry_works::Point(0x2516, 0x1c94),
                             }),
                         },
                     .dead_zones =
                         std::vector<utils::geometry_works::Zone>{
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0x1dfd, 0x161a),
                                 utils::geometry_works::Point(0x2453, 0x161a),
                                 utils::geometry_works::Point(0x2453, 0x0f07),
                                 utils::geometry_works::Point(0x1dfd, 0x0f07),
                             }),
                         },
                 },
             .encounters{
                 pe2_encounter_t{
                     .id = 1,
                     .mask = RANDOMIZED,
                     .encounter_value = 400,
                     .enemy_record_descriptor_list_addr_offset = 0x9f04,
                     .enemy_resources_desciptors_list_offset = 0x9e90,
                 },
                 pe2_encounter_t{
                     .id = 2,
                     .mask = UNRANDOMIZED,
                     .encounter_value = 0,
                     .enemy_record_descriptor_list_addr_offset = 0x9f0c,
                     .enemy_resources_desciptors_list_offset = 0x9ea8,
                 },
                 pe2_encounter_t{
                     .id = 3,
                     .mask = UNRANDOMIZED,
                     .encounter_value = 0,
                     .enemy_record_descriptor_list_addr_offset = 0x9f14,
                     .enemy_resources_desciptors_list_offset = 0x9ecc,
                 },
                 pe2_encounter_t{
                     .id = 4,
                     .mask = RANDOMIZED,
                     .encounter_value = 400,
                     .enemy_record_descriptor_list_addr_offset = 0x9f1c,
                     .enemy_resources_desciptors_list_offset = 0x9ee4,
                 },
             },
             .forbidden_enemies{0x01},
         },
         pe2_room_encounters_t{
             .id = 0x19,
             .name = game::ROOM_DRYFIELD_NIGHT_DRIVEWAY,
             .encounters_offset = 0x2c68,
             .additional_file_end_pointer = 0x11c,
             .environment =
                 pe2_room_environment_t{
                     .spawn_zones =
                         std::vector<utils::geometry_works::Zone>{
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0x1773, 0x01ba),
                                 utils::geometry_works::Point(0x1705, 0x085b),
                                 utils::geometry_works::Point(0xfe17, 0x085b),
                                 utils::geometry_works::Point(0xfe17, 0x019d),
                             }),
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0xf9b8, 0x0639),
                                 utils::geometry_works::Point(0xf1f8, 0x0635),
                                 utils::geometry_works::Point(0xf1f8, 0xf9c1),
                                 utils::geometry_works::Point(0xf9b8, 0xf9c1),
                             }),
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0xef82, 0xf9c0),
                                 utils::geometry_works::Point(0xdf84, 0xf9c0),
                                 utils::geometry_works::Point(0xdf84, 0xfaba),
                                 utils::geometry_works::Point(0xef82, 0xfaba),
                             }),
                         },
                 },
             .encounters{
                 pe2_encounter_t{
                     .id = 1,
                     .mask = RANDOMIZED,
                     .encounter_value = 400,
                     .enemy_record_descriptor_list_addr_offset = 0x4994,
                     .enemy_resources_desciptors_list_offset = 0x4938,
                 },
                 pe2_encounter_t{
                     .id = 3,
                     .mask = RANDOMIZED,
                     .encounter_value = 400,
                     .enemy_record_descriptor_list_addr_offset = 0x49a4,
                     .enemy_resources_desciptors_list_offset = 0x4950,
                 },
                 pe2_encounter_t{
                     .id = 21,
                     .mask = RANDOMIZED,
                     .encounter_value = 400,
                     .enemy_record_descriptor_list_addr_offset = 0x4a34,
                     .enemy_resources_desciptors_list_offset = 0x4968,
                 },
             },
         },
         pe2_room_encounters_t{.id = 0x1a,
                               .name = game::ROOM_DRYFIELD_NIGHT_JUNK_YARD,
                               .encounters_offset = 0x2ea8,
                               .additional_file_end_pointer = 0x124,
                               .environment =
                                   pe2_room_environment_t{
                                       .spawn_zones =
                                           std::vector<utils::geometry_works::Zone>{
                                               utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                                   utils::geometry_works::Point(0x56b4, 0x0faf),
                                                   utils::geometry_works::Point(0x38a6, 0x0fa3),
                                                   utils::geometry_works::Point(0x389c, 0x0d11),
                                                   utils::geometry_works::Point(0x574a, 0x0d11),
                                               }),
                                               utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                                   utils::geometry_works::Point(0x328b, 0x104e),
                                                   utils::geometry_works::Point(0x328b, 0x0143),
                                                   utils::geometry_works::Point(0x120c, 0x014c),
                                                   utils::geometry_works::Point(0x120c, 0x104e),
                                               }),
                                               utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                                   utils::geometry_works::Point(0x0e09, 0x0c62),
                                                   utils::geometry_works::Point(0x0e09, 0x02af),
                                                   utils::geometry_works::Point(0x0424, 0x02af),
                                                   utils::geometry_works::Point(0x0424, 0x0c62),
                                               }),
                                           },
                                   },
                               .encounters{
                                   pe2_encounter_t{
                                       .id = 1,
                                       .mask = RANDOMIZED,
                                       .encounter_value = 400,
                                       .enemy_record_descriptor_list_addr_offset = 0x6e38,
                                       .enemy_resources_desciptors_list_offset = 0x6d94,
                                   },
                                   pe2_encounter_t{
                                       .id = 2,
                                       .mask = RANDOMIZED,
                                       .encounter_value = 400,
                                       .enemy_record_descriptor_list_addr_offset = 0x6e40,
                                       .enemy_resources_desciptors_list_offset = 0x6dac,
                                   },
                                   pe2_encounter_t{
                                       .id = 7,
                                       .mask = RANDOMIZED,
                                       .encounter_value = 400,
                                       .enemy_record_descriptor_list_addr_offset = 0x6e68,
                                       .enemy_resources_desciptors_list_offset = 0x6de8,
                                   },
                                   pe2_encounter_t{
                                       .id = 4,
                                       .mask = RANDOMIZED,
                                       .encounter_value = 400,
                                       .enemy_record_descriptor_list_addr_offset = 0x6e50,
                                       .enemy_resources_desciptors_list_offset = 0x6dd0,
                                   },
                                   pe2_encounter_t{
                                       .id = 21,
                                       .mask = RANDOMIZED,
                                       .encounter_value = 400,
                                       .enemy_record_descriptor_list_addr_offset = 0x6ed8,
                                       .enemy_resources_desciptors_list_offset = 0x6e0c,
                                   },
                               },
                               .file_patch_map{// Spider drop on the car cutscene
                                               {0x2a6, 0x45}}},
         pe2_room_encounters_t{
             .id = 0x1b,
             .name = game::ROOM_DRYFIELD_NIGHT_TRAILER_COACH,
             .encounters_offset = 0x2ff8,
             .additional_file_end_pointer = 0x12c,
             .encounters{
                 pe2_encounter_t{
                     .id = 1,
                     .mask = UNRANDOMIZED,
                     .encounter_value = 0,
                     .enemy_record_descriptor_list_addr_offset = 0xeba4,
                     .enemy_resources_desciptors_list_offset = 0xeb84,
                 },
             },
         },
         pe2_room_encounters_t{
             .id = 0x1c,
             .name = game::ROOM_DRYFIELD_NIGHT_MOTEL_ROOM_5,
             .encounters_offset = 0x3018,
             .additional_file_end_pointer = 0x134,
             .environment =
                 pe2_room_environment_t{
                     .spawn_zones =
                         std::vector<utils::geometry_works::Zone>{
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0x1280, 0x1480),
                                 utils::geometry_works::Point(0x1280, 0x0100),
                                 utils::geometry_works::Point(0x0a00, 0x0100),
                                 utils::geometry_works::Point(0x0a00, 0x1480),
                             }),
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0x0300, 0x0480),
                                 utils::geometry_works::Point(0x0300, 0x0a00),
                                 utils::geometry_works::Point(0x0a00, 0x0a00),
                                 utils::geometry_works::Point(0x0a00, 0x0480),
                             }),
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0x0100, 0x1400),
                                 utils::geometry_works::Point(0x0100, 0x0c00),
                                 utils::geometry_works::Point(0x0700, 0x0c00),
                                 utils::geometry_works::Point(0x0700, 0x1400),
                             }),
                         },
                     .spawn_aerial_zones =
                         std::vector<utils::geometry_works::Zone>{
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0x1280, 0x0900),
                                 utils::geometry_works::Point(0x1280, 0x0000),
                                 utils::geometry_works::Point(0x0200, 0x0000),
                                 utils::geometry_works::Point(0x0200, 0x0900),
                             }),
                         },
                 },
             .encounters{
                 pe2_encounter_t{
                     .id = 1,
                     .mask = RANDOMIZED,
                     .encounter_value = 400,
                     .enemy_record_descriptor_list_addr_offset = 0x3bd8,
                     .enemy_resources_desciptors_list_offset = 0x3b94,
                 },
                 pe2_encounter_t{
                     .id = 2,
                     .mask = RANDOMIZED,
                     .encounter_value = 400,
                     .enemy_record_descriptor_list_addr_offset = 0x3be0,
                     .enemy_resources_desciptors_list_offset = 0x3bb8,
                 },
             },
         },
         pe2_room_encounters_t{
             .id = 0x1d,
             .name = game::ROOM_DRYFIELD_NIGHT_MOTEL_BALCONY,
             .encounters_offset = 0x30d8,
             .additional_file_end_pointer = 0x13c,
             .environment =
                 pe2_room_environment_t{
                     .separated_zones = true,
                     .spawn_zones =
                         std::vector<utils::geometry_works::Zone>{
                             utils::geometry_works::Zone(
                                 utils::geometry_works::Polygon{
                                     utils::geometry_works::Point(0xe5d4, 0xedfe),
                                     utils::geometry_works::Point(0xeaaa, 0xedfe),
                                     utils::geometry_works::Point(0xeac2, 0x22be),
                                     utils::geometry_works::Point(0xe5d4, 0x22be),
                                 },
                                 0xf402),
                             utils::geometry_works::Zone(
                                 utils::geometry_works::Polygon{
                                     utils::geometry_works::Point(0xe5d5, 0x29cc),
                                     utils::geometry_works::Point(0xfdb2, 0x29cc),
                                     utils::geometry_works::Point(0xfdb2, 0x2576),
                                     utils::geometry_works::Point(0xe5d5, 0x2576),
                                 },
                                 0xf402),
                             utils::geometry_works::Zone(
                                 utils::geometry_works::Polygon{
                                     utils::geometry_works::Point(0xe384, 0x0a8c),
                                     utils::geometry_works::Point(0xe3a0, 0x0516),
                                     utils::geometry_works::Point(0xcea1, 0x0516),
                                     utils::geometry_works::Point(0xcea1, 0x0ab0),
                                 },
                                 0xf402),
                         },
                     .spawn_aerial_zones =
                         std::vector<utils::geometry_works::Zone>{
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0xeaa4, 0x1263),
                                 utils::geometry_works::Point(0xe5d5, 0x10a4),
                                 utils::geometry_works::Point(0xe5d5, 0x066e),
                                 utils::geometry_works::Point(0xeaa4, 0x0469),
                             }),
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0xdaaa, 0x0a7a),
                                 utils::geometry_works::Point(0xdaaa, 0x0875),
                                 utils::geometry_works::Point(0xe4ad, 0x06a9),
                                 utils::geometry_works::Point(0xe4ad, 0x0a7a),
                             }),
                         },
                     .dead_zones =
                         std::vector<utils::geometry_works::Zone>{
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0xeaa4, 0x1263),
                                 utils::geometry_works::Point(0xe5d5, 0x10a4),
                                 utils::geometry_works::Point(0xe5d5, 0x066e),
                                 utils::geometry_works::Point(0xeaa4, 0x0469),
                             }),
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0xdaaa, 0x0a7a),
                                 utils::geometry_works::Point(0xdaaa, 0x0875),
                                 utils::geometry_works::Point(0xe4ad, 0x06a9),
                                 utils::geometry_works::Point(0xe4ad, 0x0a7a),
                             }),
                         },
                 },
             .encounters{
                 pe2_encounter_t{
                     .id = 1,
                     .mask = RANDOMIZED,
                     .encounter_value = 400,
                     .enemy_record_descriptor_list_addr_offset = 0x114dc,
                     .enemy_resources_desciptors_list_offset = 0x1145c,
                 },
                 pe2_encounter_t{
                     .id = 2,
                     .mask = UNRANDOMIZED,
                     .encounter_value = 0,
                     .enemy_record_descriptor_list_addr_offset = 0x114e4,
                     .enemy_resources_desciptors_list_offset = 0x11474,
                     .force_reward_value = 600,
                 },
                 pe2_encounter_t{
                     .id = 3,
                     .mask = RANDOMIZED,
                     .encounter_value = 400,
                     .enemy_record_descriptor_list_addr_offset = 0x114ec,
                     .enemy_resources_desciptors_list_offset = 0x114a4,
                 },
                 pe2_encounter_t{
                     .id = 11,
                     .mask = RANDOMIZED,
                     .encounter_value = 400,
                     .enemy_record_descriptor_list_addr_offset = 0x1152c,
                     .enemy_resources_desciptors_list_offset = 0x114bc,
                 },
             },
         },
         pe2_room_encounters_t{.id = 0x1f,
                               .name = game::ROOM_DRYFIELD_NIGHT_MOTEL_LOFT,
                               .encounters_offset = 0x3208,
                               .additional_file_end_pointer = 0x14c,
                               .environment =
                                   pe2_room_environment_t{
                                       .spawn_zones =
                                           std::vector<utils::geometry_works::Zone>{
                                               utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                                   utils::geometry_works::Point(0x12a2, 0x0614),
                                                   utils::geometry_works::Point(0xea3c, 0x0614),
                                                   utils::geometry_works::Point(0xea3c, 0xf9a2),
                                                   utils::geometry_works::Point(0x12a2, 0xf9a2),
                                               }),
                                           },
                                       .dead_zones =
                                           std::vector<utils::geometry_works::Zone>{
                                               utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                                   utils::geometry_works::Point(0xecd4, 0x0370),
                                                   utils::geometry_works::Point(0x04f1, 0x0370),
                                                   utils::geometry_works::Point(0x04f1, 0xfce0),
                                                   utils::geometry_works::Point(0xecd4, 0xfce0),
                                               }),
                                           },
                                   },
                               .encounters{
                                   pe2_encounter_t{
                                       .id = 1,
                                       .mask = RANDOMIZED,
                                       .encounter_value = 400,
                                       .enemy_record_descriptor_list_addr_offset = 0x32d0,
                                       .enemy_resources_desciptors_list_offset = 0x325c,
                                   },
                                   pe2_encounter_t{
                                       .id = 3,
                                       .mask = RANDOMIZED,
                                       .encounter_value = 400,
                                       .enemy_record_descriptor_list_addr_offset = 0x32e0,
                                       .enemy_resources_desciptors_list_offset = 0x3274,
                                   },
                                   pe2_encounter_t{
                                       .id = 4,
                                       .mask = RANDOMIZED,
                                       .encounter_value = 400,
                                       .enemy_record_descriptor_list_addr_offset = 0x32e8,
                                       .enemy_resources_desciptors_list_offset = 0x3298,
                                   },
                                   pe2_encounter_t{
                                       .id = 11,
                                       .mask = RANDOMIZED,
                                       .encounter_value = 400,
                                       .enemy_record_descriptor_list_addr_offset = 0x3320,
                                       .enemy_resources_desciptors_list_offset = 0x32b0,
                                   },
                               },
                               .file_patch_map{// Spider breaks window cutscene
                                               {0x34e, 0x45}}},
         pe2_room_encounters_t{.id = 0x20,
                               .name = game::ROOM_DRYFIELD_NIGHT_WATER_HOLE,
                               .encounters_offset = 0x3398,
                               .additional_file_end_pointer = 0x154,
                               .environment =
                                   pe2_room_environment_t{
                                       .spawn_zones =
                                           std::vector<utils::geometry_works::Zone>{
                                               utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                                   utils::geometry_works::Point(0x553c, 0xf6a0),
                                                   utils::geometry_works::Point(0x553c, 0xf1f0),
                                                   utils::geometry_works::Point(0x2aeb, 0xf1f0),
                                                   utils::geometry_works::Point(0x2aeb, 0xf6a0),
                                               }),
                                               utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                                   utils::geometry_works::Point(0x12c1, 0xf9c0),
                                                   utils::geometry_works::Point(0x12b2, 0xfe70),
                                                   utils::geometry_works::Point(0x2bb0, 0xfe70),
                                                   utils::geometry_works::Point(0x2bb0, 0xf9c0),
                                               }),
                                           },
                                   },
                               .encounters{
                                   pe2_encounter_t{
                                       .id = 1,
                                       .mask = FORBID_MULTI_ENEMY_ENC_AND_SLOT_3,
                                       .encounter_value = 400,
                                       .enemy_record_descriptor_list_addr_offset = 0x5e60,
                                       .enemy_resources_desciptors_list_offset = 0x5df8,
                                   },
                                   pe2_encounter_t{
                                       .id = 2,
                                       .mask = FORBID_MULTI_ENEMY_ENC_AND_SLOT_3,
                                       .encounter_value = 400,
                                       .enemy_record_descriptor_list_addr_offset = 0x5e68,
                                       .enemy_resources_desciptors_list_offset = 0x5e10,
                                   },
                                   pe2_encounter_t{
                                       .id = 11,
                                       .mask = UNRANDOMIZED,
                                       .encounter_value = 0,
                                       .enemy_record_descriptor_list_addr_offset = 0x5ea8,
                                       .enemy_resources_desciptors_list_offset = 0x5e28,
                                   },
                                   pe2_encounter_t{
                                       .id = 22,
                                       .mask = FORBID_MULTI_ENEMY_ENC_AND_SLOT_3,
                                       .encounter_value = 400,
                                       .enemy_record_descriptor_list_addr_offset = 0x5f00,
                                       .enemy_resources_desciptors_list_offset = 0x5e40,
                                   },
                               },
                               .file_patch_map{
                                   // Disable stalker cut scene
                                   {0x844, 0x00},
                                   {0x845, 0x00},
                                   {0x846, 0x00},
                                   {0x847, 0x00},
                               }},
         pe2_room_encounters_t{
             .id = 0x22,
             .name = game::ROOM_DRYFIELD_NIGHT_WINE_BARRELS,
             .encounters_offset = 0x3438,
             .additional_file_end_pointer = 0x164,
             .environment =
                 pe2_room_environment_t{
                     .spawn_zones =
                         std::vector<utils::geometry_works::Zone>{
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0x0ed8, 0x1068),
                                 utils::geometry_works::Point(0x236e, 0x1068),
                                 utils::geometry_works::Point(0x236e, 0x012c),
                                 utils::geometry_works::Point(0x0ed8, 0x012c),
                             }),
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0x0514, 0x0ce4),
                                 utils::geometry_works::Point(0x0898, 0x0cc1),
                                 utils::geometry_works::Point(0x0898, 0x012c),
                                 utils::geometry_works::Point(0x0514, 0x012c),
                             }),
                         },
                     .spawn_aerial_zones =
                         std::vector<utils::geometry_works::Zone>{
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0x0ed8, 0x1068),
                                 utils::geometry_works::Point(0x236e, 0x1068),
                                 utils::geometry_works::Point(0x236e, 0x012c),
                                 utils::geometry_works::Point(0x0ed8, 0x012c),
                             }),
                         },
                     .dead_zones =
                         std::vector<utils::geometry_works::Zone>{
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0x233e, 0x0e10),
                                 utils::geometry_works::Point(0x233e, 0x0318),
                                 utils::geometry_works::Point(0x161e, 0x0318),
                                 utils::geometry_works::Point(0x161e, 0x0e10),
                             }),
                         },
                 },
             .encounters{
                 pe2_encounter_t{
                     .id = 1,
                     .mask = RANDOMIZED,
                     .encounter_value = 400,
                     .enemy_record_descriptor_list_addr_offset = 0x31c8,
                     .enemy_resources_desciptors_list_offset = 0x3160,
                 },
                 pe2_encounter_t{
                     .id = 3,
                     .mask = RANDOMIZED,
                     .encounter_value = 400,
                     .enemy_record_descriptor_list_addr_offset = 0x31d8,
                     .enemy_resources_desciptors_list_offset = 0x3178,
                 },
                 pe2_encounter_t{
                     .id = 11,
                     .mask = RANDOMIZED,
                     .encounter_value = 400,
                     .enemy_record_descriptor_list_addr_offset = 0x3218,
                     .enemy_resources_desciptors_list_offset = 0x319c,
                 },
             },
             .file_patch_map{
                 // Enable snail magazine shelf
                 {0x48, 0x00},
                 {0x49, 0x00},
                 {0x4a, 0x00},
                 {0x4b, 0x00},
             }},
         pe2_room_encounters_t{
             .id = 0x26,
             .name = game::ROOM_DRYFIELD_NIGHT_CELLAR,
             .encounters_offset = 0x3608,
             .additional_file_end_pointer = 0x184,
             .environment =
                 pe2_room_environment_t{
                     .spawn_zones =
                         std::vector<utils::geometry_works::Zone>{
                             utils::geometry_works::Zone(
                                 utils::geometry_works::Polygon{
                                     utils::geometry_works::Point(0x4524, 0xfd44),
                                     utils::geometry_works::Point(0x3caa, 0xfd2e),
                                     utils::geometry_works::Point(0x3caa, 0xe447),
                                     utils::geometry_works::Point(0x4524, 0xe447),
                                 },
                                 0xfc99),
                             utils::geometry_works::Zone(
                                 utils::geometry_works::Polygon{
                                     utils::geometry_works::Point(0x393e, 0xe2f3),
                                     utils::geometry_works::Point(0xfa88, 0xe2f3),
                                     utils::geometry_works::Point(0xfa88, 0xdaa2),
                                     utils::geometry_works::Point(0x393e, 0xdaa2),
                                 },
                                 0xfc99),
                         },
                 },
             .encounters{pe2_encounter_t{
                             .id = 1,
                             .mask = UNRANDOMIZED,
                             .encounter_value = 0,
                             .enemy_record_descriptor_list_addr_offset = 0x2d24,
                             .enemy_resources_desciptors_list_offset = 0x2ca4,
                         },
                         pe2_encounter_t{
                             .id = 2,
                             .mask = RANDOMIZED,
                             .encounter_value = 400,
                             .enemy_record_descriptor_list_addr_offset = 0x2d2c,
                             .enemy_resources_desciptors_list_offset = 0x2cbc,
                         },
                         pe2_encounter_t{
                             .id = 7,
                             .mask = RANDOMIZED,
                             .encounter_value = 400,
                             .enemy_record_descriptor_list_addr_offset = 0x2d54,
                             .enemy_resources_desciptors_list_offset = 0x2cec,
                         },
                         pe2_encounter_t{
                             .id = 3,
                             .mask = RANDOMIZED,
                             .encounter_value = 400,
                             .enemy_record_descriptor_list_addr_offset = 0x2d34,
                             .enemy_resources_desciptors_list_offset = 0x2cd4,
                         },
                         pe2_encounter_t{
                             .id = 11,
                             .mask = RANDOMIZED,
                             .encounter_value = 400,
                             .enemy_record_descriptor_list_addr_offset = 0x2d74,
                             .enemy_resources_desciptors_list_offset = 0x2d04,
                         }},
         },
     }},
    {game::MINES_AND_SHELTER_LEVEL,
     {
         pe2_room_encounters_t{.id = 0x01,
                               .name = game::ROOM_MINES_AND_SHELTER_MESA,
                               .encounters_offset = 0xc144,
                               .encounters_end = 0xc324,
                               .additional_file_end_pointer = 0x1f8,
                               .environment =
                                   pe2_room_environment_t{
                                       .spawn_zones =
                                           std::vector<utils::geometry_works::Zone>{
                                               utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                                   utils::geometry_works::Point(0x0600, 0x1800),
                                                   utils::geometry_works::Point(0x2a00, 0x1a00),
                                                   utils::geometry_works::Point(0x2d00, 0x0300),
                                                   utils::geometry_works::Point(0x0400, 0x0100),
                                               }),
                                           },
                                       .dead_zones =
                                           std::vector<utils::geometry_works::Zone>{
                                               utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                                   utils::geometry_works::Point(0x0e00, 0x0a00),
                                                   utils::geometry_works::Point(0x0f00, 0x1500),
                                                   utils::geometry_works::Point(0x2400, 0x1400),
                                                   utils::geometry_works::Point(0x2400, 0x0b00),
                                               }),
                                           },
                                   },
                               .encounters{pe2_encounter_t{
                                               .id = 1,
                                               .mask = UNRANDOMIZED,
                                               .encounter_value = 0,
                                               .enemy_record_descriptor_list_addr_offset = 0xc33c,
                                               .enemy_resources_desciptors_list_offset = 0xc084,
                                           },
                                           pe2_encounter_t{
                                               .id = 2,
                                               .mask = RANDOMIZED,
                                               .encounter_value = 450,
                                               .enemy_record_descriptor_list_addr_offset = 0xc344,
                                               .enemy_resources_desciptors_list_offset = 0xc09c,
                                           },
                                           pe2_encounter_t{
                                               .id = 3,
                                               .mask = RANDOMIZED,
                                               .encounter_value = 450,
                                               .enemy_record_descriptor_list_addr_offset = 0xc34c,
                                               .enemy_resources_desciptors_list_offset = 0xc0b4,
                                           },
                                           pe2_encounter_t{
                                               .id = 7,
                                               .mask = RANDOMIZED,
                                               .encounter_value = 450,
                                               .enemy_record_descriptor_list_addr_offset = 0xc36c,
                                               .enemy_resources_desciptors_list_offset = 0xc0fc,
                                           }},
                               .file_patch_map{
                                   // set bridge in caves progression
                                   {0x3d4, 0xa4},
                                   {0x3d5, 0x00},
                               }},
         pe2_room_encounters_t{
             .id = 0x02,
             .name = game::ROOM_MINES_AND_SHELTER_CAVERN,
             .encounters_offset = 0x10ba8,
             .encounters_end = 0x10c68,
             .additional_file_end_pointer = 0x200,
             .environment =
                 pe2_room_environment_t{
                     .spawn_zones =
                         std::vector<utils::geometry_works::Zone>{
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0x0308, 0x1b89),
                                 utils::geometry_works::Point(0x4393, 0x1d50),
                                 utils::geometry_works::Point(0x4468, 0x01b3),
                                 utils::geometry_works::Point(0x04ea, 0x033d),
                             }),
                         },
                     .dead_zones =
                         std::vector<utils::geometry_works::Zone>{
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0x0a45, 0x1868),
                                 utils::geometry_works::Point(0x0a1d, 0x0bd9),
                                 utils::geometry_works::Point(0x3ce6, 0x0b43),
                                 utils::geometry_works::Point(0x3b60, 0x1797),
                             }),
                         },
                 },
             .encounters{pe2_encounter_t{
                             .id = 1,
                             .mask = UNRANDOMIZED,
                             .encounter_value = 0,
                             .enemy_record_descriptor_list_addr_offset = 0x10c80,
                             .enemy_resources_desciptors_list_offset = 0x10b30,
                         },
                         pe2_encounter_t{
                             .id = 2,
                             .mask = RANDOMIZED,
                             .encounter_value = 450,
                             .enemy_record_descriptor_list_addr_offset = 0x10c88,
                             .enemy_resources_desciptors_list_offset = 0x10b48,
                         },
                         pe2_encounter_t{
                             .id = 11,
                             .mask = RANDOMIZED,
                             .encounter_value = 450,
                             .enemy_record_descriptor_list_addr_offset = 0x10cd0,
                             .enemy_resources_desciptors_list_offset = 0x10b78,
                         }},
         },
         pe2_room_encounters_t{
             .id = 0x03,
             .name = game::ROOM_MINES_AND_SHELTER_TUNNEL_ENTRANCE,
             .encounters_offset = 0x1b4c,
             .encounters_end = 0x1d5c,
             .additional_file_end_pointer = 0x208,
             .environment =
                 pe2_room_environment_t{
                     .spawn_zones =
                         std::vector<utils::geometry_works::Zone>{
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0x1b00, 0x0b00),
                                 utils::geometry_works::Point(0x4300, 0x0c00),
                                 utils::geometry_works::Point(0x4500, 0x0200),
                                 utils::geometry_works::Point(0x1a00, 0x0200),
                             }),
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0x0100, 0x0600),
                                 utils::geometry_works::Point(0x1800, 0x0a00),
                                 utils::geometry_works::Point(0x1300, 0x0100),
                                 utils::geometry_works::Point(0x0200, 0x0300),
                             }),
                         },
                 },
             .encounters{pe2_encounter_t{
                             .id = 1,
                             .mask = RANDOMIZED,
                             .encounter_value = 450,
                             .enemy_record_descriptor_list_addr_offset = 0x1d74,
                             .enemy_resources_desciptors_list_offset = 0x1abc,
                         },
                         pe2_encounter_t{
                             .id = 3,
                             .mask = RANDOMIZED,
                             .encounter_value = 450,
                             .enemy_record_descriptor_list_addr_offset = 0x1d84,
                             .enemy_resources_desciptors_list_offset = 0x1af8,
                         },
                         pe2_encounter_t{
                             .id = 4,
                             .mask = RANDOMIZED,
                             .encounter_value = 450,
                             .enemy_record_descriptor_list_addr_offset = 0x1d8c,
                             .enemy_resources_desciptors_list_offset = 0x1b10,
                         }},
         },
         pe2_room_encounters_t{.id = 0x04,
                               .name = game::ROOM_MINES_AND_SHELTER_TUNNEL,
                               .encounters_offset = 0x29ac,
                               .encounters_end = 0x2bfc,
                               .additional_file_end_pointer = 0x210,
                               .environment =
                                   pe2_room_environment_t{
                                       .spawn_zones =
                                           std::vector<utils::geometry_works::Zone>{
                                               utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                                   utils::geometry_works::Point(0x35c8, 0x0a60),
                                                   utils::geometry_works::Point(0x0458, 0x0d9a),
                                                   utils::geometry_works::Point(0x02d1, 0x02b7),
                                                   utils::geometry_works::Point(0x3d41, 0x010e),
                                               }),
                                           },
                                       .dead_zones =
                                           std::vector<utils::geometry_works::Zone>{
                                               utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                                   utils::geometry_works::Point(0x306f, 0x01b6),
                                                   utils::geometry_works::Point(0x2d6e, 0x07bf),
                                                   utils::geometry_works::Point(0x27d3, 0x05df),
                                                   utils::geometry_works::Point(0x26b4, 0x002f),
                                               }),
                                               utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                                   utils::geometry_works::Point(0x2b06, 0x0e88),
                                                   utils::geometry_works::Point(0x2468, 0x0894),
                                                   utils::geometry_works::Point(0x178b, 0x0965),
                                                   utils::geometry_works::Point(0x115d, 0x0eec),
                                               }),
                                               utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                                   utils::geometry_works::Point(0x0b27, 0x00fd),
                                                   utils::geometry_works::Point(0x10ed, 0x0847),
                                                   utils::geometry_works::Point(0x16f2, 0x0616),
                                                   utils::geometry_works::Point(0x142b, 0x00a0),
                                               }),
                                           },
                                   },
                               .encounters{pe2_encounter_t{
                                               .id = 1,
                                               .mask = RANDOMIZED,
                                               .encounter_value = 450,
                                               .enemy_record_descriptor_list_addr_offset = 0x2c14,
                                               .enemy_resources_desciptors_list_offset = 0x2910,
                                           },
                                           pe2_encounter_t{
                                               .id = 3,
                                               .mask = RANDOMIZED,
                                               .encounter_value = 450,
                                               .enemy_record_descriptor_list_addr_offset = 0x2c24,
                                               .enemy_resources_desciptors_list_offset = 0x2940,
                                           },
                                           pe2_encounter_t{
                                               .id = 4,
                                               .mask = RANDOMIZED,
                                               .encounter_value = 450,
                                               .enemy_record_descriptor_list_addr_offset = 0x2c2c,
                                               .enemy_resources_desciptors_list_offset = 0x2964,
                                           }},
                               .file_patch_map{// Scavanger cutscene
                                               {0xea, 0x45}}},
         pe2_room_encounters_t{
             .id = 0x05,
             .name = game::ROOM_MINES_AND_SHELTER_GORGE,
             .encounters_offset = 0x5bd4,
             .encounters_end = 0x5f74,
             .additional_file_end_pointer = 0x218,
             .environment =
                 pe2_room_environment_t{
                     .separated_zones = true,
                     .spawn_zones =
                         std::vector<utils::geometry_works::Zone>{
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0x4700, 0x0180),
                                 utils::geometry_works::Point(0x4700, 0x0c00),
                                 utils::geometry_works::Point(0x3c00, 0x0c00),
                                 utils::geometry_works::Point(0x3c00, 0x0180),
                             }),
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0x0700, 0x0100),
                                 utils::geometry_works::Point(0x0700, 0x0d00),
                                 utils::geometry_works::Point(0x0080, 0x0d00),
                                 utils::geometry_works::Point(0x0080, 0x0100),
                             }),
                             // utils::geometry_works::Zone(utils::geometry_works::Polygon {
                             //     utils::geometry_works::Point(0x0800, 0x0400),
                             //     utils::geometry_works::Point(0x0800, 0x0800),
                             //     utils::geometry_works::Point(0x3c00, 0x0800),
                             //     utils::geometry_works::Point(0x3c00, 0x0400),
                             // }),
                         },
                     .spawn_aerial_zones =
                         std::vector<utils::geometry_works::Zone>{
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0x3524, 0x0de0),
                                 utils::geometry_works::Point(0x3524, 0x0751),
                                 utils::geometry_works::Point(0x1100, 0x0751),
                                 utils::geometry_works::Point(0x1100, 0x0de0),
                             }),
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0x3524, 0x03c0),
                                 utils::geometry_works::Point(0x3524, 0x0000),
                                 utils::geometry_works::Point(0x1100, 0x0000),
                                 utils::geometry_works::Point(0x1100, 0x03c0),
                             }),
                         },
                 },
             .encounters{pe2_encounter_t{
                             .id = 1,
                             .mask = RANDOMIZED,
                             .encounter_value = 450,
                             .enemy_record_descriptor_list_addr_offset = 0x5f8c,
                             .enemy_resources_desciptors_list_offset = 0x5b50,
                         },
                         pe2_encounter_t{
                             .id = 2,
                             .mask = RANDOMIZED,
                             .encounter_value = 450,
                             .enemy_record_descriptor_list_addr_offset = 0x5f94,
                             .enemy_resources_desciptors_list_offset = 0x5b68,
                         },
                         pe2_encounter_t{
                             .id = 3,
                             .mask = RANDOMIZED,
                             .encounter_value = 450,
                             .enemy_record_descriptor_list_addr_offset = 0x5f9c,
                             .enemy_resources_desciptors_list_offset = 0x5b80,
                         },
                         pe2_encounter_t{
                             .id = 11,
                             .mask = RANDOMIZED,
                             .encounter_value = 450,
                             .enemy_record_descriptor_list_addr_offset = 0x5fdc,
                             .enemy_resources_desciptors_list_offset = 0x5bbc,
                         }},
             .forbidden_enemies{0x15},
             .file_patch_map{// Bat cutscene
                             {0x1ea, 0x45}}},
         pe2_room_encounters_t{
             .id = 0x07,
             .name = game::ROOM_MINES_AND_SHELTER_FORKED_TUNNEL,
             .encounters_offset = 0x7da4,
             .encounters_end = 0x7f34,
             .additional_file_end_pointer = 0x228,
             .environment =
                 pe2_room_environment_t{
                     .spawn_zones =
                         std::vector<utils::geometry_works::Zone>{
                             utils::geometry_works::Zone(
                                 utils::geometry_works::Polygon{
                                     utils::geometry_works::Point(0x076a, 0x1cd1),
                                     utils::geometry_works::Point(0x0a63, 0x1cd1),
                                     utils::geometry_works::Point(0x07b8, 0x0d9e),
                                     utils::geometry_works::Point(0x0e37, 0x0407),
                                 },
                                 0x8200),
                         },
                 },
             .encounters{
                 pe2_encounter_t{
                     .id = 1,
                     .mask = UNRANDOMIZED,
                     .encounter_value = 0,
                     .enemy_record_descriptor_list_addr_offset = 0x7f4c,
                     .enemy_resources_desciptors_list_offset = 0x7d20,
                 },
                 pe2_encounter_t{
                     .id = 2,
                     .mask = UNRANDOMIZED,
                     .encounter_value = 0,
                     .enemy_record_descriptor_list_addr_offset = 0x7f54,
                     .enemy_resources_desciptors_list_offset = 0x7d2c,
                 },
                 pe2_encounter_t{
                     .id = 3,
                     .mask = UNRANDOMIZED,
                     .encounter_value = 0,
                     .enemy_record_descriptor_list_addr_offset = 0x7f5c,
                     .enemy_resources_desciptors_list_offset = 0x7d44,
                 },
             },
         },
         pe2_room_encounters_t{
             .id = 0x08,
             .name = game::ROOM_MINES_AND_SHELTER_SECRET_PASSAGE,
             .encounters_offset = 0x5cc0,
             .encounters_end = 0x5d70,
             .additional_file_end_pointer = 0x230,
             .environment =
                 pe2_room_environment_t{
                     .spawn_zones =
                         std::vector<utils::geometry_works::Zone>{
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0x0251, 0x3462),
                                 utils::geometry_works::Point(0x0a3a, 0x3462),
                                 utils::geometry_works::Point(0x014d, 0x0964),
                                 utils::geometry_works::Point(0x0cfb, 0x0983),
                             }),
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0x1105, 0x098e),
                                 utils::geometry_works::Point(0x1128, 0x114f),
                                 utils::geometry_works::Point(0x2f78, 0x114f),
                                 utils::geometry_works::Point(0x2f78, 0x098e),
                             }),
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0x318a, 0x12b6),
                                 utils::geometry_works::Point(0x4514, 0x12ad),
                                 utils::geometry_works::Point(0x44e8, 0x08f9),
                                 utils::geometry_works::Point(0x31b3, 0x08ad),
                             }),
                         },
                 },
             .encounters{
                 pe2_encounter_t{
                     .id = 1,
                     .mask = UNRANDOMIZED,
                     .encounter_value = 0,
                     .enemy_record_descriptor_list_addr_offset = 0x5d88,
                     .enemy_resources_desciptors_list_offset = 0x5c60,
                 },
                 pe2_encounter_t{
                     .id = 3,
                     .mask = RANDOMIZED,
                     .encounter_value = 450,
                     .enemy_record_descriptor_list_addr_offset = 0x5d98,
                     .enemy_resources_desciptors_list_offset = 0x5c90,
                 },
                 pe2_encounter_t{
                     .id = 11,
                     .mask = RANDOMIZED,
                     .encounter_value = 450,
                     .enemy_record_descriptor_list_addr_offset = 0x5dd8,
                     .enemy_resources_desciptors_list_offset = 0x5ca8,
                 },
             },
         },
         pe2_room_encounters_t{.id = 0x09,
                               .name = game::ROOM_MINES_AND_SHELTER_ELEVATOR_HALL,
                               .encounters_offset = 0x7230,
                               .encounters_end = 0x7370,
                               .additional_file_end_pointer = 0x238,
                               .environment =
                                   pe2_room_environment_t{
                                       .spawn_zones =
                                           std::vector<utils::geometry_works::Zone>{
                                               utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                                   utils::geometry_works::Point(0xdb8a, 0xfbd6),
                                                   utils::geometry_works::Point(0xd94a, 0x046e),
                                                   utils::geometry_works::Point(0x146a, 0xfbd0),
                                                   utils::geometry_works::Point(0x1420, 0x046e),
                                               }),
                                               utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                                   utils::geometry_works::Point(0x162b, 0x0eeb),
                                                   utils::geometry_works::Point(0x278f, 0x11bb),
                                                   utils::geometry_works::Point(0x278f, 0xfc16),
                                                   utils::geometry_works::Point(0x1673, 0xfbaf),
                                               })},
                                   },
                               .encounters{
                                   pe2_encounter_t{
                                       .id = 1,
                                       .mask = RANDOMIZED,
                                       .encounter_value = 450,
                                       .enemy_record_descriptor_list_addr_offset = 0x7390,
                                       .enemy_resources_desciptors_list_offset = 0x71c4,
                                   },
                                   pe2_encounter_t{
                                       .id = 2,
                                       .mask = RANDOMIZED,
                                       .encounter_value = 450,
                                       .enemy_record_descriptor_list_addr_offset = 0x7398,
                                       .enemy_resources_desciptors_list_offset = 0x71dc,
                                   },
                                   pe2_encounter_t{
                                       .id = 10,
                                       .mask = RANDOMIZED,
                                       .encounter_value = 450,
                                       .enemy_record_descriptor_list_addr_offset = 0x73d8,
                                       .enemy_resources_desciptors_list_offset = 0x720c,
                                   },
                               },
                               .allowed_additional_enemies{}},
         pe2_room_encounters_t{.id = 0x0a,
                               .name = game::ROOM_MINES_AND_SHELTER_SOUTH_MAINTENANCE_HALL,
                               .encounters_offset = 0x5e18,
                               .encounters_end = 0x5fc8,
                               .additional_file_end_pointer = 0x240,
                               .environment =
                                   pe2_room_environment_t{
                                       .spawn_zones =
                                           std::vector<utils::geometry_works::Zone>{
                                               utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                                   utils::geometry_works::Point(0xf5f6, 0xee58),
                                                   utils::geometry_works::Point(0xf5f6, 0xf1e2),
                                                   utils::geometry_works::Point(0x09f6, 0xf291),
                                                   utils::geometry_works::Point(0x09f6, 0xee86),
                                               }),
                                               utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                                   utils::geometry_works::Point(0x09f6, 0xf3f3),
                                                   utils::geometry_works::Point(0x09f6, 0x1216),
                                                   utils::geometry_works::Point(0x05b4, 0x1216),
                                                   utils::geometry_works::Point(0x05b4, 0xf3f3),
                                               })},
                                   },
                               .encounters{
                                   pe2_encounter_t{
                                       .id = 1,
                                       .mask = RANDOMIZED,
                                       .encounter_value = 450,
                                       .enemy_record_descriptor_list_addr_offset = 0x5fe0,
                                       .enemy_resources_desciptors_list_offset = 0x5d88,
                                   },
                                   pe2_encounter_t{
                                       .id = 2,
                                       .mask = RANDOMIZED,
                                       .encounter_value = 450,
                                       .enemy_record_descriptor_list_addr_offset = 0x5fe8,
                                       .enemy_resources_desciptors_list_offset = 0x5dac,
                                   },
                                   pe2_encounter_t{
                                       .id = 3,
                                       .mask = RANDOMIZED,
                                       .encounter_value = 450,
                                       .enemy_record_descriptor_list_addr_offset = 0x5ff0,
                                       .enemy_resources_desciptors_list_offset = 0x5dd0,
                                   },
                                   pe2_encounter_t{
                                       .id = 11,
                                       .mask = RANDOMIZED,
                                       .encounter_value = 450,
                                       .enemy_record_descriptor_list_addr_offset = 0x6030,
                                       .enemy_resources_desciptors_list_offset = 0x5df4,
                                   },
                               },
                               .allowed_additional_enemies{}},
         pe2_room_encounters_t{.id = 0x0b,
                               .name = game::ROOM_MINES_AND_SHELTER_STOREROOM,
                               .encounters_offset = 0x959c,
                               .encounters_end = 0x96cc,
                               .additional_file_end_pointer = 0x248,
                               .environment =
                                   pe2_room_environment_t{
                                       .spawn_zones =
                                           std::vector<utils::geometry_works::Zone>{
                                               utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                                   utils::geometry_works::Point(0x1898, 0xf6f9),
                                                   utils::geometry_works::Point(0x182d, 0x0910),
                                                   utils::geometry_works::Point(0xf150, 0x0833),
                                                   utils::geometry_works::Point(0xf30a, 0xf6f9),
                                               }),
                                           },
                                       .dead_zones =
                                           std::vector<utils::geometry_works::Zone>{
                                               utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                                   utils::geometry_works::Point(0xf900, 0xfc96),
                                                   utils::geometry_works::Point(0xf948, 0x0369),
                                                   utils::geometry_works::Point(0x1302, 0x0369),
                                                   utils::geometry_works::Point(0x12fd, 0xfc96),
                                               }),
                                           },
                                   },
                               .encounters{
                                   pe2_encounter_t{
                                       .id = 1,
                                       .mask = RANDOMIZED,
                                       .encounter_value = 450,
                                       .enemy_record_descriptor_list_addr_offset = 0x96e4,
                                       .enemy_resources_desciptors_list_offset = 0x9524,
                                   },
                                   pe2_encounter_t{
                                       .id = 2,
                                       .mask = RANDOMIZED,
                                       .encounter_value = 450,
                                       .enemy_record_descriptor_list_addr_offset = 0x96ec,
                                       .enemy_resources_desciptors_list_offset = 0x9548,
                                   },
                                   pe2_encounter_t{
                                       .id = 3,
                                       .mask = RANDOMIZED,
                                       .encounter_value = 450,
                                       .enemy_record_descriptor_list_addr_offset = 0x96f4,
                                       .enemy_resources_desciptors_list_offset = 0x9560,
                                   },
                                   pe2_encounter_t{
                                       .id = 11,
                                       .mask = RANDOMIZED,
                                       .encounter_value = 450,
                                       .enemy_record_descriptor_list_addr_offset = 0x9734,
                                       .enemy_resources_desciptors_list_offset = 0x9578,
                                   },
                               },
                               .allowed_additional_enemies{}},
         pe2_room_encounters_t{.id = 0x0c,
                               .name = game::ROOM_MINES_AND_SHELTER_NORTH_MAINTENANCE_HALL,
                               .encounters_offset = 0x82d8,
                               .encounters_end = 0x8468,
                               .additional_file_end_pointer = 0x250,
                               .environment =
                                   pe2_room_environment_t{
                                       .spawn_zones =
                                           std::vector<utils::geometry_works::Zone>{
                                               utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                                   utils::geometry_works::Point(0x09f6, 0xedea),
                                                   utils::geometry_works::Point(0x05b4, 0xedea),
                                                   utils::geometry_works::Point(0x05b4, 0x0b12),
                                                   utils::geometry_works::Point(0x09f6, 0x0b12),
                                               }),
                                               utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                                   utils::geometry_works::Point(0x09f6, 0x11a8),
                                                   utils::geometry_works::Point(0xf5f6, 0x11a8),
                                                   utils::geometry_works::Point(0xf5f6, 0x0da2),
                                                   utils::geometry_works::Point(0x09f6, 0x0da2),
                                               }),
                                           },
                                   },
                               .encounters{
                                   pe2_encounter_t{
                                       .id = 1,
                                       .mask = RANDOMIZED,
                                       .encounter_value = 450,
                                       .enemy_record_descriptor_list_addr_offset = 0x8480,
                                       .enemy_resources_desciptors_list_offset = 0x8230,
                                   },
                                   pe2_encounter_t{
                                       .id = 2,
                                       .mask = RANDOMIZED,
                                       .encounter_value = 450,
                                       .enemy_record_descriptor_list_addr_offset = 0x8488,
                                       .enemy_resources_desciptors_list_offset = 0x8254,
                                   },
                                   pe2_encounter_t{
                                       .id = 3,
                                       .mask = RANDOMIZED,
                                       .encounter_value = 450,
                                       .enemy_record_descriptor_list_addr_offset = 0x8490,
                                       .enemy_resources_desciptors_list_offset = 0x826c,
                                   },
                                   pe2_encounter_t{
                                       .id = 11,
                                       .mask = FORBID_SLOT_3_ENEMIES,
                                       .encounter_value = 450,
                                       .enemy_record_descriptor_list_addr_offset = 0x84d0,
                                       .enemy_resources_desciptors_list_offset = 0x82b4,
                                   },
                               },
                               .allowed_additional_enemies{}},
         pe2_room_encounters_t{
             .id = 0x0e,
             .name = game::ROOM_MINES_AND_SHELTER_SLEEPING_QUARTERS,
             .encounters_offset = 0x68ac,
             .encounters_end = 0x6a0c,
             .additional_file_end_pointer = 0x260,
             .environment =
                 pe2_room_environment_t{
                     .spawn_zones =
                         std::vector<utils::geometry_works::Zone>{
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0x206c, 0x15e4),
                                 utils::geometry_works::Point(0x2ee0, 0x1584),
                                 utils::geometry_works::Point(0x2ee0, 0x0ce4),
                                 utils::geometry_works::Point(0x20f5, 0x0ce4),
                             }),
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0x1d7f, 0x0875),
                                 utils::geometry_works::Point(0x305e, 0x06d6),
                                 utils::geometry_works::Point(0x307d, 0x015c),
                                 utils::geometry_works::Point(0x1d7f, 0x015c),
                             }),
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0x1aef, 0x1310),
                                 utils::geometry_works::Point(0x0442, 0x125b),
                                 utils::geometry_works::Point(0x0442, 0x099d),
                                 utils::geometry_works::Point(0x16f6, 0x0986),
                             }),
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0x225e, 0x1110),
                                 utils::geometry_works::Point(0x0442, 0x1110),
                                 utils::geometry_works::Point(0x0442, 0x0d9d),
                                 utils::geometry_works::Point(0x225e, 0x0d9d),
                             }),
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0x1566, 0x05d4),
                                 utils::geometry_works::Point(0x1566, 0x03e8),
                                 utils::geometry_works::Point(0x180a, 0x03e8),
                                 utils::geometry_works::Point(0x180a, 0x05d4),
                             }),
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0x0442, 0x099d),
                                 utils::geometry_works::Point(0x0442, 0xf542),
                                 utils::geometry_works::Point(0x093f, 0xf542),
                                 utils::geometry_works::Point(0x093f, 0x099d),
                             }),
                         },
                 },
             .encounters{
                 pe2_encounter_t{
                     .id = 1,
                     .mask = RANDOMIZED,
                     .encounter_value = 450,
                     .enemy_record_descriptor_list_addr_offset = 0x6a24,
                     .enemy_resources_desciptors_list_offset = 0x681c,
                 },
                 pe2_encounter_t{
                     .id = 2,
                     .mask = RANDOMIZED,
                     .encounter_value = 450,
                     .enemy_record_descriptor_list_addr_offset = 0x6a2c,
                     .enemy_resources_desciptors_list_offset = 0x6840,
                 },
                 pe2_encounter_t{
                     .id = 3,
                     .mask = RANDOMIZED,
                     .encounter_value = 450,
                     .enemy_record_descriptor_list_addr_offset = 0x6a34,
                     .enemy_resources_desciptors_list_offset = 0x6858,
                 },
                 pe2_encounter_t{
                     .id = 11,
                     .mask = RANDOMIZED,
                     .encounter_value = 450,
                     .enemy_record_descriptor_list_addr_offset = 0x6a74,
                     .enemy_resources_desciptors_list_offset = 0x687c,
                 },
                 pe2_encounter_t{
                     .id = 17,
                     .mask = RANDOMIZED,
                     .encounter_value = 450,
                     .enemy_record_descriptor_list_addr_offset = 0x6aa4,
                     .enemy_resources_desciptors_list_offset = 0x6894,
                 },
             },
         },
         pe2_room_encounters_t{
             .id = 0x0f,
             .name = game::ROOM_MINES_AND_SHELTER_MAIN_CORRIDOR,
             .encounters_offset = 0x8550,
             .encounters_end = 0x8660,
             .additional_file_end_pointer = 0x268,
             .environment =
                 pe2_room_environment_t{
                     .spawn_zones =
                         std::vector<utils::geometry_works::Zone>{
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0x057b, 0x00b5),
                                 utils::geometry_works::Point(0x057b, 0xc442),
                                 utils::geometry_works::Point(0xfb68, 0xc442),
                                 utils::geometry_works::Point(0xfb68, 0x00b5),
                             }),
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0xf19d, 0xfa64),
                                 utils::geometry_works::Point(0xf19d, 0xf5fa),
                                 utils::geometry_works::Point(0x0e64, 0xf5fa),
                                 utils::geometry_works::Point(0x0e64, 0xfa64),
                             }),
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0xf19d, 0xdb2f),
                                 utils::geometry_works::Point(0xf19d, 0xd708),
                                 utils::geometry_works::Point(0x0e64, 0xd708),
                                 utils::geometry_works::Point(0x0e64, 0xdb2f),
                             }),
                         },
                     .spawn_aerial_zones =
                         std::vector<utils::geometry_works::Zone>{
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0xfa28, 0xf2e1),
                                 utils::geometry_works::Point(0xfa28, 0xde5a),
                                 utils::geometry_works::Point(0xf248, 0xde5a),
                                 utils::geometry_works::Point(0xf248, 0xf2e1),
                             }),
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0xfa28, 0xd2e1),
                                 utils::geometry_works::Point(0xfa28, 0xc4c8),
                                 utils::geometry_works::Point(0xf248, 0xc4c8),
                                 utils::geometry_works::Point(0xf248, 0xd2e1),
                             }),
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0x0639, 0xf2e1),
                                 utils::geometry_works::Point(0x0639, 0xde5a),
                                 utils::geometry_works::Point(0x0d8f, 0xde5a),
                                 utils::geometry_works::Point(0x0d8f, 0xf2e1),
                             }),
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0x0639, 0xd2e1),
                                 utils::geometry_works::Point(0x0639, 0xc4c8),
                                 utils::geometry_works::Point(0x0d8f, 0xc4c8),
                                 utils::geometry_works::Point(0x0d8f, 0xd2e1),
                             }),
                         },
                 },
             .encounters{
                 pe2_encounter_t{
                     .id = 1,
                     .mask = RANDOMIZED,
                     .encounter_value = 450,
                     .enemy_record_descriptor_list_addr_offset = 0x8678,
                     .enemy_resources_desciptors_list_offset = 0x84c0,
                 },
                 pe2_encounter_t{
                     .id = 8, // 11
                     .mask = RANDOMIZED,
                     .encounter_value = 450,
                     .enemy_record_descriptor_list_addr_offset = 0x86c8,
                     .enemy_resources_desciptors_list_offset = 0x852c,
                 },
                 pe2_encounter_t{
                     .id = 2,
                     .mask = RANDOMIZED,
                     .encounter_value = 450,
                     .enemy_record_descriptor_list_addr_offset = 0x8680,
                     .enemy_resources_desciptors_list_offset = 0x84e4,
                 },
                 pe2_encounter_t{
                     .id = 3,
                     .mask = RANDOMIZED,
                     .encounter_value = 450,
                     .enemy_record_descriptor_list_addr_offset = 0x8688,
                     .enemy_resources_desciptors_list_offset = 0x84fc,
                 },
                 pe2_encounter_t{
                     .id = 7,
                     .mask = RANDOMIZED,
                     .encounter_value = 450,
                     .enemy_record_descriptor_list_addr_offset = 0x86a8,
                     .enemy_resources_desciptors_list_offset = 0x8514,
                 },
             },
             .allowed_additional_enemies{}},
         pe2_room_encounters_t{.id = 0x11,
                               .name = game::ROOM_MINES_AND_SHELTER_POD_ACCESS_TUNNEL,
                               .encounters_offset = 0x7490,
                               .encounters_end = 0x7630,
                               .additional_file_end_pointer = 0x278,
                               .environment =
                                   pe2_room_environment_t{
                                       .spawn_zones =
                                           std::vector<utils::geometry_works::Zone>{
                                               utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                                   utils::geometry_works::Point(0x041a, 0xe7c8),
                                                   utils::geometry_works::Point(0x041a, 0xfbe6),
                                                   utils::geometry_works::Point(0x0ad2, 0xfbe6),
                                                   utils::geometry_works::Point(0x0ad2, 0xe7c8),
                                               }),
                                               utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                                   utils::geometry_works::Point(0x0d7a, 0xfbe6),
                                                   utils::geometry_works::Point(0x0d7a, 0xf66e),
                                                   utils::geometry_works::Point(0x1b26, 0xf66e),
                                                   utils::geometry_works::Point(0x1b26, 0xfbe6),
                                               })},
                                   },
                               .encounters{
                                   pe2_encounter_t{
                                       .id = 1,
                                       .mask = UNRANDOMIZED,
                                       .encounter_value = 0,
                                       .enemy_record_descriptor_list_addr_offset = 0x7648,
                                       .enemy_resources_desciptors_list_offset = 0x73dc,
                                   },
                                   pe2_encounter_t{
                                       .id = 3,
                                       .mask = RANDOMIZED,
                                       .encounter_value = 450,
                                       .enemy_record_descriptor_list_addr_offset = 0x7658,
                                       .enemy_resources_desciptors_list_offset = 0x740c,
                                   },
                                   pe2_encounter_t{
                                       .id = 4,
                                       .mask = FORBID_SLOT_3_ENEMIES,
                                       .encounter_value = 450,
                                       .enemy_record_descriptor_list_addr_offset = 0x7660,
                                       .enemy_resources_desciptors_list_offset = 0x7424,
                                   },
                                   pe2_encounter_t{
                                       .id = 7,
                                       .mask = RANDOMIZED,
                                       .encounter_value = 450,
                                       .enemy_record_descriptor_list_addr_offset = 0x7678,
                                       .enemy_resources_desciptors_list_offset = 0x743c,
                                   },
                                   pe2_encounter_t{
                                       .id = 11,
                                       .mask = FORBID_SLOT_3_ENEMIES,
                                       .encounter_value = 450,
                                       .enemy_record_descriptor_list_addr_offset = 0x7698,
                                       .enemy_resources_desciptors_list_offset = 0x7454,
                                   },
                                   pe2_encounter_t{
                                       .id = 21,
                                       .mask = FORBID_SLOT_3_ENEMIES,
                                       .encounter_value = 450,
                                       .enemy_record_descriptor_list_addr_offset = 0x76e8,
                                       .enemy_resources_desciptors_list_offset = 0x7478,
                                   },
                               },
                               .allowed_additional_enemies{}},
         pe2_room_encounters_t{
             .id = 0x12,
             .name = game::ROOM_MINES_AND_SHELTER_CONTROL_ROOM,
             .encounters_offset = 0x6348,
             .encounters_end = 0x64c8,
             .additional_file_end_pointer = 0x280,
             .environment =
                 pe2_room_environment_t{
                     .spawn_zones =
                         std::vector<utils::geometry_works::Zone>{
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0xffce, 0xf3bd),
                                 utils::geometry_works::Point(0xffce, 0xea52),
                                 utils::geometry_works::Point(0x1558, 0xea52),
                                 utils::geometry_works::Point(0x1558, 0xf3bd),
                             }),
                             utils::geometry_works::Zone(
                                 utils::geometry_works::Polygon{
                                     utils::geometry_works::Point(0x1b26, 0xf4ac),
                                     utils::geometry_works::Point(0x1b26, 0xe9f2),
                                     utils::geometry_works::Point(0x2968, 0xe9f2),
                                     utils::geometry_works::Point(0x2968, 0xf4ac),
                                 },
                                 0xfe29)},
                     .dead_zones =
                         std::vector<utils::geometry_works::Zone>{
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0xffce, 0xecaa),
                                 utils::geometry_works::Point(0xffce, 0xf1be),
                                 utils::geometry_works::Point(0x050c, 0xf1be),
                                 utils::geometry_works::Point(0x050c, 0xecaa),
                             }),
                         },
                 },
             .encounters{
                 pe2_encounter_t{
                     .id = 1,
                     .mask = FORBID_MULTI_ENEMY_ENC_AND_SLOT_3,
                     .encounter_value = 450,
                     .enemy_record_descriptor_list_addr_offset = 0x64e0,
                     .enemy_resources_desciptors_list_offset = 0x62c4,
                 },
                 pe2_encounter_t{
                     .id = 11,
                     .mask = UNRANDOMIZED,
                     .encounter_value = 0,
                     .enemy_record_descriptor_list_addr_offset = 0x6530,
                     .enemy_resources_desciptors_list_offset = 0x6318,
                     .need_to_skip_encoutners = true,
                 },
                 pe2_encounter_t{
                     .id = 13,
                     .mask = FORBID_MULTI_ENEMY_ENC_AND_SLOT_3,
                     .encounter_value = 450,
                     .enemy_record_descriptor_list_addr_offset = 0x6540,
                     .enemy_resources_desciptors_list_offset = 0x6330,
                 },
             },
         },
         pe2_room_encounters_t{
             .id = 0x13,
             .name = game::ROOM_MINES_AND_SHELTER_ACCESS_TUNNEL,
             .encounters_offset = 0x27f0,
             .encounters_end = 0x2880,
             .additional_file_end_pointer = 0x288,
             .environment =
                 pe2_room_environment_t{
                     .spawn_zones =
                         std::vector<utils::geometry_works::Zone>{
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0x017f, 0x1d56),
                                 utils::geometry_works::Point(0x017f, 0x21fc),
                                 utils::geometry_works::Point(0x29cc, 0x21fc),
                                 utils::geometry_works::Point(0x29cc, 0x1d56),
                             }),
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0x251c, 0x1ac0),
                                 utils::geometry_works::Point(0x251c, 0x03ca),
                                 utils::geometry_works::Point(0x29cc, 0x03ca),
                                 utils::geometry_works::Point(0x29cc, 0x1ac0),
                             })},
                 },
             .encounters{
                 pe2_encounter_t{
                     .id = 1,
                     .mask = RANDOMIZED,
                     .encounter_value = 450,
                     .enemy_record_descriptor_list_addr_offset = 0x2898,
                     .enemy_resources_desciptors_list_offset = 0x27a8,
                 },
                 pe2_encounter_t{
                     .id = 11,
                     .mask = RANDOMIZED,
                     .encounter_value = 450,
                     .enemy_record_descriptor_list_addr_offset = 0x28e8,
                     .enemy_resources_desciptors_list_offset = 0x27c0,
                 },
                 pe2_encounter_t{
                     .id = 17,
                     .mask = RANDOMIZED,
                     .encounter_value = 450,
                     .enemy_record_descriptor_list_addr_offset = 0x2918,
                     .enemy_resources_desciptors_list_offset = 0x27d8,
                 },
             },
         },
         pe2_room_encounters_t{.id = 0x18,
                               .name = game::ROOM_MINES_AND_SHELTER_TRANSFER_TUNNEL,
                               .encounters_offset = 0x59f8,
                               .encounters_end = 0x5ae8,
                               .additional_file_end_pointer = 0x2b0,
                               .environment =
                                   pe2_room_environment_t{
                                       .spawn_zones =
                                           std::vector<utils::geometry_works::Zone>{
                                               utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                                   utils::geometry_works::Point(0x0297, 0xfe05),
                                                   utils::geometry_works::Point(0x0297, 0x02a5),
                                                   utils::geometry_works::Point(0x18f4, 0x02a5),
                                                   utils::geometry_works::Point(0x18f4, 0xfe05),
                                               }),
                                           },
                                   },
                               .encounters{
                                   pe2_encounter_t{
                                       .id = 1,
                                       .mask = RANDOMIZED,
                                       .encounter_value = 450,
                                       .enemy_record_descriptor_list_addr_offset = 0x5b00,
                                       .enemy_resources_desciptors_list_offset = 0x59b0,
                                   },
                                   pe2_encounter_t{
                                       .id = 11,
                                       .mask = RANDOMIZED,
                                       .encounter_value = 450,
                                       .enemy_record_descriptor_list_addr_offset = 0x5b50,
                                       .enemy_resources_desciptors_list_offset = 0x59d4,
                                   },
                               },
                               .allowed_additional_enemies{}},
         pe2_room_encounters_t{.id = 0x19,
                               .name = game::ROOM_MINES_AND_SHELTER_CONTROL_ROOM_ACCESS_TUNNEL,
                               .encounters_offset = 0x4eec,
                               .encounters_end = 0x4fcc,
                               .additional_file_end_pointer = 0x2b8,
                               .environment =
                                   pe2_room_environment_t{
                                       .spawn_zones =
                                           std::vector<utils::geometry_works::Zone>{
                                               utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                                   utils::geometry_works::Point(0x0295, 0x0289),
                                                   utils::geometry_works::Point(0x0295, 0xfe0f),
                                                   utils::geometry_works::Point(0x16a9, 0xfe0f),
                                                   utils::geometry_works::Point(0x16a9, 0x0289),
                                               }),
                                           },
                                   },
                               .encounters{
                                   pe2_encounter_t{
                                       .id = 1,
                                       .mask = RANDOMIZED,
                                       .encounter_value = 450,
                                       .enemy_record_descriptor_list_addr_offset = 0x4ff4,
                                       .enemy_resources_desciptors_list_offset = 0x4e5c,
                                   },
                                   pe2_encounter_t{
                                       .id = 2,
                                       .mask = RANDOMIZED,
                                       .encounter_value = 450,
                                       .enemy_record_descriptor_list_addr_offset = 0x4ffc,
                                       .enemy_resources_desciptors_list_offset = 0x4e80,
                                   },
                                   pe2_encounter_t{
                                       .id = 11,
                                       .mask = RANDOMIZED,
                                       .encounter_value = 450,
                                       .enemy_record_descriptor_list_addr_offset = 0x5044,
                                       .enemy_resources_desciptors_list_offset = 0x4ebc,
                                   },
                               },
                               .allowed_additional_enemies{}},
         pe2_room_encounters_t{.id = 0x1b,
                               .name = game::ROOM_MINES_AND_SHELTER_ELEVATOR_HALL_2,
                               .encounters_offset = 0x751c,
                               .encounters_end = 0x76ac,
                               .additional_file_end_pointer = 0x2c8,
                               .environment =
                                   pe2_room_environment_t{
                                       .spawn_zones =
                                           std::vector<utils::geometry_works::Zone>{
                                               utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                                   utils::geometry_works::Point(0x107b, 0xfbc6),
                                                   utils::geometry_works::Point(0x107b, 0x0428),
                                                   utils::geometry_works::Point(0xdd79, 0x0428),
                                                   utils::geometry_works::Point(0xdd79, 0xfbc6),
                                               }),
                                               utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                                   utils::geometry_works::Point(0x258a, 0xfbc6),
                                                   utils::geometry_works::Point(0x258a, 0x0eeb),
                                                   utils::geometry_works::Point(0x1354, 0x0eeb),
                                                   utils::geometry_works::Point(0x1354, 0xfbc6),
                                               })},
                                   },
                               .encounters{
                                   pe2_encounter_t{
                                       .id = 1,
                                       .mask = RANDOMIZED,
                                       .encounter_value = 450,
                                       .enemy_record_descriptor_list_addr_offset = 0x76c4,
                                       .enemy_resources_desciptors_list_offset = 0x748c,
                                   },
                                   pe2_encounter_t{
                                       .id = 2,
                                       .mask = RANDOMIZED,
                                       .encounter_value = 450,
                                       .enemy_record_descriptor_list_addr_offset = 0x76cc,
                                       .enemy_resources_desciptors_list_offset = 0x74a4,
                                   },
                                   pe2_encounter_t{
                                       .id = 3,
                                       .mask = RANDOMIZED,
                                       .encounter_value = 450,
                                       .enemy_record_descriptor_list_addr_offset = 0x76d4,
                                       .enemy_resources_desciptors_list_offset = 0x74bc,
                                   },
                                   pe2_encounter_t{
                                       .id = 11,
                                       .mask = RANDOMIZED,
                                       .encounter_value = 450,
                                       .enemy_record_descriptor_list_addr_offset = 0x7714,
                                       .enemy_resources_desciptors_list_offset = 0x74d4,
                                   },
                                   pe2_encounter_t{
                                       .id = 22,
                                       .mask = RANDOMIZED,
                                       .encounter_value = 450,
                                       .enemy_record_descriptor_list_addr_offset = 0x7764,
                                       .enemy_resources_desciptors_list_offset = 0x74f8,
                                   },
                               },
                               .allowed_additional_enemies{}},
         pe2_room_encounters_t{.id = 0x1c,
                               .name = game::ROOM_MINES_AND_SHELTER_SOUTH_MAINTENANCE_WALKWAY,
                               .encounters_offset = 0x600c,
                               .encounters_end = 0x61dc,
                               .additional_file_end_pointer = 0x2d0,
                               .environment =
                                   pe2_room_environment_t{
                                       .spawn_zones =
                                           std::vector<utils::geometry_works::Zone>{
                                               utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                                   utils::geometry_works::Point(0xf5f6, 0xee58),
                                                   utils::geometry_works::Point(0xf5f6, 0xf1e2),
                                                   utils::geometry_works::Point(0x09f6, 0xf291),
                                                   utils::geometry_works::Point(0x09f6, 0xee86),
                                               }),
                                               utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                                   utils::geometry_works::Point(0x09f6, 0xf3f3),
                                                   utils::geometry_works::Point(0x09f6, 0x1216),
                                                   utils::geometry_works::Point(0x05b4, 0x1216),
                                                   utils::geometry_works::Point(0x05b4, 0xf3f3),
                                               })},
                                   },
                               .encounters{
                                   pe2_encounter_t{
                                       .id = 1,
                                       .mask = RANDOMIZED,
                                       .encounter_value = 450,
                                       .enemy_record_descriptor_list_addr_offset = 0x61f4,
                                       .enemy_resources_desciptors_list_offset = 0x5f4c,
                                   },
                                   pe2_encounter_t{
                                       .id = 2,
                                       .mask = RANDOMIZED,
                                       .encounter_value = 450,
                                       .enemy_record_descriptor_list_addr_offset = 0x61fc,
                                       .enemy_resources_desciptors_list_offset = 0x5f70,
                                   },
                                   pe2_encounter_t{
                                       .id = 11,
                                       .mask = RANDOMIZED,
                                       .encounter_value = 450,
                                       .enemy_record_descriptor_list_addr_offset = 0x6244,
                                       .enemy_resources_desciptors_list_offset = 0x5fc4,
                                   },
                                   pe2_encounter_t{
                                       .id = 21,
                                       .mask = RANDOMIZED,
                                       .encounter_value = 450,
                                       .enemy_record_descriptor_list_addr_offset = 0x6294,
                                       .enemy_resources_desciptors_list_offset = 0x5fe8,
                                   },
                               },
                               .allowed_additional_enemies{}},
         pe2_room_encounters_t{
             .id = 0x1d,
             .name = game::ROOM_MINES_AND_SHELTER_OPERATING_ROOM,
             .encounters_offset = 0x6994,
             .encounters_end = 0x6b54,
             .additional_file_end_pointer = 0x2d8,
             .environment =
                 pe2_room_environment_t{
                     .spawn_zones =
                         std::vector<utils::geometry_works::Zone>{
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0x2968, 0x012c),
                                 utils::geometry_works::Point(0x2968, 0x1644),
                                 utils::geometry_works::Point(0x2471, 0x1644),
                                 utils::geometry_works::Point(0x2471, 0x012c),
                             }),
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0x1e14, 0x125c),
                                 utils::geometry_works::Point(0x1e14, 0x0572),
                                 utils::geometry_works::Point(0x0c3c, 0x0572),
                                 utils::geometry_works::Point(0x0c3c, 0x125c),
                             }),
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0xffce, 0x0320),
                                 utils::geometry_works::Point(0xffce, 0x1255),
                                 utils::geometry_works::Point(0x0337, 0x1255),
                                 utils::geometry_works::Point(0x0337, 0x0320),
                             })},
                     .dead_zones =
                         std::vector<utils::geometry_works::Zone>{
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0x0e74, 0x076c),
                                 utils::geometry_works::Point(0x0e74, 0x0f60),
                                 utils::geometry_works::Point(0x19c8, 0x0f60),
                                 utils::geometry_works::Point(0x19c8, 0x076c),
                             })},
                 },
             .encounters{
                 pe2_encounter_t{
                     .id = 1,
                     .mask = RANDOMIZED,
                     .encounter_value = 450,
                     .enemy_record_descriptor_list_addr_offset = 0x6b6c,
                     .enemy_resources_desciptors_list_offset = 0x68f8,
                 },
                 pe2_encounter_t{
                     .id = 3,
                     .mask = RANDOMIZED,
                     .encounter_value = 450,
                     .enemy_record_descriptor_list_addr_offset = 0x6b7c,
                     .enemy_resources_desciptors_list_offset = 0x694c,
                 },
                 pe2_encounter_t{
                     .id = 11,
                     .mask = RANDOMIZED,
                     .encounter_value = 450,
                     .enemy_record_descriptor_list_addr_offset = 0x6bbc,
                     .enemy_resources_desciptors_list_offset = 0x6964,
                 },
             },
         },
         pe2_room_encounters_t{.id = 0x1e,
                               .name = game::ROOM_MINES_AND_SHELTER_NORTH_MAINTENANCE_WALKWAY,
                               .encounters_offset = 0x8b08,
                               .encounters_end = 0x8c88,
                               .additional_file_end_pointer = 0x2e0,
                               .environment =
                                   pe2_room_environment_t{
                                       .spawn_zones =
                                           std::vector<utils::geometry_works::Zone>{
                                               utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                                   utils::geometry_works::Point(0x09f6, 0xedea),
                                                   utils::geometry_works::Point(0x05b4, 0xedea),
                                                   utils::geometry_works::Point(0x05b4, 0x0b12),
                                                   utils::geometry_works::Point(0x09f6, 0x0b12),
                                               }),
                                               utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                                   utils::geometry_works::Point(0x09f6, 0x11a8),
                                                   utils::geometry_works::Point(0xf5f6, 0x11a8),
                                                   utils::geometry_works::Point(0xf5f6, 0x0da2),
                                                   utils::geometry_works::Point(0x09f6, 0x0da2),
                                               }),
                                           },
                                   },
                               .encounters{
                                   pe2_encounter_t{
                                       .id = 1,
                                       .mask = UNRANDOMIZED,
                                       .encounter_value = 0,
                                       .enemy_record_descriptor_list_addr_offset = 0x8ca0,
                                       .enemy_resources_desciptors_list_offset = 0x8a48,
                                   },
                                   pe2_encounter_t{
                                       .id = 3,
                                       .mask = RANDOMIZED,
                                       .encounter_value = 450,
                                       .enemy_record_descriptor_list_addr_offset = 0x8cb0,
                                       .enemy_resources_desciptors_list_offset = 0x8a6c,
                                   },
                                   pe2_encounter_t{
                                       .id = 4,
                                       .mask = RANDOMIZED,
                                       .encounter_value = 450,
                                       .enemy_record_descriptor_list_addr_offset = 0x8cb8,
                                       .enemy_resources_desciptors_list_offset = 0x8a90,
                                   },
                                   pe2_encounter_t{
                                       .id = 11,
                                       .mask = RANDOMIZED,
                                       .encounter_value = 450,
                                       .enemy_record_descriptor_list_addr_offset = 0x8cf0,
                                       .enemy_resources_desciptors_list_offset = 0x8ab4,
                                   },
                                   pe2_encounter_t{
                                       .id = 21,
                                       .mask = RANDOMIZED,
                                       .encounter_value = 450,
                                       .enemy_record_descriptor_list_addr_offset = 0x8d40,
                                       .enemy_resources_desciptors_list_offset = 0x8ae4,
                                   },
                               },
                               .allowed_additional_enemies{},
                               .forbidden_enemies{0x03}},
         pe2_room_encounters_t{
             .id = 0x20,
             .name = game::ROOM_MINES_AND_SHELTER_BREEDING_ROOM,
             .encounters_offset = 0x671c,
             .encounters_end = 0x691c,
             .additional_file_end_pointer = 0x2f0,
             .environment =
                 pe2_room_environment_t{
                     .spawn_zones =
                         std::vector<utils::geometry_works::Zone>{
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0x0532, 0x1360),
                                 utils::geometry_works::Point(0x0532, 0x0dd6),
                                 utils::geometry_works::Point(0x3150, 0x0dd6),
                                 utils::geometry_works::Point(0x3150, 0x1360),
                             }),
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0x2f62, 0x092e),
                                 utils::geometry_works::Point(0x2f62, 0x003c),
                                 utils::geometry_works::Point(0x2742, 0x003c),
                                 utils::geometry_works::Point(0x2742, 0x092e),
                             }),
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0x1798, 0x0032),
                                 utils::geometry_works::Point(0x1798, 0x07bc),
                                 utils::geometry_works::Point(0x1f56, 0x07bc),
                                 utils::geometry_works::Point(0x1f56, 0x0032),
                             }),
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0x0615, 0x0981),
                                 utils::geometry_works::Point(0x0615, 0xfd7e),
                                 utils::geometry_works::Point(0x09a8, 0xfd7e),
                                 utils::geometry_works::Point(0x09a8, 0x0981),
                             }),
                         },
                 },
             .encounters{
                 pe2_encounter_t{
                     .id = 1,
                     .mask = RANDOMIZED,
                     .encounter_value = 450,
                     .enemy_record_descriptor_list_addr_offset = 0x6934,
                     .enemy_resources_desciptors_list_offset = 0x662c,
                 },
                 pe2_encounter_t{
                     .id = 2,
                     .mask = RANDOMIZED,
                     .encounter_value = 450,
                     .enemy_record_descriptor_list_addr_offset = 0x693c,
                     .enemy_resources_desciptors_list_offset = 0x6644,
                 },
                 pe2_encounter_t{
                     .id = 7,
                     .mask = RANDOMIZED,
                     .encounter_value = 450,
                     .enemy_record_descriptor_list_addr_offset = 0x6964,
                     .enemy_resources_desciptors_list_offset = 0x66c8,
                 },
                 pe2_encounter_t{
                     .id = 3,
                     .mask = RANDOMIZED,
                     .encounter_value = 450,
                     .enemy_record_descriptor_list_addr_offset = 0x6944,
                     .enemy_resources_desciptors_list_offset = 0x6668,
                 },
                 pe2_encounter_t{
                     .id = 4,
                     .mask = RANDOMIZED,
                     .encounter_value = 450,
                     .enemy_record_descriptor_list_addr_offset = 0x694c,
                     .enemy_resources_desciptors_list_offset = 0x6698,
                 },
                 pe2_encounter_t{
                     .id = 11,
                     .mask = RANDOMIZED,
                     .encounter_value = 450,
                     .enemy_record_descriptor_list_addr_offset = 0x6984,
                     .enemy_resources_desciptors_list_offset = 0x66e0,
                 },
                 pe2_encounter_t{
                     .id = 21,
                     .mask = RANDOMIZED,
                     .encounter_value = 450,
                     .enemy_record_descriptor_list_addr_offset = 0x69d4,
                     .enemy_resources_desciptors_list_offset = 0x6704,
                 },
             },
         },
         pe2_room_encounters_t{.id = 0x21,
                               .name = game::ROOM_MINES_AND_SHELTER_MAIN_CORRIDOR_2,
                               .encounters_offset = 0xbc3c,
                               .encounters_end = 0xbd6c,
                               .additional_file_end_pointer = 0x2f8,
                               .environment =
                                   pe2_room_environment_t{
                                       .spawn_zones =
                                           std::vector<utils::geometry_works::Zone>{
                                               utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                                   utils::geometry_works::Point(0x057b, 0x00b5),
                                                   utils::geometry_works::Point(0x057b, 0xc442),
                                                   utils::geometry_works::Point(0xfb68, 0xc442),
                                                   utils::geometry_works::Point(0xfb68, 0x00b5),
                                               }),
                                               utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                                   utils::geometry_works::Point(0xf19d, 0xfa64),
                                                   utils::geometry_works::Point(0xf19d, 0xf5fa),
                                                   utils::geometry_works::Point(0x0e64, 0xf5fa),
                                                   utils::geometry_works::Point(0x0e64, 0xfa64),
                                               }),
                                               utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                                   utils::geometry_works::Point(0xf19d, 0xdb2f),
                                                   utils::geometry_works::Point(0xf19d, 0xd708),
                                                   utils::geometry_works::Point(0x0e64, 0xd708),
                                                   utils::geometry_works::Point(0x0e64, 0xdb2f),
                                               }),
                                           },
                                       .spawn_water_zones =
                                           std::vector<utils::geometry_works::Zone>{
                                               utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                                   utils::geometry_works::Point(0xfa28, 0xf2e1),
                                                   utils::geometry_works::Point(0xfa28, 0xde5a),
                                                   utils::geometry_works::Point(0xf248, 0xde5a),
                                                   utils::geometry_works::Point(0xf248, 0xf2e1),
                                               }),
                                               utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                                   utils::geometry_works::Point(0xfa28, 0xd2e1),
                                                   utils::geometry_works::Point(0xfa28, 0xc4c8),
                                                   utils::geometry_works::Point(0xf248, 0xc4c8),
                                                   utils::geometry_works::Point(0xf248, 0xd2e1),
                                               }),
                                               utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                                   utils::geometry_works::Point(0x0639, 0xf2e1),
                                                   utils::geometry_works::Point(0x0639, 0xde5a),
                                                   utils::geometry_works::Point(0x0d8f, 0xde5a),
                                                   utils::geometry_works::Point(0x0d8f, 0xf2e1),
                                               }),
                                               utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                                   utils::geometry_works::Point(0x0639, 0xd2e1),
                                                   utils::geometry_works::Point(0x0639, 0xc4c8),
                                                   utils::geometry_works::Point(0x0d8f, 0xc4c8),
                                                   utils::geometry_works::Point(0x0d8f, 0xd2e1),
                                               }),
                                           },
                                   },
                               .encounters{
                                   pe2_encounter_t{
                                       .id = 1,
                                       .mask = FORBID_MULTI_ENEMY_ENC_AND_SLOT_3,
                                       .encounter_value = 450,
                                       .enemy_record_descriptor_list_addr_offset = 0xbd84,
                                       .enemy_resources_desciptors_list_offset = 0xbb94,
                                   },
                                   pe2_encounter_t{
                                       .id = 2,
                                       .mask = FORBID_MULTI_ENEMY_ENC_AND_SLOT_3,
                                       .encounter_value = 450,
                                       .enemy_record_descriptor_list_addr_offset = 0xbd8c,
                                       .enemy_resources_desciptors_list_offset = 0xbbac,
                                   },
                                   pe2_encounter_t{
                                       .id = 3,
                                       .mask = FORBID_MULTI_ENEMY_ENC_AND_SLOT_3,
                                       .encounter_value = 450,
                                       .enemy_record_descriptor_list_addr_offset = 0xbd94,
                                       .enemy_resources_desciptors_list_offset = 0xbbc4,
                                   },
                                   pe2_encounter_t{
                                       .id = 4,
                                       .mask = UNRANDOMIZED,
                                       .encounter_value = 450,
                                       .enemy_record_descriptor_list_addr_offset = 0xbd9c,
                                       .enemy_resources_desciptors_list_offset = 0xbbe8,
                                   },
                                   pe2_encounter_t{
                                       .id = 11,
                                       .mask = FORBID_MULTI_ENEMY_ENC_AND_SLOT_3,
                                       .encounter_value = 450,
                                       .enemy_record_descriptor_list_addr_offset = 0xbdd4,
                                       .enemy_resources_desciptors_list_offset = 0xbbf4,
                                   },
                                   pe2_encounter_t{
                                       .id = 21,
                                       .mask = FORBID_MULTI_ENEMY_ENC_AND_SLOT_3,
                                       .encounter_value = 450,
                                       .enemy_record_descriptor_list_addr_offset = 0xbe24,
                                       .enemy_resources_desciptors_list_offset = 0xbc18,
                                   },
                               },
                               .allowed_additional_enemies{0x04}},
         pe2_room_encounters_t{.id = 0x22,
                               .name = game::ROOM_MINES_AND_SHELTER_SEPTIC_TANK,
                               .encounters_offset = 0x9840,
                               .encounters_end = 0x9970,
                               .additional_file_end_pointer = 0x300,
                               .environment =
                                   pe2_room_environment_t{
                                       .spawn_zones =
                                           std::vector<utils::geometry_works::Zone>{
                                               utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                                   utils::geometry_works::Point(0xfc8f, 0xf998),
                                                   utils::geometry_works::Point(0xfc8f, 0xcc61),
                                                   utils::geometry_works::Point(0x03ad, 0xcc61),
                                                   utils::geometry_works::Point(0x03ad, 0xf998),
                                               }),
                                           },
                                       .spawn_water_zones =
                                           std::vector<utils::geometry_works::Zone>{
                                               utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                                   utils::geometry_works::Point(0xfc05, 0xf998),
                                                   utils::geometry_works::Point(0xfc05, 0xcc61),
                                                   utils::geometry_works::Point(0xf048, 0xcc61),
                                                   utils::geometry_works::Point(0xf048, 0xf998),
                                               }),
                                               utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                                   utils::geometry_works::Point(0x04bf, 0xf998),
                                                   utils::geometry_works::Point(0x04bf, 0xcc61),
                                                   utils::geometry_works::Point(0x1048, 0xcc61),
                                                   utils::geometry_works::Point(0x1048, 0xf998),
                                               }),
                                           },
                                   },
                               .encounters{
                                   pe2_encounter_t{
                                       .id = 1,
                                       .mask = FORBID_MULTI_ENEMY_ENC_AND_SLOT_3,
                                       .encounter_value = 450,
                                       .enemy_record_descriptor_list_addr_offset = 0x9988,
                                       .enemy_resources_desciptors_list_offset = 0x978c,
                                   },
                                   pe2_encounter_t{
                                       .id = 2,
                                       .mask = FORBID_MULTI_ENEMY_ENC_AND_SLOT_3,
                                       .encounter_value = 450,
                                       .enemy_record_descriptor_list_addr_offset = 0x9990,
                                       .enemy_resources_desciptors_list_offset = 0x97a4,
                                   },
                                   pe2_encounter_t{
                                       .id = 3,
                                       .mask = FORBID_MULTI_ENEMY_ENC_AND_SLOT_3,
                                       .encounter_value = 450,
                                       .enemy_record_descriptor_list_addr_offset = 0x9998,
                                       .enemy_resources_desciptors_list_offset = 0x97bc,
                                   },
                                   pe2_encounter_t{
                                       .id = 4,
                                       .mask = FORBID_MULTI_ENEMY_ENC_AND_SLOT_3,
                                       .encounter_value = 450,
                                       .enemy_record_descriptor_list_addr_offset = 0x99a0,
                                       .enemy_resources_desciptors_list_offset = 0x97e0,
                                   },
                                   pe2_encounter_t{
                                       .id = 11,
                                       .mask = FORBID_MULTI_ENEMY_ENC_AND_SLOT_3,
                                       .encounter_value = 450,
                                       .enemy_record_descriptor_list_addr_offset = 0x99d8,
                                       .enemy_resources_desciptors_list_offset = 0x97f8,
                                   },
                                   pe2_encounter_t{
                                       .id = 21,
                                       .mask = FORBID_MULTI_ENEMY_ENC_AND_SLOT_3,
                                       .encounter_value = 450,
                                       .enemy_record_descriptor_list_addr_offset = 0x9a28,
                                       .enemy_resources_desciptors_list_offset = 0x981c,
                                   },
                               },
                               .allowed_additional_enemies{0x04},
                               .file_patch_map{// Diver cutscene
                                               {0x506, 0x45}}},
         pe2_room_encounters_t{
             .id = 0x23,
             .name = game::ROOM_MINES_AND_SHELTER_POD_ACCESS_TUNNEL_2,
             .encounters_offset = 0x7e0c,
             .encounters_end = 0x7fdc,
             .additional_file_end_pointer = 0x308,
             .environment =
                 pe2_room_environment_t{.spawn_zones =
                                            std::vector<utils::geometry_works::Zone>{
                                                utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                                    utils::geometry_works::Point(0x041a, 0xd634),
                                                    utils::geometry_works::Point(0x041a, 0xf40d),
                                                    utils::geometry_works::Point(0x0992, 0xf40d),
                                                    utils::geometry_works::Point(0x0992, 0xd634),
                                                }),
                                                utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                                    utils::geometry_works::Point(0x041a, 0xfbe6),
                                                    utils::geometry_works::Point(0x041a, 0xf6dd),
                                                    utils::geometry_works::Point(0x173e, 0xf6dd),
                                                    utils::geometry_works::Point(0x173e, 0xfbe6),
                                                })}},
             .encounters{
                 pe2_encounter_t{
                     .id = 1,
                     .mask = RANDOMIZED,
                     .encounter_value = 450,
                     .enemy_record_descriptor_list_addr_offset = 0x7ff4,
                     .enemy_resources_desciptors_list_offset = 0x7d10,
                 },
                 pe2_encounter_t{
                     .id = 3,
                     .mask = RANDOMIZED,
                     .encounter_value = 450,
                     .enemy_record_descriptor_list_addr_offset = 0x8004,
                     .enemy_resources_desciptors_list_offset = 0x7d58,
                 },
                 pe2_encounter_t{
                     .id = 4,
                     .mask = FORBID_SLOT_3_ENEMIES,
                     .encounter_value = 450,
                     .enemy_record_descriptor_list_addr_offset = 0x800c,
                     .enemy_resources_desciptors_list_offset = 0x7d7c,
                 },
                 pe2_encounter_t{
                     .id = 7,
                     .mask = RANDOMIZED,
                     .encounter_value = 450,
                     .enemy_record_descriptor_list_addr_offset = 0x8024,
                     .enemy_resources_desciptors_list_offset = 0x7d94,
                 },
                 pe2_encounter_t{
                     .id = 11,
                     .mask = RANDOMIZED,
                     .encounter_value = 450,
                     .enemy_record_descriptor_list_addr_offset = 0x8044,
                     .enemy_resources_desciptors_list_offset = 0x7dac,
                 },
                 pe2_encounter_t{
                     .id = 21,
                     .mask = FORBID_SLOT_3_ENEMIES,
                     .encounter_value = 450,
                     .enemy_record_descriptor_list_addr_offset = 0x8094,
                     .enemy_resources_desciptors_list_offset = 0x7dd0,
                 },
                 pe2_encounter_t{
                     .id = 22,
                     .mask = FORBID_SLOT_3_ENEMIES,
                     .encounter_value = 450,
                     .enemy_record_descriptor_list_addr_offset = 0x809c,
                     .enemy_resources_desciptors_list_offset = 0x7df4,
                 },
             },
             .allowed_additional_enemies{},
         },
         pe2_room_encounters_t{
             .id = 0x2a,
             .name = game::ROOM_MINES_AND_SHELTER_ELEVATOR_HALL_3,
             .encounters_offset = 0x711c,
             .encounters_end = 0x71ac,
             .additional_file_end_pointer = 0x340,
             .environment =
                 pe2_room_environment_t{.spawn_zones =
                                            std::vector<utils::geometry_works::Zone>{
                                                utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                                    utils::geometry_works::Point(0xe670, 0xf5b2),
                                                    utils::geometry_works::Point(0xe670, 0x06a4),
                                                    utils::geometry_works::Point(0xeccc, 0x06a4),
                                                    utils::geometry_works::Point(0xeccc, 0xf5b2),
                                                }),
                                                utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                                    utils::geometry_works::Point(0xef89, 0xf5b2),
                                                    utils::geometry_works::Point(0xef89, 0xfac2),
                                                    utils::geometry_works::Point(0x1356, 0xfac2),
                                                    utils::geometry_works::Point(0x1356, 0xf5b2),
                                                })}},
             .encounters{pe2_encounter_t{
                             .id = 1,
                             .mask = FORBID_MULTI_ENEMY_ENC_AND_SLOT_3,
                             .encounter_value = 450,
                             .enemy_record_descriptor_list_addr_offset = 0x71c4,
                             .enemy_resources_desciptors_list_offset = 0x70c8,
                         },
                         pe2_encounter_t{
                             .id = 3,
                             .mask = FORBID_MULTI_ENEMY_ENC_AND_SLOT_3,
                             .encounter_value = 450,
                             .enemy_record_descriptor_list_addr_offset = 0x71cc,
                             .enemy_resources_desciptors_list_offset = 0x70e0,
                         },
                         pe2_encounter_t{
                             .id = 4,
                             .mask = FORBID_MULTI_ENEMY_ENC_AND_SLOT_3,
                             .encounter_value = 450,
                             .enemy_record_descriptor_list_addr_offset = 0x7214,
                             .enemy_resources_desciptors_list_offset = 0x7104,
                             .need_to_skip_encoutners = true,
                         }},
             .forbidden_enemies{0x03, 0x14, 0x17, 0x16, 0x27, 0x38, 0x39},
         },
         pe2_room_encounters_t{.id = 0x2c,
                               .name = game::ROOM_MINES_AND_SHELTER_UPPER_SEWER,
                               .encounters_offset = 0x0,
                               .encounters_end = 0x0,
                               .additional_file_end_pointer = 0x350,
                               .environment = pe2_room_environment_t{},
                               .encounters{},
                               .file_patch_map{
                                   {0xb448, 0x04}, {0xb449, 0x00}, {0xb44a, 0x04}, {0xb44b, 0x00},
                                   {0xb44c, 0x01}, {0xb44d, 0x00}, {0xb44e, 0x00}, {0xb44f, 0x00},
                                   {0xb450, 0x48}, {0xb451, 0xfe}, {0xb452, 0x15}, {0xb453, 0x80},
                                   {0xb454, 0xff}, {0xb455, 0x00}, {0xb456, 0x00}, {0xb457, 0x00},
                                   {0xb458, 0x00}, {0xb459, 0x00}, {0xb45a, 0x00}, {0xb45b, 0x00},
                                   {0xb45c, 0x00}, {0xb45d, 0x00}, {0xb45e, 0x00}, {0xb45f, 0x00},
                                   {0xb5ac, 0x04}, {0xb5c9, 0x00}, {0xb5ca, 0x02}, {0xb5cb, 0x00},
                               }},
     }},
    {game::NEOARK_AND_SHELTER_LEVEL,
     {
         pe2_room_encounters_t{
             .id = 0x02,
             .name = game::ROOM_NEO_ARK_AND_SHELTER_VEHICULAR_AIRLOCK,
             .encounters_offset = 0x1580,
             .additional_file_end_pointer = 0x478,
             .environment =
                 pe2_room_environment_t{
                     .spawn_zones =
                         std::vector<utils::geometry_works::Zone>{
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0xf8f8, 0xf956),
                                 utils::geometry_works::Point(0xf8f8, 0x06c2),
                                 utils::geometry_works::Point(0xe3e0, 0x06c2),
                                 utils::geometry_works::Point(0xe3e0, 0xf956),
                             }),
                         },
                 },
             .encounters{pe2_encounter_t{
                 .id = 1,
                 .mask = RANDOMIZED,
                 .encounter_value = 500,
                 .enemy_record_descriptor_list_addr_offset = 0x544c,
                 .enemy_resources_desciptors_list_offset = 0x5420,
             }},
         },
         pe2_room_encounters_t{
             .id = 0x03,
             .name = game::ROOM_NEO_ARK_AND_SHELTER_BULWARK,
             .encounters_offset = 0x15b0,
             .additional_file_end_pointer = 0x480,
             .environment =
                 pe2_room_environment_t{
                     .spawn_zones =
                         std::vector<utils::geometry_works::Zone>{
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0xf7a4, 0xf436),
                                 utils::geometry_works::Point(0xf7a4, 0x0c80),
                                 utils::geometry_works::Point(0xf092, 0x0c80),
                                 utils::geometry_works::Point(0xf092, 0xf436),
                             }),
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0xfc59, 0x042e),
                                 utils::geometry_works::Point(0xfc59, 0xfb00),
                                 utils::geometry_works::Point(0x0f6e, 0xfb00),
                                 utils::geometry_works::Point(0x0f6e, 0x042e),
                             }),
                         },
                 },
             .encounters{pe2_encounter_t{
                 .id = 1,
                 .mask = RANDOMIZED,
                 .encounter_value = 500,
                 .enemy_record_descriptor_list_addr_offset = 0x37f0,
                 .enemy_resources_desciptors_list_offset = 0x37c4,
             }},
             .allowed_additional_enemies{},
         },
         pe2_room_encounters_t{
             .id = 0x04,
             .name = game::ROOM_NEO_ARK_AND_SHELTER_HELIPORT,
             .encounters_offset = 0x15e0,
             .additional_file_end_pointer = 0x488,
             .encounters{pe2_encounter_t{
                             .id = 1,
                             .mask = UNRANDOMIZED,
                             .encounter_value = 0,
                             .enemy_record_descriptor_list_addr_offset = 0x563c,
                             .enemy_resources_desciptors_list_offset = 0x55ec,
                         },
                         pe2_encounter_t{
                             .id = 2,
                             .mask = UNRANDOMIZED,
                             .encounter_value = 0,
                             .enemy_record_descriptor_list_addr_offset = 0x5644,
                             .enemy_resources_desciptors_list_offset = 0x5610,
                         }},
         },
         pe2_room_encounters_t{
             .id = 0x05,
             .name = game::ROOM_NEO_ARK_AND_SHELTER_AIRLOCK,
             .encounters_offset = 0x1680,
             .additional_file_end_pointer = 0x490,
             .environment =
                 pe2_room_environment_t{
                     .spawn_zones =
                         std::vector<utils::geometry_works::Zone>{
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0xeab6, 0x1450),
                                 utils::geometry_works::Point(0xeab6, 0x0df3),
                                 utils::geometry_works::Point(0x0993, 0x0df3),
                                 utils::geometry_works::Point(0x0993, 0x1450),
                             }),
                         },
                 },
             .encounters{pe2_encounter_t{
                             .id = 1,
                             .mask = RANDOMIZED,
                             .encounter_value = 500,
                             .enemy_record_descriptor_list_addr_offset = 0x21a0,
                             .enemy_resources_desciptors_list_offset = 0x2168,
                         },
                         pe2_encounter_t{
                             .id = 7,
                             .mask = RANDOMIZED,
                             .encounter_value = 500,
                             .enemy_record_descriptor_list_addr_offset = 0x21d0,
                             .enemy_resources_desciptors_list_offset = 0x2180,
                         }},
             .allowed_additional_enemies{},
         },
         pe2_room_encounters_t{
             .id = 0x07,
             .name = game::ROOM_NEO_ARK_AND_SHELTER_OBSERVATORY,
             .encounters_offset = 0x16c0,
             .additional_file_end_pointer = 0x4a0,
             .encounters{
                 pe2_encounter_t{
                     .id = 1,
                     .mask = UNRANDOMIZED,
                     .encounter_value = 0,
                     .enemy_record_descriptor_list_addr_offset = 0xa2b4,
                     .enemy_resources_desciptors_list_offset = 0xa294,
                 },
             },
         },
         pe2_room_encounters_t{
             .id = 0x08,
             .name = game::ROOM_NEO_ARK_AND_SHELTER_EVE_ACCESS_TUNNEL,
             .encounters_offset = 0x16e0,
             .additional_file_end_pointer = 0x4a8,
             .environment =
                 pe2_room_environment_t{
                     .spawn_zones =
                         std::vector<utils::geometry_works::Zone>{
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0xf57a, 0x05b7),
                                 utils::geometry_works::Point(0xf03f, 0x065e),
                                 utils::geometry_works::Point(0xf03e, 0x018b),
                                 utils::geometry_works::Point(0xf5fc, 0x018b),
                             }),
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0xfd49, 0x1b02),
                                 utils::geometry_works::Point(0xf93f, 0x1b02),
                                 utils::geometry_works::Point(0xf986, 0x0acf),
                                 utils::geometry_works::Point(0xfd4a, 0x0a6f),
                             }),
                         },
                     .dead_zones =
                         std::vector<utils::geometry_works::Zone>{
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0xfb0c, 0x179d),
                                 utils::geometry_works::Point(0xf59d, 0x179d),
                                 utils::geometry_works::Point(0xfb0c, 0x0e37),
                                 utils::geometry_works::Point(0xf59d, 0x0e37),
                             }),
                         },
                 },
             .encounters{
                 pe2_encounter_t{
                     .id = 1,
                     .mask = FORBID_MULTI_ENEMY_ENC_AND_SLOT_3,
                     .encounter_value = 500,
                     .enemy_record_descriptor_list_addr_offset = 0x3100,
                     .enemy_resources_desciptors_list_offset = 0x30bc,
                 },
                 pe2_encounter_t{
                     .id = 11,
                     .mask = FORBID_MULTI_ENEMY_ENC_AND_SLOT_3,
                     .encounter_value = 500,
                     .enemy_record_descriptor_list_addr_offset = 0x3150,
                     .enemy_resources_desciptors_list_offset = 0x30d4,
                 },
             },
         },
         pe2_room_encounters_t{
             .id = 0x0a,
             .name = game::ROOM_NEO_ARK_AND_SHELTER_NORTH_PROMENADE,
             .encounters_offset = 0x17d0,
             .additional_file_end_pointer = 0x4b8,
             .environment =
                 pe2_room_environment_t{
                     .spawn_zones =
                         std::vector<utils::geometry_works::Zone>{
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0x32c8, 0x0c03),
                                 utils::geometry_works::Point(0x2134, 0x0c03),
                                 utils::geometry_works::Point(0x2134, 0x1474),
                                 utils::geometry_works::Point(0x32c8, 0x1474),
                             }),
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0x26ac, 0x1a2f),
                                 utils::geometry_works::Point(0x32c8, 0x1a2f),
                                 utils::geometry_works::Point(0x32c8, 0x242d),
                                 utils::geometry_works::Point(0x266c, 0x242d),
                             }),
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0x323a, 0x26de),
                                 utils::geometry_works::Point(0x2487, 0x32c8),
                                 utils::geometry_works::Point(0x0b4c, 0x32c8),
                                 utils::geometry_works::Point(0x0b4c, 0x26de),
                             }),
                         },
                 },
             .encounters{
                 pe2_encounter_t{
                     .id = 1,
                     .mask = FORBID_SLOT_3_ENEMIES,
                     .encounter_value = 500,
                     .enemy_record_descriptor_list_addr_offset = 0x5ab4,
                     .enemy_resources_desciptors_list_offset = 0x5a6c,
                 },
                 pe2_encounter_t{
                     .id = 11,
                     .mask = FORBID_SLOT_3_ENEMIES,
                     .encounter_value = 500,
                     .enemy_record_descriptor_list_addr_offset = 0x5af4,
                     .enemy_resources_desciptors_list_offset = 0x5a84,
                 },
             },
             .allowed_additional_enemies{},
         },
         pe2_room_encounters_t{.id = 0x0b,
                               .name = game::ROOM_NEO_ARK_AND_SHELTER_FOREST_ZONE,
                               .encounters_offset = 0x1840,
                               .additional_file_end_pointer = 0x4c0,
                               .environment =
                                   pe2_room_environment_t{
                                       .spawn_zones =
                                           std::vector<utils::geometry_works::Zone>{
                                               utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                                   utils::geometry_works::Point(0xe1ec, 0xfc19),
                                                   utils::geometry_works::Point(0xe1ec, 0x03e7),
                                                   utils::geometry_works::Point(0x25e4, 0x03e7),
                                                   utils::geometry_works::Point(0x25e4, 0xfc19),
                                               }),
                                           },
                                   },
                               .encounters{
                                   pe2_encounter_t{
                                       .id = 1,
                                       .mask = RANDOMIZED,
                                       .encounter_value = 500,
                                       .enemy_record_descriptor_list_addr_offset = 0x53b0,
                                       .enemy_resources_desciptors_list_offset = 0x52bc,
                                   },
                                   pe2_encounter_t{
                                       .id = 2,
                                       .mask = RANDOMIZED,
                                       .encounter_value = 500,
                                       .enemy_record_descriptor_list_addr_offset = 0x53b8,
                                       .enemy_resources_desciptors_list_offset = 0x52e0,
                                   },
                                   pe2_encounter_t{
                                       .id = 3,
                                       .mask = RANDOMIZED,
                                       .encounter_value = 500,
                                       .enemy_record_descriptor_list_addr_offset = 0x53c0,
                                       .enemy_resources_desciptors_list_offset = 0x52f8,
                                   },
                                   pe2_encounter_t{
                                       .id = 4,
                                       .mask = RANDOMIZED,
                                       .encounter_value = 500,
                                       .enemy_record_descriptor_list_addr_offset = 0x53c8,
                                       .enemy_resources_desciptors_list_offset = 0x5310,
                                   },
                                   pe2_encounter_t{
                                       .id = 5,
                                       .mask = RANDOMIZED,
                                       .encounter_value = 500,
                                       .enemy_record_descriptor_list_addr_offset = 0x53d0,
                                       .enemy_resources_desciptors_list_offset = 0x5328,
                                   },
                                   pe2_encounter_t{
                                       .id = 11,
                                       .mask = RANDOMIZED,
                                       .encounter_value = 500,
                                       .enemy_record_descriptor_list_addr_offset = 0x5400,
                                       .enemy_resources_desciptors_list_offset = 0x534c,
                                   },
                               },
                               .allowed_additional_enemies{},
                               .file_patch_map{
                                   // Disable jumping raptor stranger cutscene
                                   {0x3d2, 0x45},
                               }},
         pe2_room_encounters_t{
             .id = 0x0c,
             .name = game::ROOM_NEO_ARK_AND_SHELTER_SUBMARINE_TUNNEL,
             .encounters_offset = 0x19e0,
             .additional_file_end_pointer = 0x4c8,
             .environment =
                 pe2_room_environment_t{
                     .spawn_zones =
                         std::vector<utils::geometry_works::Zone>{
                             utils::geometry_works::Zone(
                                 utils::geometry_works::Polygon{
                                     utils::geometry_works::Point(0x1500, 0x0380),
                                     utils::geometry_works::Point(0x1500, 0xfd80),
                                     utils::geometry_works::Point(0xeb00, 0xfd80),
                                     utils::geometry_works::Point(0xeb00, 0x0380),
                                 },
                                 0xcb9),
                         },
                 },
             .encounters{
                 pe2_encounter_t{
                     .id = 1,
                     .mask = UNRANDOMIZED,
                     .encounter_value = 500,
                     .enemy_record_descriptor_list_addr_offset = 0x9eb8,
                     .enemy_resources_desciptors_list_offset = 0x9e50,
                 },
                 pe2_encounter_t{
                     .id = 2,
                     .mask = RANDOMIZED,
                     .encounter_value = 500,
                     .enemy_record_descriptor_list_addr_offset = 0x9ec0,
                     .enemy_resources_desciptors_list_offset = 0x9e68,
                 },
                 pe2_encounter_t{
                     .id = 3,
                     .mask = UNRANDOMIZED,
                     .encounter_value = 500,
                     .enemy_record_descriptor_list_addr_offset = 0x9ec8,
                     .enemy_resources_desciptors_list_offset = 0x9e80,
                 },
                 pe2_encounter_t{
                     .id = 4,
                     .mask = UNRANDOMIZED,
                     .encounter_value = 500,
                     .enemy_record_descriptor_list_addr_offset = 0x9ed0,
                     .enemy_resources_desciptors_list_offset = 0x9ea4,
                 },
             },
         },
         pe2_room_encounters_t{
             .id = 0x0d,
             .name = game::ROOM_NEO_ARK_AND_SHELTER_PAVILION,
             .encounters_offset = 0x1a80,
             .additional_file_end_pointer = 0x4d0,
             .environment =
                 pe2_room_environment_t{
                     .spawn_zones =
                         std::vector<utils::geometry_works::Zone>{
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0xee6c, 0x1d4c),
                                 utils::geometry_works::Point(0xee6c, 0x2cec),
                                 utils::geometry_works::Point(0x0fa0, 0x2cec),
                                 utils::geometry_works::Point(0x0fa0, 0x1d4c),
                             }),
                         },
                     .spawn_water_zones =
                         std::vector<utils::geometry_works::Zone>{
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0xe36e, 0x2f76),
                                 utils::geometry_works::Point(0xe36e, 0x1750),
                                 utils::geometry_works::Point(0xd95a, 0x1750),
                                 utils::geometry_works::Point(0xd95a, 0x2f76),
                             }),
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0xe627, 0x356a),
                                 utils::geometry_works::Point(0xe627, 0x3c40),
                                 utils::geometry_works::Point(0x1cee, 0x3c40),
                                 utils::geometry_works::Point(0x1cee, 0x356a),
                             }),
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0x2017, 0x2fdd),
                                 utils::geometry_works::Point(0x2c41, 0x2fdd),
                                 utils::geometry_works::Point(0x2c41, 0x1738),
                                 utils::geometry_works::Point(0x2017, 0x1738),
                             }),
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0x1b55, 0x14dd),
                                 utils::geometry_works::Point(0xf8b3, 0x14dd),
                                 utils::geometry_works::Point(0xf8b3, 0x0baf),
                                 utils::geometry_works::Point(0x1b55, 0x0baf),
                             }),
                         },
                     .dead_zones =
                         std::vector<utils::geometry_works::Zone>{
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0x0fa0, 0x2a30),
                                 utils::geometry_works::Point(0x044c, 0x2a30),
                                 utils::geometry_works::Point(0x044c, 0x2008),
                                 utils::geometry_works::Point(0x0fa0, 0x2008),
                             }),
                         },
                 },
             .encounters{
                 pe2_encounter_t{
                     .id = 1,
                     .mask = FORBID_SLOT_3_ENEMIES,
                     .encounter_value = 500,
                     .enemy_record_descriptor_list_addr_offset = 0xa10c,
                     .enemy_resources_desciptors_list_offset = 0xa05c,
                 },
                 pe2_encounter_t{
                     .id = 3,
                     .mask = FORBID_SLOT_3_ENEMIES,
                     .encounter_value = 500,
                     .enemy_record_descriptor_list_addr_offset = 0xa11c,
                     .enemy_resources_desciptors_list_offset = 0xa098,
                 },
                 pe2_encounter_t{
                     .id = 4,
                     .mask = FORBID_SLOT_3_ENEMIES,
                     .encounter_value = 500,
                     .enemy_record_descriptor_list_addr_offset = 0xa124,
                     .enemy_resources_desciptors_list_offset = 0xa0b0,
                 },
                 pe2_encounter_t{
                     .id = 5,
                     .mask = FORBID_SLOT_3_ENEMIES,
                     .encounter_value = 500,
                     .enemy_record_descriptor_list_addr_offset = 0xa12c,
                     .enemy_resources_desciptors_list_offset = 0xa0d4,
                 },
                 pe2_encounter_t{
                     .id = 11,
                     .mask = FORBID_SLOT_3_ENEMIES,
                     .encounter_value = 500,
                     .enemy_record_descriptor_list_addr_offset = 0xa15c,
                     .enemy_resources_desciptors_list_offset = 0xa0ec,
                 },
             },
             .allowed_additional_enemies{0x04},
         },
         pe2_room_encounters_t{
             .id = 0x0e,
             .name = game::ROOM_NEO_ARK_AND_SHELTER_ISLAND,
             .encounters_offset = 0x1c00,
             .additional_file_end_pointer = 0x4d8,
             .environment =
                 pe2_room_environment_t{
                     .spawn_zones =
                         std::vector<utils::geometry_works::Zone>{
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0x0739, 0xf9d2),
                                 utils::geometry_works::Point(0x2008, 0xf9d2),
                                 utils::geometry_works::Point(0x1708, 0xec45),
                                 utils::geometry_works::Point(0x04b0, 0xec45),
                             }),
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0x0df2, 0xfd3b),
                                 utils::geometry_works::Point(0x0df2, 0x15d5),
                                 utils::geometry_works::Point(0x08fc, 0x15d5),
                                 utils::geometry_works::Point(0x08fc, 0xfd3b),
                             }),
                         },
                     .spawn_water_zones =
                         std::vector<utils::geometry_works::Zone>{
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0x03e4, 0x16ee),
                                 utils::geometry_works::Point(0xfcd6, 0x16ee),
                                 utils::geometry_works::Point(0xfcd6, 0x02c8),
                                 utils::geometry_works::Point(0x03e4, 0x02c8),
                             }),
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0x128c, 0x0765),
                                 utils::geometry_works::Point(0x128c, 0x16d8),
                                 utils::geometry_works::Point(0x1c50, 0x16d8),
                                 utils::geometry_works::Point(0x1c50, 0x0765),
                             }),
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0x253d, 0x03cc),
                                 utils::geometry_works::Point(0x30f9, 0x03cc),
                                 utils::geometry_works::Point(0x30f9, 0xeb92),
                                 utils::geometry_works::Point(0x253d, 0xeb92),
                             }),
                         },
                     .dead_zones =
                         std::vector<utils::geometry_works::Zone>{
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0x0fa0, 0x2a30),
                                 utils::geometry_works::Point(0x044c, 0x2a30),
                                 utils::geometry_works::Point(0x044c, 0x2008),
                                 utils::geometry_works::Point(0x0fa0, 0x2008),
                             }),
                         },
                 },
             .encounters{
                 pe2_encounter_t{
                     .id = 1,
                     .mask = FORBID_SLOT_3_ENEMIES,
                     .encounter_value = 500,
                     .enemy_record_descriptor_list_addr_offset = 0x6990,
                     .enemy_resources_desciptors_list_offset = 0x691c,
                 },
                 pe2_encounter_t{
                     .id = 2,
                     .mask = FORBID_SLOT_3_ENEMIES,
                     .encounter_value = 500,
                     .enemy_record_descriptor_list_addr_offset = 0x6998,
                     .enemy_resources_desciptors_list_offset = 0x6934,
                 },
                 pe2_encounter_t{
                     .id = 3,
                     .mask = FORBID_SLOT_3_ENEMIES,
                     .encounter_value = 500,
                     .enemy_record_descriptor_list_addr_offset = 0x69a0,
                     .enemy_resources_desciptors_list_offset = 0x694c,
                 },
                 pe2_encounter_t{
                     .id = 11,
                     .mask = FORBID_SLOT_3_ENEMIES,
                     .encounter_value = 500,
                     .enemy_record_descriptor_list_addr_offset = 0x69e0,
                     .enemy_resources_desciptors_list_offset = 0x6970,
                 },
             },
             .allowed_additional_enemies{0x04},
         },
         pe2_room_encounters_t{
             .id = 0x0f,
             .name = game::ROOM_NEO_ARK_AND_SHELTER_GARDEN,
             .encounters_offset = 0x1ce0,
             .additional_file_end_pointer = 0x4e0,
             .environment =
                 pe2_room_environment_t{
                     .spawn_zones =
                         std::vector<utils::geometry_works::Zone>{
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0x05dc, 0xb832),
                                 utils::geometry_works::Point(0x05dc, 0xc75c),
                                 utils::geometry_works::Point(0xdecc, 0xc75c),
                                 utils::geometry_works::Point(0xdecc, 0xb832),
                             }),
                         },
                 },
             .encounters{
                 pe2_encounter_t{
                     .id = 1,
                     .mask = FORBID_SLOT_3_ENEMIES,
                     .encounter_value = 500,
                     .enemy_record_descriptor_list_addr_offset = 0x559c,
                     .enemy_resources_desciptors_list_offset = 0x5528,
                 },
                 pe2_encounter_t{
                     .id = 2,
                     .mask = UNRANDOMIZED,
                     .encounter_value = 0,
                     .enemy_record_descriptor_list_addr_offset = 0x55a4,
                     .enemy_resources_desciptors_list_offset = 0x5540,
                 },
                 pe2_encounter_t{
                     .id = 3,
                     .mask = FORBID_SLOT_3_ENEMIES,
                     .encounter_value = 500,
                     .enemy_record_descriptor_list_addr_offset = 0x55ac,
                     .enemy_resources_desciptors_list_offset = 0x5558,
                 },
             },
         },
         pe2_room_encounters_t{
             .id = 0x10,
             .name = game::ROOM_NEO_ARK_AND_SHELTER_POWER_PLANT_2,
             .encounters_offset = 0x1e80,
             .additional_file_end_pointer = 0x4e8,
             .environment =
                 pe2_room_environment_t{
                     .spawn_zones =
                         std::vector<utils::geometry_works::Zone>{
                             utils::geometry_works::Zone(
                                 utils::geometry_works::Polygon{
                                     utils::geometry_works::Point(0x1a91, 0xde03),
                                     utils::geometry_works::Point(0x21fa, 0xde03),
                                     utils::geometry_works::Point(0x21fc, 0xfa26),
                                     utils::geometry_works::Point(0x1a91, 0xfa26),
                                 },
                                 0xecdc),
                             utils::geometry_works::Zone(
                                 utils::geometry_works::Polygon{
                                     utils::geometry_works::Point(0x166c, 0xf6cb),
                                     utils::geometry_works::Point(0x166c, 0xfa24),
                                     utils::geometry_works::Point(0x012d, 0xfa24),
                                     utils::geometry_works::Point(0x012d, 0xf6cb),
                                 },
                                 0xecdc),
                         },
                 },
             .encounters{
                 pe2_encounter_t{
                     .id = 1,
                     .mask = UNRANDOMIZED,
                     .encounter_value = 0,
                     .enemy_record_descriptor_list_addr_offset = 0x5828,
                     .enemy_resources_desciptors_list_offset = 0x57c0,
                 },
                 pe2_encounter_t{
                     .id = 11,
                     .mask = RANDOMIZED,
                     .encounter_value = 500,
                     .enemy_record_descriptor_list_addr_offset = 0x5878,
                     .enemy_resources_desciptors_list_offset = 0x57e4,
                 },
                 pe2_encounter_t{
                     .id = 17,
                     .mask = RANDOMIZED,
                     .encounter_value = 500,
                     .enemy_record_descriptor_list_addr_offset = 0x58a8,
                     .enemy_resources_desciptors_list_offset = 0x5808,
                 },
             },
         },
         pe2_room_encounters_t{
             .id = 0x11,
             .name = game::ROOM_NEO_ARK_AND_SHELTER_POWER_PLANT_1,
             .encounters_offset = 0x1f80,
             .additional_file_end_pointer = 0x4f0,
             .environment =
                 pe2_room_environment_t{
                     .spawn_zones =
                         std::vector<utils::geometry_works::Zone>{
                             utils::geometry_works::Zone(
                                 utils::geometry_works::Polygon{
                                     utils::geometry_works::Point(0x1a90, 0xdf30),
                                     utils::geometry_works::Point(0x21fc, 0xdf30),
                                     utils::geometry_works::Point(0x21fc, 0xd24c),
                                     utils::geometry_works::Point(0x1a90, 0xd24c),
                                 },
                                 0x006e),
                             utils::geometry_works::Zone(
                                 utils::geometry_works::Polygon{
                                     utils::geometry_works::Point(0x012c, 0xf31c),
                                     utils::geometry_works::Point(0x04b0, 0xf31c),
                                     utils::geometry_works::Point(0x04b0, 0xd84c),
                                     utils::geometry_works::Point(0x012c, 0xd84c),
                                 },
                                 0x006e),
                             utils::geometry_works::Zone(
                                 utils::geometry_works::Polygon{
                                     utils::geometry_works::Point(0x17a3, 0xd5d0),
                                     utils::geometry_works::Point(0x17a3, 0xd24c),
                                     utils::geometry_works::Point(0x012c, 0xd24c),
                                     utils::geometry_works::Point(0x012c, 0xd5d0),
                                 },
                                 0x006e),
                         },
                 },
             .encounters{
                 pe2_encounter_t{
                     .id = 1,
                     .mask = UNRANDOMIZED,
                     .encounter_value = 0,
                     .enemy_record_descriptor_list_addr_offset = 0x4540,
                     .enemy_resources_desciptors_list_offset = 0x44a8,
                 },
                 pe2_encounter_t{
                     .id = 2,
                     .mask = RANDOMIZED,
                     .encounter_value = 500,
                     .enemy_record_descriptor_list_addr_offset = 0x4548,
                     .enemy_resources_desciptors_list_offset = 0x44cc,
                 },
                 pe2_encounter_t{
                     .id = 4,
                     .mask = RANDOMIZED,
                     .encounter_value = 500,
                     .enemy_record_descriptor_list_addr_offset = 0x4558,
                     .enemy_resources_desciptors_list_offset = 0x44fc,
                 },
                 pe2_encounter_t{
                     .id = 11,
                     .mask = RANDOMIZED,
                     .encounter_value = 500,
                     .enemy_record_descriptor_list_addr_offset = 0x4590,
                     .enemy_resources_desciptors_list_offset = 0x4520,
                 },
             },
         },
         pe2_room_encounters_t{
             .id = 0x12,
             .name = game::ROOM_NEO_ARK_AND_SHELTER_SAVANNA_ZONE,
             .encounters_offset = 0x2220,
             .additional_file_end_pointer = 0x4f8,
             .environment =
                 pe2_room_environment_t{
                     .spawn_zones =
                         std::vector<utils::geometry_works::Zone>{
                             utils::geometry_works::Zone(
                                 utils::geometry_works::Polygon{
                                     utils::geometry_works::Point(0x00fb, 0x0a5a),
                                     utils::geometry_works::Point(0x00fb, 0x00fa),
                                     utils::geometry_works::Point(0x3553, 0x00fa),
                                     utils::geometry_works::Point(0x3553, 0x0a5a),
                                 },
                                 0x00b3),
                         },
                 },
             .encounters{
                 pe2_encounter_t{
                     .id = 1,
                     .mask = RANDOMIZED,
                     .encounter_value = 500,
                     .enemy_record_descriptor_list_addr_offset = 0x32ac,
                     .enemy_resources_desciptors_list_offset = 0x31d8,
                 },
                 pe2_encounter_t{
                     .id = 2,
                     .mask = RANDOMIZED,
                     .encounter_value = 500,
                     .enemy_record_descriptor_list_addr_offset = 0x32b4,
                     .enemy_resources_desciptors_list_offset = 0x31fc,
                 },
                 pe2_encounter_t{
                     .id = 4,
                     .mask = RANDOMIZED,
                     .encounter_value = 500,
                     .enemy_record_descriptor_list_addr_offset = 0x32bc,
                     .enemy_resources_desciptors_list_offset = 0x3214,
                 },
                 pe2_encounter_t{
                     .id = 11,
                     .mask = RANDOMIZED,
                     .encounter_value = 500,
                     .enemy_record_descriptor_list_addr_offset = 0x32cc,
                     .enemy_resources_desciptors_list_offset = 0x325c,
                 },
                 pe2_encounter_t{
                     .id = 11,
                     .mask = RANDOMIZED,
                     .encounter_value = 500,
                     .enemy_record_descriptor_list_addr_offset = 0x32fc,
                     .enemy_resources_desciptors_list_offset = 0x3280,
                 },
             },
             .allowed_additional_enemies{},
         },
         pe2_room_encounters_t{
             .id = 0x13,
             .name = game::ROOM_NEO_ARK_AND_SHELTER_SOUTH_PROMENADE,
             .encounters_offset = 0x24f0,
             .additional_file_end_pointer = 0x500,
             .environment =
                 pe2_room_environment_t{.spawn_zones =
                                            std::vector<utils::geometry_works::Zone>{
                                                utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                                    utils::geometry_works::Point(0x0cf8, 0x0384),
                                                    utils::geometry_works::Point(0x0cf8, 0x0f6e),
                                                    utils::geometry_works::Point(0x323b, 0x0f6e),
                                                    utils::geometry_works::Point(0x22f9, 0x0384),
                                                }),
                                                utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                                    utils::geometry_works::Point(0x26a7, 0x1309),
                                                    utils::geometry_works::Point(0x32c8, 0x1309),
                                                    utils::geometry_works::Point(0x32c8, 0x2981),
                                                    utils::geometry_works::Point(0x26a7, 0x2981),
                                                }),
                                                utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                                    utils::geometry_works::Point(0x2134, 0x2bf9),
                                                    utils::geometry_works::Point(0x27bf, 0x2bf9),
                                                    utils::geometry_works::Point(0x27bf, 0x2018),
                                                    utils::geometry_works::Point(0x2134, 0x22cd),
                                                }),
                                            }},
             .encounters{
                 pe2_encounter_t{
                     .id = 1,
                     .mask = RANDOMIZED,
                     .encounter_value = 500,
                     .enemy_record_descriptor_list_addr_offset = 0x334c,
                     .enemy_resources_desciptors_list_offset = 0x32e8,
                 },
                 pe2_encounter_t{
                     .id = 2, // 0x7 actually
                     .mask = RANDOMIZED,
                     .encounter_value = 500,
                     .enemy_record_descriptor_list_addr_offset = 0x337c,
                     .enemy_resources_desciptors_list_offset = 0x3300,
                 },
             },
             .allowed_additional_enemies{},
         },
         pe2_room_encounters_t{
             .id = 0x15,
             .name = game::ROOM_NEO_ARK_AND_SHELTER_SHRINE,
             .encounters_offset = 0x2560,
             .additional_file_end_pointer = 0x510,
             .environment =
                 pe2_room_environment_t{.spawn_zones =
                                            std::vector<utils::geometry_works::Zone>{
                                                utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                                    utils::geometry_works::Point(0x1e14, 0x1644),
                                                    utils::geometry_works::Point(0x1e14, 0xefc4),
                                                    utils::geometry_works::Point(0x189c, 0xefc4),
                                                    utils::geometry_works::Point(0x189c, 0x1644),
                                                }),
                                                utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                                    utils::geometry_works::Point(0x215f, 0x0898),
                                                    utils::geometry_works::Point(0x2cf9, 0x0898),
                                                    utils::geometry_works::Point(0x2cf9, 0x0320),
                                                    utils::geometry_works::Point(0x215f, 0x0320),
                                                }),
                                                utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                                    utils::geometry_works::Point(0x1533, 0x113d),
                                                    utils::geometry_works::Point(0x012c, 0x113d),
                                                    utils::geometry_works::Point(0x012c, 0x1644),
                                                    utils::geometry_works::Point(0x1533, 0x1644),
                                                }),
                                                utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                                    utils::geometry_works::Point(0x012c, 0x0ed1),
                                                    utils::geometry_works::Point(0x0564, 0x0ed1),
                                                    utils::geometry_works::Point(0x0564, 0x07f8),
                                                    utils::geometry_works::Point(0x012c, 0x07f8),
                                                }),
                                            }},
             .encounters{
                 pe2_encounter_t{
                     .id = 1,
                     .mask = RANDOMIZED,
                     .encounter_value = 500,
                     .enemy_record_descriptor_list_addr_offset = 0x9110,
                     .enemy_resources_desciptors_list_offset = 0x9054,
                 },
                 pe2_encounter_t{
                     .id = 4,
                     .mask = RANDOMIZED,
                     .encounter_value = 500,
                     .enemy_record_descriptor_list_addr_offset = 0x9128,
                     .enemy_resources_desciptors_list_offset = 0x909c,
                 },
                 pe2_encounter_t{
                     .id = 7,
                     .mask = RANDOMIZED,
                     .encounter_value = 500,
                     .enemy_record_descriptor_list_addr_offset = 0x9140,
                     .enemy_resources_desciptors_list_offset = 0x90d8,
                 },
                 pe2_encounter_t{
                     .id = 5,
                     .mask = RANDOMIZED,
                     .encounter_value = 500,
                     .enemy_record_descriptor_list_addr_offset = 0x9130,
                     .enemy_resources_desciptors_list_offset = 0x90c0,
                 },
                 pe2_encounter_t{
                     .id = 11,
                     .mask = RANDOMIZED,
                     .encounter_value = 500,
                     .enemy_record_descriptor_list_addr_offset = 0x9160,
                     .enemy_resources_desciptors_list_offset = 0x90f0,
                 },
             },
             .allowed_additional_enemies{},
             .file_patch_map{
                 // Disable beetle drop down cutscene
                 {0x1bfe, 0x45},
             },
         },
         pe2_room_encounters_t{
             .id = 0x16,
             .name = game::ROOM_NEO_ARK_AND_SHELTER_NURSERY,
             .encounters_offset = 0x2740,
             .additional_file_end_pointer = 0x518,
             .encounters{
                 pe2_encounter_t{
                     .id = 1,
                     .mask = UNRANDOMIZED,
                     .encounter_value = 0,
                     .enemy_record_descriptor_list_addr_offset = 0x9eec,
                     .enemy_resources_desciptors_list_offset = 0x9eb4,
                 },
             },
         },
         pe2_room_encounters_t{
             .id = 0x18,
             .name = game::ROOM_NEO_ARK_AND_SHELTER_CORRIDOR,
             .encounters_offset = 0x27a0,
             .additional_file_end_pointer = 0x528,
             .environment =
                 pe2_room_environment_t{
                     .spawn_zones =
                         std::vector<utils::geometry_works::Zone>{
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0x0615, 0x041a),
                                 utils::geometry_works::Point(0x0615, 0xfbe6),
                                 utils::geometry_works::Point(0x1cbd, 0xfbe6),
                                 utils::geometry_works::Point(0x1cbd, 0x041a),
                             }),
                         },
                 },
             .encounters{
                 pe2_encounter_t{
                     .id = 1,
                     .mask = UNRANDOMIZED,
                     .encounter_value = 500,
                     .enemy_record_descriptor_list_addr_offset = 0x2d4c,
                     .enemy_resources_desciptors_list_offset = 0x2d08,
                 },
             },
         },
         pe2_room_encounters_t{
             .id = 0x19,
             .name = game::ROOM_NEO_ARK_AND_SHELTER_TRAINING_ROOM,
             .encounters_offset = 0x27f0,
             .additional_file_end_pointer = 0x530,
             .encounters{
                 pe2_encounter_t{
                     .id = 1,
                     .mask = UNRANDOMIZED,
                     .encounter_value = 0,
                     .enemy_record_descriptor_list_addr_offset = 0x8424,
                     .enemy_resources_desciptors_list_offset = 0x83d0,
                 },
             },
         },
         pe2_room_encounters_t{
             .id = 0x1a,
             .name = game::ROOM_NEO_ARK_AND_SHELTER_OVAL_OFFICE,
             .encounters_offset = 0x2850,
             .additional_file_end_pointer = 0x538,
             .encounters{
                 pe2_encounter_t{
                     .id = 1,
                     .mask = UNRANDOMIZED,
                     .encounter_value = 0,
                     .enemy_record_descriptor_list_addr_offset = 0x13dc,
                     .enemy_resources_desciptors_list_offset = 0x1380,
                 },
             },
         },
         pe2_room_encounters_t{
             .id = 0x1b,
             .name = game::ROOM_NEO_ARK_AND_SHELTER_BRIDGE,
             .encounters_offset = 0x2880,
             .additional_file_end_pointer = 0x540,
             .environment =
                 pe2_room_environment_t{
                     .spawn_zones =
                         std::vector<utils::geometry_works::Zone>{
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0xf704, 0x1693),
                                 utils::geometry_works::Point(0xf18c, 0x1693),
                                 utils::geometry_works::Point(0xf18c, 0xd92b),
                                 utils::geometry_works::Point(0xf704, 0xd92b),
                             }),
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0xfe3e, 0xfe3e),
                                 utils::geometry_works::Point(0xf18c, 0xfe3e),
                                 utils::geometry_works::Point(0xfe3e, 0xf60a),
                                 utils::geometry_works::Point(0xf18c, 0xf61e),
                             }),
                         },
                     .spawn_water_zones =
                         std::vector<utils::geometry_works::Zone>{
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0xfb6f, 0x17c5),
                                 utils::geometry_works::Point(0x0657, 0x17c5),
                                 utils::geometry_works::Point(0x0657, 0x0458),
                                 utils::geometry_works::Point(0xfb6f, 0x0458),
                             }),
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0xfbeb, 0xe3ff),
                                 utils::geometry_works::Point(0xfbeb, 0xd5f1),
                                 utils::geometry_works::Point(0x0b6e, 0xd5f1),
                                 utils::geometry_works::Point(0x0b6e, 0xe1bc),
                             }),
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0xecbc, 0x1624),
                                 utils::geometry_works::Point(0xe1b2, 0x1624),
                                 utils::geometry_works::Point(0xe1b2, 0xd913),
                                 utils::geometry_works::Point(0xecbc, 0xd913),
                             }),
                         },
                 },
             .encounters{
                 pe2_encounter_t{
                     .id = 1,
                     .mask = FORBID_SLOT_3_ENEMIES,
                     .encounter_value = 500,
                     .enemy_record_descriptor_list_addr_offset = 0x7498,
                     .enemy_resources_desciptors_list_offset = 0x73c4,
                 },
                 pe2_encounter_t{
                     .id = 2,
                     .mask = FORBID_SLOT_3_ENEMIES,
                     .encounter_value = 500,
                     .enemy_record_descriptor_list_addr_offset = 0x74a0,
                     .enemy_resources_desciptors_list_offset = 0x73dc,
                 },
                 pe2_encounter_t{
                     .id = 3,
                     .mask = FORBID_SLOT_3_ENEMIES,
                     .encounter_value = 500,
                     .enemy_record_descriptor_list_addr_offset = 0x74a8,
                     .enemy_resources_desciptors_list_offset = 0x7400,
                 },
                 pe2_encounter_t{
                     .id = 4,
                     .mask = FORBID_SLOT_3_ENEMIES,
                     .encounter_value = 500,
                     .enemy_record_descriptor_list_addr_offset = 0x74b8,
                     .enemy_resources_desciptors_list_offset = 0x7448,
                 },
                 pe2_encounter_t{
                     .id = 11,
                     .mask = FORBID_SLOT_3_ENEMIES,
                     .encounter_value = 500,
                     .enemy_record_descriptor_list_addr_offset = 0x74e8,
                     .enemy_resources_desciptors_list_offset = 0x7460,
                 },
             },
             .allowed_additional_enemies{0x04},
         },
         pe2_room_encounters_t{
             .id = 0x1c,
             .name = game::ROOM_NEO_ARK_AND_SHELTER_TENT,
             .encounters_offset = 0x2a30,
             .additional_file_end_pointer = 0x548,
             .encounters{
                 pe2_encounter_t{
                     .id = 1,
                     .mask = UNRANDOMIZED,
                     .encounter_value = 0,
                     .enemy_record_descriptor_list_addr_offset = 0x6c78,
                     .enemy_resources_desciptors_list_offset = 0x6c40,
                 },
             },
         },
         pe2_room_encounters_t{
             .id = 0x1d,
             .name = game::ROOM_NEO_ARK_AND_SHELTER_JUNGLE_ZONE,
             .encounters_offset = 0x2a70,
             .additional_file_end_pointer = 0x550,
             .environment =
                 pe2_room_environment_t{
                     .separated_zones = true,
                     .spawn_zones =
                         std::vector<utils::geometry_works::Zone>{
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0x1400, 0x1280),
                                 utils::geometry_works::Point(0x1400, 0x1600),
                                 utils::geometry_works::Point(0xf000, 0x1600),
                                 utils::geometry_works::Point(0xf000, 0x1280),
                             }),
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0x1400, 0xed00),
                                 utils::geometry_works::Point(0x1400, 0x1600),
                                 utils::geometry_works::Point(0x0d00, 0x1600),
                                 utils::geometry_works::Point(0x0d00, 0xed00),
                             }),
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0xf200, 0xed00),
                                 utils::geometry_works::Point(0xf200, 0xf500),
                                 utils::geometry_works::Point(0x0d00, 0xf500),
                                 utils::geometry_works::Point(0x0d00, 0xed00),
                             }),
                         },
                 },
             .encounters{
                 pe2_encounter_t{
                     .id = 1,
                     .mask = FORBID_SLOT_3_ENEMIES,
                     .encounter_value = 500,
                     .enemy_record_descriptor_list_addr_offset = 0x7164,
                     .enemy_resources_desciptors_list_offset = 0x70fc,
                 },
                 pe2_encounter_t{
                     .id = 2,
                     .mask = FORBID_SLOT_3_ENEMIES,
                     .encounter_value = 500,
                     .enemy_record_descriptor_list_addr_offset = 0x716c,
                     .enemy_resources_desciptors_list_offset = 0x7114,
                 },
                 pe2_encounter_t{
                     .id = 3,
                     .mask = FORBID_SLOT_3_ENEMIES,
                     .encounter_value = 500,
                     .enemy_record_descriptor_list_addr_offset = 0x7174,
                     .enemy_resources_desciptors_list_offset = 0x712c,
                 },
                 pe2_encounter_t{
                     .id = 4,
                     .mask = FORBID_SLOT_3_ENEMIES,
                     .encounter_value = 500,
                     .enemy_record_descriptor_list_addr_offset = 0x717c,
                     .enemy_resources_desciptors_list_offset = 0x7144,
                 },
             },
             .forbidden_enemies = std::vector<uint8_t>{0x0a, 0x12, 0x13},
         },
         pe2_room_encounters_t{
             .id = 0x1e,
             .name = game::ROOM_NEO_ARK_AND_SHELTER_SUBMARINE_GALLERY,
             .encounters_offset = 0x2b50,
             .additional_file_end_pointer = 0x558,
             .environment =
                 pe2_room_environment_t{
                     .spawn_zones =
                         std::vector<utils::geometry_works::Zone>{
                             utils::geometry_works::Zone(
                                 utils::geometry_works::Polygon{
                                     utils::geometry_works::Point(0xf26c, 0xfd23),
                                     utils::geometry_works::Point(0xf72a, 0xfe0f),
                                     utils::geometry_works::Point(0xf886, 0x050c),
                                     utils::geometry_works::Point(0xf429, 0x07d1),
                                 },
                                 0x1409),
                             utils::geometry_works::Zone(
                                 utils::geometry_works::Polygon{
                                     utils::geometry_works::Point(0xf886, 0x050c),
                                     utils::geometry_works::Point(0xf429, 0x07d1),
                                     utils::geometry_works::Point(0xfd7f, 0x0e27),
                                     utils::geometry_works::Point(0xfda8, 0x08d6),
                                 },
                                 0x1409),
                             utils::geometry_works::Zone(
                                 utils::geometry_works::Polygon{
                                     utils::geometry_works::Point(0xfd7f, 0x0e27),
                                     utils::geometry_works::Point(0xfda8, 0x08d6),
                                     utils::geometry_works::Point(0x0818, 0x0be1),
                                     utils::geometry_works::Point(0x0563, 0x071c),
                                 },
                                 0x1409),
                             utils::geometry_works::Zone(
                                 utils::geometry_works::Polygon{
                                     utils::geometry_works::Point(0x0818, 0x0be1),
                                     utils::geometry_works::Point(0x0563, 0x071c),
                                     utils::geometry_works::Point(0x08d6, 0x019e),
                                     utils::geometry_works::Point(0x0e04, 0x02d2),
                                 },
                                 0x1409),
                             utils::geometry_works::Zone(
                                 utils::geometry_works::Polygon{
                                     utils::geometry_works::Point(0x08d6, 0x019e),
                                     utils::geometry_works::Point(0x0e04, 0x02d2),
                                     utils::geometry_works::Point(0x07d5, 0xfb54),
                                     utils::geometry_works::Point(0x0bdc, 0xf7ea),
                                 },
                                 0x1409),
                             utils::geometry_works::Zone(
                                 utils::geometry_works::Polygon{
                                     utils::geometry_works::Point(0x07d5, 0xfb54),
                                     utils::geometry_works::Point(0x0bdc, 0xf7ea),
                                     utils::geometry_works::Point(0x0378, 0xf21f),
                                     utils::geometry_works::Point(0x1d8, 0xf72a),
                                 },
                                 0x1409),
                             utils::geometry_works::Zone(
                                 utils::geometry_works::Polygon{
                                     utils::geometry_works::Point(0x0378, 0xf21f),
                                     utils::geometry_works::Point(0x1d8, 0xf72a),
                                     utils::geometry_works::Point(0xf813, 0xf40a),
                                     utils::geometry_works::Point(0xfa92, 0xf8ad),
                                 },
                                 0x1409),
                             utils::geometry_works::Zone(
                                 utils::geometry_works::Polygon{
                                     utils::geometry_works::Point(0xf813, 0xf40a),
                                     utils::geometry_works::Point(0xfa92, 0xf8ad),
                                     utils::geometry_works::Point(0xf1eb, 0xfd29),
                                     utils::geometry_works::Point(0xf72a, 0xfdda),
                                 },
                                 0x1409),
                         },
                     .spawn_water_zones =
                         std::vector<utils::geometry_works::Zone>{
                             utils::geometry_works::Zone(
                                 utils::geometry_works::Polygon{
                                     utils::geometry_works::Point(0xf72a, 0xfdda),
                                     utils::geometry_works::Point(0xfd46, 0x0870),
                                     utils::geometry_works::Point(0x08d6, 0x021e),
                                     utils::geometry_works::Point(0x035b, 0xf7d2),
                                 },
                                 0x1409),
                         },
                 },
             .encounters{
                 pe2_encounter_t{
                     .id = 1,
                     .mask = UNRANDOMIZED,
                     .encounter_value = 0,
                     .enemy_record_descriptor_list_addr_offset = 0x82a8,
                     .enemy_resources_desciptors_list_offset = 0x8234,
                     .force_reward_value = 1000,
                 },
                 pe2_encounter_t{
                     .id = 2,
                     .mask = FORBID_SLOT_3_ENEMIES,
                     .encounter_value = 500,
                     .enemy_record_descriptor_list_addr_offset = 0x82b0,
                     .enemy_resources_desciptors_list_offset = 0x8258,
                 },
                 pe2_encounter_t{
                     .id = 3,
                     .mask = FORBID_SLOT_3_ENEMIES,
                     .encounter_value = 500,
                     .enemy_record_descriptor_list_addr_offset = 0x82b8,
                     .enemy_resources_desciptors_list_offset = 0x8270,
                 },
                 pe2_encounter_t{
                     .id = 4,
                     .mask = FORBID_SLOT_3_ENEMIES,
                     .encounter_value = 500,
                     .enemy_record_descriptor_list_addr_offset = 0x82f8,
                     .enemy_resources_desciptors_list_offset = 0x8288,
                 },
             },
             .allowed_additional_enemies{0x04},
         },
         pe2_room_encounters_t{
             .id = 0x1f,
             .name = game::ROOM_NEO_ARK_AND_SHELTER_CUTSCENE,
             .encounters_offset = 0x2c00,
             .additional_file_end_pointer = 0x560,
             .encounters{
                 pe2_encounter_t{
                     .id = 1,
                     .mask = UNRANDOMIZED,
                     .encounter_value = 0,
                     .enemy_record_descriptor_list_addr_offset = 0x600,
                     .enemy_resources_desciptors_list_offset = 0x5d4,
                 },
             },
         },
         pe2_room_encounters_t{
             .id = 0x20,
             .name = game::ROOM_NEO_ARK_AND_SHELTER_PYRAMID,
             .encounters_offset = 0x2c30,
             .additional_file_end_pointer = 0x568,
             .environment =
                 pe2_room_environment_t{
                     .spawn_zones =
                         std::vector<utils::geometry_works::Zone>{
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0x1a45, 0xec14),
                                 utils::geometry_works::Point(0x00cf, 0xec14),
                                 utils::geometry_works::Point(0x0736, 0xda1c),
                                 utils::geometry_works::Point(0x1a45, 0xd72e),
                             }),
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0x0592, 0xdaf8),
                                 utils::geometry_works::Point(0xf18c, 0xf014),
                                 utils::geometry_works::Point(0xf8a6, 0xf297),
                                 utils::geometry_works::Point(0xff84, 0xec14),
                             }),
                         },
                     .dead_zones =
                         std::vector<utils::geometry_works::Zone>{
                             utils::geometry_works::Zone(utils::geometry_works::Polygon{
                                 utils::geometry_works::Point(0x0924, 0xdea9),
                                 utils::geometry_works::Point(0x0924, 0xe645),
                                 utils::geometry_works::Point(0x1a45, 0xe645),
                                 utils::geometry_works::Point(0x1a45, 0xdea9),
                             }),
                         },
                 },
             .encounters{
                 pe2_encounter_t{
                     .id = 1,
                     .mask = RANDOMIZED,
                     .encounter_value = 500,
                     .enemy_record_descriptor_list_addr_offset = 0x4170,
                     .enemy_resources_desciptors_list_offset = 0x40cc,
                 },
                 pe2_encounter_t{
                     .id = 2,
                     .mask = RANDOMIZED,
                     .encounter_value = 500,
                     .enemy_record_descriptor_list_addr_offset = 0x4178,
                     .enemy_resources_desciptors_list_offset = 0x40e4,
                 },
                 pe2_encounter_t{
                     .id = 3,
                     .mask = RANDOMIZED,
                     .encounter_value = 500,
                     .enemy_record_descriptor_list_addr_offset = 0x4180,
                     .enemy_resources_desciptors_list_offset = 0x40fc,
                 },
                 pe2_encounter_t{
                     .id = 4,
                     .mask = RANDOMIZED,
                     .encounter_value = 500,
                     .enemy_record_descriptor_list_addr_offset = 0x4188,
                     .enemy_resources_desciptors_list_offset = 0x4120,
                 },
                 pe2_encounter_t{
                     .id = 11,
                     .mask = RANDOMIZED,
                     .encounter_value = 500,
                     .enemy_record_descriptor_list_addr_offset = 0x41c0,
                     .enemy_resources_desciptors_list_offset = 0x4144,
                 },
             },
             .allowed_additional_enemies{},
         },
     }}};

std::map<game::pe2_level_name_t, pe2_encounters_list_segment_t> encounters_segment_per_level_map{
    {game::AKROPOLIS_LEVEL,
     pe2_encounters_list_segment_t{
         .from = AKROPOLIS_ENCOUNTERS_START,
         .to = AKROPOLIS_ENCOUNTERS_END,
     }},
    {game::DRYFIELD_DAY_LEVEL,
     pe2_encounters_list_segment_t{
         .from = DRYFIELD_DAY_ENCOUNTERS_START,
         .to = DRYFIELD_DAY_ENCOUNTERS_END,
     }},
    {game::DRYFIELD_NIGHT_LEVEL,
     pe2_encounters_list_segment_t{
         .from = DRYFIELD_NIGHT_ENCOUNTERS_START,
         .to = DRYFIELD_NIGHT_ENCOUNTERS_END,
     }},
    {game::MINES_AND_SHELTER_LEVEL,
     pe2_encounters_list_segment_t{
         .internal = true,
     }},
    {game::NEOARK_AND_SHELTER_LEVEL, pe2_encounters_list_segment_t{
                                         .from = NEO_ARK_ENCOUNTERS_START,
                                         .to = NEO_ARK_ENCOUNTERS_END,
                                     }}};

} // namespace domain::encounter
