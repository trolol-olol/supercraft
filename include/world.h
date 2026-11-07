#ifndef WORLD_H
#define WORLD_H

#include "types.h"
#include "blocks.h"
#include "entity.h"
#include "core/chunk.h"
#include <unordered_map>
#include <vector>
#include <cstdint>

class World {
public:
    static constexpr int WORLD_SIZE_Y = 8;

    World();
    ~World();

    void createWorld();
    void destroyWorld();

    Block getBlock(Vec3 pos);
    void setBlock(Vec3 pos, Block block);
    bool isAir(Vec3 pos);

    Chunk* getChunk(ChunkPos pos);
    Chunk& getOrLoadChunk(ChunkPos pos);
    void updateLoadedChunks(double px, double pz, int dist);
    auto& getChunks() { return chunks; }

    Entity* addEntity(Vec3d pos, std::string& id);
    Entity* getEntityByIndex(size_t index);
    std::vector<Entity*>::iterator entitiesIter();
    std::vector<Entity*>::iterator entitiesIterEnd();

    bool isFaceVisible(Vec3 pos, side_t side);
    bool isValidPos(Vec3 pos);
private:
    void generateChunk(Chunk& c);
    std::unordered_map<ChunkPos, std::unique_ptr<Chunk>> chunks;
    std::vector<Entity*> entities;
    int total_blocks;
    int getIndex(Vec3 pos);
};

#endif
