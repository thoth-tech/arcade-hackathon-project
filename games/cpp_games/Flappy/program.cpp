#include "splashkit.h"
#include "player.h"
#include "game.h"
#include "pipes.h"

using namespace std;

/**
 * Entry point.
 * 
 * Manages the initialisation of data, the event loop, and quitting.
 */

int main()
{
    open_window("Flappy Bird", 288, 512);
    load_resources();

    game_data game = new_game();

    while( not quit_requested() )
    {
        process_events();

        handle_input(game.player);

        draw_game(game.player, game);

        update_game(game);
    }
    return 0;
}