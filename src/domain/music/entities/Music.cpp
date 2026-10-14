#include "domain/music/entities/Music.hpp"

namespace domain::music {

using domain::game::AKROPOLIS_LEVEL;
using domain::game::DRYFIELD_DAY_LEVEL;
using domain::game::DRYFIELD_NIGHT_LEVEL;
using domain::game::MINES_AND_SHELTER_LEVEL;
using domain::game::NEOARK_AND_SHELTER_LEVEL;
using domain::game::pe2_level_name_t;

// TODO: Finish music list
// TODO: FInish complete music entry description
/*
    ALL AVAILABLE MUSIC IDS LIST (unfinished)

    0x05, // cafeteria jazz (outsize)
    0x06, // stranger boss music
    0x09, // cafeteria jazz (inside)
    0x0b, // ???
    0x0c, // winning no.9
    0x11, // MIST garage
    0x17, // fight 1
    0x1d, // first stranger scene (eating corpse)
    0x1e, // fight 2 (acropolis)
    0x1f, // acropolis approach scene
    0x20, // go out of the elevator scene
    0x21, // acropolis peace 1 (first enetering square)
    0x22, // acropolis peace 2
    0x23, // acropolis peace 3
    0x24, // acropolis peace 4
    0x25, // returning to MIST cut scene
    0x26, // akropolis peace 5
    0x29, // dryfield day peace 1
    0x2a, // dryfield day peace 2 (meeting Doug)
    0x2b, // bar music
    0x2c, // ???
    0x2d, // dryfield day peace 3
    0x2e, // dryfield day peace 4
    0x2f, // ???
    0x30, // dryfield fight ???
    0x31, // no 9. encounter 2 music???
    0x32, // dryfield fight ???
    0x37, // ???
    0x39, //
    0x3a, //
    0x3b, //
    0x3c, // chaser boss fight music
    0x3d, //
    0x3e, //
    0x3f, //
    0x41, //
    0x42, //
    0x43, //
    0x44, //
    0x45, //
    0x46, //
    0x47, //
    0x48, //
    0x49, //
    0x4a, //
    0x4b, //
    0x4c, //
    0x4d, //
    0x4e, //
    0x4f, // alpha/beta generator theme
    0x51, //
    0x52, //
    0x53, //
    0x56, //
    0x59, //
    0x5a, //
    0x5d, //
    0x5e, //
    0x5f, //
    0x60, //
    0x61, //
    0x62, //
    0x80, // SILENCE
*/

std::vector<uint8_t> possible_music{
    0x05,     // cafeteria jazz (outsize)
    0x06,     // stranger boss music
    0x09,     // cafeteria jazz (inside)
    /*0x0b,*/ // ???
    0x0c,     // winning no.9
    0x11,     // MIST garage
    0x17,     // fight 1
    0x1d,     // first stranger scene (eating corpse)
    0x1e,     // fight 2 (acropolis)
    0x1f,     // acropolis approach scene
    0x20,     // go out of the elevator scene
    0x21,     // acropolis peace 1 (first enetering square)
    0x22,     // acropolis peace 2
    0x23,     // acropolis peace 3
    0x24,     // acropolis peace 4
    0x25,     // returning to MIST cut scene
    0x26,     // acropolis peace 5
    0x29,     // dryfield day peace 1
    0x2a,     // dryfield day peace 2 (meeting Doug)
    0x2b,     // bar music
    0x2c,     // ???
    0x2d,     // dryfield day peace 3
    0x2e,     // dryfield day peace 4
    0x2f,
    0x30,
    0x31,
    0x32,
    0x37,
    0x39,
    0x3a,
    0x3b,
    0x3c,
    0x3d,
    0x3e,
    0x3f,
    0x41,
    0x42,
    0x43,
    0x44,
    0x45,
    0x46,
    0x47,
    0x48,
    0x49,
    0x4a,
    0x4b,
    0x4c,
    0x4d,
    0x4e,
    /*0x4f,*/ // alpha/beta generator theme
    0x51,
    0x52,
    0x53,
    0x56,
    0x59,
    0x5a,
    0x5d,
    0x5e,
    0x5f,
    0x60,
    0x61,
    0x62,
    0x80, // SILENCE
};

/*
    MUSIC IDS PER MUSIC TYPE
*/

std::vector<uint8_t> peaceful_music{0x11, 0x05, 0x09, 0x1f, 0x25, 0x2c, 0x3a, 0x50, 0x55, 0x61, 0x62};
std::vector<uint8_t> exploring_music{0x23, 0x24, 0x29, 0x2a, 0x2b, 0x2d, 0x2e, 0x2f, 0x37, 0x39, 0x3b, 0x3d,
                                     0x3f, 0x41, 0x47, 0x48, 0x49, 0x4b, 0x4c, 0x4d, 0x4e, 0x51, 0x53, 0x59};
std::vector<uint8_t> intense_music{0x20, 0x21, 0x22, 0x0c, 0x26, 0x3e, 0x52, 0x5a};
std::vector<uint8_t> fight_music{0x06, 0x14, 0x15, 0x16, 0x17, 0x1d, 0x1e, 0x30, 0x31, 0x32, 0x38, 0x3c,
                                 0x42, 0x43, 0x44, 0x45, 0x46, 0x4a, 0x52, 0x56, 0x5d, 0x5e, 0x5f, 0x60};

/*
    MUSIC ENTRIES MAP VALUES
*/

std::map<pe2_level_name_t, std::vector<pe2_music_entry_t>> music_entries_map{
    {
        AKROPOLIS_LEVEL,
        std::vector<pe2_music_entry_t>{
            pe2_music_entry_t{
                .offsets{0x2876},
                .type = MUSIC_INTENSE,
                .forbidden_music{},
            },
            pe2_music_entry_t{
                .offsets{0x2878, 0x287A, 0x2888, 0x288A, 0x289A, 0x28F8, 0x28FA, 0x2978, 0x297A},
                .type = MUSIC_INTENSE,
                .forbidden_music{},
            },
            pe2_music_entry_t{
                .offsets{0x287C, 0x289C, 0x28AC, 0x28CC, 0x28DC, 0x28EC, 0x28FC, 0x290C, 0x291C},
                .type = MUSIC_EXPLORING,
                .forbidden_music{},
            },
            pe2_music_entry_t{
                .offsets{0x287E, 0x289E, 0x28AE, 0x28CE, 0x28DE, 0x28EE, 0x28FE, 0x290E, 0x291E, 0x2920,
                         0x292C, 0x292E, 0x2930, 0x293E, 0x294E},
                .type = MUSIC_EXPLORING,
                .forbidden_music{},
            },
            pe2_music_entry_t{
                .offsets{0x2882, 0x28A2, 0x28B2, 0x28D2, 0x28E2, 0x28F2, 0x2902,
                         0x2912, 0x2922, 0x2932, 0x2942, 0x2952, 0x2956, 0x2958,
                         0x295A, 0x295C, 0x295E, 0x2960, 0x2962, 0x296E, 0x2972},
                .type = MUSIC_INTENSE,
                .forbidden_music{},
            },
            pe2_music_entry_t{
                .offsets{0x2898},
                .type = MUSIC_FIGHT,
                .forbidden_music{},
            },
            pe2_music_entry_t{
                .offsets{0x28AA},
                .type = MUSIC_PEACEFUL,
                .forbidden_music{},
            },
            pe2_music_entry_t{
                .offsets{0x28B0},
                .type = MUSIC_FIGHT,
                .forbidden_music{},
            },
            pe2_music_entry_t{
                .offsets{0x28B6},
                .type = MUSIC_PEACEFUL,
                .forbidden_music{},
            },
            pe2_music_entry_t{
                .offsets{0x2970},
                .type = MUSIC_INTENSE,
                .forbidden_music{},
            },
            pe2_music_entry_t{
                .offsets{0x2976},
                .type = MUSIC_INTENSE,
                .forbidden_music{},
            },
            pe2_music_entry_t{
                .offsets{0x2986},
                .type = MUSIC_PEACEFUL,
                .forbidden_music{},
            },
            pe2_music_entry_t{
                .offsets{0x2996, 0x29A6},
                .type = MUSIC_PEACEFUL,
                .forbidden_music{},
            },
            pe2_music_entry_t{
                .offsets{0x29B4},
                .type = MUSIC_FIGHT,
                .forbidden_music{},
            },
            pe2_music_entry_t{
                .offsets{0x29B6},
                .type = MUSIC_FIGHT,
                .forbidden_music{},
            },
            pe2_music_entry_t{
                .offsets{0x29C8},
                .type = MUSIC_FIGHT,
                .forbidden_music{},
            },
            pe2_music_entry_t{
                .offsets{0x29CA},
                .type = MUSIC_FIGHT,
                .forbidden_music{},
            },
        },
    },
    {
        DRYFIELD_DAY_LEVEL,
        std::vector<pe2_music_entry_t>{
            pe2_music_entry_t{
                .offsets{0x24A0, 0x24A2, 0x24A6, 0x24AE, 0x24B0, 0x24B4, 0x252C, 0x252E, 0x2532,
                         0x253A, 0x253C, 0x2540, 0x2564, 0x2566, 0x256A, 0x2572, 0x2574, 0x2578,
                         0x2594, 0x25A2, 0x25B0, 0x25BE, 0x25CC, 0x25D4, 0x25DA, 0x25E8, 0x25F0,
                         0x25F2, 0x25F6, 0x2604, 0x2628, 0x262A, 0x262E, 0x263C},
                .type = MUSIC_EXPLORING,
                .forbidden_music{},
            },
            pe2_music_entry_t{
                .offsets{0x24A4, 0x24A8, 0x24B2, 0x24B6, 0x24C0, 0x24C4, 0x24CE, 0x24DC, 0x24E0, 0x24EA,
                         0x24EE, 0x24F8, 0x24FC, 0x2506, 0x2514, 0x2522, 0x2530, 0x2534, 0x253E, 0x2542,
                         0x254C, 0x255A, 0x2568, 0x256C, 0x2576, 0x257A, 0x2584, 0x2592, 0x2596, 0x25A0,
                         0x25A4, 0x25AE, 0x25B2, 0x25C0, 0x25CA, 0x25CE, 0x25D8, 0x25DC, 0x25E6, 0x25EA,
                         0x25F4, 0x25F8, 0x2602, 0x2606, 0x2608, 0x2610, 0x261E, 0x262C, 0x2630, 0x263A,
                         0x263E, 0x2648, 0x2656, 0x2664, 0x2672, 0x2680, 0x268E, 0x269C, 0x26AA},
                .type = MUSIC_EXPLORING,
                .forbidden_music{},
            },
            pe2_music_entry_t{
                .offsets{0x2518},
                .type = MUSIC_FIGHT,
                .forbidden_music{},
            },
            pe2_music_entry_t{
                .offsets{0x25BC},
                .type = MUSIC_PEACEFUL,
                .forbidden_music{},
            },
            pe2_music_entry_t{
                .offsets{0x25D6, 0x25E2, 0x25E4},
                .type = MUSIC_EXPLORING,
                .forbidden_music{},
            },
            pe2_music_entry_t{
                .offsets{0x2612, 0x2614},
                .type = MUSIC_EXPLORING,
                .forbidden_music{},
            },
            pe2_music_entry_t{
                .offsets{0x265A, 0x2676, 0x26AE},
                .type = MUSIC_EXPLORING,
                .forbidden_music{},
            },
            pe2_music_entry_t{
                .offsets{0x26B4},
                .type = MUSIC_FIGHT,
                .forbidden_music{},
            },
            pe2_music_entry_t{
                .offsets{0x26B8},
                .type = MUSIC_FIGHT,
                .forbidden_music{},
            },
            pe2_music_entry_t{
                .offsets{0x26C8},
                .type = MUSIC_FIGHT,
                .forbidden_music{},
            },
            pe2_music_entry_t{
                .offsets{0x26CA},
                .type = MUSIC_FIGHT,
                .forbidden_music{},
            },
            pe2_music_entry_t{
                .offsets{0x26DC},
                .type = MUSIC_INTENSE,
                .forbidden_music{},
            },
        },
    },
    {DRYFIELD_NIGHT_LEVEL,
     std::vector<pe2_music_entry_t>{{
         pe2_music_entry_t{
             .offsets{0x38FE, 0x3914, 0x3956, 0x3A32, 0x3AA0, 0x3B0E, 0x3B66},
             .type = MUSIC_MISC,
             .forbidden_music{},
         },
         pe2_music_entry_t{
             .offsets{0x3900, 0x3908, 0x390A, 0x390C, 0x3910, 0x3912, 0x3916, 0x391E, 0x3926, 0x3928, 0x392C,
                      0x3934, 0x3936, 0x3938, 0x393C, 0x393E, 0x394C, 0x394E, 0x3952, 0x3954, 0x3958, 0x3960,
                      0x3962, 0x3964, 0x3968, 0x396A, 0x396E, 0x3976, 0x3978, 0x397A, 0x397E, 0x3980, 0x398C,
                      0x398E, 0x3990, 0x3994, 0x3996, 0x39A4, 0x39A6, 0x39AA, 0x39AC, 0x39B8, 0x39BA, 0x39BC,
                      0x39C0, 0x39C2, 0x39D0, 0x39D2, 0x39D6, 0x39D8, 0x39DC, 0x39E4, 0x39E6, 0x39E8, 0x39EC,
                      0x39EE, 0x39F2, 0x39FA, 0x39FC, 0x39FE, 0x3A02, 0x3A04, 0x3A08, 0x3A10, 0x3A12, 0x3A14,
                      0x3A18, 0x3A1A, 0x3A1E, 0x3A26, 0x3A28, 0x3A2A, 0x3A2E, 0x3A30, 0x3A34, 0x3A3C, 0x3A3E,
                      0x3A40, 0x3A44, 0x3A46, 0x3A4A, 0x3A52, 0x3A54, 0x3A56, 0x3A5A, 0x3A5C, 0x3A60, 0x3A68,
                      0x3A6A, 0x3A6C, 0x3A70, 0x3A72, 0x3A76, 0x3A80, 0x3A82, 0x3A86, 0x3A88, 0x3A8C, 0x3A94,
                      0x3A96, 0x3A98, 0x3A9C, 0x3A9E, 0x3AA2, 0x3AAA, 0x3AAC, 0x3AAE, 0x3AB2, 0x3AB4, 0x3AB8,
                      0x3AC0, 0x3AC2, 0x3AC4, 0x3ACA, 0x3ACE, 0x3AD6, 0x3AD8, 0x3ADA, 0x3ADE, 0x3AE0, 0x3AE4,
                      0x3AEE, 0x3AF0, 0x3AF4, 0x3AF6, 0x3AFA, 0x3B06, 0x3B0A, 0x3B0C, 0x3B10, 0x3B18, 0x3B1A,
                      0x3B1C, 0x3B20, 0x3B22, 0x3B26, 0x3B2E, 0x3B30, 0x3B32, 0x3B36, 0x3B38, 0x3B52, 0x3B5A,
                      0x3B5C, 0x3B5E, 0x3B62, 0x3B64, 0x3B68, 0x3B70, 0x3B74, 0x3B78, 0x3B7A, 0x3B7E, 0x3B88,
                      0x3B8A, 0x3B8E, 0x3B90, 0x3B94, 0x3B9C, 0x3B9E, 0x3BA0, 0x3BA4, 0x3BA6, 0x3BAA, 0x3BB2,
                      0x3BB4, 0x3BB6, 0x3BCA, 0x3BCC, 0x3BD6, 0x3BDE, 0x3BE0, 0x3BE2, 0x3BF6, 0x3BF8, 0x3C0C,
                      0x3C0E, 0x3C22, 0x3C24, 0x3C2E, 0x3C36, 0x3C38, 0x3C3A},
             .type = MUSIC_EXPLORING,
             .forbidden_music{},
         },
         pe2_music_entry_t{
             .offsets{0x3902, 0x3904, 0x3906, 0x3918, 0x391A, 0x391C, 0x392E, 0x3930, 0x3932, 0x395A, 0x395C,
                      0x395E, 0x3970, 0x3972, 0x3974, 0x3986, 0x3988, 0x398A, 0x39B2, 0x39B4, 0x39B6, 0x39DE,
                      0x39E0, 0x39E2, 0x39F4, 0x39F6, 0x39F8, 0x3A0A, 0x3A0C, 0x3A0E, 0x3A20, 0x3A22, 0x3A24,
                      0x3A36, 0x3A38, 0x3A3A, 0x3A4C, 0x3A4E, 0x3A50, 0x3A62, 0x3A64, 0x3A66, 0x3A78, 0x3A7A,
                      0x3A7C, 0x3A8E, 0x3A90, 0x3A92, 0x3AA4, 0x3AA6, 0x3AA8, 0x3ABA, 0x3ABC, 0x3ABE, 0x3AD0,
                      0x3AD2, 0x3AD4, 0x3AE6, 0x3AE8, 0x3AEA, 0x3AFC, 0x3AFE, 0x3B12, 0x3B14, 0x3B16, 0x3B28,
                      0x3B2A, 0x3B2C, 0x3B54, 0x3B56, 0x3B58, 0x3B6A, 0x3B6C, 0x3B6E, 0x3B80, 0x3B82, 0x3B84,
                      0x3B98, 0x3B9A, 0x3BAC, 0x3BAE, 0x3BB0, 0x3BD8, 0x3BDA, 0x3BDC, 0x3C30, 0x3C32, 0x3C34},
             .type = MUSIC_EXPLORING,
             .forbidden_music{},
         },
         pe2_music_entry_t{
             .offsets{0x390E, 0x3922, 0x3924, 0x393A, 0x3966, 0x397C, 0x3992, 0x39BE, 0x39EA, 0x3A00, 0x3A16,
                      0x3A2C, 0x3A42, 0x3A58, 0x3A6E, 0x3A84, 0x3A9A, 0x3AB0, 0x3AC6, 0x3ADC, 0x3AF2, 0x3B1E,
                      0x3B34, 0x3B4A, 0x3B60, 0x3B76, 0x3B8C, 0x3BA2, 0x3BB8, 0x3BE4, 0x3C3C},
             .type = MUSIC_PEACEFUL,
             .forbidden_music{},
         },
         pe2_music_entry_t{
             .offsets{0x3920, 0x3AEC, 0x3B00, 0x3B02},
             .type = MUSIC_EXPLORING,
             .forbidden_music{},
         },
         pe2_music_entry_t{
             .offsets{0x3A7E},
             .type = MUSIC_EXPLORING,
             .forbidden_music{},
         },
         pe2_music_entry_t{
             .offsets{0x3B04, 0x3B08},
             .type = MUSIC_EXPLORING,
             .forbidden_music{},
         },
         pe2_music_entry_t{
             .offsets{0x3B3C, 0x3B3E, 0x3B40, 0x3B42, 0x3B44, 0x3B46, 0x3B48, 0x3B4C, 0x3B4E},
             .type = MUSIC_EXPLORING,
             .forbidden_music{},
         },
         pe2_music_entry_t{
             .offsets{0x3B72},
             .type = MUSIC_FIGHT,
             .forbidden_music{},
         },
         pe2_music_entry_t{
             .offsets{0x3BBA, 0x3BBC, 0x3BE6, 0x3BE8, 0x3C3E, 0x3C40},
             .type = MUSIC_EXPLORING,
             .forbidden_music{},
         },
         pe2_music_entry_t{
             .offsets{0x3C44, 0x3C48},
             .type = MUSIC_FIGHT,
             .forbidden_music{},
         },
         pe2_music_entry_t{
             .offsets{0x3C46},
             .type = MUSIC_FIGHT,
             .forbidden_music{},
         },
         pe2_music_entry_t{
             .offsets{0x3C56},
             .type = MUSIC_FIGHT,
             .forbidden_music{},
         },
         pe2_music_entry_t{
             .offsets{0x3C58},
             .type = MUSIC_FIGHT,
             .forbidden_music{},
         },
         pe2_music_entry_t{
             .offsets{0x3C58},
             .type = MUSIC_FIGHT,
             .forbidden_music{},
         },
     }}},
    {MINES_AND_SHELTER_LEVEL,
     std::vector<pe2_music_entry_t>{{
         pe2_music_entry_t{
             .offsets{0x24F2, 0x298C},
             .type = MUSIC_FIGHT,
             .forbidden_music{},
         },
         pe2_music_entry_t{
             .offsets{0x24F4, 0x24F6, 0x24FA, 0x24FC, 0x24FE, 0x250E, 0x2512, 0x2514, 0x2516, 0x2522,
                      0x2524, 0x2526, 0x252A, 0x252C, 0x252E, 0x253C, 0x253E, 0x2542, 0x2544, 0x2546,
                      0x2554, 0x2556, 0x255A, 0x255C, 0x255E, 0x256A, 0x256C, 0x256E, 0x2570, 0x2572,
                      0x2574, 0x2576, 0x2584, 0x2586, 0x258A, 0x258C, 0x258E},
             .type = MUSIC_EXPLORING,
             .forbidden_music{},
         },
         pe2_music_entry_t{
             .offsets{0x2500, 0x2518, 0x2530, 0x2548, 0x2560, 0x2578, 0x2590, 0x25A8, 0x25C0, 0x25D8,
                      0x25F0, 0x2608, 0x2620, 0x2638, 0x2650, 0x2668, 0x2680, 0x2698, 0x26B0, 0x26E0,
                      0x26F8, 0x2710, 0x2728, 0x2740, 0x2758, 0x2770, 0x2788, 0x27A0, 0x27B8, 0x27D0,
                      0x27E8, 0x2800, 0x2818, 0x2830, 0x2848, 0x2860, 0x2878, 0x2890, 0x28A8, 0x28C0,
                      0x28D8, 0x28F0, 0x2908, 0x2920, 0x2938, 0x2950, 0x299A},
             .type = MUSIC_INTENSE,
             .forbidden_music{},
         },
         pe2_music_entry_t{
             .offsets{0x2502, 0x251A, 0x2532, 0x254A, 0x2562, 0x257A, 0x2592, 0x25AA, 0x25C2,
                      0x25DA, 0x25F2, 0x260A, 0x2622, 0x263A, 0x2652, 0x266A, 0x2682, 0x269A,
                      0x26B2, 0x26CA, 0x26E2, 0x26FA, 0x272A, 0x2742, 0x275A, 0x2772, 0x278A,
                      0x27A2, 0x27BA, 0x27D2, 0x27EA, 0x2802, 0x281A, 0x2832, 0x284A, 0x2862,
                      0x287A, 0x2892, 0x28AA, 0x28C2, 0x28DA, 0x28F2, 0x290A, 0x2922, 0x293A},
             .type = MUSIC_EXPLORING,
             .forbidden_music{},
         },
         pe2_music_entry_t{
             .offsets{0x2504, 0x251C, 0x2534, 0x254C, 0x2564, 0x257C, 0x2594, 0x25AC, 0x25C4, 0x25DC,
                      0x25F4, 0x260C, 0x2624, 0x263C, 0x2654, 0x266C, 0x2684, 0x2686, 0x269C, 0x26B4,
                      0x26CC, 0x26E4, 0x2714, 0x272C, 0x2744, 0x275C, 0x2774, 0x278C, 0x27A4, 0x27BC,
                      0x27D4, 0x27EC, 0x2804, 0x281C, 0x2834, 0x2864, 0x287C, 0x2894, 0x28AC, 0x28C4,
                      0x28DC, 0x28F4, 0x290C, 0x2924, 0x293C, 0x2954, 0x2998},
             .type = MUSIC_FIGHT,
             .forbidden_music{},
         },
         pe2_music_entry_t{
             .offsets{0x250C},
             .type = MUSIC_INTENSE,
             .forbidden_music{},
         },
         pe2_music_entry_t{
             .offsets{0x259E, 0x25A0, 0x25A2, 0x25A4, 0x25A6},
             .type = MUSIC_EXPLORING,
             .forbidden_music{},
         },
         pe2_music_entry_t{
             .offsets{0x25B6, 0x25CE, 0x25E6, 0x25FE, 0x2616, 0x262E, 0x2646, 0x265E, 0x2676, 0x2678, 0x268E,
                      0x2690, 0x26A6, 0x26A8, 0x26BE, 0x26C0, 0x26D6, 0x26D8, 0x26EE, 0x26F0, 0x2706, 0x2708,
                      0x271E, 0x2720, 0x2736, 0x2738, 0x274E, 0x2750, 0x2766, 0x2768, 0x277E, 0x2780, 0x2796,
                      0x2798, 0x27AE, 0x27B0, 0x27C6, 0x27C8, 0x27F6, 0x27F8, 0x280E, 0x2810, 0x2826, 0x2828},
             .type = MUSIC_EXPLORING,
             .forbidden_music{},
         },
         pe2_music_entry_t{
             .offsets{0x25BA, 0x25D2, 0x25EA, 0x2602, 0x261A, 0x2632, 0x264A, 0x2662, 0x267A,
                      0x2692, 0x26AA, 0x26C2, 0x26DA, 0x26F2, 0x270A, 0x2722, 0x273A, 0x2752,
                      0x276A, 0x2782, 0x279A, 0x27B2, 0x27CA, 0x27FA, 0x2812, 0x282A},
             .type = MUSIC_EXPLORING,
             .forbidden_music{},
         },
         pe2_music_entry_t{
             .offsets{0x25BC, 0x25D4, 0x25EC, 0x2604, 0x261C, 0x2634, 0x264C, 0x2664, 0x267C,
                      0x2694, 0x26AC, 0x26C4, 0x26DC, 0x26F4, 0x270C, 0x2724, 0x273C, 0x2754,
                      0x276C, 0x2784, 0x279C, 0x27B4, 0x27CC, 0x27E4, 0x27FC, 0x2814, 0x282C},
             .type = MUSIC_INTENSE,
             .forbidden_music{},
         },
         pe2_music_entry_t{
             .offsets{0x25BE, 0x25D6, 0x25EE, 0x2606, 0x261E, 0x2636, 0x264E, 0x2666, 0x267E,
                      0x2696, 0x26AE, 0x26C6, 0x26DE, 0x26F6, 0x270E, 0x2726, 0x273E, 0x2756,
                      0x276E, 0x2786, 0x279E, 0x27B6, 0x27CE, 0x27FE, 0x2816, 0x282E},
             .type = MUSIC_EXPLORING,
             .forbidden_music{},
         },
         pe2_music_entry_t{
             .offsets{0x2660},
             .type = MUSIC_FIGHT,
             .forbidden_music{},
         },
         pe2_music_entry_t{
             .offsets{0x26FC, 0x2994},
             .type = MUSIC_FIGHT,
             .forbidden_music{},
         },
         pe2_music_entry_t{
             .offsets{0x2712},
             .type = MUSIC_INTENSE,
             .forbidden_music{},
         },
         pe2_music_entry_t{
             .offsets{0x27DE, 0x27E0, 0x27E2, 0x27E6},
             .type = MUSIC_EXPLORING,
             .forbidden_music{},
         },
         pe2_music_entry_t{
             .offsets{0x284C},
             .type = MUSIC_FIGHT,
             .forbidden_music{},
         },
         pe2_music_entry_t{
             .offsets{0x284E},
             .type = MUSIC_PEACEFUL,
             .forbidden_music{},
         },
         pe2_music_entry_t{
             .offsets{0x2888, 0x298E},
             .type = MUSIC_FIGHT,
             .forbidden_music{},
         },
         pe2_music_entry_t{
             .offsets{0x28A0, 0x2990},
             .type = MUSIC_FIGHT,
             .forbidden_music{},
         },
         pe2_music_entry_t{
             .offsets{0x28BA},
             .type = MUSIC_PEACEFUL,
             .forbidden_music{},
         },
         pe2_music_entry_t{
             .offsets{0x28BC, 0x28BE, 0x28E6, 0x28EA, 0x28EC, 0x28EE, 0x28FE, 0x2902, 0x2904, 0x2906, 0x2916,
                      0x291A, 0x291C, 0x291E},
             .type = MUSIC_EXPLORING,
             .forbidden_music{},
         },
         pe2_music_entry_t{
             .offsets{0x292E, 0x2932, 0x2934, 0x2936},
             .type = MUSIC_EXPLORING,
             .forbidden_music{},
         },
         pe2_music_entry_t{
             .offsets{0x2946, 0x294A, 0x294C},
             .type = MUSIC_EXPLORING,
             .forbidden_music{},
         },
         pe2_music_entry_t{
             .offsets{0x296C, 0x2992},
             .type = MUSIC_FIGHT,
             .forbidden_music{},
         },
         pe2_music_entry_t{
             .offsets{0x297E},
             .type = MUSIC_EXPLORING,
             .forbidden_music{},
         },
         pe2_music_entry_t{
             .offsets{0x2988},
             .type = MUSIC_FIGHT,
             .forbidden_music{},
         },
         pe2_music_entry_t{
             .offsets{0x298A},
             .type = MUSIC_FIGHT,
             .forbidden_music{},
         },
         pe2_music_entry_t{
             .offsets{0x299C},
             .type = MUSIC_FIGHT,
             .forbidden_music{},
         },
         pe2_music_entry_t{
             .offsets{0x299E},
             .type = MUSIC_FIGHT,
             .forbidden_music{},
         },
     }}},
    {NEOARK_AND_SHELTER_LEVEL,
     std::vector<pe2_music_entry_t>{{
         pe2_music_entry_t{
             .offsets{0x321A, 0x322E, 0x3242, 0x3256, 0x326A, 0x327E, 0x3292, 0x32CE,
                      0x32E2, 0x32F6, 0x330A, 0x331E, 0x3332, 0x3346, 0x335A, 0x336E,
                      0x3382, 0x3396, 0x33AA, 0x3422, 0x344A, 0x3486, 0x34BC},
             .type = MUSIC_FIGHT,
             .forbidden_music{},
         },
         pe2_music_entry_t{
             .offsets{0x322A, 0x323E, 0x3252, 0x327A, 0x328E, 0x32A2, 0x32DE, 0x32F2,
                      0x3306, 0x331A, 0x332E, 0x3342, 0x3356, 0x336A, 0x337E, 0x3392,
                      0x33A6, 0x33BA, 0x3432, 0x345A, 0x346E, 0x3496, 0x34AA},
             .type = MUSIC_EXPLORING,
             .forbidden_music{},
         },
         pe2_music_entry_t{
             .offsets{0x3290, 0x3294, 0x3296, 0x3298, 0x329A, 0x329C, 0x32D0, 0x32D2, 0x32D4, 0x32D6, 0x32D8,
                      0x3384, 0x3386, 0x3388, 0x338A, 0x338C},
             .type = MUSIC_EXPLORING,
             .forbidden_music{},
         },
         pe2_music_entry_t{
             .offsets{0x329E, 0x32DA, 0x32EE, 0x3300, 0x3302, 0x3316, 0x332A, 0x333E, 0x3352, 0x3366,
                      0x337A, 0x338E, 0x33A2, 0x33B6, 0x342E, 0x3456, 0x346A, 0x3492, 0x34A6, 0x34BE},
             .type = MUSIC_INTENSE,
             .forbidden_music{},
         },
         pe2_music_entry_t{
             .offsets{0x32A0, 0x32DC, 0x32F0, 0x3304, 0x3314, 0x3318, 0x332C, 0x333C, 0x3340, 0x3354,
                      0x3368, 0x337C, 0x3390, 0x33A4, 0x33B8, 0x342C, 0x3430, 0x3458, 0x3494, 0x34A8},
             .type = MUSIC_EXPLORING,
             .forbidden_music{},
         },
         pe2_music_entry_t{
             .offsets{0x32A4, 0x32A8, 0x32AA, 0x32AC, 0x33E8, 0x33EA, 0x33EC, 0x33EE, 0x34B0},
             .type = MUSIC_EXPLORING,
             .forbidden_music{
                 0x4f // for some reason if this track is playing during the returning from Eve Access Tunnel
                      // or Neo Ark (after defeating Brain Stinger Boss) will make the game freeze on loading
                      // upper level shelter map file. To be investigated what is going on here.
             },
         },
         pe2_music_entry_t{
             .offsets{0x32E4, 0x32E6, 0x32E8, 0x32EA, 0x32EC, 0x32F8, 0x32FA, 0x32FC, 0x32FE, 0x330C, 0x330E,
                      0x3310, 0x3312, 0x3320, 0x3322, 0x3324, 0x3326, 0x3328, 0x3334, 0x3336, 0x3338, 0x333A,
                      0x3370, 0x3372, 0x3374, 0x3376, 0x3378, 0x33AC, 0x33AE, 0x33B0, 0x33B2, 0x33B4, 0x3424,
                      0x3426, 0x3428, 0x342A, 0x344C, 0x344E, 0x3450, 0x3452, 0x3454},
             .type = MUSIC_EXPLORING,
             .forbidden_music{},
         },
         pe2_music_entry_t{
             .offsets{0x3348, 0x334A, 0x335C, 0x349C, 0x349E, 0x34AE},
             .type = MUSIC_EXPLORING,
             .forbidden_music{},
         },
         pe2_music_entry_t{
             .offsets{0x3398, 0x339A, 0x339C, 0x339E, 0x33A0, 0x3488, 0x348A, 0x348C, 0x348E, 0x3490},
             .type = MUSIC_EXPLORING,
             .forbidden_music{},
         },
         pe2_music_entry_t{
             .offsets{0x33C6},
             .type = MUSIC_PEACEFUL,
             .forbidden_music{},
         },
         pe2_music_entry_t{
             .offsets{0x33C6},
             .type = MUSIC_FIGHT,
             .forbidden_music{},
         },
         pe2_music_entry_t{
             .offsets{0x341C},
             .type = MUSIC_FIGHT,
             .forbidden_music{},
         },
         pe2_music_entry_t{
             .offsets{0x3444},
             .type = MUSIC_EXPLORING,
             .forbidden_music{},
         },
         pe2_music_entry_t{
             .offsets{0x34AC},
             .type = MUSIC_FIGHT,
             .forbidden_music{},
         },
         pe2_music_entry_t{
             .offsets{0x34C0},
             .type = MUSIC_FIGHT,
             .forbidden_music{},
         },
         pe2_music_entry_t{
             .offsets{0x34C2},
             .type = MUSIC_FIGHT,
             .forbidden_music{},
         },
     }}}};

/*
    MUSIC TYPES MAP VALUES
*/

std::map<pe2_music_type_t, std::vector<uint8_t>> music_types_map{
    {MUSIC_PEACEFUL, std::vector<uint8_t>{0x11, 0x05, 0x09, 0x1f, 0x25, 0x2c, 0x3a, 0x50, 0x55, 0x61, 0x62}},
    {MUSIC_EXPLORING,
     std::vector<uint8_t>{0x23, 0x24, 0x29, 0x2a, 0x2b, 0x2d, 0x2e, 0x2f, 0x37, 0x39, 0x3b, 0x3d,
                          0x3f, 0x41, 0x47, 0x48, 0x49, 0x4b, 0x4c, 0x4d, 0x4e, 0x51, 0x53, 0x59}},
    {MUSIC_INTENSE, std::vector<uint8_t>{0x20, 0x21, 0x22, 0x0c, 0x18, 0x26, 0x3e, 0x52, 0x5a}},
    {MUSIC_FIGHT,
     std::vector<uint8_t>{0x06, 0x14, 0x15, 0x16, 0x17, 0x1d, 0x1e, 0x30, 0x31, 0x32, 0x38, 0x3c,
                          0x42, 0x43, 0x44, 0x45, 0x46, 0x4a, 0x52, 0x56, 0x5d, 0x5e, 0x5f, 0x60}},
    {MUSIC_MISC,
     std::vector<uint8_t>{0x05, 0x06, 0x09, 0x0c, 0x11, 0x17, 0x1d, 0x1e, 0x1f, 0x20, 0x21, 0x22, 0x23,
                          0x24, 0x25, 0x26, 0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f, 0x30, 0x31, 0x32,
                          0x37, 0x39, 0x3a, 0x3b, 0x3c, 0x3d, 0x3e, 0x3f, 0x41, 0x42, 0x43, 0x44, 0x45,
                          0x46, 0x47, 0x48, 0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x51, 0x52, 0x53, 0x56,
                          0x59, 0x5a, 0x5d, 0x5e, 0x5f, 0x60, 0x61, 0x62, 0x80}}};
} // namespace domain::music
