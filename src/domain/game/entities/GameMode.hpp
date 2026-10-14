#pragma once

namespace domain::game {

typedef enum {
    MODE_NORMAL = 0x00000001,
    MODE_REPLAY = 0x00000010,
    MODE_BOUNTY = 0x00000100,
    MODE_SCAVENGER = 0x00001000,
    MODE_NIGTHMARE = 0x00010000
} pe2_game_mode_t;

// Aliases for compatibility
constexpr pe2_game_mode_t NORMAL_MODE = MODE_NORMAL;
constexpr pe2_game_mode_t REPLAY_MODE = MODE_REPLAY;
constexpr pe2_game_mode_t BOUNTY_MODE = MODE_BOUNTY;
constexpr pe2_game_mode_t SCAVENGER_MODE = MODE_SCAVENGER;
constexpr pe2_game_mode_t NIGHTMARE_MODE = MODE_NIGTHMARE;

} // namespace domain::game
