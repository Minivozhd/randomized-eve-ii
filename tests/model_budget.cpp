#include "domain/enemy/entities/ModelBudget.hpp"
#include <iostream>
#include <stdexcept>
using namespace domain::enemy::models;
void require(bool condition) {
    if (!condition)
        throw std::runtime_error("model budget regression");
}
int main() {
    // Native model-heap negative controls: these used to be written to ROM.
    constexpr uint32_t cavern = 0x444b0 - 0x10000, mesa = 0x4b5f0 - 0x10000;
    std::map<uint8_t, size_t> badCavern{{0x16, 1}, {0x38, 3}};
    require(!fits(cavern, badCavern));
    fit(cavern, badCavern);
    require(fits(cavern, badCavern) && badCavern.at(0x16) == 1 && badCavern.at(0x38) >= 1);
    std::map<uint8_t, size_t> badMesa{{0x17, 1}, {0x27, 2}};
    require(!fits(mesa, badMesa));
    fit(mesa, badMesa);
    require(fits(mesa, badMesa) && badMesa.at(0x17) == 1 && badMesa.at(0x27) == 1);
    size_t checked = 0;
    for (auto a : costs)
        for (auto b : costs)
            for (uint32_t capacity : {cavern, mesa, 0x30000u, 0x4d6b0u}) {
                std::map<uint8_t, size_t> minimum{{a.id, 1}, {b.id, 1}};
                if (!fits(capacity, minimum))
                    continue;
                for (size_t n = 1; n <= 64; ++n) {
                    auto counts = minimum;
                    counts[a.id] = n;
                    counts[b.id] += n / 2;
                    fit(capacity, counts);
                    require(fits(capacity, counts));
                    require(counts.size() == minimum.size());
                    for (auto [id, count] : counts)
                        require(count >= 1);
                    auto stable = counts;
                    fit(capacity, stable);
                    require(stable == counts);
                    ++checked;
                }
            }
    bool rejected = false;
    try {
        std::map<uint8_t, size_t> impossible{{0x16, 1}, {0x27, 1}};
        fit(cavern, impossible);
    } catch (const std::runtime_error &) {
        rejected = true;
    }
    require(rejected);
    require(actorBytes(0xff) == 0);
    std::cout << checked << " model budget fits preserve selected species and allocation bound\n";
}
