#include "world.h"
#include <cmath>
#include <cstdlib>

using namespace std;

World::World() {
    createWorld();
}

World::~World() {
    destroyWorld();
}

void World::createWorld(){
    int total_blocks = WORLD_SIZE_X * WORLD_SIZE_Y * WORLD_SIZE_Z;
    blocks = (Block*)calloc(total_blocks, sizeof(Block));
    if (!blocks) return;

    for (int x = -HALF_X; x < HALF_X; x++){
        for (int z = -HALF_Z; z < HALF_Z; z++){
            float height = 3.0f;
            int h = (int)height;
            if (h < 0) h = 0;
            if (h >= WORLD_SIZE_Y) h = WORLD_SIZE_Y - 1;

            for (int y = 0; y <= h; y++){
                Vec3 pos = {x, y, z};
                int idx = getIndex(pos);

                if (y == h) blocks[idx].type = 1;
                else if (y > h - 2) blocks[idx].type = 2;
                else blocks[idx].type = 3;
            }
        }
    }
}

void World::destroyWorld() {
    if (blocks) {
        free(blocks);
        blocks = nullptr;
    }
}

Block World::getBlock(Vec3 pos) {
    Block empty_block = {65535, 0};
    if (!isValidPos(pos)) return empty_block;

    int idx = getIndex(pos);
    return blocks[idx];
}

void World::setBlock(Vec3 pos, Block block) {
    if (!isValidPos(pos)) return;
    int idx = getIndex(pos);
    blocks[idx] = block;
}

bool World::isAir(Vec3 pos){
    Block block = getBlock(pos);
    BlockDef def = getBlockDef(block.type);
    return def.is_air;
}

bool World::isFaceVisible(Vec3 pos, side_t side) {
    Vec3 neighbor = pos;

    switch (side) {
        case UP:    neighbor.y += 1; break;
        case DOWN:  neighbor.y -= 1; break;
        case LEFT:  neighbor.x -= 1; break;
        case RIGHT: neighbor.x += 1; break;
        case FRONT: neighbor.z += 1; break;
        case BACK:  neighbor.z -= 1; break;
    }

    return isAir(neighbor);
}

bool World::isValidPos(Vec3 pos) {
    return pos.x >= -HALF_X && pos.x < HALF_X &&
           pos.y >= 0 && pos.y < WORLD_SIZE_Y &&
           pos.z >= -HALF_Z && pos.z < HALF_Z;
}

int World::getIndex(Vec3 pos) {
    int x = pos.x + HALF_X;
    int y = pos.y;
    int z = pos.z + HALF_Z;
    return (x * WORLD_SIZE_Y + y) * WORLD_SIZE_Z + z;
}
