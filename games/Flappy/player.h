#ifndef FLAPPY_BIRD_PLAYER
#define FLAPPY_BIRD_PLAYER

#include "splashkit.h"

/**
 * Different options for the kind of ship.
 * Adjusts the image used.
 */
enum bird_type
{
    UP_FLAP,
    MID_FLAP,
    DOWN_FLAP
};

/**
 * The player data keeps track of all of the information related to the player.
 * 
 * @field   player_sprite   The players sprite
 * @field   dead            If the player is dead or not.
 * @field   score           The players score
 * @field   kind            The type of the birds wings
 * 
 **/
struct player_data
{
    sprite              player_sprite;
    bool                dead;
    int                 score;
    bird_type           kind;
};

player_data new_player();

/**
 * Handles the game control input
 * 
 * @param player    The player to act on the input given
 **/
void handle_input(player_data &player);

/**
 * Switches the bird type to the target and hides the others
 * 
 * @param player    The player that is changing the types
 * @param target    The bird type you want to change too
 * @param hide1     The other bird_type
 * @param hide2     The other bird_type
 **/
void switch_bird_sprites(player_data &player, bird_type target, bird_type hide1, bird_type hide2);

/**
 * Draws the player in the game
 * 
 * @param player_to_draw    Draws the player to screen
 * 
 **/
void draw_player(const player_data &player_to_draw);

/**
 * Updates the player in the game
 * 
 * @param player_to_update   The player to update
 * 
 **/
void update_player(player_data &player_to_update);

#endif