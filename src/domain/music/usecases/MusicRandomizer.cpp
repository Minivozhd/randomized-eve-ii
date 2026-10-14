#include "domain/music/usecases/MusicRandomizer.hpp"

using namespace domain::music;
using namespace domain::game;
using namespace domain::file;

/*
    RANDOMIZE

    Goes through all available levels and randomizes music
*/
pe2_randomizer_context usecase::music::MusicRandomizer::randomize(pe2_randomizer_context context) {
    (void)context;
    for (const auto &entry_map_entry : music_entries_map) {
        std::string filename = temp_dir_name + "/" + getNameFromStage(STAGE0) + "/" +
                               getNameFromLevel(entry_map_entry.first) + FORMAT_LEVEL_FILE +
                               FORMAT_DECOMPRESSED;
        randomizeMusicInStage(filename, entry_map_entry.second);
    }
    return context;
}

/*
    RANDOMIZE MUSIC IN STAGE

    Goes through every entry in ENTRIES vector and applies randomization
    according to the entry info, then writes to the file with name
    FILENAME.
*/
void usecase::music::MusicRandomizer::randomizeMusicInStage(std::string filename,
                                                            std::vector<pe2_music_entry_t> entries) {
    std::fstream outfile(filename, std::ios::in | std::ios::out | std::ios::binary | std::ios::ate);
    if (!outfile.is_open()) {
        logger->log("[ERR] [Randomizer] [MusicRandomizer] couldn't open file: ", filename);
        logger->log("[ERR] [Randomizer] [MusicRandomizer] Error code : () ", std::strerror(errno), " )");
        throw std::runtime_error("[MusicRandomizer] couldn't open file.");
    }
    logger->log("[INF] [Randomizer] [MusicRandomizer] opened file: ", filename,
                " and starting randomizing...");

    for (pe2_music_entry_t entry : entries) {
        std::vector<uint8_t> initial_music_pool =
            utils::vector_works::intersecVector(music_types_map[entry.type], config.randomized_music_pool);
        std::vector<uint8_t> allowed_music_ids =
            utils::vector_works::substractVector(initial_music_pool, entry.forbidden_music);
        if (allowed_music_ids.size() <= 0) {
            allowed_music_ids =
                utils::vector_works::substractVector(music_types_map[MUSIC_MISC], entry.forbidden_music);
        }

        // Setting default music if couldn't find anything suitable. Let it be acropolis first peace music.
        uint8_t random_track_id = utils::vector_works::getRandomElement(gen, allowed_music_ids, 0x21);

        for (size_t offset : entry.offsets) {
            utils::file_works::writeByte(outfile, random_track_id, offset);
        }
    }

    logger->log("[INF] [Randomizer] [MusicRandomizer] finished randomizing");
}
