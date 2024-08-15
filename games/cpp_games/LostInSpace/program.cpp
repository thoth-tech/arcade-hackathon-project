#include "splashkit.h"
#include "player.h"
#include "planet.h"
#include "screen.h"
#include "lost_in_space.h"
#include <memory>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 700

/**
 * Load the game images, sounds, etc.
 */
void load_resources()
{
    load_resource_bundle("game_bundle", "lost_in_space.txt");
    load_resource_bundle("menu_bundle", "menu_bundle.txt");
}

/**
 * Entry point.
 *
 * Manages the initialisation of data, the event loop, and quitting.
 */

int main()
{
    load_resources();
    open_window("Lost In Space", SCREEN_WIDTH, SCREEN_HEIGHT);

    create_timer("ScreenTimer");

    game_data game = new_game();

    // holds the screen data
    shared_ptr<Screen> screen; // new "smart" c++ pointer that removes itself when no longer in use - avoid memory leaks

    shared_ptr<Screen> normal_screen(new Screen(new CompanyIntroScreen));
    screen = normal_screen;

    // added ability to quit with escape, was just nice to have to correspond to the other game
    while (!quit_requested() && !key_typed(ESCAPE_KEY))
    {
        // Handle input to adjust player movement
        process_events();

        // if the game is in a menu state, we continue in the menu state until the menu 
        // tells us that it no longer needs to exist
        // we can remove references to it and the smart pointer will clear it up
        if (game.state == MENU)
        {
            screen->update(); // this starts the screen

            if (screen->menu_completed == true)
            {
                start_game(game);
                screen = nullptr; // should trigger the destructor (smart ptr), which will trigger the state destructor
            }
        }
        else if (game.state == PLAY) // if play, work as normal
        {
            handle_input(game.player);

            // Perform movement and update the camera
            update_game(game);

            // draw everything
            draw_game(game);
        }

        refresh_screen(60);
    }

    free_resource_bundle("game_bundle");
    // free_resource_bundle("menu_bundle");
    // do not uncomment the above line, everything will immediately go to hell
    free_all_timers();

    return 0;
}