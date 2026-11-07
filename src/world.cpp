#include "world.h"
#include "core/chunk.h"
#include "core/Registries.h"
#include <cmath>
#include <unordered_set>

using namespace std;

World::World() {
    updateLoadedChunks(0, 0, 4);
}

World::~World() {
    for (Entity *e : entities) delete e;
    entities.clear();
}

Block World::getBlock(Vec3 pos) {
    if (pos.y < 0 || pos.y >= WORLD_SIZE_Y) return {0, 0};

    ChunkPos cp = worldToChunk(pos);
    auto it = chunks.find(cp);
    if (it == chunks.end()) return {0, 0};

    return it->second->getBlock({floorMod(pos.x, 16), pos.y, floorMod(pos.z, 16)});
}

void World::setBlock(Vec3 pos, Block block) {
    if (pos.y < 0 || pos.y > WORLD_SIZE_Y) return;

    ChunkPos cp = worldToChunk(pos);
    Chunk& c = getOrLoadChunk(cp);
    c.setBlock({floorMod(pos.x, 16), pos.y, floorMod(pos.z, 16)}, block);
    c.dirty = true;

    int lx = floorMod(pos.x, 16), lz = floorMod(pos.z, 16);
    auto mark = [&](ChunkPos p) {
        auto it = chunks.find(p);
        if (it != chunks.end()) it->second->dirty = true;
    };
    if (lx == 0) mark({cp.x - 1, cp.z});
    if (lx == 15) mark({cp.x + 1, cp.z});
    if (lz == 0) mark({cp.x, cp.z - 1});
    if (lz == 15) mark({cp.x, cp.z + 1});
}

bool World::isAir(Vec3 pos){
    Block block = getBlock(pos);
    BlockDef def = Registries::BLOCKS.getObject(block.type);
    return def.isAir;
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

Chunk* World::getChunk(ChunkPos pos) {
    auto it = chunks.find(pos);
    return it == chunks.end() ? nullptr : it->second.get();
}

Chunk& World::getOrLoadChunk(ChunkPos pos) {
    auto it = chunks.find(pos);
    if (it != chunks.end()) return *it->second;

    auto chunk = std::make_unique<Chunk>(pos);
    generateChunk(*chunk);
    Chunk& ref = *chunk;
    chunks.emplace(pos, std::move(chunk));
    return ref;
}

void World::updateLoadedChunks(double px, double pz, int dist) {
    int pcx = floorDiv(std::floor(px), 16);
    int pcz = floorDiv(std::floor(pz), 16);

    std::unordered_set<ChunkPos> needed;
    for (int dx = -dist; dx <= dist; ++dx) {
        for (int dz = -dist; dz <= dist; ++dz) {
            if (dx * dx + dz * dz <= dist * dist) {
                needed.insert({pcx + dx, pcz + dz});
            }
        }

    }

    for (auto it = chunks.begin(); it != chunks.end(); ) {
        if (!needed.count(it->first)) it = chunks.erase(it);
        else ++it;
    }
    for (auto& p : needed) getOrLoadChunk(p);
}

void World::generateChunk(Chunk& c) {
    for (int lx = 0; lx < 16; lx++) {
        for (int lz = 0; lz < 16; lz++) {
            int h = 3;

            for (int y = 0; y <= h; y++) {
                Block b;
                if (y == h) b.type = 1;
                else if (y > h - 2) b.type = 2;
                else b.type = 3;
                c.setBlock({lx, y, lz}, b);
            }
        }
    }
}

Entity* World::addEntity(Vec3d pos, std::string& id) {
    Entity *ent=new Entity(id);
    ent->pos=pos;
    entities.push_back(ent);
    return ent;
}

Entity* World::getEntityByIndex(size_t index) {
    return entities.at(index);
}

std::vector<Entity*>::iterator World::entitiesIter() {
    return entities.begin();
}

std::vector<Entity*>::iterator World::entitiesIterEnd() {
    return entities.end();
}
