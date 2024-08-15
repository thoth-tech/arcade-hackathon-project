#include "splashkit.h"
#include "lost_in_space.h"

#define STARTING_PLANETS 10
#define WORMHOLE_MAX_SIZE 1.3
#define DEBUG 0 // 1 for yes, prints num of planets, 0 for no, does not.

void setup_level(game_data &game, int width, int height, int num_planets)
{
    if (DEBUG == 1)
        write_line(num_planets);

    if (!game.game_over)
    {
        // Delete old planets (otherwise discovered planets from last level would remain)
        for (int i = 0; i < game.planets.size(); i++)
        {
            free_sprite(game.planets[i].planet_sprite);
        }
        game.planets.clear();
        game.wormhole_active = false;

        planet_data planet;

        if (num_planets <= 0)
        {
            num_planets = 1;
        }

        // Position player in the centre of the initial screen
        sprite_set_x(game.player.player_sprite, width / 2 - sprite_width(game.player.player_sprite) / 2);   // Subtracts half the player's width to x, centring
        sprite_set_y(game.player.player_sprite, height / 2 - sprite_height(game.player.player_sprite) / 2); // Subtracts half the player's height to y, centring

        // Add planets
        for (int i = 0; i < num_planets; i++)
        {
            planet = new_planet(rnd(width), rnd(height));
            game.planets.push_back(planet);
        }
    }
}

game_data new_game()
{
    // variable to return
    game_data game;

    // Define player
    game.player = new_player();

    // Create level and planets and center player
    setup_level(game, 3000, 3000, STARTING_PLANETS); // added a variable for debugging purposes

    // Set up timer
    game.game_timer = create_timer("game_timer");

    game.state = MENU;
    game.wormhole_active = false;

    reset_timer(game.game_timer); // Ensures timer isn't using old data

    return game;
}

// start the game after the menu has been handled
void start_game(game_data &game)
{
    start_timer(game.game_timer);

    game.state = PLAY;

    game.player.score = 0;
    game.game_over = false;
}

int planets_left(vector<planet_data> planets)
{
    int sum = 0;
    for (int i = 0; i < planets.size(); i++)
    {
        if (!planets[i].visited)
        {
            sum++;
        }
    }

    return sum;
}

void draw_game(game_data &game)
{
    // Redraw everything
    clear_screen(COLOR_BLACK);

    for (int i = 0; i < game.planets.size(); i++)
    {
        if (game.planets[i].visited == false && planets_left(game.planets) == 1)
        {
            if (!game.wormhole_active)
            {
                point_2d position = sprite_position(game.planets[i].planet_sprite);

                game.planets[i].planet_sprite = create_sprite(bitmap_named("wormhole")); // change sprite to wormhole

                sprite_set_position(game.planets[i].planet_sprite, position); // make sure new sprite spawns same position
                sprite_set_scale(game.planets[i].planet_sprite, 0.3); // start it off small and slowly increase
                
                game.wormhole_active = true;
            }
            else
            {
                if (sprite_scale(game.planets[i].planet_sprite) < WORMHOLE_MAX_SIZE)
                {
                    sprite_set_scale(game.planets[i].planet_sprite, sprite_scale(game.planets[i].planet_sprite) + 0.0016667);
                    // 60 frames a second, going up 0.1 frames per second = 0.1 / 60 = 0.0016667
                }
            }
        }

        draw_planet(game.planets[i]);
    }

    // Draw player
    draw_player(game.player);

    int index = closest_planet_index(game.player, game.planets);

    double time_percent = (LEVEL_TIME - timer_ticks(game.game_timer)) / LEVEL_TIME;
    if (time_percent < 0)
    {
        time_percent = 0; // Ensures doesn't go into the negatives
    }

    // Are there any planets left?
    if (index != -1)
    {
        // Draw hud
        draw_hud(game.player, game.planets[index], time_percent);
    }
    else
    {
        draw_hud(game.player, game.planets[0], time_percent);
    }

    if (game.game_over)
    {
        draw_text("GAME OVER!", COLOR_WHITE, 370, 350, option_to_screen());
    }
}

void check_collisions(game_data &game) // this also updates the score when a planet is
{
    if (!game.game_over)
    {
        for (int i = 0; i < game.planets.size(); i++)
        {
            if (!game.planets[i].visited and sprite_collision(game.player.player_sprite, game.planets[i].planet_sprite))
            {
                game.planets[i].visited = true;
                game.player.score = game.player.score + 1000;
                reset_timer(game.game_timer);
            }
        }
    }
}

void update_game(game_data &game)
{
    if (game.state != PLAY)
        return; // game needs to have been started to be able to be updated

    double elapsed_time = timer_ticks(game.game_timer) / 1000;
    update_player(game.player, elapsed_time);

    // Perform movement and update the camera
    update_player(game.player, elapsed_time);
    for (int i = 0; i < game.planets.size(); i++)
    {
        update_planet(game.planets[i]);
    }

    check_collisions(game);

    if (timer_ticks(game.game_timer) > LEVEL_TIME)
    {
        game.game_over = true;
        stop_timer(game.game_timer);
    }

    // New level upon no more planets
    if (closest_planet_index(game.player, game.planets) == -1)
    {
        setup_level(game, 3000, 3000, game.planets.size() - 1);
    }
}