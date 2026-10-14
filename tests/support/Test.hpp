#pragma once
#include <sstream>
#include <stdexcept>
#include <string_view>

namespace test {
inline void require(bool value, std::string_view message) {
    if (!value)
        throw std::runtime_error(std::string(message));
}
template <class A, class B> void expectEqual(const A &actual, const B &expected, std::string_view message) {
    if (actual != expected) {
        std::ostringstream error;
        error << message << ": expected " << expected << ", got " << actual;
        throw std::runtime_error(error.str());
    }
}
} // namespace test
