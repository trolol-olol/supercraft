#include "core/chunkloader.h"
#include "world.h"

ChunkLoader::ChunkLoader(World* world) : worker([this](std::stop_token tok) { run(tok); }) {
    this->world = world;
}

ChunkLoader::~ChunkLoader() {
    worker.request_stop();
}

void ChunkLoader::requestLoad(ChunkPos pos) {
    mtx.lock();
    if (!requested_set.contains(pos)) {
        requested.push(pos);
        requested_set.insert(pos);
        mtx.unlock();
        cv.notify_one();
        return;
    }
    mtx.unlock();
}

void ChunkLoader::run(std::stop_token tok) {
    while (!tok.stop_requested()) {
        ChunkPos to_load;
        {
            std::unique_lock<std::mutex> lock(mtx);
            cv.wait(lock, tok, [&] {
                return !requested.empty();
            });
            if (requested.empty()) break;
            to_load = requested.front();
            requested.pop();
            requested_set.erase(to_load);
        }
        world->getOrLoadChunk(to_load);
    }
}
