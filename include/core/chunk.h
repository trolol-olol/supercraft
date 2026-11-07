#ifndef CHUNK_H
#define CHUNK_H

#include "types.h"
#include <memory>
#include <functional>

struct ChunkPos {
    int32_t x, z;
    bool operator==(const ChunkPos& o) const { return x == o.x && z == o.z; }
};

inline int32_t floorDiv(int32_t a, int b) {
    int32_t q = a/b, r = a%b;
    if (r!=0 && ((r < 0) != (b < 0))) --q;
    return q;
}

inline int32_t floorMod(int32_t a, int b) {
    int32_t r = a%b;
    if (r!=0 && ((r < 0) != (b < 0))) r += b;
    return r;
}

inline ChunkPos worldToChunk(Vec3 pos) { return { floorDiv(pos.x, 16), floorDiv(pos.z, 16)}; }

namespace std {
    template<> struct hash<ChunkPos> {
        size_t operator()(const ChunkPos& p) const noexcept {
            uint64_t h = (uint64_t)p.z << 32 | (uint32_t)p.x;
            h = h * 0x9E3779B97F4A7C15ull;
            return (size_t)h;
        }
    };
}

class Chunk {
public:
    ChunkPos pos;
    bool dirty = true;

    Chunk(ChunkPos pos);
    ~Chunk();

    Block getBlock(Vec3 pos);
    void setBlock(Vec3 pos, Block block);
    bool isAir(Vec3 pos);

    bool isValidPos(Vec3 pos);
private:
    Block *blocks;
    int getIndex(Vec3 pos);
};

#endif
