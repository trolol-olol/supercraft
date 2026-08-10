#ifndef BLOCKS_H
#define BLOCKS_H

#include "raylib.h"
#include <stdbool.h>

typedef struct {
    int id;
    const char *idstr;
    int texture_top;
    int texture_bottom;
    int texture_left;
    int texture_right;
    int texture_front;
    int texture_back;
    bool is_air;
} BlockDef;

#define ATLAS_SIZE 16

#define TEX_UV(index) \
    (float)((index) % ATLAS_SIZE) / ATLAS_SIZE, \
    (float)((index) / ATLAS_SIZE) / ATLAS_SIZE, \
    (float)((index) % ATLAS_SIZE + 1) / ATLAS_SIZE, \
    (float)((index) / ATLAS_SIZE + 1) / ATLAS_SIZE

BlockDef getBlockDef(int id);
BlockDef getBlockDefByName(const char *idstr);
int blockExists(int id);
int getBlockCount(void);

#endif
