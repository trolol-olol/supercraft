#ifndef ENTITY_H
#define ENTITY_H

#include "raylib.h"
#include "types.h"

class World;

class Entity {
public:
    Vec3d pos;
    Vec3d vel;
    double width, height, depth;
    double pitch, yaw;
    double health;
    bool alive;

    Entity();
    virtual ~Entity() = default;

    virtual void update(double deltaTime) = 0;
    virtual void render() = 0;

    bool checkBlockCollision(World *world);
    bool checkBlockCollision(World *world, Vec3d pos);
    bool checkEntityCollision(const Entity *other);
};

#endif
