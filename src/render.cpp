#include "render.h"
#include "world.h"
#include "blocks.h"
#include "core/Registries.h"
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

static void turnTexture(Vector3 *v1, Vector3 *v2, Vector3 *v3, Vector3 *v4, int turn_type) {
    Vector3 tmp;
    Vector3 tmp2;
    switch (turn_type) {
        case 0: break;
        case 1:
            tmp=*v1;
            *v1=*v2;
            *v2=*v3;
            *v3=*v4;
            *v4=tmp;
            break;
        case 2:
            tmp=*v1;
            tmp2=*v2;
            *v1=*v3;
            *v2=*v4;
            *v3=tmp;
            *v4=tmp2;
            break;
        case 3:
            tmp=*v4;
            *v4=*v3;
            *v3=*v2;
            *v2=*v1;
            *v1=tmp;
            break;
    }
}

static inline uint32_t mix32(uint32_t x) {
    x^=x>>16;
    x*=0x85ebca6b;
    x^=x>>13;
    x*=0xc2b2ae35;
    x^=x>>16;
    return x;
}

static inline int getTurnType(Vec3 pos) {
    return ((mix32(pos.x)^mix32(pos.z))+pos.y)&0b11;
}

void rebuildChunkMesh(ChunkRenderData& out, World* world, ChunkPos cp) {
    loadAtlasTexture();
    if (out.hasModel) {
        UnloadModel(out.model);
        out.hasModel = false;
    }

    Chunk *chunk = world->getChunk(cp);
    if (!chunk) return;

    const int maxBlocks =16 * world->WORLD_SIZE_Y * 16;
    const int maxFaces = maxBlocks * 6;

    Mesh mesh = { 0 };
    mesh.triangleCount = maxFaces * 2;
    mesh.vertexCount = mesh.triangleCount * 3;

    mesh.vertices = (float *)malloc(mesh.vertexCount * 3 * sizeof(float));
    mesh.texcoords = (float *)malloc(mesh.vertexCount * 2 * sizeof(float));

    int vCount = 0;

    for (int lx = 0; lx < 16; lx++) {
        for (int ly = 0; ly < world->WORLD_SIZE_Y; ly++) {
            for (int lz = 0; lz < 16; lz++) {
                Block block = chunk->getBlock({lx, ly, lz});
                BlockDef def = Registries::BLOCKS.getObject(block.type);
                if (def.isAir) continue;

                int wx = cp.x * 16 + lx;
                int wz = cp.z * 16 + lz;
                Vec3 pos = { wx, ly, wz };

                float fx = (float)wx, fy = (float)ly, fz = (float)wz;

                if (world->isFaceVisible(pos, UP)) {
                    Vector3 v1 = { fx, fy + 1.0f, fz + 1.0f };
                    Vector3 v2 = { fx + 1.0f, fy + 1.0f, fz + 1.0f };
                    Vector3 v3 = { fx + 1.0f, fy + 1.0f, fz };
                    Vector3 v4 = { fx, fy + 1.0f, fz };
                    if (def.textures.randturn&0b100000)
                        turnTexture(&v1, &v2, &v3, &v4, getTurnType(pos));
                    addFace(&mesh, &vCount, v1, v2, v3, v4, TEX_UV(def.textures.tex_top));
                }

                if (world->isFaceVisible(pos, DOWN)) {
                    Vector3 v1 = { fx, fy, fz };
                    Vector3 v2 = { fx + 1.0f, fy, fz };
                    Vector3 v3 = { fx + 1.0f, fy, fz + 1.0f };
                    Vector3 v4 = { fx, fy, fz + 1.0f };
                    if (def.textures.randturn&0b010000)
                        turnTexture(&v1, &v2, &v3, &v4, getTurnType(pos));
                    addFace(&mesh, &vCount, v1, v2, v3, v4, TEX_UV(def.textures.tex_bottom));
                }

                if (world->isFaceVisible(pos, LEFT)) {
                    Vector3 v1 = { fx, fy, fz + 1.0f };
                    Vector3 v2 = { fx, fy + 1.0f, fz + 1.0f };
                    Vector3 v3 = { fx, fy + 1.0f, fz };
                    Vector3 v4 = { fx, fy, fz };
                    if (def.textures.randturn&0b001000)
                        turnTexture(&v1, &v2, &v3, &v4, getTurnType(pos));
                    addFace(&mesh, &vCount, v3, v2, v1, v4, TEX_UV(def.textures.tex_left));
                }

                if (world->isFaceVisible(pos, RIGHT)) {
                    Vector3 v1 = { fx + 1.0f, fy, fz };
                    Vector3 v2 = { fx + 1.0f, fy + 1.0f, fz };
                    Vector3 v3 = { fx + 1.0f, fy + 1.0f, fz + 1.0f };
                    Vector3 v4 = { fx + 1.0f, fy, fz + 1.0f };
                    if (def.textures.randturn&0b000100)
                        turnTexture(&v1, &v2, &v3, &v4, getTurnType(pos));
                    addFace(&mesh, &vCount, v3, v2, v1, v4, TEX_UV(def.textures.tex_right));
                }

                if (world->isFaceVisible(pos, BACK)) {
                    Vector3 v1 = { fx + 1.0f, fy, fz };
                    Vector3 v2 = { fx + 1.0f, fy + 1.0f, fz };
                    Vector3 v3 = { fx, fy + 1.0f, fz };
                    Vector3 v4 = { fx, fy, fz };
                    if (def.textures.randturn&0b000010)
                        turnTexture(&v1, &v2, &v3, &v4, getTurnType(pos));
                    addFace(&mesh, &vCount, v3, v2, v1, v4, TEX_UV(def.textures.tex_back));
                }

                if (world->isFaceVisible(pos, FRONT)) {
                    Vector3 v1 = { fx, fy, fz + 1.0f };
                    Vector3 v2 = { fx + 1.0f, fy, fz + 1.0f };
                    Vector3 v3 = { fx + 1.0f, fy + 1.0f, fz + 1.0f };
                    Vector3 v4 = { fx, fy + 1.0f, fz + 1.0f };
                    if (def.textures.randturn&0b000001)
                        turnTexture(&v1, &v2, &v3, &v4, getTurnType(pos));
                    addFace(&mesh, &vCount, v3, v4, v1, v2, TEX_UV(def.textures.tex_front));
                }
            }
        }
    }

    mesh.vertexCount = vCount;
    mesh.triangleCount = vCount / 3;

    UploadMesh(&mesh, 0);
    out.model = LoadModelFromMesh(mesh);

    if (atlasTexture.id != 0) {
        out.model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = atlasTexture;
    }

    out.hasModel = true;
}

void updateDirtyChunks(std::unordered_map<ChunkPos, ChunkRenderData>& models, World* world) {
    for (auto& [pos, chunkPtr] : world->getChunks()) {
        if (!chunkPtr->dirty) continue;
        rebuildChunkMesh(models[pos], world, pos);
        chunkPtr->dirty = false;
    }
}

void destroyAllChunkModels(std::unordered_map<ChunkPos, ChunkRenderData>& models) {
    for (auto& [pos, rd] : models)
        if (rd.hasModel) UnloadModel(rd.model);
    models.clear();
}

static void fillVertsFromModelRect(const ModelRect* rect, Vector3 verts[6][4]) {
    double x0 = rect->pos.x;
    double y0 = rect->pos.y;
    double z0 = rect->pos.z;
    double x1 = rect->pos.x + rect->size.x;
    double y1 = rect->pos.y + rect->size.y;
    double z1 = rect->pos.z + rect->size.z;

    #define SET_VERT(face, idx, vx, vy, vz) \
        do { \
            verts[face][idx].x = (vx); \
            verts[face][idx].y = (vy); \
            verts[face][idx].z = (vz); \
        } while (0)

    SET_VERT(0, 0, x0, y0, z0);
    SET_VERT(0, 1, x1, y0, z0);
    SET_VERT(0, 2, x1, y1, z0);
    SET_VERT(0, 3, x0, y1, z0);

    SET_VERT(1, 0, x1, y0, z1);
    SET_VERT(1, 1, x0, y0, z1);
    SET_VERT(1, 2, x0, y1, z1);
    SET_VERT(1, 3, x1, y1, z1);

    SET_VERT(2, 0, x0, y0, z1);
    SET_VERT(2, 1, x0, y0, z0);
    SET_VERT(2, 2, x0, y1, z0);
    SET_VERT(2, 3, x0, y1, z1);

    SET_VERT(3, 0, x1, y0, z0);
    SET_VERT(3, 1, x1, y0, z1);
    SET_VERT(3, 2, x1, y1, z1);
    SET_VERT(3, 3, x1, y1, z0);

    SET_VERT(4, 0, x0, y0, z1);
    SET_VERT(4, 1, x1, y0, z1);
    SET_VERT(4, 2, x1, y0, z0);
    SET_VERT(4, 3, x0, y0, z0);

    SET_VERT(5, 0, x0, y1, z0);
    SET_VERT(5, 1, x1, y1, z0);
    SET_VERT(5, 2, x1, y1, z1);
    SET_VERT(5, 3, x0, y1, z1);

    #undef SET_VERT
}

void renderEntity(const Entity *entity) {
    EntityModel model=Registries::MODELS.getObject(entity->edef.model);
    Mesh mesh={0};
    Model draw_model;
    const int rect_amount=model.rects.size();
    const int max_faces=rect_amount*6;
    mesh.triangleCount=max_faces*2;
    mesh.vertexCount=max_faces*6;

    mesh.vertices = (float *)malloc(mesh.vertexCount * 3 * sizeof(float));
    mesh.texcoords = (float *)malloc(mesh.vertexCount * 2 * sizeof(float));

    int vert_count=0;
    for (int i=0;i<rect_amount;i++) {
        ModelRect cur_rect=model.rects[i];
        Vector3 verts[6][4];
        fillVertsFromModelRect(&cur_rect, verts);

        addFace(&mesh, &vert_count, verts[0][0], verts[0][1], verts[0][2], verts[0][3], TEX_UV(0));
        addFace(&mesh, &vert_count, verts[1][0], verts[1][1], verts[1][2], verts[1][3], TEX_UV(1));
        addFace(&mesh, &vert_count, verts[2][0], verts[2][1], verts[2][2], verts[2][3], TEX_UV(2));
        addFace(&mesh, &vert_count, verts[3][0], verts[3][1], verts[3][2], verts[3][3], TEX_UV(3));
        addFace(&mesh, &vert_count, verts[4][0], verts[4][1], verts[4][2], verts[4][3], TEX_UV(4));
        addFace(&mesh, &vert_count, verts[5][0], verts[5][1], verts[5][2], verts[5][3], TEX_UV(5));
    }

    mesh.vertexCount = vert_count;
    mesh.triangleCount = mesh.vertexCount / 3;

    UploadMesh(&mesh, false);
    draw_model=LoadModelFromMesh(mesh);
    if (atlasTexture.id != 0) {
        draw_model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = atlasTexture;
    }

    DrawModelEx(draw_model, {(float)entity->pos.x,(float)entity->pos.y,(float)entity->pos.z}, {0, 1, 0}, (float)entity->yaw, {1, 1, 1}, WHITE);
    UnloadModel(draw_model);
}
