#ifndef LOST_IN_SPACE_DATA
#define LOST_IN_SPACE_DATA

#include "splashkit.h"
#include "player.h"

#include <vector>
#define LEVEL_TIME 12000.0

extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;

enum game_state
{
    MENU,
    PLAY,
    END
};

/**
 * The game data keeps track of all of the information related to the game.
 *
 * @field   planet_sprite   The player's sprite - used to track position and movement
 * @field   visited         The current status of the planet: visted = true or false.
 */
struct game_data
{
    player_data player;
    vector<planet_data> planets;
    timer game_timer;
    bool game_over;
    game_state state;
    bool wormhole_active;
};

/**
 * Creates a specified area of Space, and specified number of planets.
 */
void setup_level(game_data &game, int width, int height, int num_planets);

/**
 * Creates a new game.
 *
 * @returns     The new game data
 */
game_data new_game();

/**
 * Switch the state from in the menu to playing the game
 *
 * @param game The game data
 */
void start_game(game_data &game);

/**
 * Draws the planets and player to the screen.
 *
 * @param planets   The planets to draw to the screen
 * @param player    The player to draw to the screen
 */
void draw_game(game_data &game);

/**
 * update of the planet - moving them and giving them speed and rotation.
 *
 * @param game      The player and planets being updated
 */
void update_game(game_data &game);

// /**
//  * Returns the number of planets that the player has yet to visit.
//  *
//  * @param game
//  * @return the number of planets remaining
//  */
// int planets_left(vector<planet_data> planets);

// /**
//  * Returns all unvisited planets
//  *
//  * @param planets An array of all of the planets
//  * @returns An array of the unvisited planets
//  */
// vector<planet_data> return_unvisited_planets(vector<planet_data> planets);

#endif