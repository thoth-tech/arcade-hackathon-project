#include "splashkit.h"
#include "lost_in_space.h"

void setup_level(game_data &game, int width, int height, int num_planets)
{
    if (!game.game_over)
    {
        // Delete old planets (otherwise discovered planets from last level would remain)
        for (int i = 0; i < game.planets.size(); i++)
        {
            free_sprite(game.planets[i].planet_sprite);
        }
        game.planets.clear();

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
    setup_level(game, 3000, 3000, 10);

    // Set up timer
    game.game_timer = create_timer("game_timer");

    reset_timer(game.game_timer); // Ensures timer isn't using old data
    start_timer(game.game_timer);

    game.game_over = false;
    game.player.score = 0;

    return game;
}

void draw_game(game_data &game)
{
    // Redraw everything
    clear_screen(COLOR_BLACK);

    // Draw planets first, so player is in front
    for (int i = 0; i < game.planets.size(); i++)
    {
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

    // Refresh screen
    refresh_screen(60);
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
    update_player(game.player);

    // Perform movement and update the camera
    update_player(game.player);
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