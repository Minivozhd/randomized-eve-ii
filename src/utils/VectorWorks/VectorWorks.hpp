#pragma once

#include <algorithm>
#include <cstdint>
#include <random>
#include <unordered_set>
#include <vector>

namespace utils::vector_works {
/*
    SUBSTRACT VECTOR

    Substracts all elements of VEC2 from VEC1
*/
std::vector<uint8_t> substractVector(const std::vector<uint8_t> &vec1, const std::vector<uint8_t> &vec2);

std::vector<uint8_t> concatVector(const std::vector<uint8_t> &, const std::vector<uint8_t> &);

std::vector<uint8_t> intersecVector(const std::vector<uint8_t> &, const std::vector<uint8_t> &);

std::vector<uint8_t> unionVector(const std::vector<uint8_t> &, const std::vector<uint8_t> &);

/*
    GET RANDOM ELEMENT

    Finds random element from the VEC vector. Returns DEFAULT_VALUE if VEC
    size equals to 0. Also needs GEN Random Generator Engine to forward
    seed from main randomizer class.
*/
uint8_t getRandomElement(std::mt19937 &gen, std::vector<uint8_t> vec, uint8_t default_value);
} // namespace utils::vector_works
