#include "player.h"
#include "blocks.h"
#include "world.h"
#include "core/Registries.h"
#include <cmath>
#include <cstdio>

using namespace std;

void initPlayer(Player *player) {
    player->x = 0.0;
    player->y = 12.0;
    player->z = 0.0;
    player->angleX = 0.0;
    player->angleY = -0.3;
    player->speed = 8.0;
    player->height = 1.5;
    player->block = 2;
}

void updatePlayerInput(Player *player, double deltaTime) {
    Vector2 mouseDelta = GetMouseDelta();
    player->angleX -= mouseDelta.x * 0.002;
    player->angleY -= mouseDelta.y * 0.002;

    if (player->angleY > 1.5) player->angleY = 1.5;
    if (player->angleY < -1.5) player->angleY = -1.5;

    Vector3 forward = { (float)sin(player->angleX), 0, (float)cos(player->angleX) };
    Vector3 right = { -forward.z, 0, forward.x };

    double speed = player->speed * deltaTime;

    if (IsKeyDown(KEY_W)) {
        player->x += forward.x * speed;
        player->z += forward.z * speed;
    }
    if (IsKeyDown(KEY_S)) {
        player->x -= forward.x * speed;
        player->z -= forward.z * speed;
    }
    if (IsKeyDown(KEY_A)) {
        player->x -= right.x * speed;
        player->z -= right.z * speed;
    }
    if (IsKeyDown(KEY_D)) {
        player->x += right.x * speed;
        player->z += right.z * speed;
    }

    if (IsKeyDown(KEY_SPACE)) player->y += 5.0 * deltaTime;
    if (IsKeyDown(KEY_LEFT_SHIFT)) player->y -= 5.0 * deltaTime;

    if (IsKeyPressed(KEY_ONE)) player->block = 2;
    if (IsKeyPressed(KEY_TWO)) player->block = 3;
    if (IsKeyPressed(KEY_THREE)) player->block = 4;
}

void updatePlayer(Player *player, float deltaTime) {
    updatePlayerInput(player, deltaTime);
}

void updateCamera(Camera3D *camera, Player *player) {
    Vector3 camPos = { (float)player->x, (float)player->y + (float)player->height, (float)player->z };
    Vector3 camTarget = {
        camPos.x + (float)sin(player->angleX) * (float)cos(player->angleY),
        camPos.y + (float)sin(player->angleY),
        camPos.z + (float)cos(player->angleX) * (float)cos(player->angleY)
    };
    camera->position = camPos;
    camera->target = camTarget;
}

int raycast(World *world, Vector3 origin, Vector3 direction, int maxDist,
            Vec3 *hit, side_t *hitSide) {
    float x = origin.x;
    float y = origin.y;
    float z = origin.z;

    int blockX = (int)floorf(x);
    int blockY = (int)floorf(y);
    int blockZ = (int)floorf(z);

    int stepX = (direction.x > 0) ? 1 : -1;
    int stepY = (direction.y > 0) ? 1 : -1;
    int stepZ = (direction.z > 0) ? 1 : -1;

    float tMaxX = (direction.x != 0) ? ((stepX > 0 ? (blockX + 1) : blockX) - x) / direction.x : INFINITY;
    float tMaxY = (direction.y != 0) ? ((stepY > 0 ? (blockY + 1) : blockY) - y) / direction.y : INFINITY;
    float tMaxZ = (direction.z != 0) ? ((stepZ > 0 ? (blockZ + 1) : blockZ) - z) / direction.z : INFINITY;

    float tDeltaX = (direction.x != 0) ? 1.0f / fabsf(direction.x) : INFINITY;
    float tDeltaY = (direction.y != 0) ? 1.0f / fabsf(direction.y) : INFINITY;
    float tDeltaZ = (direction.z != 0) ? 1.0f / fabsf(direction.z) : INFINITY;

    int side = -1;

    for (int i = 0; i < maxDist; i++) {
        Block block = world->getBlock((Vec3){blockX, blockY, blockZ});
        BlockDef def = Registries::BLOCKS.getObject(block.type);

        if (def.id != "unknown" && !def.isAir) {
            *hit = (Vec3){blockX, blockY, blockZ};
            switch (side) {
                case 0: *hitSide = (stepX < 0) ? RIGHT : LEFT; break;
                case 1: *hitSide = (stepY < 0) ? UP : DOWN; break;
                case 2: *hitSide = (stepZ < 0) ? FRONT : BACK; break;
            }
            return 1;
        }

        if (tMaxX < tMaxY && tMaxX < tMaxZ) {
            blockX += stepX;
            tMaxX += tDeltaX;
            side = 0;
        } else if (tMaxY < tMaxZ) {
            blockY += stepY;
            tMaxY += tDeltaY;
            side = 1;
        } else {
            blockZ += stepZ;
            tMaxZ += tDeltaZ;
            side = 2;
        }
    }

    return 0;
}
