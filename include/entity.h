#ifndef ENTITY_H
#define ENTITY_H

#include "raylib.h"
#include "types.h"

class World;

class Entity {
public:
    Vec3d pos;
    Vec3d vel;
    EntityDef edef;
    double width, height, depth;
    double pitch, yaw;
    double health;
    bool alive;

    Entity(std::string& id);
    virtual ~Entity() = default;

    virtual void update(double dt);

    bool checkBlockCollision(World *world);
    bool checkBlockCollision(World *world, Vec3d pos);
    bool checkEntityCollision(const Entity *other);
};

#endif
