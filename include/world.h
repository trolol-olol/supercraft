#ifndef WORLD_H
#define WORLD_H

#include "types.h"
#include "blocks.h"
#include "entity.h"
#include <vector>
#include <cstdint>

class World {
public:
    static constexpr int WORLD_SIZE_X = 64;
    static constexpr int WORLD_SIZE_Y = 8;
    static constexpr int WORLD_SIZE_Z = 64;
    static constexpr int HALF_X = WORLD_SIZE_X / 2;
    static constexpr int HALF_Z = WORLD_SIZE_Z / 2;

    World();
    ~World();

    void createWorld();
    void destroyWorld();

    Block getBlock(Vec3 pos);
    void setBlock(Vec3 pos, Block block);
    bool isAir(Vec3 pos);

    Entity* addEntity(Vec3d pos, std::string& id);
    Entity* getEntityByIndex(size_t index);
    std::vector<Entity*>::iterator entitiesIter();
    std::vector<Entity*>::iterator entitiesIterEnd();

    bool isFaceVisible(Vec3 pos, side_t side);
    bool isValidPos(Vec3 pos);
private:
    Block* blocks;
    std::vector<Entity*> entities;
    int total_blocks;
    int getIndex(Vec3 pos);
};

#endif
