#ifndef CHUNKLOADER_H
#define CHUNKLOADER_H

#include <queue>
#include <stop_token>
#include <unordered_set>
#include <condition_variable>
#include <mutex>
#include <thread>
#include "types.h"
#include "core/chunk.h"

typedef class World World;

class ChunkLoader {
public:
    ChunkLoader(World *world);
    ~ChunkLoader();

    void requestLoad(ChunkPos pos);
private:
    void run(std::stop_token tok);
    std::jthread worker;
    std::mutex mtx;
    std::condition_variable_any cv;
    std::unordered_set<ChunkPos> requested_set;
    std::queue<ChunkPos> requested;
    World *world;
};

#endif
