#include "splashkit.h"
#include "pipes.h"
#include "game.h"

using namespace std;

/**
 * The pipe bitmap function converts a pipe_type kind into a 
 * bitmap that can be used.
 * 
 * Not exposed by the header.
 * 
 * @param kind  The kind of pipe you want the bitmap of
 * @return      The bitmap matching this pipe kind
 */
bitmap pipe_bitmap(pipe_type kind)
{
    switch(kind)
    {
        case TOP:
        {
            return bitmap_named("pipe-green-top");
            break;
        }
        case BOTTOM:
        {
            return bitmap_named("pipe-green");
            break;
        }
    };
return 0;
}

pipe_data new_pipe_set(double random, double increment)
{
    pipe_data result;

    bitmap bottom_pipe_bitmap = pipe_bitmap(BOTTOM);

    result.bottom_pipe = create_sprite(bottom_pipe_bitmap);

    bitmap top_pipe_bitmap = pipe_bitmap(TOP);

    result.top_pipe = create_sprite(top_pipe_bitmap);

    // Bottom Pipe
    sprite_set_x(result.bottom_pipe, increment);
    sprite_set_y(result.bottom_pipe, random);
    // Top Pipe
    double top_pipe_distance = (random + PIPE_GAP) + (- sprite_height(result.top_pipe) - 2 * PIPE_GAP);
    sprite_set_x(result.top_pipe, increment);
    sprite_set_y(result.top_pipe, top_pipe_distance);

    return result;
}

void draw_pipe_set(const pipe_data &pipe_pair)
{
    draw_sprite(pipe_pair.bottom_pipe);
    draw_sprite(pipe_pair.top_pipe);
}

void update_pipe_set(pipe_data &pipe_pair)
{
    update_sprite(pipe_pair.bottom_pipe);
    update_sprite(pipe_pair.top_pipe);
}