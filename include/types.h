#ifndef TYPES_H
#define TYPES_H

#include <stdint.h>

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

#endif
