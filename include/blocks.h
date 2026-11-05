#ifndef BLOCKS_H
#define BLOCKS_H

#include "raylib.h"
#include "types.h"
#include <stdbool.h>

#define ATLAS_SIZE 16

#define TEX_UV(index) \
    (float)((index) % ATLAS_SIZE) / ATLAS_SIZE, \
    (float)((index) / ATLAS_SIZE) / ATLAS_SIZE, \
    (float)((index) % ATLAS_SIZE + 1) / ATLAS_SIZE, \
    (float)((index) / ATLAS_SIZE + 1) / ATLAS_SIZE

#endif
