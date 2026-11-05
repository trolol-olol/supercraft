#include <math.h>
#include "entity.h"
#include "world.h"
#include "blocks.h"
#include "core/Registries.h"

using namespace std;

Entity::Entity() {
    pos = {0.0, 0.0, 0.0};
    vel = {0.0, 0.0, 0.0};
    width = 0.6;
    height = 1.8;
    depth = 0.6;
    pitch = 0.0;
    yaw = 0.0;
    health = 20.0;
    alive = true;
}

bool Entity::checkBlockCollision(World *world, Vec3d pos) {
    double halfW = width / 2.0;
    double halfH = height / 2.0;
    double halfD = depth / 2.0;

    int minX = (int)floor(pos.x - halfW);
    int maxX = (int)ceil(pos.x + halfW);
    int minY = (int)floor(pos.y - halfH);
    int maxY = (int)ceil(pos.y + halfH);
    int minZ = (int)floor(pos.z - halfD);
    int maxZ = (int)ceil(pos.z + halfD);

    for (int bx = minX; bx <= maxX; bx++) {
        for (int by = minY; by <= maxY; by++) {
            for (int bz = minZ; bz <= maxZ; bz++) {
                Vec3 blockPos = { bx, by, bz };
                Block block = world->getBlock(blockPos);
                BlockDef def = Registries::BLOCKS.getObject(block.type);
                if (!def.isAir) {
                    return true;
                }
            }
        }
    }
    return false;
}

bool Entity::checkBlockCollision(World *world) {
    return checkBlockCollision(world, pos);
}

bool Entity::checkEntityCollision(const Entity *other) {
    double dx = pos.x - other->pos.x;
    double dy = pos.y - other->pos.y;
    double dz = pos.z - other->pos.z;

    double thresX = (width + other->width) / 2.0;
    double thresY = (height + other->height) / 2.0;
    double thresZ = (depth + other->depth) / 2.0;

    return (fabs(dx) < thresX) &&
           (fabs(dy) < thresY) &&
           (fabs(dz) < thresZ);
}
