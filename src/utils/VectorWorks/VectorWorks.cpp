#include "utils/VectorWorks/VectorWorks.hpp"

std::vector<uint8_t> utils::vector_works::substractVector(const std::vector<uint8_t> &vec1,
                                                          const std::vector<uint8_t> &vec2) {
    std::unordered_set<uint8_t> forbidden_set(vec2.begin(), vec2.end());
    std::vector<uint8_t> difference;

    for (uint8_t num : vec1) {
        if (forbidden_set.find(num) == forbidden_set.end()) {
            difference.push_back(num);
        }
    }

    return difference;
}

std::vector<uint8_t> utils::vector_works::concatVector(const std::vector<uint8_t> &vec1,
                                                       const std::vector<uint8_t> &vec2) {
    std::vector<uint8_t> result;

    result.reserve(vec1.size() + vec2.size());
    result.insert(result.end(), vec1.begin(), vec1.end());
    result.insert(result.end(), vec2.begin(), vec2.end());

    return result;
}

std::vector<uint8_t> utils::vector_works::intersecVector(const std::vector<uint8_t> &v1,
                                                         const std::vector<uint8_t> &v2) {
    std::vector<uint8_t> v1_copy = v1;
    std::vector<uint8_t> v2_copy = v2;

    std::sort(v1_copy.begin(), v1_copy.end());
    std::sort(v2_copy.begin(), v2_copy.end());

    std::vector<uint8_t> v3;
    std::set_intersection(v1_copy.begin(), v1_copy.end(), v2_copy.begin(), v2_copy.end(), back_inserter(v3));
    return v3;
}

std::vector<uint8_t> utils::vector_works::unionVector(const std::vector<uint8_t> &vec1,
                                                      const std::vector<uint8_t> &vec2) {
    std::vector<uint8_t> unionVec(vec1.begin(), vec1.end());
    unionVec.insert(unionVec.end(), vec2.begin(), vec2.end());

    std::sort(unionVec.begin(), unionVec.end());

    auto last = std::unique(unionVec.begin(), unionVec.end());
    unionVec.erase(last, unionVec.end());

    return unionVec;
}

uint8_t utils::vector_works::getRandomElement(std::mt19937 &gen, std::vector<uint8_t> vec,
                                              uint8_t default_value) {
    if (vec.size() == 0) {
        return default_value;
    }
    std::uniform_int_distribution<size_t> dist(0, vec.size() - 1);
    return vec[dist(gen)];
}
