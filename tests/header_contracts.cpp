// This order used to fail: armor macros expanded item enum names in config.
#include "delivery/Config.hpp"
#include "domain/armor/entities/Armor.hpp"
#include "domain/config/innerconfig.hpp"
#include "resources/EmbeddedResources.hpp"
#include "support/Test.hpp"

static_assert(domain::armor::HP_RECOVERY == 0x00010000u);
static_assert(domain::armor::MP_RECOVERY == 0x80000000u);
static_assert(domain::item::HP_RECOVERY == 1);
static_assert(domain::item::MP_RECOVERY == 2);
int main() {
    const domain::inner_config_t config{};
    test::require(!config.item_classes_distribution.empty(), "Default item distribution is missing");
}
