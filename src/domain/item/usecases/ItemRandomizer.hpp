#pragma once

#include "domain/config/innerconfig.hpp"
#include "domain/encounter/entities/Encounter.hpp"
#include "domain/encounter/entities/Reward.hpp"
#include "domain/file/entities/PE2File.hpp"
#include "domain/game/entities/Stage.hpp"
#include "domain/item/entities/Item.hpp"
#include "domain/randomizer/entities/Strategy.hpp"
#include "domain/randomizer/usecases/IFeatureRandomizer.hpp"
#include "utils/FileWorks/FileWorks.hpp"
#include "utils/Logger.hpp"
#include "utils/VectorWorks/VectorWorks.hpp"
#include <algorithm>
#include <cstdint>
#include <cstring>
#include <fstream>
#include <map>
#include <random>
#include <set>
#include <vector>

#define MEDICINE_WHEEL_RPOBABILITY 0.5
#define MAXIMUM_SHOP_COST 65535

namespace usecase::items {
class ItemRandomizer : public IFeatureRandomizer {
  public:
    ItemRandomizer(std::shared_ptr<PE2Logger> _logger, std::mt19937 _gen, std::string _temp_dir_name,
                   domain::inner_config_t _config)
        : config(_config), logger(_logger), temp_dir_name(_temp_dir_name), gen(_gen) {};
    pe2_randomizer_context randomize(pe2_randomizer_context);

  private:
    friend struct ItemRandomizerTestAccess;
    std::set<domain::game::pe2_level_name_t> randomized_box_levels;
    uint8_t randomizeItem(std::vector<domain::item::pe2_item_type_t>, domain::item::pe2_item_pickup_t, bool);
    void randomizePickups(domain::game::pe2_level_name_t level,
                          std::vector<std::pair<size_t, domain::item::pe2_item_pickup_t>>, bool, bool);
    void randomizeRewards(domain::game::pe2_level_name_t level, domain::encounter::pe2_enc_rewards_segment_t,
                          std::vector<domain::encounter::pe2_enc_reward_descriptor_t>);
    void randomizeStartingWeaponAndArmor(pe2_randomizer_context &);
    void randomizeStartingItems();
    void randomizeShops();
    std::vector<uint8_t> getShopRandomItemsList(std::vector<uint8_t>, size_t, bool);
    void updateAkropolisScript();
    void updateShopCost();

    domain::inner_config_t config;
    std::shared_ptr<PE2Logger> logger;
    std::string temp_dir_name;
    std::mt19937 gen;

    std::vector<std::pair<uint8_t, size_t>> best_dropped_unique_items{};

    std::vector<uint8_t> shop_items{
        0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x0b, 0x0d, 0x3a, 0x3b, 0x3c, 0x3d, 0x3e,
        0x41, 0x09, 0x0a, 0x0c, 0x42, 0x43, 0x44, 0x45, 0x46, 0x60, 0x61, 0x62, 0x63, 0x64, 0x65,
        0x66, 0x67, 0x68, 0x69, 0x6a, 0x6b, 0x6c, 0x6d, 0x81, 0x82, 0x83, 0x84, 0x88, 0x8a, 0x8b,
        0x8c, 0x8d, 0x8e, 0x8f, 0x90, 0x95, 0x96, 0x9d, 0xa0, 0xa1, 0xa2, 0xa6, 0xa7, 0xa9, 0xaa,
        0xab, 0xac, 0xad, 0xae, 0xaf, 0x36, 0x37, 0x38, 0x39, 0x3f, 0x40, 0x0e,
    };

    std::map<domain::item::pe2_item_rarity_t, std::vector<uint8_t>> available_unique_items{
        {domain::item::COMMON, {0x60, 0x63, 0x64, 0x67, 0x6c, 0x0e, 0x3f, 0x40, 0x81}},
        {domain::item::RARE, {0x62, 0x65, 0x68, 0x69, 0x6a, 0x82, 0x83, 0x84, 0x8a, 0x8c, 0x8f, 0x9d}},
        {domain::item::LEGENDARY,
         {0x36, 0x37, 0x38, 0x39, 0x61, 0x66, 0x6b, 0x88, 0x8b, 0x8d, 0x8e, 0x90, 0x95, 0x96}}};

    // Unlocked by the starting weapon and subsequently selected weapons.
    std::map<domain::item::pe2_item_rarity_t, std::vector<uint8_t>> available_ammo_items{
        {domain::item::COMMON, {}}, {domain::item::RARE, {}}, {domain::item::LEGENDARY, {}}};
};
}; // namespace usecase::items
