#include "raylib.h"
#include "raymath.h"
#include "rlgl.h"
#include "types.h"
#include "world.h"
#include "render.h"
#include "player.h"
#include "blocks.h"
#include "core/Registries.h"
#include <stdio.h>
#include <cmath>

using namespace std;

const char *SUPERCRAFT_VER = "0.2.0-PreAlpha";

int main() {
    Registries::init();

    const int screenWidth = 1280;
    const int screenHeight = 720;
    SetTraceLogLevel(LOG_NONE);
    InitWindow(screenWidth, screenHeight, "Supercraft");

    rlDisableBackfaceCulling();

    World world;

    std::unordered_map<ChunkPos, ChunkRenderData> chunkModels;

    Player player;
    initPlayer(&player);

    Camera3D camera = { 0 };
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    camera.fovy = 70.0f;
    camera.projection = CAMERA_PERSPECTIVE;
    updateCamera(&camera, &player);

    SetTargetFPS(60);
    DisableCursor();

    std::string ent_type="test";
    world.addEntity({0,10,0},ent_type);

    while (!WindowShouldClose()) {
        float deltaTime = GetFrameTime();

        updatePlayer(&player, deltaTime);
        updateCamera(&camera, &player);
        world.updateLoadedChunks(player.x, player.z, 4);
        for (auto it = chunkModels.begin(); it != chunkModels.end(); ) {
            if (world.getChunk(it->first) == nullptr) {
                if (it->second.hasModel) UnloadModel(it->second.model);
                    it = chunkModels.erase(it);
            } else {
                it++;
            }
        }

        for (auto it=world.entitiesIter();it!=world.entitiesIterEnd();++it) {
            Entity *ent=*it;
            ent->update(deltaTime);
        }

        Vector3 camPos = { player.x, player.y + player.height, player.z };
        Vector3 camTarget = {
            camPos.x + sinf(player.angleX) * cosf(player.angleY),
            camPos.y + sinf(player.angleY),
            camPos.z + cosf(player.angleX) * cosf(player.angleY)
        };
        Vector3 dir = Vector3Normalize(Vector3Subtract(camTarget, camPos));

        Vec3 hitBlock;
        side_t hitSide;

        int hit = raycast(&world, camPos, dir, 8, &hitBlock, &hitSide);

        BeginDrawing();
        ClearBackground((Color){ 135, 206, 235, 255 });

        BeginMode3D(camera);

        updateDirtyChunks(chunkModels, &world);

        for (auto& [pos, rd] : chunkModels) {
            if (rd.hasModel) DrawModel(rd.model, (Vector3){0, 0, 0}, 1.0f, WHITE);
        }

        for (auto it=world.entitiesIter();it!=world.entitiesIterEnd();++it) {
            Entity *ent=*it;
            renderEntity(ent);
        }

        if (hit) {
            Vector3 cubePos = { hitBlock.x + 0.5f, hitBlock.y + 0.5f, hitBlock.z + 0.5f };
            DrawCubeWires(cubePos, 1.00f, 1.00f, 1.00f, BLACK);
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                Block empty_block = {0, 0};
                world.setBlock(hitBlock, empty_block);
            }

            if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
                Vec3 placePos = hitBlock;
                switch (hitSide) {
                    case UP:    placePos.y += 1; break;
                    case DOWN:  placePos.y -= 1; break;
                    case LEFT:  placePos.x -= 1; break;
                    case RIGHT: placePos.x += 1; break;
                    case FRONT: placePos.z += 1; break;
                    case BACK:  placePos.z -= 1; break;
                }
                if (world.isAir(placePos)) {
                    Block newBlock = {player.block, 0};
                    world.setBlock(placePos, newBlock);
                }
            }
            if (IsMouseButtonPressed(MOUSE_BUTTON_MIDDLE)) {
                Vec3 pos = hitBlock;
                if (!world.isAir(pos)) {
                    player.block=world.getBlock(pos).type;
                }
            }
        }

        EndMode3D();

        char info[100];
        sprintf(info, "Supercraft v%s", SUPERCRAFT_VER);
        DrawText(info, 10, 40, 20, BLACK);
        sprintf(info, "Selected block: %s", Registries::BLOCKS.getObject(player.block).id.c_str());
        DrawText(info, 10, 60, 20, BLACK);
        DrawFPS(10, 10);

        int cx = screenWidth / 2;
        int cy = screenHeight / 2;
        int size = 10;
        int gap = 3;
        int thickness = 2;

        DrawRectangle(cx - size, cy - thickness / 2, size - gap, thickness, BLACK);
        DrawRectangle(cx + gap, cy - thickness / 2, size - gap, thickness, BLACK);

        DrawRectangle(cx - thickness / 2, cy - size, thickness, size - gap, BLACK);
        DrawRectangle(cx - thickness / 2, cy + gap, thickness, size - gap, BLACK);
        EndDrawing();
    }

    destroyAllChunkModels(chunkModels);
    CloseWindow();

    return 0;
}
