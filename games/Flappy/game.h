#ifndef FLAPPY_BIRD_GAME
#define FLAPPY_BIRD_GAME

#include "splashkit.h"
#include "pipes.h"
#include "player.h"
#include "score.h"

/**
 * The game data keeps track of all of the information related to the game.
 * 
 * @field   welcome_screen_sprite   The welcome screen sprite
 * @field   game_over_sprite        The game over screen sprite
 * @field   player                  The player data struct
 * @field   pipe_data               The pipe data vector.
 * @field   kind                    Current pipe_type kind
 * @field   score_sprite            Current score sprite kind
 * @field   game_started            Bool. Keeps track if the game has started or not.
 */
struct game_data
{
    sprite            welcome_screen_sprite;
    sprite            game_over_sprite;
    player_data       player;
    vector<pipe_data> pipe_data;
    pipe_type         kind;
    score_sprite      num;
    bool              game_started;
};

/**
 * Creates a new game in the sets up the welcome and game over screen sprites.
 * 
 * @returns     The new game data
 */
game_data new_game();

/**
 * Loads resource bundle
 */
void load_resources();

/**
 * Adds a new pipe pair to the game
 * 
 * @param Game    The game to add the pipe pair too
 */
void add_pipe_pair(game_data &game);

/**
 * Removes a pipe pair from the game
 * 
 * @param Game    The game to remove the pipe pair from
 */
void remove_pipes(game_data &game, int index);

/**
 * Calculates a random increment for the pipes to be generated with
 * 
 * @returns  A random increment
 */
double pipe_increment(game_data &game);

/**
 * Draws the game to the screen. 
 * 
 * @param player_to_draw   The player to draw to the screen
 * @param game             The game to draw to the screen
 */
void draw_game(const player_data &player_to_draw, game_data &game);

/**
 * Freezes the games gravity
 * 
 * @param Game              The game to freeze the gravity of
 * @param gravity_exists    1 or 0. Gravity exists or does not.
 */
void freeze_gravity(player_data &player, bool gravity_exists);

/**
 * Updates the game 
 * 
 * @param Game    The game to update
 */
void update_game(game_data &game);

#endif