#include "render.h"
#include "world.h"
#include "blocks.h"
#include <stdlib.h>
#include <string.h>

static Texture2D atlasTexture = { 0 };
static int textureLoaded = 0;

static void loadAtlasTexture(void) {
    if (textureLoaded) return;
    atlasTexture = LoadTexture("assets/textures/atlas.png");
    textureLoaded = 1;
}

static void addFace(Mesh *mesh, int *vCount,
                    Vector3 v1, Vector3 v2, Vector3 v3, Vector3 v4,
                    float u1, float v1_uv, float u2, float v2_uv) {
    mesh->vertices[(*vCount) * 3 + 0] = v1.x;
    mesh->vertices[(*vCount) * 3 + 1] = v1.y;
    mesh->vertices[(*vCount) * 3 + 2] = v1.z;
    mesh->texcoords[(*vCount) * 2 + 0] = u1;
    mesh->texcoords[(*vCount) * 2 + 1] = v1_uv;
    (*vCount)++;

    mesh->vertices[(*vCount) * 3 + 0] = v2.x;
    mesh->vertices[(*vCount) * 3 + 1] = v2.y;
    mesh->vertices[(*vCount) * 3 + 2] = v2.z;
    mesh->texcoords[(*vCount) * 2 + 0] = u2;
    mesh->texcoords[(*vCount) * 2 + 1] = v1_uv;
    (*vCount)++;

    mesh->vertices[(*vCount) * 3 + 0] = v3.x;
    mesh->vertices[(*vCount) * 3 + 1] = v3.y;
    mesh->vertices[(*vCount) * 3 + 2] = v3.z;
    mesh->texcoords[(*vCount) * 2 + 0] = u2;
    mesh->texcoords[(*vCount) * 2 + 1] = v2_uv;
    (*vCount)++;

    mesh->vertices[(*vCount) * 3 + 0] = v1.x;
    mesh->vertices[(*vCount) * 3 + 1] = v1.y;
    mesh->vertices[(*vCount) * 3 + 2] = v1.z;
    mesh->texcoords[(*vCount) * 2 + 0] = u1;
    mesh->texcoords[(*vCount) * 2 + 1] = v1_uv;
    (*vCount)++;

    mesh->vertices[(*vCount) * 3 + 0] = v3.x;
    mesh->vertices[(*vCount) * 3 + 1] = v3.y;
    mesh->vertices[(*vCount) * 3 + 2] = v3.z;
    mesh->texcoords[(*vCount) * 2 + 0] = u2;
    mesh->texcoords[(*vCount) * 2 + 1] = v2_uv;
    (*vCount)++;

    mesh->vertices[(*vCount) * 3 + 0] = v4.x;
    mesh->vertices[(*vCount) * 3 + 1] = v4.y;
    mesh->vertices[(*vCount) * 3 + 2] = v4.z;
    mesh->texcoords[(*vCount) * 2 + 0] = u1;
    mesh->texcoords[(*vCount) * 2 + 1] = v2_uv;
    (*vCount)++;
}

Model buildWorldMesh(World *world) {
    loadAtlasTexture();

    const int maxBlocks = world->WORLD_SIZE_X * world->WORLD_SIZE_Y * world->WORLD_SIZE_Z;
    const int maxFaces = maxBlocks * 6;

    Mesh mesh = { 0 };
    mesh.triangleCount = maxFaces * 2;
    mesh.vertexCount = mesh.triangleCount * 3;

    mesh.vertices = (float *)malloc(mesh.vertexCount * 3 * sizeof(float));
    mesh.texcoords = (float *)malloc(mesh.vertexCount * 2 * sizeof(float));

    int vCount = 0;

    for (int x = -world->HALF_X; x < world->HALF_X; x++) {
        for (int y = 0; y < world->WORLD_SIZE_Y; y++) {
            for (int z = -world->HALF_Z; z < world->HALF_Z; z++) {
                Vec3 pos = { x, y, z };
                Block block = world->getBlock(pos);

                BlockDef def = getBlockDef(block.type);
                if (def.is_air) continue;

                float fx = (float)x, fy = (float)y, fz = (float)z;

                if (world->isFaceVisible(pos, UP)) {
                    Vector3 v1 = { fx, fy + 1.0f, fz + 1.0f };
                    Vector3 v2 = { fx + 1.0f, fy + 1.0f, fz + 1.0f };
                    Vector3 v3 = { fx + 1.0f, fy + 1.0f, fz };
                    Vector3 v4 = { fx, fy + 1.0f, fz };
                    addFace(&mesh, &vCount, v1, v2, v3, v4, TEX_UV(def.texture_top));
                }

                if (world->isFaceVisible(pos, DOWN)) {
                    Vector3 v1 = { fx, fy, fz };
                    Vector3 v2 = { fx + 1.0f, fy, fz };
                    Vector3 v3 = { fx + 1.0f, fy, fz + 1.0f };
                    Vector3 v4 = { fx, fy, fz + 1.0f };
                    addFace(&mesh, &vCount, v1, v2, v3, v4, TEX_UV(def.texture_bottom));
                }

                if (world->isFaceVisible(pos, LEFT)) {
                    Vector3 v1 = { fx, fy, fz + 1.0f };
                    Vector3 v2 = { fx, fy + 1.0f, fz + 1.0f };
                    Vector3 v3 = { fx, fy + 1.0f, fz };
                    Vector3 v4 = { fx, fy, fz };
                    addFace(&mesh, &vCount, v1, v2, v3, v4, TEX_UV(def.texture_left));
                }

                if (world->isFaceVisible(pos, RIGHT)) {
                    Vector3 v1 = { fx + 1.0f, fy, fz };
                    Vector3 v2 = { fx + 1.0f, fy + 1.0f, fz };
                    Vector3 v3 = { fx + 1.0f, fy + 1.0f, fz + 1.0f };
                    Vector3 v4 = { fx + 1.0f, fy, fz + 1.0f };
                    addFace(&mesh, &vCount, v1, v2, v3, v4, TEX_UV(def.texture_right));
                }

                if (world->isFaceVisible(pos, BACK)) {
                    Vector3 v1 = { fx + 1.0f, fy, fz };
                    Vector3 v2 = { fx + 1.0f, fy + 1.0f, fz };
                    Vector3 v3 = { fx, fy + 1.0f, fz };
                    Vector3 v4 = { fx, fy, fz };
                    addFace(&mesh, &vCount, v1, v2, v3, v4, TEX_UV(def.texture_back));
                }

                if (world->isFaceVisible(pos, FRONT)) {
                    Vector3 v1 = { fx, fy, fz + 1.0f };
                    Vector3 v2 = { fx + 1.0f, fy, fz + 1.0f };
                    Vector3 v3 = { fx + 1.0f, fy + 1.0f, fz + 1.0f };
                    Vector3 v4 = { fx, fy + 1.0f, fz + 1.0f };
                    addFace(&mesh, &vCount, v1, v2, v3, v4, TEX_UV(def.texture_front));
                }
            }
        }
    }

    mesh.vertexCount = vCount;
    mesh.triangleCount = vCount / 3;

    UploadMesh(&mesh, 0);
    Model model = LoadModelFromMesh(mesh);

    if (atlasTexture.id != 0) {
        model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = atlasTexture;
    }

    return model;
}

void updateWorldMesh(Model *model, World *world) {
    //veri smart
    UnloadModel(*model);
    *model = buildWorldMesh(world);
}

void destroyWorldModel(Model *model) {
    UnloadModel(*model);
}
