#ifndef __PROJECT_H
#define __PROJECT_H

#include <stdint.h>

typedef enum {
    DOT,
    LINE,
    SQUARE,
    LAST_SHAPE = LINE
} shape_t;

void project(shape_t shape,int depth,int length,int speed,int t);

#endif