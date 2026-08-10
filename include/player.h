#ifndef PLAYER_H
#define PLAYER_H

#include "raylib.h"
#include "types.h"

class World;

typedef struct {
    float x, y, z;
    float angleX;
    float angleY;
    float speed;
    float height;
    uint16_t block;
} Player;

void initPlayer(Player *player);
void updatePlayer(Player *player, float deltaTime);
void updateCamera(Camera3D *camera, Player *player);
int raycast(World *world, Vector3 origin, Vector3 direction, int maxDist,
            Vec3 *hit, side_t *hitSide);

#endif
