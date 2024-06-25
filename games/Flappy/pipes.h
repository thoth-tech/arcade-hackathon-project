#ifndef FLAPPY_BIRD_PIPES
#define FLAPPY_BIRD_PIPES

#define MAX_PIPE 100 // The heighest the top pipe can go
#define MIN_PIPE 370 // The lowest the bottom pipe can go
#define PIPE_GAP 100 // The gap inbetween

/**
 * The player data keeps track of all of the information related to the player.
 * 
 * @field   bottom_pipe     The bottom pipe's sprite
 * @field   top_pipe        The top pipe's sprite
 */
struct pipe_data
{
    sprite       bottom_pipe;
    sprite       top_pipe;
};

/**
 * Different options for the kind of pipe.
 * Either a bottom pipe or a top pipe.
 */
enum pipe_type
{
    BOTTOM,
    TOP
};

pipe_data new_pipe_set(double random, double increment);

/**
 * Draws the pipe set to the screen
 * 
 * @param pipe_set     The pipe pair
 */
void draw_pipe_set(const pipe_data &pipe_pair);

/**
 * Updates the pipe pair in the game
 * 
 * @param   pipe_pair The pipe pair
 */
void update_pipe_set(pipe_data &pipe_pair);


#endif