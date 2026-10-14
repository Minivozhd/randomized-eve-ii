#include "ui/context/UIContext.hpp"

const std::string UIContext::_BASE64_CHARS =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

UIContext::UIContext(uint32_t _seed, std::shared_ptr<PE2Logger> _logger)
    : seed(_seed),
      flags(_UI_CONTEXT_FLAG__RANDOMIZE_ENEMIES | _UI_CONTEXT_FLAG__RANDOMIZE_ITEMS |
            _UI_CONTEXT_FLAG__RANDOMIZE_MAGIC | _UI_CONTEXT_FLAG__RANDOMIZE_MUSIC |
            _UI_CONTEXT_FLAG__AMMO_BOXES__DEFAULT | _UI_CONTEXT_FLAG__AMMO_BOXES_RANDOMIZATION__UNRANDOMIZED |
            _UI_CONTEXT_FLAG__ENEMY_ENHANCEMENT__NO_ENHANCEMENT | _UI_CONTEXT_FLAG__ENABLE_ALL_MODES),
      enemies_pool(_UI_CONTEXT_ENEMIES_POOL__CHASER | _UI_CONTEXT_ENEMIES_POOL__BRAIN_STINGER |
                   _UI_CONTEXT_ENEMIES_POOL__DIVER | _UI_CONTEXT_ENEMIES_POOL__STALKER |
                   _UI_CONTEXT_ENEMIES_POOL__RAT | _UI_CONTEXT_ENEMIES_POOL__BUTTERFLY |
                   _UI_CONTEXT_ENEMIES_POOL__LESSER_STRANGER | _UI_CONTEXT_ENEMIES_POOL__BRUTE |
                   _UI_CONTEXT_ENEMIES_POOL__SUCKLER | _UI_CONTEXT_ENEMIES_POOL__HORNED_STRANGER |
                   _UI_CONTEXT_ENEMIES_POOL__MIND_SUCKLER | _UI_CONTEXT_ENEMIES_POOL__SCAVANGER |
                   _UI_CONTEXT_ENEMIES_POOL__ODD_STRANGER | _UI_CONTEXT_ENEMIES_POOL__GRINNING_STRANGER |
                   _UI_CONTEXT_ENEMIES_POOL__PAWN_BLADE_GOLEM | _UI_CONTEXT_ENEMIES_POOL__TURRET |
                   _UI_CONTEXT_ENEMIES_POOL__KNIGHT_GOLEM | _UI_CONTEXT_ENEMIES_POOL__ROOK_BLADE_GOLEM |
                   _UI_CONTEXT_ENEMIES_POOL__AMOEBA | _UI_CONTEXT_ENEMIES_POOL__SCORPION |
                   _UI_CONTEXT_ENEMIES_POOL__WHITE_WORM | _UI_CONTEXT_ENEMIES_POOL__BAT |
                   _UI_CONTEXT_ENEMIES_POOL__BEETLE | _UI_CONTEXT_ENEMIES_POOL__BISHOP_GOLEM |
                   _UI_CONTEXT_ENEMIES_POOL__BLOOD_SUCKLER | _UI_CONTEXT_ENEMIES_POOL__FAILED_CHASER |
                   _UI_CONTEXT_ENEMIES_POOL__FAILED_SUCKLER | _UI_CONTEXT_ENEMIES_POOL__MOSSBACK_BRUTE |
                   _UI_CONTEXT_ENEMIES_POOL__GREEN_WORM | _UI_CONTEXT_ENEMIES_POOL__PAWN_GUN_GOLEM |
                   _UI_CONTEXT_ENEMIES_POOL__ROOK_GUN_GOLEM | _UI_CONTEXT_ENEMIES_POOL__FAILED_BRUTE |
                   _UI_CONTEXT_ENEMIES_POOL__FAILED_STRANGER),

      enemies_mults{
          100, // Enemy health multiplier
          100, // Enemy damage multiplier
          100, // Boss health multiplier
          100, // Boss damage multiplier
      },

      items_drop_pool{
          _UI_CONTEXT_ITEMS_POOL_LOW_INT__CONSUMABLE__RECOVERY1 |
              _UI_CONTEXT_ITEMS_POOL_LOW_INT__CONSUMABLE__RECOVERY2 |
              _UI_CONTEXT_ITEMS_POOL_LOW_INT__CONSUMABLE__RECOVERY3 |
              _UI_CONTEXT_ITEMS_POOL_LOW_INT__CONSUMABLE__STIM |
              _UI_CONTEXT_ITEMS_POOL_LOW_INT__CONSUMABLE__COLA |
              _UI_CONTEXT_ITEMS_POOL_LOW_INT__CONSUMABLE__MP_BOOST1 |
              _UI_CONTEXT_ITEMS_POOL_LOW_INT__CONSUMABLE__MP_BOOST2 |
              _UI_CONTEXT_ITEMS_POOL_LOW_INT__CONSUMABLE__PENICILLIN |
              _UI_CONTEXT_ITEMS_POOL_LOW_INT__CONSUMABLE__LIPSTICK |
              _UI_CONTEXT_ITEMS_POOL_LOW_INT__CONSUMABLE__BELT_POUCH |
              _UI_CONTEXT_ITEMS_POOL_LOW_INT__CONSUMABLE__FLARE |
              _UI_CONTEXT_ITEMS_POOL_LOW_INT__CONSUMABLE__PEPPER_SPRAY |
              _UI_CONTEXT_ITEMS_POOL_LOW_INT__CONSUMABLE__PROTEINE_CAPSULE |
              _UI_CONTEXT_ITEMS_POOL_LOW_INT__CONSUMABLE__RINGERS_SOLUTION |
              _UI_CONTEXT_ITEMS_POOL_LOW_INT__CONSUMABLE__EAU_DE_TOILETTE |
              _UI_CONTEXT_ITEMS_POOL_LOW_INT__CONSUMABLE__COMBAT_LIGHT |
              _UI_CONTEXT_ITEMS_POOL_LOW_INT__ATTACHMENT__SMG_CLIP_HOLDER |
              _UI_CONTEXT_ITEMS_POOL_LOW_INT__ATTACHMENT__RIFLE_CLIP_HOLDER |
              _UI_CONTEXT_ITEMS_POOL_LOW_INT__ATTACHMENT__SNAIL_MAGAZINE |
              _UI_CONTEXT_ITEMS_POOL_LOW_INT__ATTACHMENT__HAMMER_M4A1 |
              _UI_CONTEXT_ITEMS_POOL_LOW_INT__ATTACHMENT__PYKE_M4A1 |
              _UI_CONTEXT_ITEMS_POOL_LOW_INT__ATTACHMENT__JAVELIN_M4A1 |
              _UI_CONTEXT_ITEMS_POOL_LOW_INT__ATTACHMENT__M203_M4A1 |
              _UI_CONTEXT_ITEMS_POOL_LOW_INT__ATTACHMENT__M9_M4A1 |
              _UI_CONTEXT_ITEMS_POOL_LOW_INT__ARMOR__LEATHER_JACKET |
              _UI_CONTEXT_ITEMS_POOL_LOW_INT__ARMOR__TACTICAL_ARMOR |
              _UI_CONTEXT_ITEMS_POOL_LOW_INT__ARMOR__COMBAT_ARMOR |
              _UI_CONTEXT_ITEMS_POOL_LOW_INT__ARMOR__ASSAULT_SUIT |
              _UI_CONTEXT_ITEMS_POOL_LOW_INT__ARMOR__PASGT_VEST |
              _UI_CONTEXT_ITEMS_POOL_LOW_INT__ARMOR__TACTICAL_VEST |
              _UI_CONTEXT_ITEMS_POOL_LOW_INT__ARMOR__EOD_SUIT |
              _UI_CONTEXT_ITEMS_POOL_LOW_INT__ARMOR__TURTLE_VEST |
              _UI_CONTEXT_ITEMS_POOL_LOW_INT__ARMOR__CHICKEN_PLATE |
              _UI_CONTEXT_ITEMS_POOL_LOW_INT__ARMOR__NBC_SUIT |
              _UI_CONTEXT_ITEMS_POOL_LOW_INT__ARMOR__PSYSUIT |
              _UI_CONTEXT_ITEMS_POOL_LOW_INT__ARMOR__AYA_SPECIAL |
              _UI_CONTEXT_ITEMS_POOL_LOW_INT__ARMOR__SHOULDER_HOLSTER |
              _UI_CONTEXT_ITEMS_POOL_LOW_INT__WEAPON__M93R | _UI_CONTEXT_ITEMS_POOL_LOW_INT__WEAPON__M950 |
              _UI_CONTEXT_ITEMS_POOL_LOW_INT__WEAPON__P08 | _UI_CONTEXT_ITEMS_POOL_LOW_INT__WEAPON__P229 |
              _UI_CONTEXT_ITEMS_POOL_LOW_INT__WEAPON__MONGOOSE |
              _UI_CONTEXT_ITEMS_POOL_LOW_INT__WEAPON__GRENADE_PISTOL |
              _UI_CONTEXT_ITEMS_POOL_LOW_INT__WEAPON__MM1 | _UI_CONTEXT_ITEMS_POOL_LOW_INT__WEAPON__PA3 |
              _UI_CONTEXT_ITEMS_POOL_LOW_INT__WEAPON__SP12 | _UI_CONTEXT_ITEMS_POOL_LOW_INT__WEAPON__AS12 |
              _UI_CONTEXT_ITEMS_POOL_LOW_INT__WEAPON__M4A1 | _UI_CONTEXT_ITEMS_POOL_LOW_INT__WEAPON__MP5A5 |
              _UI_CONTEXT_ITEMS_POOL_LOW_INT__AMMO__PISTOL_HYDRA |
              _UI_CONTEXT_ITEMS_POOL_LOW_INT__AMMO__PISTOL_SPARTAN |
              _UI_CONTEXT_ITEMS_POOL_LOW_INT__AMMO__MAGNUM |
              _UI_CONTEXT_ITEMS_POOL_LOW_INT__AMMO__MAGNUM_MAEDA |
              _UI_CONTEXT_ITEMS_POOL_LOW_INT__AMMO__GRENADES_RIOT |
              _UI_CONTEXT_ITEMS_POOL_LOW_INT__AMMO__GRENADES |
              _UI_CONTEXT_ITEMS_POOL_LOW_INT__AMMO__GRENADES_AIRBIRST |
              _UI_CONTEXT_ITEMS_POOL_LOW_INT__AMMO__SHELLS_BUCKSHOT |
              _UI_CONTEXT_ITEMS_POOL_LOW_INT__AMMO__SHELLS_FIREFLY,

          _UI_CONTEXT_ITEMS_POOL_HIGH_INT__AMMO__SHELLS_SLUG |
              _UI_CONTEXT_ITEMS_POOL_HIGH_INT__AMMO__RIFLE_ROUNDS |
              _UI_CONTEXT_ITEMS_POOL_HIGH_INT__MISC__CRYSTAL_SKULL |
              _UI_CONTEXT_ITEMS_POOL_HIGH_INT__MISC__MEDICINE_WHEEL |
              _UI_CONTEXT_ITEMS_POOL_HIGH_INT__MISC__HOLY_WATER |
              _UI_CONTEXT_ITEMS_POOL_HIGH_INT__MISC__OFUDA_CHARM |
              _UI_CONTEXT_ITEMS_POOL_HIGH_INT__MISC__HUNTER_GOGGLES |
              _UI_CONTEXT_ITEMS_POOL_HIGH_INT__MISC__GPS | _UI_CONTEXT_ITEMS_POOL_HIGH_INT__MISC__MD_PLAYER},

      items_shop_pool{
          _UI_CONTEXT_ITEMS_POOL_LOW_INT__CONSUMABLE__RECOVERY1 |
              _UI_CONTEXT_ITEMS_POOL_LOW_INT__CONSUMABLE__RECOVERY2 |
              _UI_CONTEXT_ITEMS_POOL_LOW_INT__CONSUMABLE__RECOVERY3 |
              _UI_CONTEXT_ITEMS_POOL_LOW_INT__CONSUMABLE__STIM |
              _UI_CONTEXT_ITEMS_POOL_LOW_INT__CONSUMABLE__COLA |
              _UI_CONTEXT_ITEMS_POOL_LOW_INT__CONSUMABLE__MP_BOOST1 |
              _UI_CONTEXT_ITEMS_POOL_LOW_INT__CONSUMABLE__MP_BOOST2 |
              _UI_CONTEXT_ITEMS_POOL_LOW_INT__CONSUMABLE__PENICILLIN |
              _UI_CONTEXT_ITEMS_POOL_LOW_INT__CONSUMABLE__LIPSTICK |
              _UI_CONTEXT_ITEMS_POOL_LOW_INT__CONSUMABLE__BELT_POUCH |
              _UI_CONTEXT_ITEMS_POOL_LOW_INT__CONSUMABLE__FLARE |
              _UI_CONTEXT_ITEMS_POOL_LOW_INT__CONSUMABLE__PEPPER_SPRAY |
              _UI_CONTEXT_ITEMS_POOL_LOW_INT__CONSUMABLE__PROTEINE_CAPSULE |
              _UI_CONTEXT_ITEMS_POOL_LOW_INT__CONSUMABLE__RINGERS_SOLUTION |
              _UI_CONTEXT_ITEMS_POOL_LOW_INT__CONSUMABLE__EAU_DE_TOILETTE |
              _UI_CONTEXT_ITEMS_POOL_LOW_INT__CONSUMABLE__COMBAT_LIGHT |
              _UI_CONTEXT_ITEMS_POOL_LOW_INT__ATTACHMENT__SMG_CLIP_HOLDER |
              _UI_CONTEXT_ITEMS_POOL_LOW_INT__ATTACHMENT__RIFLE_CLIP_HOLDER |
              _UI_CONTEXT_ITEMS_POOL_LOW_INT__ATTACHMENT__SNAIL_MAGAZINE |
              _UI_CONTEXT_ITEMS_POOL_LOW_INT__ATTACHMENT__HAMMER_M4A1 |
              _UI_CONTEXT_ITEMS_POOL_LOW_INT__ATTACHMENT__PYKE_M4A1 |
              _UI_CONTEXT_ITEMS_POOL_LOW_INT__ATTACHMENT__JAVELIN_M4A1 |
              _UI_CONTEXT_ITEMS_POOL_LOW_INT__ATTACHMENT__M203_M4A1 |
              _UI_CONTEXT_ITEMS_POOL_LOW_INT__ATTACHMENT__M9_M4A1 |
              _UI_CONTEXT_ITEMS_POOL_LOW_INT__ARMOR__LEATHER_JACKET |
              _UI_CONTEXT_ITEMS_POOL_LOW_INT__ARMOR__TACTICAL_ARMOR |
              _UI_CONTEXT_ITEMS_POOL_LOW_INT__ARMOR__COMBAT_ARMOR |
              _UI_CONTEXT_ITEMS_POOL_LOW_INT__ARMOR__ASSAULT_SUIT |
              _UI_CONTEXT_ITEMS_POOL_LOW_INT__ARMOR__PASGT_VEST |
              _UI_CONTEXT_ITEMS_POOL_LOW_INT__ARMOR__TACTICAL_VEST |
              _UI_CONTEXT_ITEMS_POOL_LOW_INT__ARMOR__EOD_SUIT |
              _UI_CONTEXT_ITEMS_POOL_LOW_INT__ARMOR__TURTLE_VEST |
              _UI_CONTEXT_ITEMS_POOL_LOW_INT__ARMOR__CHICKEN_PLATE |
              _UI_CONTEXT_ITEMS_POOL_LOW_INT__ARMOR__NBC_SUIT |
              _UI_CONTEXT_ITEMS_POOL_LOW_INT__ARMOR__PSYSUIT |
              _UI_CONTEXT_ITEMS_POOL_LOW_INT__ARMOR__AYA_SPECIAL |
              _UI_CONTEXT_ITEMS_POOL_LOW_INT__ARMOR__SHOULDER_HOLSTER |
              _UI_CONTEXT_ITEMS_POOL_LOW_INT__ARMOR__MONK_ROBE |
              _UI_CONTEXT_ITEMS_POOL_LOW_INT__WEAPON__M93R | _UI_CONTEXT_ITEMS_POOL_LOW_INT__WEAPON__M950 |
              _UI_CONTEXT_ITEMS_POOL_LOW_INT__WEAPON__P08 | _UI_CONTEXT_ITEMS_POOL_LOW_INT__WEAPON__P229 |
              _UI_CONTEXT_ITEMS_POOL_LOW_INT__WEAPON__MONGOOSE |
              _UI_CONTEXT_ITEMS_POOL_LOW_INT__WEAPON__GRENADE_PISTOL |
              _UI_CONTEXT_ITEMS_POOL_LOW_INT__WEAPON__MM1 | _UI_CONTEXT_ITEMS_POOL_LOW_INT__WEAPON__PA3 |
              _UI_CONTEXT_ITEMS_POOL_LOW_INT__WEAPON__SP12 | _UI_CONTEXT_ITEMS_POOL_LOW_INT__WEAPON__AS12 |
              _UI_CONTEXT_ITEMS_POOL_LOW_INT__WEAPON__M4A1 | _UI_CONTEXT_ITEMS_POOL_LOW_INT__WEAPON__M249 |
              _UI_CONTEXT_ITEMS_POOL_LOW_INT__WEAPON__HYPERVELOCITY |
              _UI_CONTEXT_ITEMS_POOL_LOW_INT__WEAPON__GUNBLADE |
              _UI_CONTEXT_ITEMS_POOL_LOW_INT__WEAPON__MP5A5 |
              _UI_CONTEXT_ITEMS_POOL_LOW_INT__AMMO__PISTOL_PB |
              _UI_CONTEXT_ITEMS_POOL_LOW_INT__AMMO__PISTOL_HYDRA |
              _UI_CONTEXT_ITEMS_POOL_LOW_INT__AMMO__PISTOL_SPARTAN |
              _UI_CONTEXT_ITEMS_POOL_LOW_INT__AMMO__MAGNUM |
              _UI_CONTEXT_ITEMS_POOL_LOW_INT__AMMO__MAGNUM_MAEDA |
              _UI_CONTEXT_ITEMS_POOL_LOW_INT__AMMO__GRENADES_RIOT |
              _UI_CONTEXT_ITEMS_POOL_LOW_INT__AMMO__GRENADES |
              _UI_CONTEXT_ITEMS_POOL_LOW_INT__AMMO__GRENADES_AIRBIRST |
              _UI_CONTEXT_ITEMS_POOL_LOW_INT__AMMO__SHELLS_BUCKSHOT |
              _UI_CONTEXT_ITEMS_POOL_LOW_INT__AMMO__SHELLS_FIREFLY,

          _UI_CONTEXT_ITEMS_POOL_HIGH_INT__AMMO__SHELLS_SLUG |
              _UI_CONTEXT_ITEMS_POOL_HIGH_INT__AMMO__RIFLE_ROUNDS |
              _UI_CONTEXT_ITEMS_POOL_HIGH_INT__MISC__CRYSTAL_SKULL |
              _UI_CONTEXT_ITEMS_POOL_HIGH_INT__MISC__MEDICINE_WHEEL |
              _UI_CONTEXT_ITEMS_POOL_HIGH_INT__MISC__HOLY_WATER |
              _UI_CONTEXT_ITEMS_POOL_HIGH_INT__MISC__OFUDA_CHARM |
              _UI_CONTEXT_ITEMS_POOL_HIGH_INT__MISC__HUNTER_GOGGLES |
              _UI_CONTEXT_ITEMS_POOL_HIGH_INT__MISC__GPS | _UI_CONTEXT_ITEMS_POOL_HIGH_INT__MISC__MD_PLAYER},

      items_shop_costs{_UI_CONTEXT_ITEMS_COST__CONSUMABLE__RECOVERY1,
                       _UI_CONTEXT_ITEMS_COST__CONSUMABLE__RECOVERY2,
                       _UI_CONTEXT_ITEMS_COST__CONSUMABLE__RECOVERY3,
                       _UI_CONTEXT_ITEMS_COST__CONSUMABLE__STIM,
                       _UI_CONTEXT_ITEMS_COST__CONSUMABLE__COLA,
                       _UI_CONTEXT_ITEMS_COST__CONSUMABLE__MP_BOOST1,
                       _UI_CONTEXT_ITEMS_COST__CONSUMABLE__MP_BOOST2,
                       _UI_CONTEXT_ITEMS_COST__CONSUMABLE__PENICILLIN,
                       _UI_CONTEXT_ITEMS_COST__CONSUMABLE__LIPSTICK,
                       _UI_CONTEXT_ITEMS_COST__CONSUMABLE__BELT_POUCH,
                       _UI_CONTEXT_ITEMS_COST__CONSUMABLE__FLARE,
                       _UI_CONTEXT_ITEMS_COST__CONSUMABLE__PEPPER_SPRAY,
                       _UI_CONTEXT_ITEMS_COST__CONSUMABLE__PROTEINE_CAPSULE,
                       _UI_CONTEXT_ITEMS_COST__CONSUMABLE__RINGERS_SOLUTION,
                       _UI_CONTEXT_ITEMS_COST__CONSUMABLE__EAU_DE_TOILETTE,
                       _UI_CONTEXT_ITEMS_COST__CONSUMABLE__COMBAT_LIGHT,
                       _UI_CONTEXT_ITEMS_COST__ATTACHMENT__SMG_CLIP_HOLDER,
                       _UI_CONTEXT_ITEMS_COST__ATTACHMENT__RIFLE_CLIP_HOLDER,
                       _UI_CONTEXT_ITEMS_COST__ATTACHMENT__SNAIL_MAGAZINE,
                       _UI_CONTEXT_ITEMS_COST__ATTACHMENT__HAMMER_M4A1,
                       _UI_CONTEXT_ITEMS_COST__ATTACHMENT__PYKE_M4A1,
                       _UI_CONTEXT_ITEMS_COST__ATTACHMENT__JAVELIN_M4A1,
                       _UI_CONTEXT_ITEMS_COST__ATTACHMENT__M203_M4A1,
                       _UI_CONTEXT_ITEMS_COST__ATTACHMENT__M9_M4A1,
                       _UI_CONTEXT_ITEMS_COST__ARMOR__LEATHER_JACKET,
                       _UI_CONTEXT_ITEMS_COST__ARMOR__TACTICAL_ARMOR,
                       _UI_CONTEXT_ITEMS_COST__ARMOR__COMBAT_ARMOR,
                       _UI_CONTEXT_ITEMS_COST__ARMOR__ASSAULT_SUIT,
                       _UI_CONTEXT_ITEMS_COST__ARMOR__PASGT_VEST,
                       _UI_CONTEXT_ITEMS_COST__ARMOR__TACTICAL_VEST,
                       _UI_CONTEXT_ITEMS_COST__ARMOR__EOD_SUIT,
                       _UI_CONTEXT_ITEMS_COST__ARMOR__TURTLE_VEST,
                       _UI_CONTEXT_ITEMS_COST__ARMOR__CHICKEN_PLATE,
                       _UI_CONTEXT_ITEMS_COST__ARMOR__NBC_SUIT,
                       _UI_CONTEXT_ITEMS_COST__ARMOR__PSYSUIT,
                       _UI_CONTEXT_ITEMS_COST__ARMOR__AYA_SPECIAL,
                       _UI_CONTEXT_ITEMS_COST__ARMOR__SHOULDER_HOLSTER,
                       _UI_CONTEXT_ITEMS_COST__ARMOR__MONK_ROBE,
                       _UI_CONTEXT_ITEMS_COST__WEAPON__M93R,
                       _UI_CONTEXT_ITEMS_COST__WEAPON__M950,
                       _UI_CONTEXT_ITEMS_COST__WEAPON__P08,
                       _UI_CONTEXT_ITEMS_COST__WEAPON__P229,
                       _UI_CONTEXT_ITEMS_COST__WEAPON__MONGOOSE,
                       _UI_CONTEXT_ITEMS_COST__WEAPON__GRENADE_PISTOL,
                       _UI_CONTEXT_ITEMS_COST__WEAPON__MM1,
                       _UI_CONTEXT_ITEMS_COST__WEAPON__PA3,
                       _UI_CONTEXT_ITEMS_COST__WEAPON__SP12,
                       _UI_CONTEXT_ITEMS_COST__WEAPON__AS12,
                       _UI_CONTEXT_ITEMS_COST__WEAPON__M4A1,
                       _UI_CONTEXT_ITEMS_COST__WEAPON__M249,
                       _UI_CONTEXT_ITEMS_COST__WEAPON__HYPERVELOCITY,
                       _UI_CONTEXT_ITEMS_COST__WEAPON__GUNBLADE,
                       _UI_CONTEXT_ITEMS_COST__WEAPON__MP5A5,
                       _UI_CONTEXT_ITEMS_COST__AMMO__PISTOL_PB,
                       _UI_CONTEXT_ITEMS_COST__AMMO__PISTOL_HYDRA,
                       _UI_CONTEXT_ITEMS_COST__AMMO__PISTOL_SPARTAN,
                       _UI_CONTEXT_ITEMS_COST__AMMO__MAGNUM,
                       _UI_CONTEXT_ITEMS_COST__AMMO__MAGNUM_MAEDA,
                       _UI_CONTEXT_ITEMS_COST__AMMO__GRENADES_RIOT,
                       _UI_CONTEXT_ITEMS_COST__AMMO__GRENADES,
                       _UI_CONTEXT_ITEMS_COST__AMMO__GRENADES_AIRBIRST,
                       _UI_CONTEXT_ITEMS_COST__AMMO__SHELLS_BUCKSHOT,
                       _UI_CONTEXT_ITEMS_COST__AMMO__SHELLS_FIREFLY,
                       _UI_CONTEXT_ITEMS_COST__AMMO__SHELLS_SLUG,
                       _UI_CONTEXT_ITEMS_COST__AMMO__RIFLE_ROUNDS,
                       _UI_CONTEXT_ITEMS_COST__MISC__CRYSTAL_SKULL,
                       _UI_CONTEXT_ITEMS_COST__MISC__MEDICINE_WHEEL,
                       _UI_CONTEXT_ITEMS_COST__MISC__HOLY_WATER,
                       _UI_CONTEXT_ITEMS_COST__MISC__OFUDA_CHARM,
                       _UI_CONTEXT_ITEMS_COST__MISC__HUNTER_GOGGLES,
                       _UI_CONTEXT_ITEMS_COST__MISC__GPS,
                       _UI_CONTEXT_ITEMS_COST__MISC__MD_PLAYER},

      items_shop_cost_mult(100),

      items_dist{
          16, 14, 48, 17, 5,
      },

      items_start_options{_UI_CONTEXT_ITEMS_OPTIONS__STARTING_WEAPON__RANDOM_STARTING_WEAPON,
                          _UI_CONTEXT_ITEMS_OPTIONS__STARTING_ARMOR__RANDOM_STARTING_ARMOR,
                          _UI_CONTEXT_ITEMS_OPTIONS__STARTING_ARMOR__RANDOM_STARTING_ARMOR},

      spell_cost_mult(100),

      spell_start_option(_UI_CONTEXT_MAGIC_OPTIONS__STARTING_MAGIC__RANDOM_FIRST_LEVEL_SPELL),

      music_pool(_UI_CONTEXT_MUSIC_OPTIONS_POOL__CAFETERIA_JAZZ_OUTSIDE |
                 _UI_CONTEXT_MUSIC_OPTIONS_POOL__STRANGER_BOSS_MUSIC |
                 _UI_CONTEXT_MUSIC_OPTIONS_POOL__CAFETERIA_JAZZ_INSIDE |
                 _UI_CONTEXT_MUSIC_OPTIONS_POOL__WINNING_NO_9 | _UI_CONTEXT_MUSIC_OPTIONS_POOL__MIST_GARAGE |
                 _UI_CONTEXT_MUSIC_OPTIONS_POOL__FIGHT_1 |
                 _UI_CONTEXT_MUSIC_OPTIONS_POOL__FIRST_STRANGER_SCENE_EATING_CORPSE |
                 _UI_CONTEXT_MUSIC_OPTIONS_POOL__AMBUSH |
                 _UI_CONTEXT_MUSIC_OPTIONS_POOL__ACROPOLIS_APPROACH_SCENE |
                 _UI_CONTEXT_MUSIC_OPTIONS_POOL__GO_OUT_OF_ACROPOLIS_ELEVATOR |
                 _UI_CONTEXT_MUSIC_OPTIONS_POOL__ACROPOLIS_PEACE_ENTERING_SQUARE |
                 _UI_CONTEXT_MUSIC_OPTIONS_POOL__ACROPOLIS_PEACE_2 |
                 _UI_CONTEXT_MUSIC_OPTIONS_POOL__ACROPOLIS_PEACE_3 |
                 _UI_CONTEXT_MUSIC_OPTIONS_POOL__ACROPOLIS_PEACE_4 |
                 _UI_CONTEXT_MUSIC_OPTIONS_POOL__RETURNING_TO_MIST |
                 _UI_CONTEXT_MUSIC_OPTIONS_POOL__ACROPOLIS_PEACE_5 |
                 _UI_CONTEXT_MUSIC_OPTIONS_POOL__DRYFIELD_DAY_PEACE_1 |
                 _UI_CONTEXT_MUSIC_OPTIONS_POOL__DRYFIELD_DAY_PEACE_2_MEETING_DOUGH |
                 _UI_CONTEXT_MUSIC_OPTIONS_POOL__DRYFIELD_BAR_MUSIC | _UI_CONTEXT_MUSIC_OPTIONS_POOL__0x2c |
                 _UI_CONTEXT_MUSIC_OPTIONS_POOL__DRYFIELD_DAY_PEACE_3 |
                 _UI_CONTEXT_MUSIC_OPTIONS_POOL__DRYFIELD_DAY_PEACE_4 | _UI_CONTEXT_MUSIC_OPTIONS_POOL__0x2f |
                 _UI_CONTEXT_MUSIC_OPTIONS_POOL__0x30 | _UI_CONTEXT_MUSIC_OPTIONS_POOL__0x31 |
                 _UI_CONTEXT_MUSIC_OPTIONS_POOL__HUNT_IN_DRYFIELD | _UI_CONTEXT_MUSIC_OPTIONS_POOL__0x37 |
                 _UI_CONTEXT_MUSIC_OPTIONS_POOL__0x39 | _UI_CONTEXT_MUSIC_OPTIONS_POOL__0x3a |
                 _UI_CONTEXT_MUSIC_OPTIONS_POOL__0x3b | _UI_CONTEXT_MUSIC_OPTIONS_POOL__0x3c |
                 _UI_CONTEXT_MUSIC_OPTIONS_POOL__0x3d | _UI_CONTEXT_MUSIC_OPTIONS_POOL__0x3e |
                 _UI_CONTEXT_MUSIC_OPTIONS_POOL__0x3f | _UI_CONTEXT_MUSIC_OPTIONS_POOL__0x41 |
                 _UI_CONTEXT_MUSIC_OPTIONS_POOL__0x42 | _UI_CONTEXT_MUSIC_OPTIONS_POOL__0x43 |
                 _UI_CONTEXT_MUSIC_OPTIONS_POOL__0x44 | _UI_CONTEXT_MUSIC_OPTIONS_POOL__0x45 |
                 _UI_CONTEXT_MUSIC_OPTIONS_POOL__0x46 | _UI_CONTEXT_MUSIC_OPTIONS_POOL__0x47 |
                 _UI_CONTEXT_MUSIC_OPTIONS_POOL__0x48 | _UI_CONTEXT_MUSIC_OPTIONS_POOL__0x49 |
                 _UI_CONTEXT_MUSIC_OPTIONS_POOL__0x4a | _UI_CONTEXT_MUSIC_OPTIONS_POOL__0x4b |
                 _UI_CONTEXT_MUSIC_OPTIONS_POOL__0x4c | _UI_CONTEXT_MUSIC_OPTIONS_POOL__0x4d |
                 _UI_CONTEXT_MUSIC_OPTIONS_POOL__0x4e | _UI_CONTEXT_MUSIC_OPTIONS_POOL__0x51 |
                 _UI_CONTEXT_MUSIC_OPTIONS_POOL__0x52 | _UI_CONTEXT_MUSIC_OPTIONS_POOL__0x53 |
                 _UI_CONTEXT_MUSIC_OPTIONS_POOL__0x56 | _UI_CONTEXT_MUSIC_OPTIONS_POOL__0x59 |
                 _UI_CONTEXT_MUSIC_OPTIONS_POOL__0x5a | _UI_CONTEXT_MUSIC_OPTIONS_POOL__0x5d |
                 _UI_CONTEXT_MUSIC_OPTIONS_POOL__0x5e | _UI_CONTEXT_MUSIC_OPTIONS_POOL__0x5f |
                 _UI_CONTEXT_MUSIC_OPTIONS_POOL__0x60 | _UI_CONTEXT_MUSIC_OPTIONS_POOL__0x61 |
                 _UI_CONTEXT_MUSIC_OPTIONS_POOL__0x62 | _UI_CONTEXT_MUSIC_OPTIONS_POOL__SIGH_OF_RELIEF |
                 _UI_CONTEXT_MUSIC_OPTIONS_POOL__CRUELTY_OF_EVES_FATE |
                 _UI_CONTEXT_MUSIC_OPTIONS_POOL__NO9_CUT_SCENE | _UI_CONTEXT_MUSIC_OPTIONS_POOL__WEIRD_MAN

                 ),

      player_model_option(_UI_CONTEXT_PLAYER_MODEL__DEFAULT),

      hud_color_r(255),

      hud_color_g(255),

      hud_color_b(255),

      logger(_logger)

{};

std::string UIContext::serialize() const {
    std::ostringstream oss;
    oss.write(reinterpret_cast<const char *>(&seed), sizeof(seed));
    const uint32_t persistedFlags = flags & ~0x08000000u; // Retired New weapons.
    oss.write(reinterpret_cast<const char *>(&persistedFlags), sizeof(persistedFlags));
    oss.write(reinterpret_cast<const char *>(&enemies_pool), sizeof(enemies_pool));
    oss.write(reinterpret_cast<const char *>(enemies_mults), sizeof(enemies_mults));
    oss.write(reinterpret_cast<const char *>(items_drop_pool), sizeof(items_drop_pool));
    oss.write(reinterpret_cast<const char *>(items_shop_pool), sizeof(items_shop_pool));
    oss.write(reinterpret_cast<const char *>(items_shop_costs), sizeof(items_shop_costs));
    oss.write(reinterpret_cast<const char *>(&items_shop_cost_mult), sizeof(items_shop_cost_mult));
    oss.write(reinterpret_cast<const char *>(items_dist), sizeof(items_dist));
    oss.write(reinterpret_cast<const char *>(items_start_options), sizeof(items_start_options));
    oss.write(reinterpret_cast<const char *>(&spell_cost_mult), sizeof(spell_cost_mult));
    oss.write(reinterpret_cast<const char *>(&spell_start_option), sizeof(spell_start_option));
    oss.write(reinterpret_cast<const char *>(&music_pool), sizeof(music_pool));
    oss.write(reinterpret_cast<const char *>(&player_model_option), sizeof(player_model_option));
    // Versioned trailer keeps the legacy payload intact. Old INIs have no RGB.
    oss.write("HUD1", 4);
    oss.put(static_cast<char>(hud_color_r));
    oss.put(static_cast<char>(hud_color_g));
    oss.put(static_cast<char>(hud_color_b));

    std::string binaryData = oss.str();
    return base64_encode(binaryData);
}

void UIContext::deserialize(const std::string &base64String) {
    std::string binaryData = base64_decode(base64String);
    size_t data_size = sizeof(seed) + sizeof(flags) + sizeof(enemies_pool) + sizeof(enemies_mults) +
                       sizeof(items_drop_pool) + sizeof(items_shop_pool) + sizeof(items_shop_costs) +
                       sizeof(items_shop_cost_mult) + sizeof(items_dist) + sizeof(items_start_options) +
                       sizeof(spell_cost_mult) + sizeof(spell_start_option) + sizeof(music_pool) +
                       sizeof(player_model_option);
    const bool legacy = binaryData.size() == data_size;
    if (!legacy && (binaryData.size() != data_size + 7 || binaryData.compare(data_size, 4, "HUD1") != 0))
        throw std::runtime_error("Invalid or unsupported settings format");

    std::istringstream iss(binaryData);

    iss.read(reinterpret_cast<char *>(&seed), sizeof(seed));
    iss.read(reinterpret_cast<char *>(&flags), sizeof(flags));
    flags &= ~0x08000000u; // Ignore the retired mutator in older presets.
    iss.read(reinterpret_cast<char *>(&enemies_pool), sizeof(enemies_pool));
    iss.read(reinterpret_cast<char *>(enemies_mults), sizeof(enemies_mults));
    iss.read(reinterpret_cast<char *>(items_drop_pool), sizeof(items_drop_pool));
    iss.read(reinterpret_cast<char *>(items_shop_pool), sizeof(items_shop_pool));
    iss.read(reinterpret_cast<char *>(items_shop_costs), sizeof(items_shop_costs));
    iss.read(reinterpret_cast<char *>(&items_shop_cost_mult), sizeof(items_shop_cost_mult));
    iss.read(reinterpret_cast<char *>(items_dist), sizeof(items_dist));
    iss.read(reinterpret_cast<char *>(items_start_options), sizeof(items_start_options));
    iss.read(reinterpret_cast<char *>(&spell_cost_mult), sizeof(spell_cost_mult));
    iss.read(reinterpret_cast<char *>(&spell_start_option), sizeof(spell_start_option));
    iss.read(reinterpret_cast<char *>(&music_pool), sizeof(music_pool));
    iss.read(reinterpret_cast<char *>(&player_model_option), sizeof(player_model_option));
    // Do not inherit a previous profile's color when loading a legacy INI.
    hud_color_r = legacy ? 255 : static_cast<uint8_t>(binaryData[data_size + 4]);
    hud_color_g = legacy ? 255 : static_cast<uint8_t>(binaryData[data_size + 5]);
    hud_color_b = legacy ? 255 : static_cast<uint8_t>(binaryData[data_size + 6]);
}

delivery::config_t UIContext::convert() {
    int ammo_boxes_option = 0; // default
    if (flags & UIContext::_UI_CONTEXT_FLAG__AMMO_BOXES__NO_AMMO_BOXES) {
        ammo_boxes_option = 1;
    } else if (flags & UIContext::_UI_CONTEXT_FLAG__AMMO_BOXES__ONE_TIME) {
        ammo_boxes_option = 2;
    }

    int ammo_boxes_rando_option = 0; // default
    if (flags & UIContext::_UI_CONTEXT_FLAG__AMMO_BOXES_RANDOMIZATION__RANDOMIZED_BOXES) {
        ammo_boxes_rando_option = 1;
    } else if (flags & UIContext::_UI_CONTEXT_FLAG__AMMO_BOXES_RANDOMIZATION__FULL_RANDOM_BOXES) {
        ammo_boxes_rando_option = 2;
    }
    std::cout << "[DEBUG] UIContext::convert() - flags=0x" << std::hex << flags << ", RANDOMIZED_BOXES flag="
              << ((flags & _UI_CONTEXT_FLAG__AMMO_BOXES_RANDOMIZATION__RANDOMIZED_BOXES) ? "SET" : "NOT SET")
              << ", FULL_RANDOM_BOXES flag="
              << ((flags & _UI_CONTEXT_FLAG__AMMO_BOXES_RANDOMIZATION__FULL_RANDOM_BOXES) ? "SET" : "NOT SET")
              << ", UNRANDOMIZED flag="
              << ((flags & _UI_CONTEXT_FLAG__AMMO_BOXES_RANDOMIZATION__UNRANDOMIZED) ? "SET" : "NOT SET")
              << ", ammo_boxes_rando_option=" << std::dec << ammo_boxes_rando_option << std::endl;

    int enemy_enhancement_option = 0; // default
    if (flags & UIContext::_UI_CONTEXT_FLAG__ENEMY_ENHANCEMENT__ENHANCE_ALL_ENEMIES) {
        enemy_enhancement_option = 1;
    } else if (flags & UIContext::_UI_CONTEXT_FLAG__ENEMY_ENHANCEMENT__INCLUDE_ENHANCED) {
        enemy_enhancement_option = 2;
    } else if (flags & UIContext::_UI_CONTEXT_FLAG__ENEMY_ENHANCEMENT__ENHANCE_DISC2_ENEMIES) {
        enemy_enhancement_option = 3;
    }

    std::vector<uint8_t> randomized_enemies_pool{};
    if (enemies_pool & UIContext::_UI_CONTEXT_ENEMIES_POOL__CHASER) {
        randomized_enemies_pool.push_back(0x1);
    }
    if (enemies_pool & UIContext::_UI_CONTEXT_ENEMIES_POOL__BRAIN_STINGER) {
        randomized_enemies_pool.push_back(0x3);
    }
    if (enemies_pool & UIContext::_UI_CONTEXT_ENEMIES_POOL__DIVER) {
        randomized_enemies_pool.push_back(0x4);
    }
    if (enemies_pool & UIContext::_UI_CONTEXT_ENEMIES_POOL__STALKER) {
        randomized_enemies_pool.push_back(0x6);
    }
    if (enemies_pool & UIContext::_UI_CONTEXT_ENEMIES_POOL__RAT) {
        randomized_enemies_pool.push_back(0x7);
    }
    if (enemies_pool & UIContext::_UI_CONTEXT_ENEMIES_POOL__BUTTERFLY) {
        randomized_enemies_pool.push_back(0x8);
    }
    if (enemies_pool & UIContext::_UI_CONTEXT_ENEMIES_POOL__LESSER_STRANGER) {
        randomized_enemies_pool.push_back(0xa);
    }
    if (enemies_pool & UIContext::_UI_CONTEXT_ENEMIES_POOL__BRUTE) {
        randomized_enemies_pool.push_back(0xb);
    }
    if (enemies_pool & UIContext::_UI_CONTEXT_ENEMIES_POOL__SUCKLER) {
        randomized_enemies_pool.push_back(0xc);
    }
    if (enemies_pool & UIContext::_UI_CONTEXT_ENEMIES_POOL__HORNED_STRANGER) {
        randomized_enemies_pool.push_back(0xd);
    }
    if (enemies_pool & UIContext::_UI_CONTEXT_ENEMIES_POOL__MIND_SUCKLER) {
        randomized_enemies_pool.push_back(0xf);
    }
    if (enemies_pool & UIContext::_UI_CONTEXT_ENEMIES_POOL__SCAVANGER) {
        randomized_enemies_pool.push_back(0x10);
    }
    if (enemies_pool & UIContext::_UI_CONTEXT_ENEMIES_POOL__ODD_STRANGER) {
        randomized_enemies_pool.push_back(0x12);
    }
    if (enemies_pool & UIContext::_UI_CONTEXT_ENEMIES_POOL__GRINNING_STRANGER) {
        randomized_enemies_pool.push_back(0x13);
    }
    if (enemies_pool & UIContext::_UI_CONTEXT_ENEMIES_POOL__PAWN_BLADE_GOLEM) {
        randomized_enemies_pool.push_back(0x14);
    }
    if (enemies_pool & UIContext::_UI_CONTEXT_ENEMIES_POOL__TURRET) {
        randomized_enemies_pool.push_back(0x15);
    }
    if (enemies_pool & UIContext::_UI_CONTEXT_ENEMIES_POOL__KNIGHT_GOLEM) {
        randomized_enemies_pool.push_back(0x16);
    }
    if (enemies_pool & UIContext::_UI_CONTEXT_ENEMIES_POOL__ROOK_BLADE_GOLEM) {
        randomized_enemies_pool.push_back(0x17);
    }
    if (enemies_pool & UIContext::_UI_CONTEXT_ENEMIES_POOL__AMOEBA) {
        randomized_enemies_pool.push_back(0x18);
    }
    if (enemies_pool & UIContext::_UI_CONTEXT_ENEMIES_POOL__SCORPION) {
        randomized_enemies_pool.push_back(0x19);
    }
    if (enemies_pool & UIContext::_UI_CONTEXT_ENEMIES_POOL__WHITE_WORM) {
        randomized_enemies_pool.push_back(0x1a);
    }
    if (enemies_pool & UIContext::_UI_CONTEXT_ENEMIES_POOL__BAT) {
        randomized_enemies_pool.push_back(0x25);
    }
    if (enemies_pool & UIContext::_UI_CONTEXT_ENEMIES_POOL__BEETLE) {
        randomized_enemies_pool.push_back(0x26);
    }
    if (enemies_pool & UIContext::_UI_CONTEXT_ENEMIES_POOL__BISHOP_GOLEM) {
        randomized_enemies_pool.push_back(0x27);
    }
    if (enemies_pool & UIContext::_UI_CONTEXT_ENEMIES_POOL__FAILED_CHASER) {
        randomized_enemies_pool.push_back(0x2c);
    }
    if (enemies_pool & UIContext::_UI_CONTEXT_ENEMIES_POOL__FAILED_SUCKLER) {
        randomized_enemies_pool.push_back(0x2e);
    }
    if (enemies_pool & UIContext::_UI_CONTEXT_ENEMIES_POOL__MOSSBACK_BRUTE) {
        randomized_enemies_pool.push_back(0x31);
    }
    if (enemies_pool & UIContext::_UI_CONTEXT_ENEMIES_POOL__GREEN_WORM) {
        randomized_enemies_pool.push_back(0x37);
    }
    if (enemies_pool & UIContext::_UI_CONTEXT_ENEMIES_POOL__PAWN_GUN_GOLEM) {
        randomized_enemies_pool.push_back(0x38);
    }
    if (enemies_pool & UIContext::_UI_CONTEXT_ENEMIES_POOL__ROOK_GUN_GOLEM) {
        randomized_enemies_pool.push_back(0x39);
    }
    if (enemies_pool & UIContext::_UI_CONTEXT_ENEMIES_POOL__FAILED_BRUTE) {
        randomized_enemies_pool.push_back(0x46);
    }
    if (enemies_pool & UIContext::_UI_CONTEXT_ENEMIES_POOL__FAILED_STRANGER) {
        randomized_enemies_pool.push_back(0x48);
    }

    std::vector<uint8_t> randomized_items_drop_pool{};

    if (items_drop_pool[0] & UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__CONSUMABLE__RECOVERY1) {
        randomized_items_drop_pool.push_back(0x1);
    }
    if (items_drop_pool[0] & UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__CONSUMABLE__RECOVERY2) {
        randomized_items_drop_pool.push_back(0x2);
    }
    if (items_drop_pool[0] & UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__CONSUMABLE__RECOVERY3) {
        randomized_items_drop_pool.push_back(0x3);
    }
    if (items_drop_pool[0] & UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__CONSUMABLE__STIM) {
        randomized_items_drop_pool.push_back(0x4);
    }
    if (items_drop_pool[0] & UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__CONSUMABLE__COLA) {
        randomized_items_drop_pool.push_back(0x5);
    }
    if (items_drop_pool[0] & UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__CONSUMABLE__MP_BOOST1) {
        randomized_items_drop_pool.push_back(0x6);
    }
    if (items_drop_pool[0] & UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__CONSUMABLE__MP_BOOST2) {
        randomized_items_drop_pool.push_back(0x7);
    }
    if (items_drop_pool[0] & UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__CONSUMABLE__PENICILLIN) {
        randomized_items_drop_pool.push_back(0x8);
    }
    if (items_drop_pool[0] & UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__CONSUMABLE__LIPSTICK) {
        randomized_items_drop_pool.push_back(0xb);
    }
    if (items_drop_pool[0] & UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__CONSUMABLE__BELT_POUCH) {
        randomized_items_drop_pool.push_back(0xd);
    }
    if (items_drop_pool[0] & UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__CONSUMABLE__FLARE) {
        randomized_items_drop_pool.push_back(0x3a);
    }
    if (items_drop_pool[0] & UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__CONSUMABLE__PEPPER_SPRAY) {
        randomized_items_drop_pool.push_back(0x3b);
    }
    if (items_drop_pool[0] & UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__CONSUMABLE__PROTEINE_CAPSULE) {
        randomized_items_drop_pool.push_back(0x3c);
    }
    if (items_drop_pool[0] & UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__CONSUMABLE__EAU_DE_TOILETTE) {
        randomized_items_drop_pool.push_back(0x3e);
    }
    if (items_drop_pool[0] & UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__CONSUMABLE__COMBAT_LIGHT) {
        randomized_items_drop_pool.push_back(0x41);
    }
    if (items_drop_pool[0] & UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__ATTACHMENT__SMG_CLIP_HOLDER) {
        randomized_items_drop_pool.push_back(0x9);
    }
    if (items_drop_pool[0] & UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__ATTACHMENT__RIFLE_CLIP_HOLDER) {
        randomized_items_drop_pool.push_back(0xa);
    }
    if (items_drop_pool[0] & UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__ATTACHMENT__SNAIL_MAGAZINE) {
        randomized_items_drop_pool.push_back(0xc);
    }
    if (items_drop_pool[0] & UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__ATTACHMENT__HAMMER_M4A1) {
        randomized_items_drop_pool.push_back(0x42);
    }
    if (items_drop_pool[0] & UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__ATTACHMENT__PYKE_M4A1) {
        randomized_items_drop_pool.push_back(0x43);
    }
    if (items_drop_pool[0] & UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__ATTACHMENT__JAVELIN_M4A1) {
        randomized_items_drop_pool.push_back(0x44);
    }
    if (items_drop_pool[0] & UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__ATTACHMENT__M203_M4A1) {
        randomized_items_drop_pool.push_back(0x45);
    }
    if (items_drop_pool[0] & UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__ATTACHMENT__M9_M4A1) {
        randomized_items_drop_pool.push_back(0x46);
    }
    if (items_drop_pool[0] & UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__ARMOR__LEATHER_JACKET) {
        randomized_items_drop_pool.push_back(0x60);
    }
    if (items_drop_pool[0] & UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__ARMOR__TACTICAL_ARMOR) {
        randomized_items_drop_pool.push_back(0x61);
    }
    if (items_drop_pool[0] & UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__ARMOR__COMBAT_ARMOR) {
        randomized_items_drop_pool.push_back(0x62);
    }
    if (items_drop_pool[0] & UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__ARMOR__ASSAULT_SUIT) {
        randomized_items_drop_pool.push_back(0x63);
    }
    if (items_drop_pool[0] & UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__ARMOR__PASGT_VEST) {
        randomized_items_drop_pool.push_back(0x64);
    }
    if (items_drop_pool[0] & UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__ARMOR__TACTICAL_VEST) {
        randomized_items_drop_pool.push_back(0x65);
    }
    if (items_drop_pool[0] & UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__ARMOR__EOD_SUIT) {
        randomized_items_drop_pool.push_back(0x66);
    }
    if (items_drop_pool[0] & UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__ARMOR__TURTLE_VEST) {
        randomized_items_drop_pool.push_back(0x67);
    }
    if (items_drop_pool[0] & UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__ARMOR__CHICKEN_PLATE) {
        randomized_items_drop_pool.push_back(0x68);
    }
    if (items_drop_pool[0] & UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__ARMOR__NBC_SUIT) {
        randomized_items_drop_pool.push_back(0x69);
    }
    if (items_drop_pool[0] & UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__ARMOR__PSYSUIT) {
        randomized_items_drop_pool.push_back(0x6a);
    }
    if (items_drop_pool[0] & UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__ARMOR__AYA_SPECIAL) {
        randomized_items_drop_pool.push_back(0x6b);
    }
    if (items_drop_pool[0] & UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__ARMOR__SHOULDER_HOLSTER) {
        randomized_items_drop_pool.push_back(0x6c);
    }
    if (items_drop_pool[0] & UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__ARMOR__MONK_ROBE) {
        randomized_items_drop_pool.push_back(0x6d);
    }
    if (items_drop_pool[0] & UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__WEAPON__M93R) {
        randomized_items_drop_pool.push_back(0x81);
    }
    if (items_drop_pool[0] & UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__WEAPON__M950) {
        randomized_items_drop_pool.push_back(0x82);
    }
    if (items_drop_pool[0] & UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__WEAPON__P08) {
        randomized_items_drop_pool.push_back(0x83);
    }
    if (items_drop_pool[0] & UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__WEAPON__P229) {
        randomized_items_drop_pool.push_back(0x84);
    }
    if (items_drop_pool[0] & UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__WEAPON__MONGOOSE) {
        randomized_items_drop_pool.push_back(0x88);
    }
    if (items_drop_pool[0] & UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__WEAPON__GRENADE_PISTOL) {
        randomized_items_drop_pool.push_back(0x8a);
    }
    if (items_drop_pool[0] & UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__WEAPON__MM1) {
        randomized_items_drop_pool.push_back(0x8b);
    }
    if (items_drop_pool[0] & UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__WEAPON__PA3) {
        randomized_items_drop_pool.push_back(0x8c);
    }
    if (items_drop_pool[0] & UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__WEAPON__SP12) {
        randomized_items_drop_pool.push_back(0x8d);
    }
    if (items_drop_pool[0] & UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__WEAPON__AS12) {
        randomized_items_drop_pool.push_back(0x8e);
    }
    if (items_drop_pool[0] & UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__WEAPON__M4A1) {
        randomized_items_drop_pool.push_back(0x8f);
    }
    if (items_drop_pool[0] & UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__WEAPON__M249) {
        randomized_items_drop_pool.push_back(0x90);
    }
    if (items_drop_pool[0] & UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__WEAPON__HYPERVELOCITY) {
        randomized_items_drop_pool.push_back(0x95);
    }
    if (items_drop_pool[0] & UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__WEAPON__GUNBLADE) {
        randomized_items_drop_pool.push_back(0x96);
    }
    if (items_drop_pool[0] & UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__WEAPON__MP5A5) {
        randomized_items_drop_pool.push_back(0x9d);
    }
    if (items_drop_pool[0] & UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__AMMO__PISTOL_PB) {
        randomized_items_drop_pool.push_back(0xa0);
    }
    if (items_drop_pool[0] & UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__AMMO__PISTOL_HYDRA) {
        randomized_items_drop_pool.push_back(0xa1);
    }
    if (items_drop_pool[0] & UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__AMMO__PISTOL_SPARTAN) {
        randomized_items_drop_pool.push_back(0xa2);
    }
    if (items_drop_pool[0] & UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__AMMO__MAGNUM) {
        randomized_items_drop_pool.push_back(0xa6);
    }
    if (items_drop_pool[0] & UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__AMMO__MAGNUM_MAEDA) {
        randomized_items_drop_pool.push_back(0xa7);
    }
    if (items_drop_pool[0] & UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__AMMO__GRENADES) {
        randomized_items_drop_pool.push_back(0xa9);
    }
    if (items_drop_pool[0] & UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__AMMO__GRENADES_AIRBIRST) {
        randomized_items_drop_pool.push_back(0xab);
    }

    if (items_drop_pool[0] & UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__AMMO__GRENADES_RIOT) {
        randomized_items_drop_pool.push_back(0xaa);
    }
    if (items_drop_pool[0] & UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__AMMO__SHELLS_BUCKSHOT) {
        randomized_items_drop_pool.push_back(0xac);
    }
    if (items_drop_pool[0] & UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__AMMO__SHELLS_FIREFLY) {
        randomized_items_drop_pool.push_back(0xad);
    }
    if (items_drop_pool[1] & UIContext::_UI_CONTEXT_ITEMS_POOL_HIGH_INT__AMMO__SHELLS_SLUG) {
        randomized_items_drop_pool.push_back(0xae);
    }
    if (items_drop_pool[1] & UIContext::_UI_CONTEXT_ITEMS_POOL_HIGH_INT__AMMO__RIFLE_ROUNDS) {
        randomized_items_drop_pool.push_back(0xaf);
    }
    if (items_drop_pool[1] & UIContext::_UI_CONTEXT_ITEMS_POOL_HIGH_INT__MISC__CRYSTAL_SKULL) {
        randomized_items_drop_pool.push_back(0x36);
    }
    if (items_drop_pool[1] & UIContext::_UI_CONTEXT_ITEMS_POOL_HIGH_INT__MISC__MEDICINE_WHEEL) {
        randomized_items_drop_pool.push_back(0x37);
    }
    if (items_drop_pool[1] & UIContext::_UI_CONTEXT_ITEMS_POOL_HIGH_INT__MISC__HOLY_WATER) {
        randomized_items_drop_pool.push_back(0x38);
    }
    if (items_drop_pool[1] & UIContext::_UI_CONTEXT_ITEMS_POOL_HIGH_INT__MISC__OFUDA_CHARM) {
        randomized_items_drop_pool.push_back(0x39);
    }
    if (items_drop_pool[1] & UIContext::_UI_CONTEXT_ITEMS_POOL_HIGH_INT__MISC__HUNTER_GOGGLES) {
        randomized_items_drop_pool.push_back(0x3f);
    }
    if (items_drop_pool[1] & UIContext::_UI_CONTEXT_ITEMS_POOL_HIGH_INT__MISC__GPS) {
        randomized_items_drop_pool.push_back(0x40);
    }
    if (items_drop_pool[1] & UIContext::_UI_CONTEXT_ITEMS_POOL_HIGH_INT__MISC__MD_PLAYER) {
        randomized_items_drop_pool.push_back(0x0e);
    }

    std::vector<uint8_t> randomized_items_shop_pool{};

    if (items_shop_pool[0] & UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__CONSUMABLE__RECOVERY1) {
        randomized_items_shop_pool.push_back(0x1);
    }
    if (items_shop_pool[0] & UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__CONSUMABLE__RECOVERY2) {
        randomized_items_shop_pool.push_back(0x2);
    }
    if (items_shop_pool[0] & UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__CONSUMABLE__RECOVERY3) {
        randomized_items_shop_pool.push_back(0x3);
    }
    if (items_shop_pool[0] & UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__CONSUMABLE__STIM) {
        randomized_items_shop_pool.push_back(0x4);
    }
    if (items_shop_pool[0] & UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__CONSUMABLE__COLA) {
        randomized_items_shop_pool.push_back(0x5);
    }
    if (items_shop_pool[0] & UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__CONSUMABLE__MP_BOOST1) {
        randomized_items_shop_pool.push_back(0x6);
    }
    if (items_shop_pool[0] & UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__CONSUMABLE__MP_BOOST2) {
        randomized_items_shop_pool.push_back(0x7);
    }
    if (items_shop_pool[0] & UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__CONSUMABLE__PENICILLIN) {
        randomized_items_shop_pool.push_back(0x8);
    }
    if (items_shop_pool[0] & UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__CONSUMABLE__LIPSTICK) {
        randomized_items_shop_pool.push_back(0xb);
    }
    if (items_shop_pool[0] & UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__CONSUMABLE__BELT_POUCH) {
        randomized_items_shop_pool.push_back(0xd);
    }
    if (items_shop_pool[0] & UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__CONSUMABLE__FLARE) {
        randomized_items_shop_pool.push_back(0x3a);
    }
    if (items_shop_pool[0] & UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__CONSUMABLE__PEPPER_SPRAY) {
        randomized_items_shop_pool.push_back(0x3b);
    }
    if (items_shop_pool[0] & UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__CONSUMABLE__PROTEINE_CAPSULE) {
        randomized_items_shop_pool.push_back(0x3c);
    }
    if (items_shop_pool[0] & UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__CONSUMABLE__EAU_DE_TOILETTE) {
        randomized_items_shop_pool.push_back(0x3e);
    }
    if (items_shop_pool[0] & UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__CONSUMABLE__COMBAT_LIGHT) {
        randomized_items_shop_pool.push_back(0x41);
    }
    if (items_shop_pool[0] & UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__ATTACHMENT__SMG_CLIP_HOLDER) {
        randomized_items_shop_pool.push_back(0x9);
    }
    if (items_shop_pool[0] & UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__ATTACHMENT__RIFLE_CLIP_HOLDER) {
        randomized_items_shop_pool.push_back(0xa);
    }
    if (items_shop_pool[0] & UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__ATTACHMENT__SNAIL_MAGAZINE) {
        randomized_items_shop_pool.push_back(0xc);
    }
    if (items_shop_pool[0] & UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__ATTACHMENT__HAMMER_M4A1) {
        randomized_items_shop_pool.push_back(0x42);
    }
    if (items_shop_pool[0] & UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__ATTACHMENT__PYKE_M4A1) {
        randomized_items_shop_pool.push_back(0x43);
    }
    if (items_shop_pool[0] & UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__ATTACHMENT__JAVELIN_M4A1) {
        randomized_items_shop_pool.push_back(0x44);
    }
    if (items_shop_pool[0] & UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__ATTACHMENT__M203_M4A1) {
        randomized_items_shop_pool.push_back(0x45);
    }
    if (items_shop_pool[0] & UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__ATTACHMENT__M9_M4A1) {
        randomized_items_shop_pool.push_back(0x46);
    }
    if (items_shop_pool[0] & UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__ARMOR__LEATHER_JACKET) {
        randomized_items_shop_pool.push_back(0x60);
    }
    if (items_shop_pool[0] & UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__ARMOR__TACTICAL_ARMOR) {
        randomized_items_shop_pool.push_back(0x61);
    }
    if (items_shop_pool[0] & UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__ARMOR__COMBAT_ARMOR) {
        randomized_items_shop_pool.push_back(0x62);
    }
    if (items_shop_pool[0] & UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__ARMOR__ASSAULT_SUIT) {
        randomized_items_shop_pool.push_back(0x63);
    }
    if (items_shop_pool[0] & UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__ARMOR__PASGT_VEST) {
        randomized_items_shop_pool.push_back(0x64);
    }
    if (items_shop_pool[0] & UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__ARMOR__TACTICAL_VEST) {
        randomized_items_shop_pool.push_back(0x65);
    }
    if (items_shop_pool[0] & UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__ARMOR__EOD_SUIT) {
        randomized_items_shop_pool.push_back(0x66);
    }
    if (items_shop_pool[0] & UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__ARMOR__TURTLE_VEST) {
        randomized_items_shop_pool.push_back(0x67);
    }
    if (items_shop_pool[0] & UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__ARMOR__CHICKEN_PLATE) {
        randomized_items_shop_pool.push_back(0x68);
    }
    if (items_shop_pool[0] & UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__ARMOR__NBC_SUIT) {
        randomized_items_shop_pool.push_back(0x69);
    }
    if (items_shop_pool[0] & UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__ARMOR__PSYSUIT) {
        randomized_items_shop_pool.push_back(0x6a);
    }
    if (items_shop_pool[0] & UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__ARMOR__AYA_SPECIAL) {
        randomized_items_shop_pool.push_back(0x6b);
    }
    if (items_shop_pool[0] & UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__ARMOR__SHOULDER_HOLSTER) {
        randomized_items_shop_pool.push_back(0x6c);
    }
    if (items_shop_pool[0] & UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__ARMOR__MONK_ROBE) {
        randomized_items_shop_pool.push_back(0x6d);
    }
    if (items_shop_pool[0] & UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__WEAPON__M93R) {
        randomized_items_shop_pool.push_back(0x81);
    }
    if (items_shop_pool[0] & UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__WEAPON__M950) {
        randomized_items_shop_pool.push_back(0x82);
    }
    if (items_shop_pool[0] & UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__WEAPON__P08) {
        randomized_items_shop_pool.push_back(0x83);
    }
    if (items_shop_pool[0] & UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__WEAPON__P229) {
        randomized_items_shop_pool.push_back(0x84);
    }
    if (items_shop_pool[0] & UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__WEAPON__MONGOOSE) {
        randomized_items_shop_pool.push_back(0x88);
    }
    if (items_shop_pool[0] & UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__WEAPON__GRENADE_PISTOL) {
        randomized_items_shop_pool.push_back(0x8a);
    }
    if (items_shop_pool[0] & UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__WEAPON__MM1) {
        randomized_items_shop_pool.push_back(0x8b);
    }
    if (items_shop_pool[0] & UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__WEAPON__PA3) {
        randomized_items_shop_pool.push_back(0x8c);
    }
    if (items_shop_pool[0] & UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__WEAPON__SP12) {
        randomized_items_shop_pool.push_back(0x8d);
    }
    if (items_shop_pool[0] & UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__WEAPON__AS12) {
        randomized_items_shop_pool.push_back(0x8e);
    }
    if (items_shop_pool[0] & UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__WEAPON__M4A1) {
        randomized_items_shop_pool.push_back(0x8f);
    }
    if (items_shop_pool[0] & UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__WEAPON__M249) {
        randomized_items_shop_pool.push_back(0x90);
    }
    if (items_shop_pool[0] & UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__WEAPON__HYPERVELOCITY) {
        randomized_items_shop_pool.push_back(0x95);
    }
    if (items_shop_pool[0] & UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__WEAPON__GUNBLADE) {
        randomized_items_shop_pool.push_back(0x96);
    }
    if (items_shop_pool[0] & UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__WEAPON__MP5A5) {
        randomized_items_shop_pool.push_back(0x9d);
    }
    if (items_shop_pool[0] & UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__AMMO__PISTOL_PB) {
        randomized_items_shop_pool.push_back(0xa0);
    }
    if (items_shop_pool[0] & UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__AMMO__PISTOL_HYDRA) {
        randomized_items_shop_pool.push_back(0xa1);
    }
    if (items_shop_pool[0] & UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__AMMO__PISTOL_SPARTAN) {
        randomized_items_shop_pool.push_back(0xa2);
    }
    if (items_shop_pool[0] & UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__AMMO__MAGNUM) {
        randomized_items_shop_pool.push_back(0xa6);
    }
    if (items_shop_pool[0] & UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__AMMO__MAGNUM_MAEDA) {
        randomized_items_shop_pool.push_back(0xa7);
    }
    if (items_shop_pool[0] & UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__AMMO__GRENADES) {
        randomized_items_shop_pool.push_back(0xa9);
    }
    if (items_shop_pool[0] & UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__AMMO__GRENADES_AIRBIRST) {
        randomized_items_shop_pool.push_back(0xab);
    }

    if (items_shop_pool[0] & UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__AMMO__GRENADES_RIOT) {
        randomized_items_shop_pool.push_back(0xaa);
    }
    if (items_shop_pool[0] & UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__AMMO__SHELLS_BUCKSHOT) {
        randomized_items_shop_pool.push_back(0xac);
    }
    if (items_shop_pool[0] & UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__AMMO__SHELLS_FIREFLY) {
        randomized_items_shop_pool.push_back(0xad);
    }
    if (items_shop_pool[1] & UIContext::_UI_CONTEXT_ITEMS_POOL_HIGH_INT__AMMO__SHELLS_SLUG) {
        randomized_items_shop_pool.push_back(0xae);
    }
    if (items_shop_pool[1] & UIContext::_UI_CONTEXT_ITEMS_POOL_HIGH_INT__AMMO__RIFLE_ROUNDS) {
        randomized_items_shop_pool.push_back(0xaf);
    }
    if (items_shop_pool[1] & UIContext::_UI_CONTEXT_ITEMS_POOL_HIGH_INT__MISC__CRYSTAL_SKULL) {
        randomized_items_shop_pool.push_back(0x36);
    }
    if (items_shop_pool[1] & UIContext::_UI_CONTEXT_ITEMS_POOL_HIGH_INT__MISC__MEDICINE_WHEEL) {
        randomized_items_shop_pool.push_back(0x37);
    }
    if (items_shop_pool[1] & UIContext::_UI_CONTEXT_ITEMS_POOL_HIGH_INT__MISC__HOLY_WATER) {
        randomized_items_shop_pool.push_back(0x38);
    }
    if (items_shop_pool[1] & UIContext::_UI_CONTEXT_ITEMS_POOL_HIGH_INT__MISC__OFUDA_CHARM) {
        randomized_items_shop_pool.push_back(0x39);
    }
    if (items_shop_pool[1] & UIContext::_UI_CONTEXT_ITEMS_POOL_HIGH_INT__MISC__HUNTER_GOGGLES) {
        randomized_items_shop_pool.push_back(0x3f);
    }
    if (items_shop_pool[1] & UIContext::_UI_CONTEXT_ITEMS_POOL_HIGH_INT__MISC__GPS) {
        randomized_items_shop_pool.push_back(0x40);
    }
    if (items_shop_pool[1] & UIContext::_UI_CONTEXT_ITEMS_POOL_HIGH_INT__MISC__MD_PLAYER) {
        randomized_items_shop_pool.push_back(0x0e);
    }

    std::vector<uint8_t> randomized_music_pool{};
    // 1) cafeteria jazz (outside)
    if (music_pool & UIContext::_UI_CONTEXT_MUSIC_OPTIONS_POOL__CAFETERIA_JAZZ_OUTSIDE) {
        randomized_music_pool.push_back(0x05);
    }

    // 2) metamorphosis (stranger boss fight)
    if (music_pool & UIContext::_UI_CONTEXT_MUSIC_OPTIONS_POOL__STRANGER_BOSS_MUSIC) {
        randomized_music_pool.push_back(0x06);
    }

    // 3) cafeteria jazz (inside)
    //   NOTE: The structure shows the same bit as "cafeteria jazz (outside)" (possibly a data quirk).
    if (music_pool & UIContext::_UI_CONTEXT_MUSIC_OPTIONS_POOL__CAFETERIA_JAZZ_OUTSIDE) {
        randomized_music_pool.push_back(0x09);
    }

    // 4) winning no.9
    if (music_pool & UIContext::_UI_CONTEXT_MUSIC_OPTIONS_POOL__WINNING_NO_9) {
        randomized_music_pool.push_back(0x0c);
    }

    // 5) M.I.S.T. (garage)
    if (music_pool & UIContext::_UI_CONTEXT_MUSIC_OPTIONS_POOL__MIST_GARAGE) {
        randomized_music_pool.push_back(0x11);
    }

    // 6) no.9 cutscene
    if (music_pool & UIContext::_UI_CONTEXT_MUSIC_OPTIONS_POOL__NO9_CUT_SCENE) {
        randomized_music_pool.push_back(0x12);
    }

    // 7) weird man (no.9 fight)
    if (music_pool & UIContext::_UI_CONTEXT_MUSIC_OPTIONS_POOL__WEIRD_MAN) {
        randomized_music_pool.push_back(0x14);
    }

    // 8) ambush! (akropolis fight)
    if (music_pool & UIContext::_UI_CONTEXT_MUSIC_OPTIONS_POOL__AMBUSH) {
        randomized_music_pool.push_back(0x15);
    }

    // 9) hunt in dryfield (dryfield day fight)
    if (music_pool & UIContext::_UI_CONTEXT_MUSIC_OPTIONS_POOL__HUNT_IN_DRYFIELD) {
        randomized_music_pool.push_back(0x16);
    }

    // 10) stealth assault (dryfield night fight)
    if (music_pool & UIContext::_UI_CONTEXT_MUSIC_OPTIONS_POOL__FIGHT_1) {
        randomized_music_pool.push_back(0x17);
    }

    // 11) the first encounter (stranger scene)
    if (music_pool & UIContext::_UI_CONTEXT_MUSIC_OPTIONS_POOL__FIRST_STRANGER_SCENE_EATING_CORPSE) {
        randomized_music_pool.push_back(0x1d);
    }

    // 12) aya again (akropolis approach scene)
    if (music_pool & UIContext::_UI_CONTEXT_MUSIC_OPTIONS_POOL__ACROPOLIS_APPROACH_SCENE) {
        randomized_music_pool.push_back(0x1f);
    }

    // 13) don't move (elevator scene)
    if (music_pool & UIContext::_UI_CONTEXT_MUSIC_OPTIONS_POOL__GO_OUT_OF_ACROPOLIS_ELEVATOR) {
        randomized_music_pool.push_back(0x20);
    }

    // 14) nightmare in the battlefield (first entering square)
    if (music_pool & UIContext::_UI_CONTEXT_MUSIC_OPTIONS_POOL__ACROPOLIS_PEACE_ENTERING_SQUARE) {
        randomized_music_pool.push_back(0x21);
    }

    // 15) deadly calm (akropolis peace)
    if (music_pool & UIContext::_UI_CONTEXT_MUSIC_OPTIONS_POOL__ACROPOLIS_PEACE_2) {
        randomized_music_pool.push_back(0x22);
    }

    // 16) watch out (akropolis peace 2)
    if (music_pool & UIContext::_UI_CONTEXT_MUSIC_OPTIONS_POOL__ACROPOLIS_PEACE_3) {
        randomized_music_pool.push_back(0x23);
    }

    // 17) watch out! (akropolis peace 3)
    if (music_pool & UIContext::_UI_CONTEXT_MUSIC_OPTIONS_POOL__ACROPOLIS_PEACE_4) {
        randomized_music_pool.push_back(0x24);
    }

    // 18) returning to the base (Baldwin cabinet)
    if (music_pool & UIContext::_UI_CONTEXT_MUSIC_OPTIONS_POOL__RETURNING_TO_MIST) {
        randomized_music_pool.push_back(0x25);
    }

    // 19) do something! (akropolis roof peace)
    if (music_pool & UIContext::_UI_CONTEXT_MUSIC_OPTIONS_POOL__ACROPOLIS_PEACE_5) {
        randomized_music_pool.push_back(0x26);
    }

    // 20) ghost town (dryfield day peace)
    if (music_pool & UIContext::_UI_CONTEXT_MUSIC_OPTIONS_POOL__DRYFIELD_DAY_PEACE_1) {
        randomized_music_pool.push_back(0x29);
    }

    // 21) Douglas' blues (meeting Doug)
    if (music_pool & UIContext::_UI_CONTEXT_MUSIC_OPTIONS_POOL__DRYFIELD_DAY_PEACE_2_MEETING_DOUGH) {
        randomized_music_pool.push_back(0x2a);
    }

    // 22) vagrants (bar music)
    if (music_pool & UIContext::_UI_CONTEXT_MUSIC_OPTIONS_POOL__DRYFIELD_BAR_MUSIC) {
        randomized_music_pool.push_back(0x2b);
    }

    // 23) water tower
    if (music_pool & UIContext::_UI_CONTEXT_MUSIC_OPTIONS_POOL__0x2c) {
        randomized_music_pool.push_back(0x2c);
    }

    // 24) hiding place (Doug's truck)
    if (music_pool & UIContext::_UI_CONTEXT_MUSIC_OPTIONS_POOL__DRYFIELD_DAY_PEACE_3) {
        randomized_music_pool.push_back(0x2d);
    }

    // 25) dryfield (dryfield day peace 2)
    if (music_pool & UIContext::_UI_CONTEXT_MUSIC_OPTIONS_POOL__DRYFIELD_DAY_PEACE_4) {
        randomized_music_pool.push_back(0x2e);
    }

    // 26) the bottom of the well
    if (music_pool & UIContext::_UI_CONTEXT_MUSIC_OPTIONS_POOL__0x2f) {
        randomized_music_pool.push_back(0x2f);
    }

    // 27) stealth assault
    if (music_pool & UIContext::_UI_CONTEXT_MUSIC_OPTIONS_POOL__0x30) {
        randomized_music_pool.push_back(0x30);
    }

    // 28) heaven-sent killer
    if (music_pool & UIContext::_UI_CONTEXT_MUSIC_OPTIONS_POOL__0x31) {
        randomized_music_pool.push_back(0x31);
    }

    // 29) from dusk til down (dryfield night peace)
    if (music_pool & UIContext::_UI_CONTEXT_MUSIC_OPTIONS_POOL__0x37) {
        randomized_music_pool.push_back(0x37);
    }

    // 30) gigantic burner (burner fight)
    //    Note: uses the same flag 0x37, so if set, also push_back(0x38)
    if (music_pool & UIContext::_UI_CONTEXT_MUSIC_OPTIONS_POOL__0x37) {
        randomized_music_pool.push_back(0x38);
    }

    // 31) dark field (dryfield night Doug)
    if (music_pool & UIContext::_UI_CONTEXT_MUSIC_OPTIONS_POOL__0x39) {
        randomized_music_pool.push_back(0x39);
    }

    // 32) Douglas' grief
    if (music_pool & UIContext::_UI_CONTEXT_MUSIC_OPTIONS_POOL__0x3a) {
        randomized_music_pool.push_back(0x3a);
    }

    // 33) voice of the mitochondria
    if (music_pool & UIContext::_UI_CONTEXT_MUSIC_OPTIONS_POOL__0x3b) {
        randomized_music_pool.push_back(0x3b);
    }

    // 34) pick up the gauntlet (chaser boss fight)
    if (music_pool & UIContext::_UI_CONTEXT_MUSIC_OPTIONS_POOL__0x3c) {
        randomized_music_pool.push_back(0x3c);
    }

    // 35) abandoned mine (mines peace)
    if (music_pool & UIContext::_UI_CONTEXT_MUSIC_OPTIONS_POOL__0x3d) {
        randomized_music_pool.push_back(0x3d);
    }

    // 36) chaser boss stand up???
    if (music_pool & UIContext::_UI_CONTEXT_MUSIC_OPTIONS_POOL__0x3e) {
        randomized_music_pool.push_back(0x3e);
    }

    // 37) breeding room sounds
    if (music_pool & UIContext::_UI_CONTEXT_MUSIC_OPTIONS_POOL__0x3f) {
        randomized_music_pool.push_back(0x3f);
    }

    // 38) into the shelter
    if (music_pool & UIContext::_UI_CONTEXT_MUSIC_OPTIONS_POOL__0x41) {
        randomized_music_pool.push_back(0x41);
    }

    // 39) crawling waste emperor (glutton boss fight)
    if (music_pool & UIContext::_UI_CONTEXT_MUSIC_OPTIONS_POOL__0x42) {
        randomized_music_pool.push_back(0x42);
    }

    // 40) hold your breath (gas puzzle music)
    if (music_pool & UIContext::_UI_CONTEXT_MUSIC_OPTIONS_POOL__0x43) {
        randomized_music_pool.push_back(0x43);
    }

    // 41) wipe out the creatures (shelter fight music 1)
    if (music_pool & UIContext::_UI_CONTEXT_MUSIC_OPTIONS_POOL__0x44) {
        randomized_music_pool.push_back(0x44);
    }

    // 42) stalker (brain stinger fight)
    if (music_pool & UIContext::_UI_CONTEXT_MUSIC_OPTIONS_POOL__0x45) {
        randomized_music_pool.push_back(0x45);
    }

    // 43) battle on the waterside (shelter sewer fight music)
    if (music_pool & UIContext::_UI_CONTEXT_MUSIC_OPTIONS_POOL__0x46) {
        randomized_music_pool.push_back(0x46);
    }

    // 44) inner most part of the shelter
    if (music_pool & UIContext::_UI_CONTEXT_MUSIC_OPTIONS_POOL__0x47) {
        randomized_music_pool.push_back(0x47);
    }

    // 45) 0x48 ???
    if (music_pool & UIContext::_UI_CONTEXT_MUSIC_OPTIONS_POOL__0x48) {
        randomized_music_pool.push_back(0x48);
    }

    // 46) mitochondria reactor
    if (music_pool & UIContext::_UI_CONTEXT_MUSIC_OPTIONS_POOL__0x49) {
        randomized_music_pool.push_back(0x49);
    }

    // 47) chase (glutton stage 2 fight)
    if (music_pool & UIContext::_UI_CONTEXT_MUSIC_OPTIONS_POOL__0x4a) {
        randomized_music_pool.push_back(0x4a);
    }

    // 48) passing through the sewer you'll find...
    if (music_pool & UIContext::_UI_CONTEXT_MUSIC_OPTIONS_POOL__0x4b) {
        randomized_music_pool.push_back(0x4b);
    }

    // 49) man made nature (entering neo ark)
    if (music_pool & UIContext::_UI_CONTEXT_MUSIC_OPTIONS_POOL__0x4c) {
        randomized_music_pool.push_back(0x4c);
    }

    // 50) ark (neo ark peace)
    if (music_pool & UIContext::_UI_CONTEXT_MUSIC_OPTIONS_POOL__0x4d) {
        randomized_music_pool.push_back(0x4d);
    }

    // 51) fool's paradise (neo ark peace 2)
    if (music_pool & UIContext::_UI_CONTEXT_MUSIC_OPTIONS_POOL__0x4e) {
        randomized_music_pool.push_back(0x4e);
    }

    // 52) sigh of relief
    if (music_pool & UIContext::_UI_CONTEXT_MUSIC_OPTIONS_POOL__SIGH_OF_RELIEF) {
        randomized_music_pool.push_back(0x50);
    }

    // 53) negative heritage (military music)
    if (music_pool & UIContext::_UI_CONTEXT_MUSIC_OPTIONS_POOL__0x51) {
        randomized_music_pool.push_back(0x51);
    }

    // 54) golem soldiers (golems after neo ark)
    if (music_pool & UIContext::_UI_CONTEXT_MUSIC_OPTIONS_POOL__0x52) {
        randomized_music_pool.push_back(0x52);
    }

    // 55) killing field
    if (music_pool & UIContext::_UI_CONTEXT_MUSIC_OPTIONS_POOL__0x53) {
        randomized_music_pool.push_back(0x53);
    }

    // 56) cruelty of eve's fate (eve room)
    if (music_pool & UIContext::_UI_CONTEXT_MUSIC_OPTIONS_POOL__CRUELTY_OF_EVES_FATE) {
        randomized_music_pool.push_back(0x55);
    }

    // 57) prestige of a nation (president's music)
    if (music_pool & UIContext::_UI_CONTEXT_MUSIC_OPTIONS_POOL__0x56) {
        randomized_music_pool.push_back(0x56);
    }

    // 58) intrusion (later shelter peace)
    if (music_pool & UIContext::_UI_CONTEXT_MUSIC_OPTIONS_POOL__0x59) {
        randomized_music_pool.push_back(0x59);
    }

    // 59) pod room
    if (music_pool & UIContext::_UI_CONTEXT_MUSIC_OPTIONS_POOL__0x5a) {
        randomized_music_pool.push_back(0x5a);
    }

    // 60) brace yourself (before brahman intense)
    if (music_pool & UIContext::_UI_CONTEXT_MUSIC_OPTIONS_POOL__0x5d) {
        randomized_music_pool.push_back(0x5d);
    }

    // 61) brahman
    if (music_pool & UIContext::_UI_CONTEXT_MUSIC_OPTIONS_POOL__0x5e) {
        randomized_music_pool.push_back(0x5e);
    }

    // 62) distorted evolution (eve fight)
    if (music_pool & UIContext::_UI_CONTEXT_MUSIC_OPTIONS_POOL__0x5f) {
        randomized_music_pool.push_back(0x5f);
    }

    // 63) logic of superpower (president music 2)
    if (music_pool & UIContext::_UI_CONTEXT_MUSIC_OPTIONS_POOL__0x60) {
        randomized_music_pool.push_back(0x60);
    }

    // 64) aya's diary
    if (music_pool & UIContext::_UI_CONTEXT_MUSIC_OPTIONS_POOL__0x61) {
        randomized_music_pool.push_back(0x61);
    }

    // 65) game over music
    if (music_pool & UIContext::_UI_CONTEXT_MUSIC_OPTIONS_POOL__0x62) {
        randomized_music_pool.push_back(0x62);
    }

    uint8_t starting_spell = spell_start_option > 3 ? (spell_start_option - 4) / 3 : 0;
    uint8_t starting_spell_level = spell_start_option > 3 ? (spell_start_option - 4) % 3 : 0;
    uint8_t spell_randomization_strategy = 0;
    switch (spell_start_option) {
    case _UI_CONTEXT_MAGIC_OPTIONS__STARTING_MAGIC__RANDOM_FIRST_LEVEL_SPELL:
        spell_randomization_strategy = 1;
        break;
    case _UI_CONTEXT_MAGIC_OPTIONS__STARTING_MAGIC__FULL_RANDOM_SPELL:
        spell_randomization_strategy = 2;
        break;
    case _UI_CONTEXT_MAGIC_OPTIONS__STARTING_MAGIC__RANDOM_FIRST_CIRCLE_SPELL:
        spell_randomization_strategy = 3;
        break;
    case _UI_CONTEXT_MAGIC_OPTIONS__STARTING_MAGIC__NO_RANDOM_SPELL:
        spell_randomization_strategy = 4;
        break;
    default:
        spell_randomization_strategy = 0;
        break;
    }

    uint8_t starting_weapon = 0;
    uint8_t weapon_randomization_strategy = 0;

    switch (items_start_options[0]) {
    case _UI_CONTEXT_ITEMS_OPTIONS__STARTING_WEAPON__RANDOM_STARTING_WEAPON:
        starting_weapon = 0;
        weapon_randomization_strategy = 1;
        break;
    case _UI_CONTEXT_ITEMS_OPTIONS__STARTING_WEAPON__FULL_RANDOM_WEAPON:
        starting_weapon = 0;
        weapon_randomization_strategy = 2;
        break;
    case _UI_CONTEXT_ITEMS_OPTIONS__STARTING_WEAPON__NO_RANDOM_WEAPON:
        starting_weapon = 0;
        weapon_randomization_strategy = 3;
        break;
    case _UI_CONTEXT_ITEMS_OPTIONS__STARTING_WEAPON__TONFA_ONLY_START:
        starting_weapon = 0x92;
        weapon_randomization_strategy = 0;
        break;
    case _UI_CONTEXT_ITEMS_OPTIONS__STARTING_WEAPON__M93R:
        starting_weapon = 0x81;
        weapon_randomization_strategy = 0;
        break;
    case _UI_CONTEXT_ITEMS_OPTIONS__STARTING_WEAPON__M950:
        starting_weapon = 0x82;
        weapon_randomization_strategy = 0;
        break;
    case _UI_CONTEXT_ITEMS_OPTIONS__STARTING_WEAPON__P08:
        starting_weapon = 0x83;
        weapon_randomization_strategy = 0;
        break;
    case _UI_CONTEXT_ITEMS_OPTIONS__STARTING_WEAPON__P229:
        starting_weapon = 0x84;
        weapon_randomization_strategy = 0;
        break;
    case _UI_CONTEXT_ITEMS_OPTIONS__STARTING_WEAPON__MONGOOSE:
        starting_weapon = 0x88;
        weapon_randomization_strategy = 0;
        break;
    case _UI_CONTEXT_ITEMS_OPTIONS__STARTING_WEAPON__GRENADE_PISTOL:
        starting_weapon = 0x8a;
        weapon_randomization_strategy = 0;
        break;
    case _UI_CONTEXT_ITEMS_OPTIONS__STARTING_WEAPON__MM1:
        starting_weapon = 0x8b;
        weapon_randomization_strategy = 0;
        break;
    case _UI_CONTEXT_ITEMS_OPTIONS__STARTING_WEAPON__PA3:
        starting_weapon = 0x8c;
        weapon_randomization_strategy = 0;
        break;
    case _UI_CONTEXT_ITEMS_OPTIONS__STARTING_WEAPON__SP12:
        starting_weapon = 0x8d;
        weapon_randomization_strategy = 0;
        break;
    case _UI_CONTEXT_ITEMS_OPTIONS__STARTING_WEAPON__AS12:
        starting_weapon = 0x8e;
        weapon_randomization_strategy = 0;
        break;
    case _UI_CONTEXT_ITEMS_OPTIONS__STARTING_WEAPON__M4A1:
        starting_weapon = 0x8f;
        weapon_randomization_strategy = 0;
        break;
    case _UI_CONTEXT_ITEMS_OPTIONS__STARTING_WEAPON__M249:
        starting_weapon = 0x90;
        weapon_randomization_strategy = 0;
        break;
    case _UI_CONTEXT_ITEMS_OPTIONS__STARTING_WEAPON__HYPERVELOCITY:
        starting_weapon = 0x95;
        weapon_randomization_strategy = 0;
        break;
    case _UI_CONTEXT_ITEMS_OPTIONS__STARTING_WEAPON__GUNBLADE:
        starting_weapon = 0x96;
        weapon_randomization_strategy = 0;
        break;
    case _UI_CONTEXT_ITEMS_OPTIONS__STARTING_WEAPON__MP5A5:
        starting_weapon = 0x9d;
        weapon_randomization_strategy = 0;
        break;
    case _UI_CONTEXT_ITEMS_OPTIONS__STARTING_WEAPON__P08_WITH_SNAIL:
        starting_weapon = 0x80;
        weapon_randomization_strategy = 0;
        break;
    case _UI_CONTEXT_ITEMS_OPTIONS__STARTING_WEAPON__M4A1_WITH_CLIP:
        starting_weapon = 0x93;
        weapon_randomization_strategy = 0;
        break;
    case _UI_CONTEXT_ITEMS_OPTIONS__STARTING_WEAPON__M4A1_WITH_2_CLIPS:
        starting_weapon = 0x94;
        weapon_randomization_strategy = 0;
        break;
    case _UI_CONTEXT_ITEMS_OPTIONS__STARTING_WEAPON__M4A1_WITH_HAMMER:
        starting_weapon = 0x98;
        weapon_randomization_strategy = 0;
        break;
    case _UI_CONTEXT_ITEMS_OPTIONS__STARTING_WEAPON__M4A1_WITH_M9:
        starting_weapon = 0x99;
        weapon_randomization_strategy = 0;
        break;
    case _UI_CONTEXT_ITEMS_OPTIONS__STARTING_WEAPON__M4A1_WITH_M203:
        starting_weapon = 0x9a;
        weapon_randomization_strategy = 0;
        break;
    case _UI_CONTEXT_ITEMS_OPTIONS__STARTING_WEAPON__M4A1_WITH_PYKE:
        starting_weapon = 0x9b;
        weapon_randomization_strategy = 0;
        break;
    case _UI_CONTEXT_ITEMS_OPTIONS__STARTING_WEAPON__M4A1_WITH_JAVELIN:
        starting_weapon = 0x9c;
        weapon_randomization_strategy = 0;
        break;
    case _UI_CONTEXT_ITEMS_OPTIONS__STARTING_WEAPON__MP5A5_WITH_CLIP:
        starting_weapon = 0x9e;
        weapon_randomization_strategy = 0;
        break;
    case _UI_CONTEXT_ITEMS_OPTIONS__STARTING_WEAPON__MP5A5_WITH_2_CLIPS:
        starting_weapon = 0x9f;
        weapon_randomization_strategy = 0;
        break;
    default:
        break;
    }

    uint8_t starting_armor = 0;
    uint8_t armor_randomization_strategy = 0;

    switch (items_start_options[1]) {
    case _UI_CONTEXT_ITEMS_OPTIONS__STARTING_ARMOR__RANDOM_STARTING_ARMOR:
        starting_armor = 0;
        armor_randomization_strategy = 1;
        break;
    case _UI_CONTEXT_ITEMS_OPTIONS__STARTING_ARMOR__FULL_RANDOM_ARMOR:
        starting_armor = 0;
        armor_randomization_strategy = 2;
        break;
    case _UI_CONTEXT_ITEMS_OPTIONS__STARTING_ARMOR__NO_RANDOM_ARMOR:
        starting_armor = 0;
        armor_randomization_strategy = 3;
        break;
    case _UI_CONTEXT_ITEMS_OPTIONS__STARTING_ARMOR__LEATHER_JACKET:
        starting_armor = 0x60;
        armor_randomization_strategy = 0;
        break;
    case _UI_CONTEXT_ITEMS_OPTIONS__STARTING_ARMOR__TACTICAL_ARMOR:
        starting_armor = 0x61;
        armor_randomization_strategy = 0;
        break;
    case _UI_CONTEXT_ITEMS_OPTIONS__STARTING_ARMOR__COMBAT_ARMOR:
        starting_armor = 0x62;
        armor_randomization_strategy = 0;
        break;
    case _UI_CONTEXT_ITEMS_OPTIONS__STARTING_ARMOR__ASSAULT_SUIT:
        starting_armor = 0x63;
        armor_randomization_strategy = 0;
        break;
    case _UI_CONTEXT_ITEMS_OPTIONS__STARTING_ARMOR__PASGT_VEST:
        starting_armor = 0x64;
        armor_randomization_strategy = 0;
        break;
    case _UI_CONTEXT_ITEMS_OPTIONS__STARTING_ARMOR__TACTICAL_VEST:
        starting_armor = 0x65;
        armor_randomization_strategy = 0;
        break;
    case _UI_CONTEXT_ITEMS_OPTIONS__STARTING_ARMOR__EOD_SUIT:
        starting_armor = 0x66;
        armor_randomization_strategy = 0;
        break;
    case _UI_CONTEXT_ITEMS_OPTIONS__STARTING_ARMOR__TURTLE_VEST:
        starting_armor = 0x67;
        armor_randomization_strategy = 0;
        break;
    case _UI_CONTEXT_ITEMS_OPTIONS__STARTING_ARMOR__CHICKEN_PLATE:
        starting_armor = 0x68;
        armor_randomization_strategy = 0;
        break;
    case _UI_CONTEXT_ITEMS_OPTIONS__STARTING_ARMOR__NBC_SUIT:
        starting_armor = 0x69;
        armor_randomization_strategy = 0;
        break;
    case _UI_CONTEXT_ITEMS_OPTIONS__STARTING_ARMOR__PSY_SUIT:
        starting_armor = 0x6a;
        armor_randomization_strategy = 0;
        break;
    case _UI_CONTEXT_ITEMS_OPTIONS__STARTING_ARMOR__AYA_SPECIAL:
        starting_armor = 0x6b;
        armor_randomization_strategy = 0;
        break;
    case _UI_CONTEXT_ITEMS_OPTIONS__STARTING_ARMOR__SHOULDER_HOLSTER:
        starting_armor = 0x6c;
        armor_randomization_strategy = 0;
        break;
    case _UI_CONTEXT_ITEMS_OPTIONS__STARTING_ARMOR__MONK_ROBE:
        starting_armor = 0x6d;
        armor_randomization_strategy = 0;
        break;
    default:
        break;
    }

    uint8_t dryfield_armor = 0;
    uint8_t dryfield_armor_randomization_strategy = 0;

    switch (items_start_options[2]) {
    case _UI_CONTEXT_ITEMS_OPTIONS__STARTING_ARMOR__RANDOM_STARTING_ARMOR:
        dryfield_armor = 0;
        dryfield_armor_randomization_strategy = 1;
        break;
    case _UI_CONTEXT_ITEMS_OPTIONS__STARTING_ARMOR__FULL_RANDOM_ARMOR:
        dryfield_armor = 0;
        dryfield_armor_randomization_strategy = 2;
        break;
    case _UI_CONTEXT_ITEMS_OPTIONS__STARTING_ARMOR__NO_RANDOM_ARMOR:
        dryfield_armor = 0;
        dryfield_armor_randomization_strategy = 3;
        break;
    case _UI_CONTEXT_ITEMS_OPTIONS__STARTING_ARMOR__LEATHER_JACKET:
        dryfield_armor = 0x60;
        dryfield_armor_randomization_strategy = 0;
        break;
    case _UI_CONTEXT_ITEMS_OPTIONS__STARTING_ARMOR__TACTICAL_ARMOR:
        dryfield_armor = 0x61;
        dryfield_armor_randomization_strategy = 0;
        break;
    case _UI_CONTEXT_ITEMS_OPTIONS__STARTING_ARMOR__COMBAT_ARMOR:
        dryfield_armor = 0x62;
        dryfield_armor_randomization_strategy = 0;
        break;
    case _UI_CONTEXT_ITEMS_OPTIONS__STARTING_ARMOR__ASSAULT_SUIT:
        dryfield_armor = 0x63;
        dryfield_armor_randomization_strategy = 0;
        break;
    case _UI_CONTEXT_ITEMS_OPTIONS__STARTING_ARMOR__PASGT_VEST:
        dryfield_armor = 0x64;
        dryfield_armor_randomization_strategy = 0;
        break;
    case _UI_CONTEXT_ITEMS_OPTIONS__STARTING_ARMOR__TACTICAL_VEST:
        dryfield_armor = 0x65;
        dryfield_armor_randomization_strategy = 0;
        break;
    case _UI_CONTEXT_ITEMS_OPTIONS__STARTING_ARMOR__EOD_SUIT:
        dryfield_armor = 0x66;
        dryfield_armor_randomization_strategy = 0;
        break;
    case _UI_CONTEXT_ITEMS_OPTIONS__STARTING_ARMOR__TURTLE_VEST:
        dryfield_armor = 0x67;
        dryfield_armor_randomization_strategy = 0;
        break;
    case _UI_CONTEXT_ITEMS_OPTIONS__STARTING_ARMOR__CHICKEN_PLATE:
        dryfield_armor = 0x68;
        dryfield_armor_randomization_strategy = 0;
        break;
    case _UI_CONTEXT_ITEMS_OPTIONS__STARTING_ARMOR__NBC_SUIT:
        dryfield_armor = 0x69;
        dryfield_armor_randomization_strategy = 0;
        break;
    case _UI_CONTEXT_ITEMS_OPTIONS__STARTING_ARMOR__PSY_SUIT:
        dryfield_armor = 0x6a;
        dryfield_armor_randomization_strategy = 0;
        break;
    case _UI_CONTEXT_ITEMS_OPTIONS__STARTING_ARMOR__AYA_SPECIAL:
        dryfield_armor = 0x6b;
        dryfield_armor_randomization_strategy = 0;
        break;
    case _UI_CONTEXT_ITEMS_OPTIONS__STARTING_ARMOR__SHOULDER_HOLSTER:
        dryfield_armor = 0x6c;
        dryfield_armor_randomization_strategy = 0;
        break;
    case _UI_CONTEXT_ITEMS_OPTIONS__STARTING_ARMOR__MONK_ROBE:
        dryfield_armor = 0x6d;
        dryfield_armor_randomization_strategy = 0;
        break;
    default:
        break;
    }

    std::vector<uint16_t> costs{};
    for (size_t i = 0; i < sizeof(items_shop_costs) / sizeof(items_shop_costs[0]); i++) {
        costs.push_back(items_shop_costs[i]);
    }

    return delivery::config_t{
        .flags =
            delivery::Flags{
                .randomize_enemies = (flags & UIContext::_UI_CONTEXT_FLAG__RANDOMIZE_ENEMIES) != 0,
                .randomize_items = (flags & UIContext::_UI_CONTEXT_FLAG__RANDOMIZE_ITEMS) != 0,
                .randomize_shops = (flags & UIContext::_UI_CONTEXT_FLAG__RANDOMIZE_ITEMS) != 0,
                .randomize_music = (flags & UIContext::_UI_CONTEXT_FLAG__RANDOMIZE_MUSIC) != 0,
                .randomize_magic = (flags & UIContext::_UI_CONTEXT_FLAG__RANDOMIZE_MAGIC) != 0,
                .enhanced_bosses = (flags & UIContext::_UI_CONTEXT_FLAG__ENHANCED_BOSSES) != 0,
                .full_ammo = (flags & UIContext::_UI_CONTEXT_FLAG__FULL_AMMO_START) != 0, // TODO: fix it
                .cloak_golem_mult = (flags & UIContext::_UI_CONTEXT_FLAG__CLOAK_GOLEM_MULT) != 0,
                .enable_hud_coloring = (flags & UIContext::_UI_CONTEXT_FLAG__HUD_COLORING) != 0,
                .enable_special_modes =
                    (flags & UIContext::_UI_CONTEXT_FLAG__ENABLE_ALL_MODES) != 0, // TODO: fix it
                .enable_map_in_fight = false,                                     // TODO: fix it
                .additional_armors = (flags & UIContext::_UI_CONTEXT_FLAG__NEW_ARMORS) != 0,
                .magic_pickup_mode = (flags & UIContext::_UI_CONTEXT_FLAG__MAGIC_ELEMENTS) != 0,
                .active_time_battle = (flags & UIContext::_UI_CONTEXT_FLAG__ACTIVE_TIME_BATTLE) != 0,
                .increased_inv = (flags & UIContext::_UI_CONTEXT_FLAG__INCREASED_INV_FRAMES) != 0,
                .no_white_screen = (flags & UIContext::_UI_CONTEXT_FLAG__NO_WHITE_SCREEN) != 0,
                .no_win_pose = (flags & UIContext::_UI_CONTEXT_FLAG__NO_END_BATTLE_ANIM) != 0,
                .no_turn = (flags & UIContext::_UI_CONTEXT_FLAG__NO_TURN) != 0,
                .inventory_mode = (flags & UIContext::_UI_CONTEXT_FLAG__INVENTORY_MODE) != 0,
            },
        .ammo_boxes_option = static_cast<delivery::AmmoBoxesOption>(ammo_boxes_option),
        .ammo_boxes_rando_option = static_cast<delivery::AmmoBoxesRandomization>(ammo_boxes_rando_option),
        .enemy_enhancement_option = static_cast<delivery::EnemyEnhancementOption>(enemy_enhancement_option),
        .randomized_stages{0, 1, 2, 3, 4, 5},
        .randomized_enemies_pool = randomized_enemies_pool,
        .randomized_items_drop_pool = randomized_items_drop_pool,
        .randomized_items_shop_pool = randomized_items_shop_pool,
        .randomized_music_pool = randomized_music_pool,
        .shop_costs = costs,
        .starting_spell = starting_spell,
        .starting_spell_level = starting_spell_level,
        .starting_weapon = starting_weapon,
        .starting_armor = starting_armor,
        .dryfield_armor = dryfield_armor,
        .spell_randomization_strategy = spell_randomization_strategy,
        .weapon_randomization_strategy = weapon_randomization_strategy,
        .armor_randomization_strategy = armor_randomization_strategy,
        .dryfield_armor_randomization_strategy = dryfield_armor_randomization_strategy,
        .shop_cost_multiplier = (float)items_shop_cost_mult / 100,
        .spell_cost_multiplier = (float)spell_cost_mult / 100,
        .item_classes_distribution = {(double)items_dist[0] / 100, (double)items_dist[1] / 100,
                                      (double)items_dist[2] / 100, (double)items_dist[3] / 100,
                                      (double)items_dist[4] / 100},
        .enemy_damage = enemies_mults[1],
        .enemy_hp = enemies_mults[0],
        .boss_damage = enemies_mults[3],
        .boss_hp = enemies_mults[2],
        .player_model_option = player_model_option,
        .hud_color_r = hud_color_r,
        .hud_color_g = hud_color_g,
        .hud_color_b = hud_color_b,
        .seed = seed,
        .input_file_1 = disc_1,
        .input_file_2 = disc_2,
        .randomization_state_string = serialize(),
        .boss_prototype = (flags & _UI_CONTEXT_FLAG__BOSS_PROTOTYPE) != 0,
        .stranger_row = (flags & _UI_CONTEXT_FLAG__STRANGER_ROW) != 0,
        .rupert_revolver = (flags & _UI_CONTEXT_FLAG__RUPERT_REVOLVER) != 0,
        .kyle_p229 = (flags & _UI_CONTEXT_FLAG__KYLE_P229) != 0};
}
std::string UIContext::base64_encode(const std::string &bytes_to_encode) {
    unsigned char const *bytes = reinterpret_cast<const unsigned char *>(bytes_to_encode.data());
    unsigned int in_len = bytes_to_encode.size();
    std::string ret;
    int i = 0;
    int j = 0;
    unsigned char char_array_3[3];
    unsigned char char_array_4[4];

    while (in_len--) {
        char_array_3[i++] = *(bytes++);
        if (i == 3) {
            char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
            char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
            char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
            char_array_4[3] = char_array_3[2] & 0x3f;

            for (i = 0; (i < 4); i++) {
                ret += _BASE64_CHARS[char_array_4[i]];
            }
            i = 0;
        }
    }

    if (i) {
        for (j = i; j < 3; j++) {
            char_array_3[j] = '\0';
        }

        char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
        char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
        char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
        char_array_4[3] = char_array_3[2] & 0x3f;

        for (j = 0; (j < i + 1); j++) {
            ret += _BASE64_CHARS[char_array_4[j]];
        }

        while ((i++ < 3)) {
            ret += '=';
        }
    }

    return ret;
}

std::string UIContext::base64_decode(const std::string &encoded_string) {
    int in_len = encoded_string.size();
    int i = 0;
    int j = 0;
    int in_ = 0;
    unsigned char char_array_4[4], char_array_3[3];
    std::string ret;

    while (in_len-- && (encoded_string[in_] != '=') && is_base64(encoded_string[in_])) {
        char_array_4[i++] = encoded_string[in_];
        in_++;
        if (i == 4) {
            for (i = 0; i < 4; i++) {
                char_array_4[i] = _BASE64_CHARS.find(char_array_4[i]);
            }

            char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
            char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
            char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

            for (i = 0; (i < 3); i++) {
                ret += char_array_3[i];
            }
            i = 0;
        }
    }

    if (i) {
        for (j = i; j < 4; j++) {
            char_array_4[j] = 0;
        }

        for (j = 0; j < 4; j++) {
            char_array_4[j] = _BASE64_CHARS.find(char_array_4[j]);
        }

        char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
        char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
        char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

        for (j = 0; (j < i - 1); j++) {
            ret += char_array_3[j];
        }
    }

    return ret;
}
