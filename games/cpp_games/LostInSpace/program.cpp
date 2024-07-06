#include "splashkit.h"
#include "player.h"
#include "planet.h"
#include "lost_in_space.h"

/**
 * Load the game images, sounds, etc.
 */
void load_resources()
{
    load_resource_bundle("game_bundle", "lost_in_space.txt");
}

/**
 * Entry point.
 *
 * Manages the initialisation of data, the event loop, and quitting.
 */

int main()
{
    load_resources();
    open_window("Lost In Space", 800, 700);

    game_data game = new_game();

    while (!quit_requested())
    {
        // Handle input to adjust player movement
        process_events();
        handle_input(game.player);

        // Perform movement and update the camera
        update_game(game);

        // Draw everything
        draw_game(game);
    }

    return 0;
}