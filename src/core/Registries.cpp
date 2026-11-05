#include "core/Registries.h"
#include "types.h"

namespace Registries {
    Registry<BlockDef> BLOCKS(BlockDef{"unknown", 0, {0, 0, 0, 0, 0, 0}, true, false, true, false, 0.0});
    Registry<EntityDef> ENTITIES(EntityDef{"unknown", 0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, false, false, 0});

void init() {
    BLOCKS.registerObject("unknown", BlockDef{
        "unknown", 0, {0, 0, 0, 0, 0, 0},
        true, false, true, false,
        0.0
    });
    BLOCKS.registerObject("air", BlockDef{
        "air", 1, {0, 0, 0, 0, 0, 0},
        true, false, true, false,
        0.0
    });
    BLOCKS.registerObject("grass", BlockDef{
        "grass", 2, {1, 2, 2, 2, 2, 3},
        false, true, false, false,
        1.0
    });
    BLOCKS.registerObject("dirt", BlockDef{
        "dirt", 3, {3, 3, 3, 3, 3, 3},
        false, true, false, false,
        0.8
    });
    BLOCKS.registerObject("stone", BlockDef{
        "stone", 4, {4, 4, 4, 4, 4, 4},
        false, true, false, false,
        5.0
    });
}
}
