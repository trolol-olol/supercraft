#ifndef RENDER_H
#define RENDER_H

#include "raylib.h"
#include "types.h"
#include "world.h"
#include <unordered_map>

struct ChunkRenderData {
    Model model{};
    bool hasModel = false;
};

void rebuildChunkMesh(ChunkRenderData& out, World* world, ChunkPos pos);
void updateDirtyChunks(std::unordered_map<ChunkPos, ChunkRenderData>& models, World* world);
void destroyAllChunkModels(std::unordered_map<ChunkPos, ChunkRenderData>& models);
void renderEntity(const Entity* entity);

#endif
