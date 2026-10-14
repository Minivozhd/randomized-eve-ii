#include "domain/armor/entities/Armor.hpp"
#include "support/Mips.hpp"
#include <algorithm>
#include <iostream>
#include <map>
#include <tuple>

namespace {
#include "patches/new_armors/generated.inc"
using test::expectEqual;
using test::require;
using test::signedWord;
struct Cpu : test::Mips {
    Cpu() {
        for (const auto &[offset, bytes] : compiled_armor_gameplay_patches)
            put(0x80093800 + offset, bytes);
    }
};
constexpr std::pair<std::uint32_t, std::uint32_t> queries[] = {
    {0x101, 1},        {0x102, 0x400},     {0x104, 2},         {0x108, 0x20},     {0x120, 0x200},
    {0x140, 0x800},    {0x200, 0x10},      {0x400, 0x20000},   {0x800, 4},        {0x1000, 0x100},
    {0x2000, 8},       {0x4000, 0x40},     {0x8000, 0x80},     {0x10000, 0x2000}, {0x20000, 0x8000},
    {0x40000, 0x4000}, {0x80000, 0x10000}, {0x100000, 0x20000}};
unsigned status(Cpu &c, unsigned query) {
    c.r[4] = query;
    c.r[31] = 0x801ffff0;
    c.run(0x800b9d80);
    return c.r[2];
}
void hook(Cpu &c, unsigned offset, unsigned resume, std::initializer_list<unsigned> outputs) {
    const auto before = c.r;
    const auto hi = c.hi, lo = c.lo;
    c.run(0x80093800 + offset, resume);
    for (unsigned reg = 2; reg < 32; ++reg) {
        if (reg == 26 || reg == 27 || std::find(outputs.begin(), outputs.end(), reg) != outputs.end())
            continue;
        expectEqual(c.r[reg], before[reg],
                    "Hook clobbered register " + std::to_string(reg) + " at " + std::to_string(offset));
    }
    expectEqual(c.hi, hi, "HI clobbered");
    expectEqual(c.lo, lo, "LO clobbered");
}
void metabolism() {
    for (auto query : {0x101, 0x102, 0x104, 0x108, 0x120, 0x140}) {
        Cpu c;
        c.armor();
        expectEqual(status(c, query), 0u, "No metabolism");
        c.write(0x80114c1c, 600, 2);
        expectEqual(status(c, query), 1u, "Metabolism active");
        expectEqual(c.read(0x80114c1c, 2), 600u, "Metabolism timer preserved");
        c.write(0x80114c1c, 0, 2);
        expectEqual(status(c, query), 0u, "Metabolism expired");
    }
}
void statusAndAttachments() {
    for (auto [query, bit] : queries) {
        Cpu c;
        c.armor(bit);
        expectEqual(status(c, query), 1u, "Armor status bit");
    }
    const std::pair<unsigned, unsigned> items[] = {{0x101, 0x3f},   {0x108, 0xb},    {0x120, 0xe},
                                                   {0x10000, 0x36}, {0x20000, 0x39}, {0x40000, 0x38},
                                                   {0x80000, 0x37}, {0x100000, 0x40}};
    for (auto [q, item] : items) {
        Cpu c;
        c.attachments.insert(item);
        expectEqual(status(c, q), 1u, "Attachment status");
    }
    for (auto q : {0x101, 0x102, 0x104, 0x108, 0x120, 0x140}) {
        Cpu c;
        c.armor(0x1000);
        expectEqual(status(c, q), 1u, "Resist all");
    }
}
void signedHp() {
    const std::tuple<unsigned, unsigned, unsigned> hooks[] = {
        {0x24330, 3, 4}, {0x24490, 3, 5}, {0x263ec, 3, 6}, {0x271ac, 2, 4}, {0x28914, 3, 5}};
    for (auto [off, ptr, base] : hooks)
        for (auto hp : {-70, -25, 0, 25, 250})
            for (auto natural : {50, 100, 105, 250}) {
                Cpu c;
                c.armor(0, hp);
                c.r[ptr] = 0x8010e2b8;
                c.r[base] = natural;
                hook(c, off, 0x80093800 + off + 8, {2});
                expectEqual(natural + signedWord(c.r[2]), std::max(1, natural + hp), "Signed HP");
                expectEqual(c.read(0x8010e2bf, 1), unsigned(hp < 0), "Sign flag preserved");
            }
    Cpu c;
    c.armor(0, -25);
    c.r[3] = 0x8010e2b8;
    c.r[6] = 105;
    hook(c, 0x263ec, 0x800b9bf4, {2});
    expectEqual(105 + signedWord(c.r[2]), 80, "Protein with negative armor");
}
void effects() {
    {
        Cpu c;
        c.armor(0x800000);
        c.r[16] = 0x80150000;
        c.r[2] = 7;
        hook(c, 0xe898, 0x800a20a4, {2, 4, 31});
        expectEqual(c.r[2], 60u, "MP effect");
        expectEqual(c.read(0x80150003, 1), 7u, "MP neighboring byte");
    }
    {
        Cpu c;
        c.armor(0x100000);
        c.r[16] = 0x80150000;
        c.write(0x8015000c, 800, 2);
        hook(c, 0xd0ec, 0x800a08f4, {2, 3});
        expectEqual(c.r[3], 900u, "Healing effect");
    }
    {
        Cpu c;
        c.armor(0x10000000);
        c.r[3] = 10;
        hook(c, 0x4f768, 0x800e2f74, {3});
        expectEqual(c.r[3], 15u, "Damage effect");
    }
}
void damageTables() {
    const std::array<unsigned, 9> normal = {70, 100, 45, 120, 140, 170, 200, 250, 300},
                                  boosted = {100, 135, 60, 140, 170, 200, 250, 300, 350};
    Cpu c;
    for (auto address : {0x80113460u, 0x80113474u})
        c.write(address, 0xbeef, 2);
    for (auto bits : {0u, 0x1000000u, 0x4000000u, 0x5000000u, 0u}) {
        c.armor(bits);
        c.r[4] = 123;
        c.r[29] = 0x801f0000;
        hook(c, 0x4e7ec, 0x800e1ff4, {8, 29});
        expectEqual(c.r[29], 0x801effe0u, "Stack frame");
        for (auto address : {0x80113460u, 0x80113474u}) {
            expectEqual(c.read(address, 2), 0xbeefu, "Tonfa adjacent parameter");
            expectEqual(c.read(address + 2, 2), bits & 0x1000000 ? 20u : 10u, "Tonfa power");
        }
        for (unsigned i = 0; i < 9; ++i)
            expectEqual(c.read(0x801139a4 + i * 16, 2), (bits & 0x4000000 ? boosted : normal)[i],
                        "Damage table");
    }
}
void catalogue() {
    unsigned count = 0;
    for (const auto &[rarity, armors] : domain::armor::armors_pool)
        for (const auto &armor : armors) {
            ++count;
            auto word = armor.status_effects;
            const auto bits = ((word & 0xff) << 24) | ((word & 0xff00) << 8) | ((word >> 8) & 0xff00) |
                              ((word >> 24) & 0xff);
            Cpu c;
            c.armor(bits);
            for (auto [query, mask] : queries)
                expectEqual(bool(status(c, query)), bool(bits & mask || (query < 0x200 && (bits & 0x1000))),
                            "Catalogue status");
            c.armor();
            for (auto [query, mask] : queries)
                expectEqual(status(c, query), 0u, "Removed armor status");
        }
    expectEqual(count, 113u, "Armor catalogue coverage");
}
void disabledEffects() {
    Cpu c;
    c.armor();
    c.r[16] = 0x80150000;
    c.r[2] = 7;
    hook(c, 0xe898, 0x800a20a4, {2, 4, 31});
    expectEqual(c.r[2], 120u, "Disabled MP effect");
    c.r[16] = 0x80150000;
    c.write(0x8015000c, 800, 2);
    hook(c, 0xd0ec, 0x800a08f4, {2, 3});
    expectEqual(c.r[3], 800u, "Disabled healing");
    for (auto value : {0u, 1u, 10u}) {
        c.r[3] = value;
        hook(c, 0x4f768, 0x800e2f74, {3});
        expectEqual(c.r[3], std::max(1u, value), "Disabled damage");
    }
    c.r[16] = 40;
    c.write(0x80073ba0, 50, 2);
    c.write(0x80073ba2, 100, 2);
    hook(c, 0x4ea74, 0x800e227c, {2, 4, 31});
    expectEqual(c.read(0x80073ba0, 2), 50u, "Disabled drain");
}
void combat() {
    {
        Cpu c;
        c.armor(0x80000 | 0x2000);
        c.r[16] = 40;
        c.write(0x80073ba0, 95, 2);
        c.write(0x80073ba2, 100, 2);
        hook(c, 0x4ea74, 0x800e227c, {2, 4, 31});
        expectEqual(c.read(0x80073ba0, 2), 100u, "Drain clamp");
        expectEqual(c.r[2], 1u, "Critical");
    }
    {
        Cpu c;
        c.armor(0x200000);
        c.r[2] = 0xabcdefff;
        c.r[7] = 0x3fff;
        c.r[6] = 0x80150000;
        c.write(c.r[6], 123);
        hook(c, 0x4f1e4, 0x800e29ec, {2, 4, 7});
        expectEqual(c.r[2], 0xfffu, "Critical mask");
        expectEqual(c.r[7], 0x1000u, "Critical cap");
        expectEqual(c.r[4], 123u, "Critical load");
    }
    const std::pair<unsigned, unsigned> timings[] = {{0, 50}, {0x40000, 80}, {0x400000, 42}, {0x440000, 42}};
    for (auto [bits, timing] : timings) {
        Cpu c;
        c.armor(bits);
        hook(c, 0x6e300, 0x80101b08, {3});
        expectEqual(c.read(0x80112e16, 2), timing, "Movement timing");
    }
    for (bool enabled : {false, true})
        for (auto chance : {0u, 1u, 512u, 3584u, 4000u, 4095u, 4096u}) {
            Cpu c;
            c.armor(enabled ? 0x200000 : 0);
            c.r[7] = chance;
            c.r[6] = 0x80150000;
            hook(c, 0x4f1e4, 0x800e29ec, {2, 4, 7});
            expectEqual(c.r[7], enabled ? std::min(4096u, chance + 512) : chance, "Critical threshold");
        }
}
void muteWardsAndDisplay() {
    for (auto bits : {0u, 0x2000000u}) {
        Cpu c;
        c.armor(bits);
        c.r[19] = 0x80150000;
        c.run(0x800a4584, 0x800a458c);
        expectEqual(c.peCalls, bits ? 0u : 1u, "Mute call");
    }
    {
        Cpu c;
        c.armor();
        c.write(0x80114c1f, 1, 1);
        expectEqual(status(c, 0x104), 1u, "Ward active");
        expectEqual(status(c, 0x108), 0u, "Other ward inactive");
        c.write(0x80114c1f, 0, 1);
        c.write(0x80114c1e, 1, 1);
        expectEqual(status(c, 0x104), 0u, "Ward removed");
        expectEqual(status(c, 0x108), 1u, "Other ward active");
    }
    {
        Cpu c;
        c.armor(0, -25);
        c.r[16] = 0x8010e2b8;
        c.r[3] = 13;
        hook(c, 0x33084, 0x800c688c, {5});
        expectEqual(signedWord(c.r[5]), -25, "Signed HP display");
        expectEqual(c.read(c.r[29] + 100), 13u, "Display stack value");
    }
}
} // namespace
int main() {
    try {
        metabolism();
        statusAndAttachments();
        signedHp();
        effects();
        damageTables();
        catalogue();
        disabledEffects();
        combat();
        muteWardsAndDisplay();
        std::cout << "All 13 original armor CPU regression groups passed (113 catalogue entries)\n";
    } catch (const std::exception &e) {
        std::cerr << e.what() << '\n';
        return 1;
    }
}
