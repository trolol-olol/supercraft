#ifndef REGISTRIES_H
#define REGISTRIES_H

#include "core/Registry.h"
#include "types.h"

namespace Registries {
    extern Registry<BlockDef> BLOCKS;
    extern Registry<EntityDef> ENTITIES;
    void init();
}

#endif
