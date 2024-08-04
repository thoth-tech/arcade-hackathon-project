//Ensures the #include's don't copy and paste the same line twice. E.g. doesn't define <string> twice.
#ifndef LOST_IN_SPACE_PLANET
#define LOST_IN_SPACE_PLANET

#include "splashkit.h"
#include <vector>

using namespace std;

//gives the planets names a value to be used in a switch case of bitmaps
enum planet_kind
{
    MERCURY,
    VENUS,
    EARTH,
    MARS,
    JUPITER,
    SATURN,
    URANUS,
    NEPTUNE,
    PLUTO
};

/**
 * The planet data keeps track of all of the information related to the planet.
 *
 * @field   planet_sprite   The player's sprite - used to track position and movement
 * @field   visited         The current status of the planet: visted = true or false.
 */
struct planet_data
{
    sprite  planet_sprite;
    bool    visited;
};

/**
 * Creates a new planet somewhere within the setup level range (3000 in this case) in the window.
 *
 * @returns     The new planet data
 */
planet_data new_planet(double x, double y);

/**
 * Draws the planet to the screen.
 *
 * @param planet   The planet to draw to the screen
 */
void draw_planet(const planet_data &planet_to_draw);

/**
 * update of the planet - moving them and giving them speed and rotation.
 *
 * @param planet_to_update    The planet being updated
 */
void update_planet(planet_data &planet_to_update);

#endif