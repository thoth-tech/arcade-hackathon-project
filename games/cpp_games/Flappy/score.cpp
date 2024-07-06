#include "splashkit.h"
#include "score.h"
#include "game.h"
#include "player.h"

using namespace std;

/**
 * The bitmap score function will return a
 * bitmap based on the num
 * 
 * Exposed by the header.
 * 
 * @param num   The kind of number you want
 * @return      The bitmap matching to the score
 */
bitmap score_bitmap(score_sprite num)
{
    switch (num)
    {
    case ZERO:
        return bitmap_named("0");
    case ONE:
        return bitmap_named("1");
    case TWO:
        return bitmap_named("2");
    case THREE:
        return bitmap_named("3");
    case FOUR:
        return bitmap_named("4");
    case FIVE:
        return bitmap_named("5");
    case SIX:
        return bitmap_named("6");
    case SEVEN:
        return bitmap_named("7");
    case EIGHT:
        return bitmap_named("8");
    case NINE:
        return bitmap_named("9");
    }
    return 0;
}
