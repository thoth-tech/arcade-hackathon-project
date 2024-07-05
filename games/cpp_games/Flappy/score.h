#ifndef FLAPPY_BIRD_SCORE
#define FLAPPY_BIRD_SCORE

#include "splashkit.h"

/**
 * Different options for the kind of score value.
 * ZERO - NINE
 */
enum score_sprite
{
    ZERO,
    ONE,
    TWO,
    THREE,
    FOUR,
    FIVE,
    SIX,
    SEVEN,
    EIGHT,
    NINE
};

/**
 * The exposed score bitmap function
 */
bitmap score_bitmap(score_sprite num);

#endif