#include "core/Registries.h"
#include "types.h"

namespace Registries {
    Registry<BlockDef> BLOCKS(BlockDef{"unknown", 0, {0, 0, 0, 0, 0, 0, 0}, true, false, true, false, 0.0});
    Registry<EntityDef> ENTITIES(EntityDef{"unknown", 0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, false, false, 0});
    Registry<EntityModel> MODELS(EntityModel{std::vector<ModelRect>()});

void init() {
    BLOCKS.registerObject("unknown", BlockDef{
        "unknown", 0, {0, 0, 0, 0, 0, 0, 0},
        true, false, true, false,
        0.0
    });
    BLOCKS.registerObject("air", BlockDef{
        "air", 1, {0, 0, 0, 0, 0, 0, 0},
        true, false, true, false,
        0.0
    });
    BLOCKS.registerObject("grass", BlockDef{
        "grass", 2, {0b110000, 1, 3, 2, 2, 2, 2},
        false, true, false, false,
        1.0
    });
    BLOCKS.registerObject("dirt", BlockDef{
        "dirt", 3, {0b111111, 3, 3, 3, 3, 3, 3},
        false, true, false, false,
        0.8
    });
    BLOCKS.registerObject("stone", BlockDef{
        "stone", 4, {0b111111, 4, 4, 4, 4, 4, 4},
        false, true, false, false,
        5.0
    });
    ENTITIES.registerObject("unknown", EntityDef{
        "unknown", 0, 0.0, 0.0, 0.0,
         0.0, 0.0, 0.0, false, false, 0
    });
    ENTITIES.registerObject("test", EntityDef{
        "test", 1, 1, 1, 1.5,
        0.0, 0.0, 0.0, false, false, 1
    });
    MODELS.registerObject("unknown", EntityModel{std::vector<ModelRect>()});
    MODELS.registerObject("test",EntityModel{std::vector<ModelRect>({{0,0,0},{1,1,1.5}})});
}
}
