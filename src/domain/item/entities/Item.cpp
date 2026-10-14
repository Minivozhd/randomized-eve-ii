#include "domain/item/entities/Item.hpp"

namespace domain::item {

using domain::game::AKROPOLIS_LEVEL;
using domain::game::BASE_LEVEL;
using domain::game::MINES_AND_SHELTER_LEVEL;
using domain::game::NEOARK_AND_SHELTER_LEVEL;
using domain::game::pe2_level_name_t;

std::map<uint8_t, pe2_item_t> items_map{
    // Leather jacket
    {
        0x60,
        pe2_item_t{
            .id = 0x60,
            .type = UNIQUE,
            .rarity = COMMON,
            .item_class = ARMOR,
            .additional_tag = NONE,
            .value = 50,
        },
    },
    // Assault suit
    {
        0x63,
        pe2_item_t{
            .id = 0x63,
            .type = UNIQUE,
            .rarity = COMMON,
            .item_class = ARMOR,
            .additional_tag = NONE,
            .value = 50,
        },
    },
    // PASGT vest
    {
        0x64,
        pe2_item_t{
            .id = 0x64,
            .type = UNIQUE,
            .rarity = COMMON,
            .item_class = ARMOR,
            .additional_tag = NONE,
            .value = 55,
        },
    },
    // Turtle vest
    {
        0x67,
        pe2_item_t{
            .id = 0x67,
            .type = UNIQUE,
            .rarity = COMMON,
            .item_class = ARMOR,
            .additional_tag = NONE,
            .value = 65,
        },
    },
    // Shoulder holster
    {
        0x6c,
        pe2_item_t{
            .id = 0x6c,
            .type = UNIQUE,
            .rarity = COMMON,
            .item_class = ARMOR,
            .additional_tag = NONE,
            .value = 65,
        },
    },
    // MD player
    {
        0x0e,
        pe2_item_t{
            .id = 0x0e,
            .type = UNIQUE,
            .rarity = COMMON,
            .item_class = ARTIFACT,
            .additional_tag = NONE,
            .value = 40,
        },
    },
    // Hunter goggles
    {
        0x3f,
        pe2_item_t{
            .id = 0x3f,
            .type = UNIQUE,
            .rarity = COMMON,
            .item_class = ARTIFACT,
            .additional_tag = NONE,
            .value = 40,
        },
    },
    // GPS
    {
        0x40,
        pe2_item_t{
            .id = 0x40,
            .type = UNIQUE,
            .rarity = COMMON,
            .item_class = ARTIFACT,
            .additional_tag = NONE,
        },
    },
    // Snail Magazine
    {
        0x0c,
        pe2_item_t{
            .id = 0x0c,
            .type = UNIQUE,
            .rarity = RARE,
            .item_class = ATTACHMENT,
            .main_tag = PISTOL,
            .additional_tag = P08,
        },
    },
    // MP5A5 Magazine
    {
        0x09,
        pe2_item_t{
            .id = 0x09,
            .type = UNIQUE,
            .rarity = RARE,
            .item_class = ATTACHMENT,
            .main_tag = PISTOL,
            .additional_tag = MP5AP5,
        },
    },
    // M4a1 Magazine
    {
        0x0a,
        pe2_item_t{
            .id = 0x0a,
            .type = UNIQUE,
            .rarity = RARE,
            .item_class = ATTACHMENT,
            .main_tag = RIFLE,
            .additional_tag = M4A1,
        },
    },
    // M9 knife M4A1 attachment
    {
        0x46,
        pe2_item_t{
            .id = 0x46,
            .type = UNIQUE,
            .rarity = RARE,
            .item_class = ATTACHMENT,
            .main_tag = RIFLE,
            .additional_tag = M4A1,
        },
    },
    // Combat armor
    {
        0x62,
        pe2_item_t{
            .id = 0x62,
            .type = UNIQUE,
            .rarity = RARE,
            .item_class = ARMOR,
            .additional_tag = NONE,
            .value = 75,
        },
    },
    // Tactical vest
    {
        0x65,
        pe2_item_t{
            .id = 0x65,
            .type = UNIQUE,
            .rarity = RARE,
            .item_class = ARMOR,
            .additional_tag = NONE,
            .value = 100,
        },
    },
    // Chicken plate
    {
        0x68,
        pe2_item_t{
            .id = 0x68,
            .type = UNIQUE,
            .rarity = RARE,
            .item_class = ARMOR,
            .additional_tag = NONE,
            .value = 80,
        },
    },
    // NBC suit
    {
        0x69,
        pe2_item_t{
            .id = 0x69,
            .type = UNIQUE,
            .rarity = RARE,
            .item_class = ARMOR,
            .additional_tag = NONE,
            .value = 90,
        },
    },
    // PsySuit
    {
        0x6a,
        pe2_item_t{
            .id = 0x6a,
            .type = UNIQUE,
            .rarity = RARE,
            .item_class = ARMOR,
            .additional_tag = NONE,
            .value = 85,
        },
    },
    // P08 with snail magazine
    {
        0x80,
        pe2_item_t{
            .id = 0x80,
            .type = UNIQUE,
            .rarity = LEGENDARY,
            .item_class = WEAPON,
            .main_tag = PISTOL,
            .additional_tag = NONE,
            .value = 250,
        },
    },
    // M93R
    {
        0x81,
        pe2_item_t{
            .id = 0x81,
            .type = UNIQUE,
            .rarity = COMMON,
            .item_class = WEAPON,
            .main_tag = PISTOL,
            .additional_tag = NONE,
            .value = 65,
        },
    },
    // M950
    {
        0x82,
        pe2_item_t{
            .id = 0x82,
            .type = UNIQUE,
            .rarity = RARE,
            .item_class = WEAPON,
            .main_tag = PISTOL,
            .additional_tag = NONE,
            .value = 100,
        },
    },
    // P08
    {
        0x83,
        pe2_item_t{
            .id = 0x83,
            .type = UNIQUE,
            .rarity = RARE,
            .item_class = WEAPON,
            .main_tag = PISTOL,
            .additional_tag = NONE,
            .value = 75,
        },
    },
    // P229
    {
        0x84,
        pe2_item_t{
            .id = 0x84,
            .type = UNIQUE,
            .rarity = RARE,
            .item_class = WEAPON,
            .main_tag = PISTOL,
            .additional_tag = NONE,
            .value = 125,
        },
    },
    // Grenade pistol
    {
        0x8a,
        pe2_item_t{
            .id = 0x8a,
            .type = UNIQUE,
            .rarity = RARE,
            .item_class = WEAPON,
            .main_tag = GRENADE,
            .additional_tag = NONE,
            .value = 150,
        },
    },
    // Shotgun PA3
    {
        0x8c,
        pe2_item_t{
            .id = 0x8c,
            .type = UNIQUE,
            .rarity = RARE,
            .item_class = WEAPON,
            .main_tag = SHOTGUN,
            .additional_tag = NONE,
            .value = 75,
        },
    },
    // M4A1
    {
        0x8f,
        pe2_item_t{
            .id = 0x8f,
            .type = UNIQUE,
            .rarity = RARE,
            .item_class = WEAPON,
            .main_tag = RIFLE,
            .additional_tag = NONE,
            .value = 100,
        },
    },
    // M4A1 + 1 clip
    {
        0x93,
        pe2_item_t{
            .id = 0x93,
            .type = UNIQUE,
            .rarity = LEGENDARY,
            .item_class = WEAPON,
            .main_tag = RIFLE,
            .additional_tag = NONE,
            .value = 250,
        },
    },
    // M4A1 + 2 clips
    {
        0x94,
        pe2_item_t{
            .id = 0x94,
            .type = UNIQUE,
            .rarity = LEGENDARY,
            .item_class = WEAPON,
            .main_tag = RIFLE,
            .additional_tag = NONE,
            .value = 250,
        },
    },
    // M4A1 + hammer
    {
        0x98,
        pe2_item_t{
            .id = 0x94,
            .type = UNIQUE,
            .rarity = LEGENDARY,
            .item_class = WEAPON,
            .main_tag = RIFLE,
            .additional_tag = NONE,
            .value = 250,
        },
    },
    // M4A1 + knife
    {
        0x99,
        pe2_item_t{
            .id = 0x94,
            .type = UNIQUE,
            .rarity = LEGENDARY,
            .item_class = WEAPON,
            .main_tag = RIFLE,
            .additional_tag = NONE,
            .value = 250,
        },
    },
    // M4A1 + grenade
    {
        0x9a,
        pe2_item_t{
            .id = 0x94,
            .type = UNIQUE,
            .rarity = LEGENDARY,
            .item_class = WEAPON,
            .main_tag = RIFLE,
            .additional_tag = NONE,
            .value = 250,
        },
    },
    // M4A1 + pyke
    {
        0x9b,
        pe2_item_t{
            .id = 0x94,
            .type = UNIQUE,
            .rarity = LEGENDARY,
            .item_class = WEAPON,
            .main_tag = RIFLE,
            .additional_tag = NONE,
            .value = 250,
        },
    },
    // M4A1 + javelin
    {
        0x9c,
        pe2_item_t{
            .id = 0x94,
            .type = UNIQUE,
            .rarity = LEGENDARY,
            .item_class = WEAPON,
            .main_tag = RIFLE,
            .additional_tag = NONE,
            .value = 250,
        },
    },
    // MP5A5
    {
        0x9d,
        pe2_item_t{
            .id = 0x9d,
            .type = UNIQUE,
            .rarity = RARE,
            .item_class = WEAPON,
            .main_tag = PISTOL,
            .additional_tag = NONE,
            .value = 150,
        },
    },
    // MP5A5 + 1 clip
    {
        0x9e,
        pe2_item_t{
            .id = 0x9d,
            .type = UNIQUE,
            .rarity = LEGENDARY,
            .item_class = WEAPON,
            .main_tag = PISTOL,
            .additional_tag = NONE,
            .value = 250,
        },
    },
    // MP5A5 + 2 clips
    {
        0x9e,
        pe2_item_t{
            .id = 0x9d,
            .type = UNIQUE,
            .rarity = LEGENDARY,
            .item_class = WEAPON,
            .main_tag = PISTOL,
            .additional_tag = NONE,
            .value = 250,
        },
    },
    // Hammer M4A1 attachment
    {
        0x42,
        pe2_item_t{
            .id = 0x42,
            .type = UNIQUE,
            .rarity = RARE,
            .item_class = ATTACHMENT,
            .main_tag = RIFLE,
            .additional_tag = M4A1,
        },
    },
    // Pyke M4A1 attachment
    {
        0x43,
        pe2_item_t{
            .id = 0x43,
            .type = UNIQUE,
            .rarity = RARE,
            .item_class = ATTACHMENT,
            .main_tag = RIFLE,
            .additional_tag = M4A1,
        },
    },
    // Javelin M4A1 attachment
    {
        0x44,
        pe2_item_t{
            .id = 0x44,
            .type = UNIQUE,
            .rarity = LEGENDARY,
            .item_class = ATTACHMENT,
            .main_tag = RIFLE,
            .additional_tag = M4A1,
        },
    },
    // Grenade M4A1 attachment
    {
        0x45,
        pe2_item_t{
            .id = 0x45,
            .type = UNIQUE,
            .rarity = LEGENDARY,
            .item_class = ATTACHMENT,
            .main_tag = RIFLE,
            .additional_tag = M4A1,
        },
    },
    // Crystal skull
    {
        0x36,
        pe2_item_t{
            .id = 0x36,
            .type = UNIQUE,
            .rarity = LEGENDARY,
            .item_class = ARTIFACT,
            .additional_tag = NONE,
            .value = 150,
        },
    },
    // Medicine wheel
    {
        0x37,
        pe2_item_t{
            .id = 0x37,
            .type = UNIQUE,
            .rarity = LEGENDARY,
            .item_class = ARTIFACT,
            .additional_tag = NONE,
            .value = 150,
        },
    },
    // Holy water
    {
        0x38,
        pe2_item_t{
            .id = 0x38,
            .type = UNIQUE,
            .rarity = LEGENDARY,
            .item_class = ARTIFACT,
            .additional_tag = NONE,
            .value = 150,
        },
    },
    // Ofuda charm
    {
        0x39,
        pe2_item_t{
            .id = 0x39,
            .type = UNIQUE,
            .rarity = LEGENDARY,
            .item_class = ARTIFACT,
            .additional_tag = NONE,
            .value = 150,
        },
    },
    // Tactical armor
    {
        0x61,
        pe2_item_t{
            .id = 0x61,
            .type = UNIQUE,
            .rarity = LEGENDARY,
            .item_class = ARMOR,
            .additional_tag = NONE,
            .value = 125,
        },
    },
    // EOD suit
    {
        0x66,
        pe2_item_t{
            .id = 0x66,
            .type = UNIQUE,
            .rarity = LEGENDARY,
            .item_class = ARMOR,
            .additional_tag = NONE,
            .value = 150,
        },
    },
    // Aya Special
    {
        0x6b,
        pe2_item_t{
            .id = 0x6b,
            .type = UNIQUE,
            .rarity = LEGENDARY,
            .item_class = ARMOR,
            .additional_tag = NONE,
            .value = 165,
        },
    },
    // Mongoose
    {
        0x88,
        pe2_item_t{
            .id = 0x88,
            .type = UNIQUE,
            .rarity = LEGENDARY,
            .item_class = WEAPON,
            .main_tag = MAGNUM,
            .additional_tag = NONE,
            .value = 200,
        },
    },
    // Grenade launcher MM1
    {
        0x8b,
        pe2_item_t{
            .id = 0x8b,
            .type = UNIQUE,
            .rarity = LEGENDARY,
            .item_class = WEAPON,
            .main_tag = GRENADE,
            .additional_tag = NONE,
            .value = 225,
        },
    },
    // Shotgun SP12
    {
        0x8d,
        pe2_item_t{
            .id = 0x8d,
            .type = UNIQUE,
            .rarity = LEGENDARY,
            .item_class = WEAPON,
            .main_tag = SHOTGUN,
            .additional_tag = NONE,
            .value = 150,
        },
    },
    // Shotgun AS12
    {
        0x8e,
        pe2_item_t{
            .id = 0x8e,
            .type = UNIQUE,
            .rarity = LEGENDARY,
            .item_class = WEAPON,
            .main_tag = SHOTGUN,
            .additional_tag = NONE,
            .value = 200,
        },
    },
    // MONK Robe
    {
        0x6d,
        pe2_item_t{
            .id = 0x6d,
            .type = UNIQUE,
            .rarity = IMPOSSIBLE,
            .item_class = ARMOR,
            .additional_tag = NONE,
            .value = 500,
        },
    },
    // M249 machine gun
    {
        0x90,
        pe2_item_t{
            .id = 0x90,
            .type = UNIQUE,
            .rarity = IMPOSSIBLE,
            .item_class = WEAPON,
            .main_tag = RIFLE,
            .additional_tag = NONE,
            .value = 500,
        },
    },
    // Hypervelocity gun
    {
        0x95,
        pe2_item_t{
            .id = 0x95,
            .type = UNIQUE,
            .rarity = IMPOSSIBLE,
            .item_class = WEAPON,
            .main_tag = PISTOL,
            .additional_tag = NONE,
            .value = 1000,
        },
    },
    // Gunblade
    {
        0x96,
        pe2_item_t{
            .id = 0x96,
            .type = UNIQUE,
            .rarity = IMPOSSIBLE,
            .item_class = WEAPON,
            .main_tag = SHOTGUN,
            .additional_tag = NONE,
            .value = 500,
        },
    },
    // Recovery 1
    {
        0x01,
        pe2_item_t{
            .id = 0x01,
            .type = HP_RECOVERY,
            .rarity = COMMON,
            .item_class = CONSUMABLE,
            .additional_tag = NONE,
        },
    },
    // Stim
    {
        0x04,
        pe2_item_t{
            .id = 0x04,
            .type = MISC,
            .rarity = COMMON,
            .item_class = CONSUMABLE,
            .additional_tag = NONE,
        },
    },
    // MP boost 1
    {
        0x06,
        pe2_item_t{
            .id = 0x06,
            .type = MP_RECOVERY,
            .rarity = COMMON,
            .item_class = CONSUMABLE,
            .additional_tag = NONE,
        },
    },
    // Penicillin
    {
        0x08,
        pe2_item_t{
            .id = 0x08,
            .type = MISC,
            .rarity = RARE,
            .item_class = CONSUMABLE,
            .additional_tag = NONE,
        },
    },
    // Combat light
    {
        0x41,
        pe2_item_t{
            .id = 0x41,
            .type = MISC,
            .rarity = COMMON,
            .item_class = CONSUMABLE,
            .additional_tag = NONE,
        },
    },
    // Pistol common ammo
    {
        0xa0,
        pe2_item_t{
            .id = 0xa0,
            .type = AMMO,
            .rarity = COMMON,
            .item_class = CONSUMABLE,
            .additional_tag = PISTOL,
        },
    },
    // Shotgun common ammo
    {
        0xac,
        pe2_item_t{
            .id = 0xac,
            .type = AMMO,
            .rarity = COMMON,
            .item_class = CONSUMABLE,
            .additional_tag = SHOTGUN,
        },
    },
    // Recovery 2
    {
        0x02,
        pe2_item_t{
            .id = 0x02,
            .type = HP_RECOVERY,
            .rarity = RARE,
            .item_class = CONSUMABLE,
            .additional_tag = NONE,
        },
    },
    // MP boost 2
    {
        0x07,
        pe2_item_t{
            .id = 0x07,
            .type = MP_RECOVERY,
            .rarity = RARE,
            .item_class = CONSUMABLE,
            .additional_tag = NONE,
        },
    },
    // Flare
    {
        0x3a,
        pe2_item_t{
            .id = 0x3a,
            .type = MISC,
            .rarity = RARE,
            .item_class = CONSUMABLE,
            .additional_tag = NONE,
        },
    },
    // Pepper spray
    {
        0x3b,
        pe2_item_t{
            .id = 0x3b,
            .type = MISC,
            .rarity = RARE,
            .item_class = CONSUMABLE,
            .additional_tag = NONE,
        },
    },
    // Pistol rare ammo
    {
        0xa1,
        pe2_item_t{
            .id = 0xa1,
            .type = AMMO,
            .rarity = RARE,
            .item_class = CONSUMABLE,
            .additional_tag = PISTOL,
        },
    },
    // Shotgun rare ammo
    {
        0xad,
        pe2_item_t{
            .id = 0xad,
            .type = AMMO,
            .rarity = RARE,
            .item_class = CONSUMABLE,
            .additional_tag = SHOTGUN,
        },
    },
    // Magnum rare ammo
    {
        0xa6,
        pe2_item_t{
            .id = 0xa6,
            .type = AMMO,
            .rarity = RARE,
            .item_class = CONSUMABLE,
            .additional_tag = MAGNUM,
        },
    },
    // Grenade rare ammo
    {
        0xab,
        pe2_item_t{
            .id = 0xab,
            .type = AMMO,
            .rarity = RARE,
            .item_class = CONSUMABLE,
            .additional_tag = GRENADE,
        },
    },
    // Rifle ammo
    {
        0xaf,
        pe2_item_t{
            .id = 0xaf,
            .type = AMMO,
            .rarity = RARE,
            .item_class = CONSUMABLE,
            .additional_tag = RIFLE,
        },
    },
    // Recovery 3
    {
        0x03,
        pe2_item_t{
            .id = 0x03,
            .type = HP_RECOVERY,
            .rarity = LEGENDARY,
            .item_class = CONSUMABLE,
            .additional_tag = NONE,
        },
    },
    // Cola
    {
        0x05,
        pe2_item_t{
            .id = 0x05,
            .type = MP_RECOVERY,
            .rarity = LEGENDARY,
            .item_class = CONSUMABLE,
            .additional_tag = NONE,
        },
    },
    // Lipstick
    {
        0x0b,
        pe2_item_t{
            .id = 0x0b,
            .type = MP_RECOVERY,
            .rarity = LEGENDARY,
            .item_class = CONSUMABLE,
            .additional_tag = NONE,
        },
    },
    // Belt pouch
    {
        0x0d,
        pe2_item_t{
            .id = 0x0d,
            .type = MISC,
            .rarity = LEGENDARY,
            .item_class = CONSUMABLE,
            .additional_tag = NONE,
        },
    },
    // Protein capsule
    {
        0x3c,
        pe2_item_t{
            .id = 0x3c,
            .type = HP_RECOVERY,
            .rarity = LEGENDARY,
            .item_class = CONSUMABLE,
            .additional_tag = NONE,
        },
    },
    // Ringer's solution
    {
        0x3d,
        pe2_item_t{
            .id = 0x3d,
            .type = MP_RECOVERY,
            .rarity = LEGENDARY,
            .item_class = CONSUMABLE,
            .additional_tag = NONE,
        },
    },
    // Eau de toilette
    {
        0x3e,
        pe2_item_t{
            .id = 0x3e,
            .type = MISC,
            .rarity = LEGENDARY,
            .item_class = CONSUMABLE,
            .additional_tag = NONE,
        },
    },
    // Pistol legendary ammo
    {
        0xa2,
        pe2_item_t{
            .id = 0xa2,
            .type = AMMO,
            .rarity = LEGENDARY,
            .item_class = CONSUMABLE,
            .additional_tag = PISTOL,
        },
    },
    // Shotgun legendary ammo
    {
        0xae,
        pe2_item_t{
            .id = 0xae,
            .type = AMMO,
            .rarity = LEGENDARY,
            .item_class = CONSUMABLE,
            .additional_tag = SHOTGUN,
        },
    },
    // Magnum legendary ammo
    {
        0xa7,
        pe2_item_t{
            .id = 0xa7,
            .type = AMMO,
            .rarity = LEGENDARY,
            .item_class = CONSUMABLE,
            .additional_tag = MAGNUM,
        },
    },
    // Grenade legendary ammo
    {
        0xa9,
        pe2_item_t{
            .id = 0xa9,
            .type = AMMO,
            .rarity = LEGENDARY,
            .item_class = CONSUMABLE,
            .additional_tag = GRENADE,
        },
    },
    // Grenade legendary ammo
    {
        0xaa,
        pe2_item_t{
            .id = 0xaa,
            .type = AMMO,
            .rarity = LEGENDARY,
            .item_class = CONSUMABLE,
            .additional_tag = GRENADE,
        },
    },
};

std::map<uint8_t, pe2_item_t> elements_map{
    {
        0x0f,
        pe2_item_t{
            .id = 0x0f,
            .type = MISC,
            .rarity = COMMON,
            .item_class = CONSUMABLE,
            .additional_tag = NONE,
        },
    },
    {
        0x10,
        pe2_item_t{
            .id = 0x10,
            .type = MISC,
            .rarity = RARE,
            .item_class = CONSUMABLE,
            .additional_tag = NONE,
        },
    },
    {
        0x11,
        pe2_item_t{
            .id = 0x11,
            .type = MISC,
            .rarity = RARE,
            .item_class = CONSUMABLE,
            .additional_tag = NONE,
        },
    },

    {
        0x12,
        pe2_item_t{
            .id = 0x12,
            .type = MISC,
            .rarity = COMMON,
            .item_class = CONSUMABLE,
            .additional_tag = NONE,
        },
    },
    {
        0x13,
        pe2_item_t{
            .id = 0x13,
            .type = MISC,
            .rarity = RARE,
            .item_class = CONSUMABLE,
            .additional_tag = NONE,
        },
    },
    {
        0x14,
        pe2_item_t{
            .id = 0x14,
            .type = MISC,
            .rarity = RARE,
            .item_class = CONSUMABLE,
            .additional_tag = NONE,
        },
    },

    {
        0x15,
        pe2_item_t{
            .id = 0x15,
            .type = MISC,
            .rarity = RARE,
            .item_class = CONSUMABLE,
            .additional_tag = NONE,
        },
    },
    {
        0x16,
        pe2_item_t{
            .id = 0x13,
            .type = MISC,
            .rarity = LEGENDARY,
            .item_class = CONSUMABLE,
            .additional_tag = NONE,
        },
    },
    {
        0x17,
        pe2_item_t{
            .id = 0x14,
            .type = MISC,
            .rarity = LEGENDARY,
            .item_class = CONSUMABLE,
            .additional_tag = NONE,
        },
    },

    {
        0x18,
        pe2_item_t{
            .id = 0x18,
            .type = MISC,
            .rarity = COMMON,
            .item_class = CONSUMABLE,
            .additional_tag = NONE,
        },
    },
    {
        0x19,
        pe2_item_t{
            .id = 0x19,
            .type = MISC,
            .rarity = RARE,
            .item_class = CONSUMABLE,
            .additional_tag = NONE,
        },
    },
    {
        0x1a,
        pe2_item_t{
            .id = 0x1a,
            .type = MISC,
            .rarity = RARE,
            .item_class = CONSUMABLE,
            .additional_tag = NONE,
        },
    },

    {
        0x1b,
        pe2_item_t{
            .id = 0x1b,
            .type = MISC,
            .rarity = COMMON,
            .item_class = CONSUMABLE,
            .additional_tag = NONE,
        },
    },
    {
        0x1c,
        pe2_item_t{
            .id = 0x1c,
            .type = MISC,
            .rarity = RARE,
            .item_class = CONSUMABLE,
            .additional_tag = NONE,
        },
    },
    {
        0x1d,
        pe2_item_t{
            .id = 0x1d,
            .type = MISC,
            .rarity = RARE,
            .item_class = CONSUMABLE,
            .additional_tag = NONE,
        },
    },

    {
        0x1e,
        pe2_item_t{
            .id = 0x1e,
            .type = MISC,
            .rarity = RARE,
            .item_class = CONSUMABLE,
            .additional_tag = NONE,
        },
    },
    {
        0x1f,
        pe2_item_t{
            .id = 0x1f,
            .type = MISC,
            .rarity = LEGENDARY,
            .item_class = CONSUMABLE,
            .additional_tag = NONE,
        },
    },
    {
        0x20,
        pe2_item_t{
            .id = 0x20,
            .type = MISC,
            .rarity = LEGENDARY,
            .item_class = CONSUMABLE,
            .additional_tag = NONE,
        },
    },

    {
        0x21,
        pe2_item_t{
            .id = 0x21,
            .type = MISC,
            .rarity = COMMON,
            .item_class = CONSUMABLE,
            .additional_tag = NONE,
        },
    },
    {
        0x22,
        pe2_item_t{
            .id = 0x22,
            .type = MISC,
            .rarity = RARE,
            .item_class = CONSUMABLE,
            .additional_tag = NONE,
        },
    },
    {
        0x23,
        pe2_item_t{
            .id = 0x23,
            .type = MISC,
            .rarity = RARE,
            .item_class = CONSUMABLE,
            .additional_tag = NONE,
        },
    },

    {
        0x24,
        pe2_item_t{
            .id = 0x24,
            .type = MISC,
            .rarity = COMMON,
            .item_class = CONSUMABLE,
            .additional_tag = NONE,
        },
    },
    {
        0x25,
        pe2_item_t{
            .id = 0x25,
            .type = MISC,
            .rarity = RARE,
            .item_class = CONSUMABLE,
            .additional_tag = NONE,
        },
    },
    {
        0x26,
        pe2_item_t{
            .id = 0x26,
            .type = MISC,
            .rarity = RARE,
            .item_class = CONSUMABLE,
            .additional_tag = NONE,
        },
    },

    {
        0x27,
        pe2_item_t{
            .id = 0x27,
            .type = MISC,
            .rarity = RARE,
            .item_class = CONSUMABLE,
            .additional_tag = NONE,
        },
    },
    {
        0x28,
        pe2_item_t{
            .id = 0x28,
            .type = MISC,
            .rarity = LEGENDARY,
            .item_class = CONSUMABLE,
            .additional_tag = NONE,
        },
    },
    {
        0x29,
        pe2_item_t{
            .id = 0x29,
            .type = MISC,
            .rarity = LEGENDARY,
            .item_class = CONSUMABLE,
            .additional_tag = NONE,
        },
    },

    {
        0x2a,
        pe2_item_t{
            .id = 0x2a,
            .type = MISC,
            .rarity = COMMON,
            .item_class = CONSUMABLE,
            .additional_tag = NONE,
        },
    },
    {
        0x2b,
        pe2_item_t{
            .id = 0x2b,
            .type = MISC,
            .rarity = RARE,
            .item_class = CONSUMABLE,
            .additional_tag = NONE,
        },
    },
    {
        0x2c,
        pe2_item_t{
            .id = 0x2c,
            .type = MISC,
            .rarity = RARE,
            .item_class = CONSUMABLE,
            .additional_tag = NONE,
        },
    },

    {
        0x2d,
        pe2_item_t{
            .id = 0x2d,
            .type = MISC,
            .rarity = COMMON,
            .item_class = CONSUMABLE,
            .additional_tag = NONE,
        },
    },
    {
        0x2e,
        pe2_item_t{
            .id = 0x2e,
            .type = MISC,
            .rarity = RARE,
            .item_class = CONSUMABLE,
            .additional_tag = NONE,
        },
    },
    {
        0x2f,
        pe2_item_t{
            .id = 0x2f,
            .type = MISC,
            .rarity = RARE,
            .item_class = CONSUMABLE,
            .additional_tag = NONE,
        },
    },

    {
        0x30,
        pe2_item_t{
            .id = 0x30,
            .type = MISC,
            .rarity = RARE,
            .item_class = CONSUMABLE,
            .additional_tag = NONE,
        },
    },
    {
        0x31,
        pe2_item_t{
            .id = 0x31,
            .type = MISC,
            .rarity = LEGENDARY,
            .item_class = CONSUMABLE,
            .additional_tag = NONE,
        },
    },
    {
        0x32,
        pe2_item_t{
            .id = 0x32,
            .type = MISC,
            .rarity = LEGENDARY,
            .item_class = CONSUMABLE,
            .additional_tag = NONE,
        },
    },
};

std::map<pe2_item_type_t, std::map<pe2_item_rarity_t, std::vector<uint8_t>>> item_map_by_type{
    {HP_RECOVERY, {{COMMON, {0x01}}, {RARE, {0x02}}, {LEGENDARY, {0x03, 0x3c}}}},
    {MP_RECOVERY, {{COMMON, {0x06}}, {RARE, {0x07}}, {LEGENDARY, {0x05, 0x0b, 0x3d}}}},
    {AMMO, {{COMMON, {}}, {RARE, {}}, {LEGENDARY, {}}}},
    {MISC, {{COMMON, {0x04, 0x41}}, {RARE, {0x08, 0x3a, 0x3b}}, {LEGENDARY, {0x0d, 0x3e}}}},
    {UNIQUE, {{COMMON, {}}, {RARE, {}}, {LEGENDARY, {}}}}};

std::map<pe2_item_tag_t, std::map<pe2_item_rarity_t, std::vector<uint8_t>>> weapon_ammo_allowance_map{
    {PISTOL, {{COMMON, {0xa0}}, {RARE, {0xa1}}, {LEGENDARY, {0xa2}}}},
    {SHOTGUN, {{COMMON, {0xac}}, {RARE, {0xad}}, {LEGENDARY, {0xae}}}},
    {RIFLE, {{COMMON, {}}, {RARE, {0xaf}}, {LEGENDARY, {}}}},
    {GRENADE, {{COMMON, {}}, {RARE, {0xaa, 0xab}}, {LEGENDARY, {0xa9}}}},
    {MAGNUM, {{COMMON, {}}, {RARE, {0xa6}}, {LEGENDARY, {0xa7}}}}};

std::map<pe2_item_tag_t, std::map<pe2_item_rarity_t, std::vector<uint8_t>>> weapon_attachment_allowance_map{
    {P08,
     {
         {RARE, {0x0c}},
     }},
    {M4A1, {{COMMON, {0x0a}}, {RARE, {0x0a, 0x46, 0x42, 0x43}}, {LEGENDARY, {0x44, 0x45}}}},
    {MP5AP5, {{COMMON, {}}, {RARE, {0x09}}, {LEGENDARY, {}}}}};

std::map<pe2_item_type_t, double> item_types_probability_map{
    {HP_RECOVERY, 0.1622}, {MP_RECOVERY, 0.1667}, {AMMO, 0.4405}, {MISC, 0.1485}, {UNIQUE, 0.0721},
};

std::map<pe2_item_rarity_t, double> item_rarity_probability_map{
    {COMMON, 0.6},
    {RARE, 0.325},
    {LEGENDARY, 0.075},
};

std::map<pe2_item_rarity_t, double> secret_item_rarity_probability_map{
    {COMMON, 0.075},
    {RARE, 0.6},
    {LEGENDARY, 0.325},
};

std::map<pe2_item_rarity_t, double> reward_item_rarity_probability_map{
    {COMMON, 0.325},
    {RARE, 0.6},
    {LEGENDARY, 0.075},
};

std::vector<uint8_t> weapons_ids{0x80, 0x81, 0x82, 0x83, 0x84, 0x88, 0x8a, 0x8b, 0x8c,
                                 0x8d, 0x8e, 0x8f, 0x90, 0x92, 0x93, 0x94, 0x95, 0x96,
                                 0x98, 0x99, 0x9a, 0x9b, 0x9c, 0x9d, 0x9e, 0x9f};

std::vector<std::pair<size_t, pe2_item_pickup_t>> acropolis_pickup_items_map{
    {0x24de, PICKUP_DEFAULT}, // Medicine 2 in square
    {0x24fe, PICKUP_DEFAULT}, // Medicine 1 in square
    {0x256e, PICKUP_DEFAULT}, // Stim in cafeteria's kitchen
    {0x257e, PICKUP_DEFAULT}, // Medicine 1 in cafeteria's kitchen
    {0x25ce, PICKUP_SECRET},  // Protein Capsule in fountain
    {0x25de, PICKUP_DEFAULT}, // Grenade Pistol in fountain
    {0x25ee, PICKUP_DEFAULT}, // MP Boost 2 in fountain
    {0x260e, PICKUP_DEFAULT}, // Penicillin in forked road
    {0x262e, PICKUP_SECRET},  // Tactical Vest in observatory
    {0x264e, PICKUP_DEFAULT}, // Grenades in promenade
    {0x265e, PICKUP_SECRET},  // MP5A5 in promenade
    {0x270e, PICKUP_DEFAULT}, // Medicine 3 in MIST Garage
    {0x271e, PICKUP_DEFAULT}, // Medicine 3 in MIST Garage
};

std::vector<std::pair<size_t, pe2_item_pickup_t>> acropolis_shooting_range_rewards_map{
    {0x272e, PICKUP_DEFAULT}, // Hydra Bullets in Shooting Range reward
    {0x273e, PICKUP_DEFAULT}, // Ringer's Solution in Shooting Range reward
    {0x274e, PICKUP_SECRET},  // Hunter's Goggles in Shooting Range reward
    {0x275e, PICKUP_DEFAULT}, // Belt Pouch in Shooting Range reward
    {0x276e, PICKUP_SECRET},  // Shoulder Holster in Shooting Range reward
};

std::vector<std::pair<size_t, pe2_item_pickup_t>> dryfield_motel_room_6_box{
    {0x24b18, PICKUP_DEFAULT}, // Motel Room 6 box
    {0x24b28, PICKUP_DEFAULT}, // Motel Room 6 box
    {0x24b38, PICKUP_DEFAULT}, // Motel Room 6 box
};

std::vector<std::pair<size_t, pe2_item_pickup_t>> dryfield_day_items_pickup_dict{
    {0x80e2a, PICKUP_DEFAULT}, // General Store Coca Cola
    {0x80e3a, PICKUP_DEFAULT}, // General Store Recovery 3
    {0x80e5a, PICKUP_DEFAULT}, // Back Street Firefly Ammo
    {0x80e7a, PICKUP_DEFAULT}, // Souvenir Shop Belt Pouch
    {0x80eaa, PICKUP_DEFAULT}, // Motel Room 2 Penicillin (Shelf)
    {0x80eba, PICKUP_DEFAULT}, // Motel Room 2 Recovery 1 (Bathroom)
    {0x80efa, PICKUP_DEFAULT}, // Parking Lot Buckshot ammo (Trash)
    {0x80f4a, PICKUP_DEFAULT}, // Saloon G&R Cola (Fridge)
    {0x80f7a, PICKUP_DEFAULT}, // Saloon Kitchen Recovery 1
    {0x80faa, PICKUP_DEFAULT}, // Water Tank Medicine 1 (Corpse)
    {0x8102a, PICKUP_DEFAULT}, // Factory R. Slug (Trunk)
    {0x8108a, PICKUP_DEFAULT}, // Driveway Medicine 1 (Trash)
    {0x8119a, PICKUP_DEFAULT}, // Capsule Underpass
    {0x811ba, PICKUP_DEFAULT}, // Wine Cellar Flare
    {0x811ca, PICKUP_DEFAULT}, // Wine Cellar Snail Magzine
};

std::vector<std::pair<size_t, pe2_item_pickup_t>> dryfield_night_items_pickup_dict{
    {0x80eda, PICKUP_DEFAULT}, // Motel Room 3 MP Boost 2 (Shelf)
    {0x8111a, PICKUP_DEFAULT}, // Motel Room 5 Belt Pouch (Near Lamp)
    {0x810fa, PICKUP_SECRET},  // Trailer Coach M950 (Sturdy Box)
    {0x8117a, PICKUP_SECRET},  // Motel Loft Holy Water (Safe)
};

std::vector<std::pair<size_t, pe2_item_pickup_t>> dryfield_night_return_items_pickup_dict{
    {0x80dba, PICKUP_SECRET},  // Tactical Vest (Pierce Car Trunk)
    {0x80dca, PICKUP_DEFAULT}, // Lipstick (Pierce Car Trunk)
    {0x80fca, PICKUP_DEFAULT}, // Water Tank Pierce reward Cola
    {0x80fda, PICKUP_DEFAULT}, // Water Tank Pierce reward MP Boost 2
    {0x80fea, PICKUP_SECRET},  // Water Tank Pierce reward Ofuda Charm
    // 0x810ba,            // Junkyard Chicken Plate (Only if Flynt is dead)
};

std::vector<std::pair<size_t, pe2_item_pickup_t>> mines_and_shelter_sterilization_room_box{
    {0x24b6c, PICKUP_DEFAULT}, // Sterilization room box
    {0x24b7c, PICKUP_DEFAULT}, // Sterilization room box
};

std::vector<std::pair<size_t, pe2_item_pickup_t>> mines_and_shelter_pickup_items_dict{
    {0x1dba, PICKUP_SECRET},  // Cavern P229 (Boss Drop)
    {0x1e6a, PICKUP_DEFAULT}, // Storeroom Ringer's Solution (Shelf)
    {0x1eaa, PICKUP_SECRET},  // Armory SP12
    // 0x1fca,             // ???
    // 0x1fda,             // ???
    // 0x1fea,             // ???
    // 0x1ffa,             // ???
    // 0x200a,             // ???
    {0x201a, PICKUP_DEFAULT}, // Underground Parking Belt Pouch
    {0x206a, PICKUP_DEFAULT}, // Operating Room Ringer's Solution
    {0x207a, PICKUP_DEFAULT}, // Operating Room Eau De Toilette
    // 0x20da,             // Breeding Room Ammo Box
    {0x20ea, PICKUP_DEFAULT}, // Breeding Room Stim
    {0x20fa, PICKUP_DEFAULT}, // Breeding Room Cola
    {0x211a, PICKUP_SECRET},  // Dumping Hole Medicine Wheel
    // 0x212a,           // Dumping Hole Infinite Hydra Ammo
    // 0x213a,           // Dumping Hole Infinite Parabellum Ammo
    // 0x217a,           // Lower or Upper Sewer??? MP Boost 2

};

std::vector<std::pair<size_t, pe2_item_pickup_t>> neo_ark_and_shelter_pickup_items_dict{
    {0x2e72, PICKUP_DEFAULT},
    {0x2e82, PICKUP_DEFAULT},
    {0x2e92, PICKUP_DEFAULT},
    // 0x2ee2,
    // 0x2ef2,
    // 0x2f02,
    // 0x2f12,
    // 0x2f22,
    {0x2f42, PICKUP_DEFAULT},
    {0x2f62, PICKUP_DEFAULT},
    {0x2f92, PICKUP_DEFAULT},
    {0x2fa2, PICKUP_DEFAULT},
    {0x2fc2, PICKUP_DEFAULT},
    {0x2ff2, PICKUP_DEFAULT},
    {0x3002, PICKUP_DEFAULT},
    {0x3022, PICKUP_DEFAULT},
    // 0x3042,
};

std::vector<uint8_t> starting_unique_items{0x0b, 0x0e, 0x3a, 0x3e, 0x3f, 0x40};
std::vector<uint8_t> starting_rare_items_id{0x01, 0x02, 0x04, 0x06, 0x07, 0x08, 0x3b, 0x41};

std::vector<uint8_t> starting_armors{0x60, 0x63, 0x64, 0x67, 0x6c};
std::vector<uint8_t> all_armors{0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66,
                                0x67, 0x68, 0x69, 0x6a, 0x6b, 0x6c, 0x6d};

std::vector<uint8_t> starting_weapons{0x81, 0x83, 0x8c, 0x8f};
std::vector<uint8_t> all_weapons{0x80, 0x81, 0x82, 0x83, 0x84, 0x88, 0x8a, 0x8b, 0x8c, 0x8d, 0x8e, 0x8f, 0x90,
                                 0x93, 0x94, 0x95, 0x96, 0x98, 0x99, 0x9a, 0x9b, 0x9c, 0x9d, 0x9e, 0x9f};
std::vector<uint8_t> all_weapons_and_attachments{0x09, 0x0a, 0x0c, 0x42, 0x43, 0x44, 0x45, 0x46,
                                                 0x81, 0x82, 0x83, 0x84, 0x88, 0x8a, 0x8b, 0x8c,
                                                 0x8d, 0x8e, 0x8f, 0x90, 0x95, 0x96, 0x9d};
std::vector<uint8_t> all_misc_items{0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x0b, 0x0d, 0x0e,
                                    0x36, 0x37, 0x38, 0x39, 0x3a, 0x3b, 0x3c, 0x3d, 0x3e, 0x3f, 0x40};
std::vector<uint8_t> all_ammo{0xa0, 0xa1, 0xa2, 0xa6, 0xa7, 0xa9, 0xaa, 0xab, 0xac, 0xad, 0xae, 0xaf};

std::vector<uint8_t> all_magic{0x0f, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,

                               0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f, 0x20,

                               0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29,

                               0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f, 0x30, 0x31, 0x32};
std::vector<uint8_t> starting_magic{0x0f, 0x10, 0x12, 0x15, 0x18, 0x19, 0x1b, 0x1e,
                                    0x21, 0x22, 0x24, 0x27, 0x2a, 0x2b, 0x2d, 0x30};
std::map<pe2_item_rarity_t, std::vector<uint8_t>> magic{
    {COMMON,
     {
         0x0f,
         0x12,
         0x18,
         0x1b,
         0x21,
         0x24,
         0x2a,
         0x2d,
     }},
    {RARE,
     {
         0x10, 0x11, 0x13, 0x14, 0x15, 0x19, 0x1a, 0x1c, 0x1d, 0x1e,
         0x22, 0x23, 0x25, 0x26, 0x27, 0x2b, 0x2c, 0x2e, 0x2f, 0x30,
     }},
    {LEGENDARY,
     {
         0x16,
         0x17,
         0x1f,
         0x20,
         0x28,
         0x29,
         0x31,
         0x32,
     }}};

std::vector<pe2_shop_t> shops_vector{

    // Bouquet's Guns and Ammo [Normal/Replay/Bounty]

    pe2_shop_t{
        .type = NRB,
        .weapon = 2,
        .ammo = 2,
        .armor = 2,
        .item = 6,
        .location{"STAGE1/18_mist_parking"},
        .offset{0x8a98},
        .has_charger = true,
        .has_gap = {{true, false, true, true}},
    },

    // Bouquet's Guns and Ammo [Scavanger/Nightmare]

    pe2_shop_t{
        .type = SN,
        .weapon = 1,
        .ammo = 1,
        .armor = 2,
        .item = 6,
        .location{"STAGE1/18_mist_parking"},
        .offset{0x8cb0},
        .has_gap = {{false, false, true, true}},
    },

    // Mr. Douglas (Disc1) [Normal/Replay/Bounty]

    pe2_shop_t{
        .type = NRB,
        .weapon = 6,
        .ammo = 5,
        .armor = 2,
        .item = 6,
        .location{
            "STAGE2/25_trailer_coach",
            "STAGE3/25_trailer_coach",
            "STAGE3/22_garage",
        },
        .offset{0x63b8, 0x6b04, 0x4060},
        .has_charger = true,
        .has_gap = {{true, true, true, true}, {true, true, true, true}, {true, true, true, true}},
    },

    // Mr. Douglas (Disc1) [Scavanger/Nightmare]

    pe2_shop_t{
        .type = SN,
        .weapon = 2,
        .ammo = 2,
        .armor = 3,
        .item = 5,
        .location{
            "STAGE2/25_trailer_coach",
            "STAGE3/25_trailer_coach",
            "STAGE3/22_garage",
        },
        .offset = {0x65c8, 0x6d14, 0x4270},
        .has_gap = {{true, true, false, false}, {true, true, false, false}, {true, true, false, false}},
    },

    // BP Armory [Normal/Replay/Bounty]

    pe2_shop_t{
        .type = NRB,
        .weapon = 7,
        .ammo = 5,
        .armor = 3,
        .item = 8,
        .location = {"STAGE4/13_armory"},
        .offset{0x493c},
        .has_charger = true,
        .has_gap = {{false, true, false, true}},
    },

    // BP Armory [Scavanger/Nightmare]

    pe2_shop_t{
        .type = SN,
        .weapon = 2,
        .ammo = 4,
        .armor = 2,
        .item = 5,
        .location = {"STAGE4/13_armory"},
        .offset = {0x4b38},
        .has_gap = {{true, true, true, false}},
    },

    // Mr. Douglas (Disc2) [Normal/Replay/Bounty]

    pe2_shop_t{
        .type = NRB,
        .weapon = 10,
        .ammo = 7,
        .armor = 3,
        .item = 6,
        .location{"STAGE3/25_trailer_coach"},
        .offset{0x6b78},
        .has_charger = true,
        .has_gap = {{true, true, false, true}},
    },

    // Mr. Douglas (Disc2) [Scavanger/Nightmare]

    pe2_shop_t{
        .type = SN,
        .weapon = 4,
        .ammo = 3,
        .armor = 3,
        .item = 5,
        .location{"STAGE3/25_trailer_coach"},
        .offset = {0x6d60},
        .has_gap = {{true, false, false, false}},
    },

    // Army Truck Bad Ending Underground Parking [Normal/Replay/Bounty]

    pe2_shop_t{
        .type = NRB,
        .weapon = 12,
        .ammo = 8,
        .armor = 3,
        .item = 9,
        .location{"STAGE4/20_underground_parking"},
        .offset{0x9774},
        .has_charger = true,
        .has_gap = {{true, false, false, false}},
    },

    // Army Truck Good Ending Underground Parking [Normal/Replay/Bounty]

    pe2_shop_t{
        .type = NRB,
        .weapon = 14,
        .ammo = 8,
        .armor = 5,
        .item = 10,
        .location{"STAGE4/20_underground_parking"},
        .offset{0x97c0},
        .has_charger = true,
        .has_gap = {{true, true, false, true}},
    },

    // Army Truck Bad Ending Underground Parking [Scavanger/Nightmare]

    pe2_shop_t{
        .type = SN,
        .weapon = 4,
        .ammo = 1,
        .armor = 5,
        .item = 5,
        .location{"STAGE4/20_underground_parking"},
        .offset{0x98d4},
        .has_charger = false,
        .has_gap = {{true, false, false, false}},
    },

    // Army Truck Good Ending Underground Parking [Scavanger/Nightmare]

    pe2_shop_t{
        .type = SN,
        .weapon = 5,
        .ammo = 1,
        .armor = 5,
        .item = 5,
        .location{"STAGE4/20_underground_parking"},
        .offset{0x9928},
        .has_charger = true,
        .has_gap = {{false, true, false, false}},
    },

    // Army Truck Bad Ending [Normal/Replay/Bounty]

    pe2_shop_t{
        .type = NRB,
        .weapon = 11,
        .ammo = 8,
        .armor = 3,
        .item = 9,
        .location{"STAGE5/04_heliport"},
        .offset{0x3674},
        .has_charger = true,
        .has_gap = {{false, false, false, false}},
    },

    // Army Truck Bad Ending [Scavanger/Nightmare]

    pe2_shop_t{
        .type = SN,
        .weapon = 4,
        .ammo = 2,
        .armor = 4,
        .item = 5,
        .location{"STAGE5/04_heliport"},
        .offset = {0x3840},
        .has_charger = false,
        .has_gap = {{true, true, true, false}},
    },

    // Army Truck Good Ending [Normal/Replay/Bounty]

    pe2_shop_t{
        .type = NRB,
        .weapon = 12,
        .ammo = 5,
        .armor = 5,
        .item = 10,
        .location{"STAGE5/04_heliport"},
        .offset{0x36bc},
        .has_charger = true,
        .has_gap = {{true, true, false, true}},
    },

    // Army Truck Good Ending [Scavanger/Nightmare]

    pe2_shop_t{
        .type = SN,
        .weapon = 4,
        .ammo = 1,
        .armor = 5,
        .item = 5,
        .location{"STAGE5/04_heliport"},
        .offset = {0x386c},
        .has_gap = {{true, false, false, false}},
    },
};

std::map<size_t, size_t> new_costs{
    {0x7a058, 160},   {0x7a060, 800},   {0x7a078, 160},   {0x7a0a8, 2000},  {0x7a1e8, 27500},
    {0x7a1f0, 27500}, {0x7a1f8, 27500}, {0x7a200, 27500}, {0x7a208, 300},   {0x7a210, 250},
    {0x7a220, 6000},  {0x7a230, 2000},  {0x7a240, 200},   {0x7a248, 10000}, {0x7a250, 10000},
    {0x7a258, 20000}, {0x7a260, 10000}, {0x7a268, 7500},  {0x7a338, 2000},  {0x7a340, 14000},
    {0x7a348, 6500},  {0x7a350, 2000},  {0x7a358, 6000},  {0x7a360, 8000},  {0x7a368, 12000},
    {0x7a370, 6000},  {0x7a378, 8500},  {0x7a380, 7500},  {0x7a388, 8000},  {0x7a390, 15000},
    {0x7a3a0, 65000}, {0x7a450, 2500},  {0x7a458, 6000},  {0x7a478, 12500}, {0x7a488, 5000},
    {0x7a498, 4000},  {0x7a4a0, 9500},  {0x7a4a8, 22500}, {0x7a4b0, 4000},  {0x7a4b8, 25000},
    {0x7a4e0, 65000}, {0x7a4e8, 30000}, {0x7a520, 10000}, {0x7a530, 60},    {0x7a538, 100}, // P.B.
    {0x7a540, 300},                                                                         // Hydra
    {0x7a548, 500},                                                                         // Spartan
    {0x7a568, 4000},                                                                        // Magnum Ammo
    {0x7a570, 9000}, // Maeda Magnum Ammo
    {0x7a580, 800},  // Grenades
    {0x7a588, 800},  // Grenades Airbirst
    {0x7a590, 400},  // Grenades Riot
    {0x7a598, 150},  // Shells Buckshot
    {0x7a5a0, 350},  // Shells Firefly
    {0x7a5a8, 800},  // R. Slug
    {0x7a5b0, 150},  // Rifle Ammo
};

std::map<pe2_level_name_t, std::vector<size_t>> ammo_boxes{
    {AKROPOLIS_LEVEL, {0x24ce}},
    {BASE_LEVEL, {0x80daa}},
    {MINES_AND_SHELTER_LEVEL,
     {0x1dda, 0x1e5a, 0x1eba, 0x1eca, 0x1eda, 0x1fca, 0x1fda, 0x1fea, 0x1ffa, 0x200a, 0x20da, 0x212a,
      0x213a}},
    {NEOARK_AND_SHELTER_LEVEL, {0x2f02, 0x2f12, 0x2f22, 0x3042}},
};

} // namespace domain::item
