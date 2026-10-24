#include "domain/magic/usecases/NoMagic.hpp"
#include <stdexcept>

namespace domain::magic::no_magic {
namespace {
unsigned readWord(std::istream &in) {
    unsigned char bytes[4];
    if (!in.read(reinterpret_cast<char *>(bytes), 4))
        throw std::runtime_error("Truncated gameplay code for No magic");
    return unsigned(bytes[0]) | unsigned(bytes[1]) << 8 | unsigned(bytes[2]) << 16 | unsigned(bytes[3]) << 24;
}
} // namespace
void apply(std::iostream &gameplay) {
    struct Edit {
        unsigned offset, before, after;
    };
    // Compact the main menu: Items, Key items, Map, Options, Exit. UiList
    // owns both the visible and selectable row counts; no empty cursor row.
    // Gp_UseItemTask: bypass opening the wheel (pad AND scripted open flag).
    // Gp_SetAttachState: reject the twelve PE slots, still permit ordinary
    // consumable attachments (flare, pepper spray, etc.). Its existing
    // prologue/epilogue and event lock remain intact. The five-word helper
    // reuses unreachable wheel code; all public entry addresses stay fixed.
    constexpr Edit edits[] = {
        {0x7b010, 0x800ce5f0, 0x800d5234}, {0x7b014, 0x800d5234, 0x800ce738},
        {0x7b018, 0x800ce738, 0x800ce894}, {0x7b01c, 0x800ce894, 0x00000000},
        {0x7b024, 0x08010606, 0x08010505}, {0xfaa0, 0x3c028011, 0x10000089},
        {0xe78c, 0x3c025555, 0x08028caa},  {0xe790, 0x34425556, 0x00000000},
        {0xfaa8, 0x00000000, 0x2cc2000c},  {0xfaac, 0x10400019, 0x1440fb91},
        {0xfab0, 0x00002821, 0x3c025555},  {0xfab4, 0x8c44001c, 0x080287e5},
        {0xfab8, 0x3c028007, 0x34425556},
    };
    for (const auto &e : edits) {
        gameplay.seekg(e.offset);
        if (readWord(gameplay) != e.before)
            throw std::runtime_error("Unsupported gameplay code for No magic");
    }
    for (const auto &e : edits) {
        gameplay.seekp(e.offset);
        for (unsigned i = 0; i != 4; ++i)
            gameplay.put(static_cast<char>(e.after >> (8 * i)));
    }
    gameplay.flush();
    if (!gameplay)
        throw std::runtime_error("Cannot write No magic patch");
}
} // namespace domain::magic::no_magic
