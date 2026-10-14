#include "domain/enhancer/enemy/Enemy.hpp"

std::map<uint8_t, domain::pe2_enemy_enhancement_t> domain::boss_enhancements{
    {{STRANGER_BOSS_ID,
      {
          .filename = "enemies/0a_greater_sranger_boss",
          .command_to_change{
              // 1. Make boss ignore tables collision
              {0x1fc8, 0x00000000},
              {0x1fe0, 0x00000000},
              // 2. Stranger starts as a stronger version
              {0x22bc, 0x24030018},
              // 3. Stranger has a bigger range on bite attack
              {0x2f58, 0x24020340},
              // 4. TODO: stranger doesn't activate second phase
              // 5. Stranger is a bit bigger
              {0x310c, 0x24021d00},
              // 6. Stranger attacks faster
              {0x3dec, 0x24020020},
              {0x3e04, 0x24020020},
              // 7. Increase health
              {0x70f8, 0x012c0320},
          },
          // TODO: new texture???

          .hp_offset = 0x70f8,
          .damage_offsets{
              0x70e4, // left or right hit stage 1 (???)
              0x70e8, // left or right hit stage 2 (???)
              0x70ec, // right or left hit stage 1 (???)
              0x70f0, // right or left hit stage 2 (???)
          },
      }},
     {AKROPOLIS_NO9_ID,
      {
          .filename = "enemies/1b_akropolis_no9_boss",
          .command_to_change{
              // 1. Walks faster and pushes Aya a bit
              {0x4574, 0x24030040},
              {0x457c, 0x2442ffd0},
              {0x463c, 0x24020040},
              {0x454c, 0x2442ffd0},
              // 2. Makes no9 advance further when slashing
              {0x4dec, 0x2c42001a},
              {0x4df8, 0x24030054},
              {0x4f90, 0x2c42001a},
              {0x4f9c, 0x24030054},
              // 3. Makes hop strike 2 times further
              {0x53cc, 0x00071043},
              // 4. Different taunt on successful hit
              {0x5030, 0x2402001a},
              // 5. Stronger charge attack
              {0x35b40, 0x0007001e},
              // 6. Greande deals damage,
              {0x35b58, 0x000200f},
          },

          .hp_offset = 0x35b64,
          .damage_offsets =
              {
                  0x35b48, // charge attack
                  0x36b4c, // hop strike
                  0x35b50, // sword slash/chop
                  0x35b54, // fatal blow
                  0x35b58, // grenade
                  0x35b5c, // touch flamed sword
              },
      }},
     {CELLAR_STALKER_ID,
      {
          .filename = "enemies/05_cellar_stalker_boss",
          .command_to_change{
              // 1. 4x getup time after knockdown with blind
              {0xa600, 0x24020040},
              {0xa604, 0x8c85001c},
              {0xa60c, 0xa4a209f8},
              {0xa610, 0xa4a309fe},
              {0xa61c, 0xa4a309fa},
              // 2. 2x getup time after knockdown from ceiling
              {0xa8d8, 0x24020020},
              // 3. 4x getup time after knockdown with paralysis
              {0xa600, 0x24020040},
              // 4. 4x getup time after knockdown with paralysis
              {0xb71c, 0x24020040},
              {0xb720, 0x8e05001c},
              {0xb728, 0xa4a209f8},
              {0xb72c, 0xa4a309fe},
              {0xb738, 0xa4a309fa},
              // 5. paralysis doesn't make stalker visible
              {0x87e4, 0x00000000},
              {0x87e8, 0x00000000},
              {0x87ec, 0x00000000},
              // 6. blind doesn't make stalker visible
              {0x73bc, 0x00000000},
              {0x73c0, 0x00000000},
              {0x73c4, 0x00000000},
              // 7. ceiling swinging attack 1.5x faster
              {0xa140, 0x24020018},
              {0x69f0, 0x24030009},
              // 8. ceiling swinging attack doesn't make stalker visible
              {0xa064, 0x00000000},
              {0xa068, 0x00000000},
              // 9. floor swinging attack doesn't make stalker visible
              {0xa1b8, 0x00000000},
              {0xa1bc, 0x00000000},
              // 10. floor swinging attack 1.5x faster
              {0xa298, 0x24020018},
              {0x6d80, 0x24030009},
          },

          .hp_offset = 0x21e74,
          .damage_offsets =
              {
                  0x21e64, // swiping attack
                  0x21e68, // smashing attack
                  0x21e6c, // ???
              },
      }},
     {DRYFIELD_NO9_ID,
      {
          .filename = "enemies/22_dryfield_no9_boss",
          .command_to_change{
              // 1. no magic vulnerability after slashing attack
              {0x13d0, 0x00000000},
              {0x13d4, 0x00000000},
              // 2. starts combo after slashing attack
              {0x1498, 0x24420048},
              {0x14b0, 0x24080001},
              {0x14bc, 0xa6480686},
              {0x14c8, 0xa648069e},
              // 3. no magic vulnerability after chopping attack
              {0x1638, 0x00000000},
              {0x163c, 0x00000000},
              // 4. faster recovery for chopping attacks
              {0x1700, 0x2442003c},
              // 5. stabbing attack longer reach and better recovery
              {0x1a44, 0x24020120},
              {0x1aa0, 0x28420038},
          },

          .hp_offset = 0x2d714,
          .damage_offsets =
              {
                  0x2d730, // slashing attack
                  0x2d734, // chopping attack
                  0x2d738, // stabbing attack
                  0x2d73c, // grab
              },
      }}}};