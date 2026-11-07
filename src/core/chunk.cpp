#include "types.h"
#include "world.h"
#include "core/chunk.h"
#include "core/Registries.h"
#include <cstdlib>

Chunk::Chunk(ChunkPos chunk_pos) {
    blocks = (Block*)calloc(16 * World::WORLD_SIZE_Y * 16, sizeof(Block));
    pos=chunk_pos;
}

Chunk::~Chunk() {
    free(blocks);
}

Block Chunk::getBlock(Vec3 pos) {
    Block empty_block = {0, 0};
    if (!isValidPos(pos)) return empty_block;

    int idx = getIndex(pos);
    return blocks[idx];
}

void Chunk::setBlock(Vec3 pos, Block block) {
    if (!isValidPos(pos)) return;
    int idx = getIndex(pos);
    blocks[idx] = block;
}

bool Chunk::isAir(Vec3 pos) {
    Block block = getBlock(pos);
    BlockDef def = Registries::BLOCKS.getObject(block.type);
    return def.isAir;
}

bool Chunk::isValidPos(Vec3 pos) {
    return pos.x >= 0 && pos.x < 16 &&
           pos.y >= 0 && pos.y < World::WORLD_SIZE_Y &&
           pos.z >= 0 && pos.z < 16;
}

int Chunk::getIndex(Vec3 pos) {
    return (pos.x * World::WORLD_SIZE_Y + pos.y) * 16 + pos.z;
}
