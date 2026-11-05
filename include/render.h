#ifndef RENDER_H
#define RENDER_H

#include "raylib.h"
#include "types.h"
#include "world.h"

Model buildWorldMesh(World *world);
void updateWorldMesh(Model *model, World *world);
void destroyWorldModel(Model *model);
void renderEntityModel(Vec3d offset, const EntityModel* model);

#endif
