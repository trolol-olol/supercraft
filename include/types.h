#ifndef TYPES_H
#define TYPES_H

#include <cstdint>
#include <string>
#include <vector>

typedef struct {
    int32_t x, y, z;
} Vec3;

typedef struct {
    double x, y, z;
} Vec3d;

typedef struct {
    uint16_t type;
    uint8_t meta;
} Block;

typedef enum {
    UP,
    DOWN,
    LEFT,
    RIGHT,
    FRONT,
    BACK
} side_t;

typedef struct {
    uint8_t randturn;
    uint8_t tex_top;
    uint8_t tex_bottom;
    uint8_t tex_left;
    uint8_t tex_right;
    uint8_t tex_back;
    uint8_t tex_front;
} Textures;

typedef struct {
    std::string id;
    uint16_t numId;
    Textures textures;
    bool isAir;
    bool isSolid;
    bool isTransparent;
    bool isLiquid;
    float hardness;
} BlockDef;

typedef struct {
    Vec3d pos;
    Vec3d size;
} ModelRect;

typedef struct {
    std::vector<ModelRect> rects;
} EntityModel;

typedef struct {
    std::string id;
    uint16_t numId;
    float width, height, depth;
    double maxHealth;
    float speed;
    double damage;
    bool isHostile;
    bool isFlying;
    uint16_t model;
} EntityDef;

#endif
