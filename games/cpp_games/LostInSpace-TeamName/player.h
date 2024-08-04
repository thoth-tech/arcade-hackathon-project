#ifndef LOST_IN_SPACE_PLAYER
#define LOST_IN_SPACE_PLAYER //<------ Player for player.h
// LOST_IN_SPACE_PLANETS <---- for planet.h
// LOST_IN_SPACE_DATA <---- for lost_in_space.h

#include "splashkit.h"
#include "planet.h"

#include <vector>

using namespace std;

#define PLAYER_SPEED 1.5
#define PLAYER_ROTATE_SPEED 3
#define SCREEN_BORDER 100

/**
 * Different options for the kind of ship.
 * Adjusts the image used.
 */
enum ship_kind
{
    AQUARII,
    GLIESE,
    PEGASI
};

/**
 * The player data keeps track of all of the information related to the player.

 * @field   player_sprite   The player's sprite - used to track position and movement
 * @field   score           The current score for the player
 * @field   kind            Current kind of player ship
 */
typedef struct player_struct
{
    sprite player_sprite;
    int score;
    ship_kind kind;
} player_data;

/**
 * Creates a new player in the centre of the screen with the default ship.
 *
 * @returns     The new player data
 */
player_data new_player();

/**
 * Draws the player to the screen.
 *
 * @param player_to_draw    The player to draw to the screen
 */
void draw_player(const player_data &player_to_draw);

/**
 * Actions a step update of the player - moving them and adjusting the camera.
 *
 * @param player_to_update      The player being updated
 */
void update_player(player_data &player_to_update);

/**
 * Read user input and update the player based on this interaction.
 *
 * @param player    The player to update
 */
void handle_input(player_data &player);

/**
 * Loads the players score, position and closest planet
 *
 * @param player    The player to update
 * @param planet    The planet data needed to check distance
 *                  and if or not it has been discovered
 */
void draw_hud(const player_data &player, const planet_data &planet, double time_percent);

/**
 * Reads player and planet loccation and indicates distance
 *
 * @param player    The player loccation vs planet
 * @param planet    The planet loccation
 */
double distance_to_planet(const player_data &player, const planet_data &planet);

/**
 * Reads distance to planet and checks for the closest planet
 *
 * @param player    The player in loccation
 * @param planet    The closest planet to the player
 */
int closest_planet_index(const player_data &player, const vector<planet_data> &planets);

#endif