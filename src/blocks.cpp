#include "blocks.h"
#include <string.h>
#include <cstdio>

static BlockDef BLOCKS[] = {
    {0, "air", 0, 0, 0, 0, 0, 0, 1},
    {1, "grass", 1, 2, 1, 1, 1, 1, 0},
    {2, "dirt", 2, 2, 2, 2, 2, 2, 0},
    {3, "stone", 3, 3, 3, 3, 3, 3, 0},
    {-1, "unknown", 0, 0, 0, 0, 0, 0, 1}
};

int getBlockCount(void) {
    int count = 0;
    while (BLOCKS[count].id != -1) {
        count++;
    }
    return count;
}

BlockDef getBlockDef(int id) {
    int i = 0;
    while (BLOCKS[i].id != -1) {
        if (BLOCKS[i].id == id) {
            return BLOCKS[i];
        }
        i++;
    }
    return (BlockDef){-1, "unknown", 0, 0, 0, 0, 0, 0, 1};
}

BlockDef getBlockDefByName(const char *idstr) {
    int i = 0;
    while (BLOCKS[i].id != -1) {
        if (strcmp(BLOCKS[i].idstr, idstr) == 0) {
            return BLOCKS[i];
        }
        i++;
    }
    return (BlockDef){-1, "unknown", 0, 0, 0, 0, 0, 0, 1};
}

int blockExists(int id) {
    BlockDef def = getBlockDef(id);
    return def.id != -1;
}
