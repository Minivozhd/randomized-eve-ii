#include "delivery/Manager.hpp"
#include "patches/title_screen/TitleBackground.hpp"
#include "patches/weapon_variants/WeaponVariants.hpp"
#include "utils/RuntimePaths.hpp"
#include <cstring>
#include <fstream>
#include <map>
#include <stdexcept>

using namespace delivery;

Manager::Manager(usecase::IRandomizer &_randomizer, domain::file::FileUsecase &_file_usecase,
                 domain::rom::RomUsecase &_rom_usecase, std::atomic<bool> &cancel_flag,
                 std::function<void(int, std::string)> callback)
    : randomizer(_randomizer), file_usecase(_file_usecase), rom_usecase(_rom_usecase),
      m_cancel_flag(cancel_flag), m_progress_callback(callback) {
    workdir = utils::paths::workDirectory();
}

Manager::~Manager() {}

inner_config_t Manager::generateInnerConfig(const config_t &config) {
    // TODO: dynamic unpacking groups depending on randomizing options

    std::map<domain::item::pe2_item_type_t, double> item_classes_distribution{
        {domain::item::HP_RECOVERY, config.item_classes_distribution[0]},
        {domain::item::MP_RECOVERY, config.item_classes_distribution[1]},
        {domain::item::AMMO, config.item_classes_distribution[2]},
        {domain::item::MISC, config.item_classes_distribution[3]},
        {domain::item::UNIQUE, config.item_classes_distribution[4]},
    };

    return inner_config_t{
        .flags =
            domain::inner_config_t::Flags{
                .randomize_enemies = config.flags.randomize_enemies,
                .randomize_items = config.flags.randomize_items,
                .randomize_shops = config.flags.randomize_shops,
                .randomize_music = config.flags.randomize_music,
                .randomize_magic = config.flags.randomize_magic,
                .active_time_battle = config.flags.active_time_battle,
                .enhanced_bosses = config.flags.enhanced_bosses,
                .full_ammo = config.flags.full_ammo,
                .cloak_golem_mult = config.flags.cloak_golem_mult,
                .enable_hud_coloring = config.flags.enable_hud_coloring,
                .enable_special_modes = config.flags.enable_special_modes,
                .enable_map_in_fight = config.flags.enable_map_in_fight,
                .additional_armors = config.flags.additional_armors,
                .magic_pickup_mode = config.flags.magic_pickup_mode,
                .increased_inv = config.flags.increased_inv,
                .no_white_screen = config.flags.no_white_screen,
                .no_win_pose = config.flags.no_win_pose,
                .no_turn = config.flags.no_turn,
                .inventory_mode = config.flags.inventory_mode,
            },
        .ammo_boxes_option = static_cast<int>(config.ammo_boxes_option),
        .ammo_boxes_rando_option = static_cast<int>(config.ammo_boxes_rando_option),
        .enemy_enhancement_option = static_cast<int>(config.enemy_enhancement_option),
        .randomized_stages = config.randomized_stages,
        .randomized_enemies_pool = config.randomized_enemies_pool,
        .randomized_items_drop_pool = config.randomized_items_drop_pool,
        .randomized_items_shop_pool = config.randomized_items_shop_pool,
        .randomized_music_pool = config.randomized_music_pool,
        .shop_costs = config.shop_costs,
        .unpacking_groups{domain::file::LEVEL_G, domain::file::ACROPOLIS_ROOM_G,
                          domain::file::DRYFIELD_DAY_ROOM_G, domain::file::DRYFIELD_NIGHT_ROOM_G,
                          domain::file::MINES_AND_UPPER_SHELTER_ROOM_G,
                          domain::file::NEO_ARK_AND_LOWER_SHELTER_ROOM_G},
        .unpacking_types{domain::file::LEVEL, domain::file::ROOM_LOGIC},
        .starting_spell = config.starting_spell,
        .starting_spell_level = config.starting_spell_level,
        .starting_weapon = config.starting_weapon,
        .starting_armor = config.starting_armor,
        .dryfield_armor = config.dryfield_armor,
        .spell_randomization_strategy = (pe2_randomizer_magic_strategy_t)config.spell_randomization_strategy,
        .weapon_randomization_strategy = (pe2_randomizer_strategy_t)config.weapon_randomization_strategy,
        .armor_randomization_strategy = (pe2_randomizer_strategy_t)config.armor_randomization_strategy,
        .dryfield_armor_randomization_strategy =
            (pe2_randomizer_strategy_t)config.dryfield_armor_randomization_strategy,
        .shop_sost_multiplier = config.shop_cost_multiplier,
        .spell_cost_multiplier = config.spell_cost_multiplier,
        .item_classes_distribution = item_classes_distribution,
        .enemy_damage = config.enemy_damage,
        .enemy_hp = config.enemy_hp,
        .bosses_damage = config.boss_damage,
        .bosses_hp = config.boss_hp,
        .player_model_option = config.player_model_option,
        .hud_color_r = config.hud_color_r,
        .hud_color_g = config.hud_color_g,
        .hud_color_b = config.hud_color_b,
        .randomize_seed = config.seed,
        .input_file_1 = config.input_file_1,
        .input_file_2 = config.input_file_2,
        .randomization_state_string = config.randomization_state_string,
        .randomize_category = static_cast<int>(config.randomize_category),
    };
}

void Manager::extractFromBinFiles(const std::string &filename,
                                  const std::vector<domain::file::pe2stage_t> &stage_files) {
    rom_usecase.extractStages(filename, stage_files, workdir, current_stage, m_progress_callback);
}

void Manager::importToBinFiles(const std::string &filename,
                               const std::vector<domain::file::pe2stage_t> &stage_files) {
    rom_usecase.importStages(filename, stage_files, workdir, current_stage, m_progress_callback);
}

std::pair<std::string, std::string> Manager::generateFilePaths(const std::string &input_file,
                                                               const std::string &serialization_string) {
    // TODO: Move to utils
    std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);
    std::tm now_tm = *std::localtime(&now_c);
    std::stringstream timestamp_stream;
    timestamp_stream << std::put_time(&now_tm, "%Y-%m-%d_%H-%M");
    std::string formatted_timestamp = timestamp_stream.str();
    std::cout << "timestamp: " << formatted_timestamp << std::endl;

    std::string::size_type pos = input_file.find_last_of(".");
    std::string new_file_path =
        input_file.substr(0, pos) + "-" + formatted_timestamp + input_file.substr(pos);
    std::string new_file_path_cue = input_file.substr(0, pos) + "-" + formatted_timestamp + ".cue";

    std::filesystem::remove(new_file_path);
    std::filesystem::remove(new_file_path_cue);

    std::filesystem::copy(input_file, new_file_path);
    std::filesystem::copy(input_file.substr(0, pos) + ".cue", new_file_path_cue);

    std::ifstream input_cue_file(new_file_path_cue);
    if (!input_cue_file.is_open()) {
        std::cerr << "Error opening CUE file: " << new_file_path_cue << std::endl;
        return std::pair("", "");
    }

    std::string cue_content((std::istreambuf_iterator<char>(input_cue_file)),
                            std::istreambuf_iterator<char>());

    std::string randomized_bin_file_path = std::filesystem::path(new_file_path).filename().string();

    size_t cue_pos = cue_content.find("FILE \"");
    if (cue_pos != std::string::npos) {
        size_t end_pos = cue_content.find("\"", cue_pos + 6); // Find the closing quote
        if (end_pos != std::string::npos) {
            cue_content.replace(cue_pos + 6, end_pos - (cue_pos + 6), randomized_bin_file_path);
        }
    }

    input_cue_file.close();

    std::ofstream output_file(new_file_path_cue);
    if (!output_file.is_open()) {
        std::cerr << "Error opening CUE file for writing: " << new_file_path_cue << std::endl;
        return std::pair("", "");
    }

    // Write the modified content back to the file
    output_file << cue_content;

    // Close the output file
    output_file.close();

    // Generate the INI file
    std::string ini_file_path = input_file.substr(0, pos) + "-" + formatted_timestamp + ".ini";
    std::ofstream ini_file(ini_file_path);
    if (!ini_file.is_open()) {
        std::cerr << "Error opening INI file for writing: " << ini_file_path << std::endl;
        return std::pair("", "");
    }

    ini_file << "[RandomizationState]\n";
    ini_file << "State=" << serialization_string << "\n";
    ini_file.close();

    // TODO: Do I really need that?
    std::filesystem::permissions(new_file_path, std::filesystem::perms::all);
    return {new_file_path, new_file_path_cue};
}

int Manager::randomize(const config_t &config) {
    last_error.clear();
    if (config.debug_weapons) {
        last_error = "Debug weapons are unavailable in this release build.";
        return -4;
    }
    // Keep old experimental INI/state bits reserved, but reject them before
    // clearing temporary files or opening either input image in release builds.
    if (config.boss_prototype || config.stranger_row) {
        last_error = "Experimental boss encounters are unavailable in this release build. "
                     "Use settings without the experimental boss prototype.";
        return -4;
    }
    auto fail = [&](int code, const char *stage, const std::exception &error) {
        last_error = std::string(stage) + "\n" + error.what();
        std::cerr << last_error << std::endl;
        return code;
    };
    inner_config_t inner_config = generateInnerConfig(config);
    current_stage = 0;
    m_progress_callback(0, "Step 1/4 : Unpacking bin files...");

    // 1. If dir for tmp files does not exist, create it
    try {
        file_usecase.initDir(workdir, true);
    } catch (const std::exception &e) {
        return fail(-1, "Cannot prepare the working directory.", e);
    }

    // 2. Extract stage files from .bin
    try {
        extractFromBinFiles(config.input_file_1,
                            {domain::file::stages[6], domain::file::stages[0], domain::file::stages[1],
                             domain::file::stages[2], domain::file::stages[3]});
    } catch (const std::exception &e) {
        return fail(-2, "Cannot extract the input disc images.", e);
    }
    if (m_cancel_flag.load()) {
        return 1;
    }
    try {
        extractFromBinFiles(config.input_file_2, {domain::file::stages[4], domain::file::stages[5]});
    } catch (const std::exception &e) {
        return fail(-2, "Cannot extract the input disc images.", e);
    }
    if (m_cancel_flag.load()) {
        return 1;
    }

    // 3. Select files by groups to unpack
    std::vector<domain::file::pe2file_t> filtered_files;

    for (auto stage : domain::file::stages) {
        for (auto tag : stage.group_tags) {
            if (std::find(inner_config.unpacking_groups.begin(), inner_config.unpacking_groups.end(), tag) !=
                inner_config.unpacking_groups.end()) {
                // TODO: better tag recofnition (especially when we have enhanced enemies and so on...)
                filtered_files.insert(filtered_files.end(), stage.inner_files.begin(),
                                      stage.inner_files.end());
                break;
            }
        }
    }
    if (config.flags.enhanced_bosses || config.boss_damage != 100 || config.boss_hp != 100) {
        filtered_files.insert(filtered_files.end(), domain::file::stage0_bosses_info.begin(),
                              domain::file::stage0_bosses_info.end());
    }

    // TODO: make it adaptive
    filtered_files.insert(filtered_files.end(), domain::file::stage0_enemies_info.begin(),
                          domain::file::stage0_enemies_info.end());

    // 4. Unpack selected files, randomize, pack
    // TOOD: Error handling
    try {
        file_usecase.unpack(workdir, filtered_files);
    } catch (const std::exception &e) {
        return fail(-3, "Cannot unpack the game resources.", e);
    }
    if (m_cancel_flag.load()) {
        return 1;
    }
    try {
        randomizer.randomize(inner_config);
    } catch (const std::exception &e) {
        return fail(-4, "Randomization failed.", e);
    }
    if (m_cancel_flag.load()) {
        return 1;
    }
    try {
        file_usecase.pack(workdir, filtered_files);
        patches::title::applyBackground(std::filesystem::path(workdir) / "STAGE0.CDF");
        patches::weapon_variants::apply(workdir, config.rupert_revolver, config.kyle_p229);
    } catch (const std::exception &e) {
        return fail(-5, "Cannot repack the game resources.", e);
    }
    if (m_cancel_flag.load()) {
        return 1;
    }

    try {
        // 5. generate files
        auto [new_file_path_1, new_file_path_cue_1] =
            generateFilePaths(config.input_file_1, config.randomization_state_string);
        auto [new_file_path_2, new_file_path_cue_2] =
            generateFilePaths(config.input_file_2, config.randomization_state_string);

        // 6. Import stage files to bin rom files
        m_progress_callback(0, "Step 4/4 : Packing bin files...");
        current_stage = 0;
        importToBinFiles(new_file_path_1,
                         {domain::file::stages[6], domain::file::stages[0], domain::file::stages[1],
                          domain::file::stages[2], domain::file::stages[3]});
        importToBinFiles(new_file_path_2,
                         {domain::file::stages[6], domain::file::stages[0], domain::file::stages[3],
                          domain::file::stages[4], domain::file::stages[5]});
    } catch (const std::exception &e) {
        return fail(-6, "Cannot create the randomized disc images.", e);
    }
    return 0;
}
