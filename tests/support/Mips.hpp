#pragma once
// Bounded MIPS-I interpreter for patch regressions. Models load and branch
// delay slots; external game functions are explicit stubs, not game emulation.
#include "Test.hpp"
#include <array>
#include <bit>
#include <cstdint>
#include <optional>
#include <set>
#include <span>
#include <vector>

namespace test {
inline std::int32_t signedWord(std::uint32_t value) {
    return std::bit_cast<std::int32_t>(value);
}
struct Mips {
    std::vector<std::uint8_t> memory = std::vector<std::uint8_t>(0x200000);
    std::array<std::uint32_t, 32> r{};
    std::uint32_t hi = 0, lo = 0, pc = 0, next = 0;
    std::optional<std::pair<unsigned, std::uint32_t>> load;
    std::set<std::uint32_t> attachments;
    unsigned peCalls = 0;

    Mips() {
        r[29] = 0x801f0000;
        r[31] = 0x801ffff0;
    }
    std::size_t address(std::uint32_t value, std::size_t length) const {
        const auto offset = value & 0x1fffff;
        require(length <= memory.size() - offset, "MIPS access crosses RAM boundary");
        return offset;
    }
    void put(std::uint32_t at, std::span<const std::uint8_t> bytes) {
        const auto offset = address(at, bytes.size());
        std::copy(bytes.begin(), bytes.end(), memory.begin() + offset);
    }
    void write(std::uint32_t at, std::uint32_t value, unsigned size = 4) {
        const auto offset = address(at, size);
        for (unsigned i = 0; i < size; ++i)
            memory[offset + i] = value >> (8 * i);
    }
    std::uint32_t read(std::uint32_t at, unsigned size = 4) const {
        const auto offset = address(at, size);
        std::uint32_t value = 0;
        for (unsigned i = 0; i < size; ++i)
            value |= std::uint32_t(memory[offset + i]) << (8 * i);
        return value;
    }
    void armor(std::uint32_t bits = 0, int hp = 0) {
        write(0x80073bab, 1, 1);
        write(0x8010e2b8, bits);
        write(0x8010e2bc, hp < 0 ? -hp : hp, 1);
        write(0x8010e2bf, hp < 0, 1);
    }
    void run(std::uint32_t start, std::uint32_t stop = 0x801ffff0) {
        pc = start;
        next = start + 4;
        load.reset();
        for (unsigned count = 0; count < 20000; ++count) {
            if (pc == stop)
                return;
            if (pc == 0x800bc3f8 || pc == 0x800a1558 || pc == 0x800a2f60) {
                std::uint32_t result = 0;
                if (pc == 0x800bc3f8)
                    result = attachments.contains(r[4]);
                else if (pc == 0x800a1558)
                    result = 120;
                else
                    ++peCalls;
                const auto target = r[31];
                for (unsigned reg = 2; reg < 16; ++reg)
                    r[reg] = 0xbadc0000 + reg;
                r[24] = 0xbadc0018;
                r[25] = 0xbadc0019;
                r[2] = result;
                pc = target;
                next = target + 4;
                load.reset();
            } else
                step();
        }
        throw std::runtime_error("MIPS instruction budget exceeded at " + std::to_string(pc));
    }
    void step() {
        const auto ins = read(pc), op = ins >> 26, rs = (ins >> 21) & 31, rt = (ins >> 16) & 31;
        const auto rd = (ins >> 11) & 31, shift = (ins >> 6) & 31, fn = ins & 63;
        const auto imm = ins & 65535;
        const auto si =
            imm & 0x8000 ? static_cast<std::int32_t>(imm) - 65536 : static_cast<std::int32_t>(imm);
        const auto a = r[rs], b = r[rt];
        std::array<std::optional<std::uint32_t>, 32> writes{};
        std::optional<std::pair<unsigned, std::uint32_t>> pending;
        auto target = next + 4;
        auto wr = [&](unsigned reg, std::uint32_t value) { writes[reg] = value; };
        if (op == 0) {
            switch (fn) {
            case 0:
                wr(rd, b << shift);
                break;
            case 2:
                wr(rd, b >> shift);
                break;
            case 3:
                wr(rd, signedWord(b) >> shift);
                break;
            case 4:
                wr(rd, b << (a & 31));
                break;
            case 6:
                wr(rd, b >> (a & 31));
                break;
            case 7:
                wr(rd, signedWord(b) >> (a & 31));
                break;
            case 8:
                target = a;
                break;
            case 9:
                target = a;
                wr(rd, pc + 8);
                break;
            case 16:
                wr(rd, hi);
                break;
            case 17:
                hi = a;
                break;
            case 18:
                wr(rd, lo);
                break;
            case 19:
                lo = a;
                break;
            case 24:
            case 25: {
                const std::uint64_t product =
                    fn == 24 ? static_cast<std::uint64_t>(std::int64_t(signedWord(a)) * signedWord(b))
                             : std::uint64_t(a) * b;
                lo = product;
                hi = product >> 32;
                break;
            }
            case 32:
            case 33:
                wr(rd, a + b);
                break;
            case 34:
            case 35:
                wr(rd, a - b);
                break;
            case 36:
                wr(rd, a & b);
                break;
            case 37:
                wr(rd, a | b);
                break;
            case 38:
                wr(rd, a ^ b);
                break;
            case 39:
                wr(rd, ~(a | b));
                break;
            case 42:
                wr(rd, signedWord(a) < signedWord(b));
                break;
            case 43:
                wr(rd, a < b);
                break;
            default:
                throw std::runtime_error("Unsupported MIPS SPECIAL instruction");
            }
        } else if (op == 2 || op == 3) {
            target = ((pc + 4) & 0xf0000000) | ((ins & 0x3ffffff) << 2);
            if (op == 3)
                wr(31, pc + 8);
        } else if (op == 1 || (op >= 4 && op <= 7)) {
            bool condition = false;
            switch (op) {
            case 1:
                require(rt <= 1, "Unsupported REGIMM variant");
                condition = rt == 0 ? signedWord(a) < 0 : signedWord(a) >= 0;
                break;
            case 4:
                condition = a == b;
                break;
            case 5:
                condition = a != b;
                break;
            case 6:
                condition = signedWord(a) <= 0;
                break;
            case 7:
                condition = signedWord(a) > 0;
                break;
            }
            if (condition)
                target = pc + 4 + static_cast<std::uint32_t>(si * 4);
        } else if (op == 8 || op == 9)
            wr(rt, a + si);
        else if (op == 10)
            wr(rt, signedWord(a) < si);
        else if (op == 11)
            wr(rt, a < static_cast<std::uint32_t>(si));
        else if (op == 12)
            wr(rt, a & imm);
        else if (op == 13)
            wr(rt, a | imm);
        else if (op == 14)
            wr(rt, a ^ imm);
        else if (op == 15)
            wr(rt, imm << 16);
        else if (op == 32 || op == 33 || op == 35 || op == 36 || op == 37) {
            const unsigned size = op == 35 ? 4 : (op == 33 || op == 37 ? 2 : 1);
            auto value = read(a + si, size);
            if ((op == 32 || op == 33) && (value & (1u << (size * 8 - 1))))
                value |= ~((1u << (size * 8)) - 1);
            pending = std::pair{rt, value};
        } else if (op == 40 || op == 41 || op == 43)
            write(a + si, b, op == 43 ? 4 : (op == 41 ? 2 : 1));
        else
            throw std::runtime_error("Unsupported MIPS instruction");
        if (load && !writes[load->first])
            r[load->first] = load->second;
        for (unsigned reg = 1; reg < 32; ++reg)
            if (writes[reg])
                r[reg] = *writes[reg];
        r[0] = 0;
        load = pending;
        pc = next;
        next = target;
    }
};
} // namespace test
