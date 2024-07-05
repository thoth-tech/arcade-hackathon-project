#include "splashkit.h"
#include "player.h"
#include "game.h"
#include "pipes.h"
#include "score.h"


using namespace std;

/**
 * Load the game images, sounds, etc.
 */
void load_resources()
{
    load_resource_bundle("game_bundle", "flappy_bird_resources.txt");
}

game_data new_game()
{
    game_data game;

    game.player = new_player();

    bitmap welcome_screen_sprite = bitmap_named("message");

    game.welcome_screen_sprite = create_sprite(welcome_screen_sprite);

    bitmap game_over_sprite = bitmap_named("gameover");

    game.game_over_sprite = create_sprite(game_over_sprite);

    game.game_started = false;

    game.player.dead = false;

    return game;
}

double pipe_increment(game_data &game)
{
    double increment = sprite_x(game.player.player_sprite) + 150; // Gives room at the start of the game for the player to react 

    for(int i = 0; i < game.pipe_data.size(); i++)
    {
        increment += 180;
    }
    return increment;
}

void freeze_gravity(player_data &player, bool gravity_exists)
{
    if (gravity_exists == false)
    {
        sprite_set_dy(player.player_sprite, 1.5);
    }
    else
    {
        sprite_set_dy(player.player_sprite, 0);
    }
}

void display_home_screen(game_data &game)
{   
    if (game.game_started == false)
    {
        draw_sprite(game.welcome_screen_sprite, 50, 50);
        freeze_gravity(game.player, true);
        sprite_set_y(game.player.player_sprite, (screen_height() / 2) - 37);
        sprite_set_x(game.player.player_sprite, (screen_width() / 2) - 19);
    }
    else
    {
        sprite_hide_layer(game.welcome_screen_sprite, 0);
        sprite_hide_layer(game.game_over_sprite, 0);
    }
}

double calculate_first_digit(game_data &game, int number) // Break the players score into the first digit only
{
    int n;

    n = number % 10;
    number /= 10;

    return n;
}

double calculate_second_digit(game_data &game, int number) // Break the players score into the second digit only
{

    int n = number / 10 % 10;

    return n;
}


void display_score(game_data &game) // Takes the two digit calculation functions and converts the enum to the bitmap name to display
{
    if((game.game_started == true and game.player.dead == false) or (game.game_started == false and game.player.dead == true))
    {
        int digit1, digit2;
        digit1 = calculate_first_digit(game, game.player.score);
        digit2 = calculate_second_digit(game, game.player.score);

        draw_bitmap(to_string(digit1), screen_width() / 2, 10, option_to_screen());
        draw_bitmap(to_string(digit2), (screen_width() / 2) - 25, 10, option_to_screen());
    }
}

void move_pipes(game_data &game, int x)
{
    for(int i = 0; i < game.pipe_data.size(); i++)
    {
        sprite_set_x(game.pipe_data[i].bottom_pipe, sprite_x(game.pipe_data[i].bottom_pipe) - x);
        sprite_set_x(game.pipe_data[i].top_pipe, sprite_x(game.pipe_data[i].top_pipe) - x);
    }
}

void draw_game_over_screen(game_data &game)
{
    if(game.game_started == false)
    {
        draw_sprite(game.game_over_sprite, 50, 150);
    }
}

void remove_pipes(game_data &game, int index)
{
    if(index >= 0 and index < game.pipe_data.size())
    {
        int last_index = game.pipe_data.size() - 1;
        game.pipe_data[index] = game.pipe_data[last_index];
        game.pipe_data.pop_back();
    }
}


void add_pipe_pair(game_data &game)
{   
    pipe_data pipes;

    pipes = new_pipe_set(rnd(MAX_PIPE, MIN_PIPE), pipe_increment(game));
    game.pipe_data.push_back(pipes);

}

void game_over(game_data &game)
{
    freeze_gravity(game.player, true);
    
    move_pipes(game, 0);

    play_sound_effect("die"); 

    game.player.dead = true;
    game.game_started = false;
}

void game_over_next(game_data &game)
{
    game.pipe_data.clear();

    move_pipes(game, 1);
    freeze_gravity(game.player, false);
    game.player.score = 0;
    game.player.dead = false;
    game.game_started = false;
}

void draw_background(game_data &game)
{
    draw_bitmap("background", 0, 0, option_to_screen());

    for (int i = 0; i < game.pipe_data.size(); i++)
    {
        draw_pipe_set(game.pipe_data[i]);
    }

    display_score(game);

    draw_bitmap("base", 0, 400, option_to_screen());

    if(game.player.dead == true and game.game_started == false)
    {
        draw_game_over_screen(game);
    }
    
    if(game.game_started == false and game.player.dead == false)
    {
        display_home_screen(game);
    }
}

void score_count(game_data &game)
{
    for(int i = 0; i < game.pipe_data.size(); i++)
    {
        if(sprite_x(game.player.player_sprite) == ((sprite_x(game.pipe_data[i].bottom_pipe)) + (sprite_width(game.pipe_data[i].bottom_pipe) / 2)))
        {
            game.player.score += 1;
            play_sound_effect("point");
        }
    }
}

void check_collisions(game_data &game)
{
    if(game.pipe_data.size() > 0)
    {
        for(int i = 0; i < game.pipe_data.size() - 1; i++)
        {
            if (sprite_collision(game.player.player_sprite, game.pipe_data[i].bottom_pipe) or sprite_collision(game.player.player_sprite, game.pipe_data[i].top_pipe) or sprite_y(game.player.player_sprite) > 399)
            {
                if(game.player.dead == false)
                {
                    game_over(game);
                }
            }
        }
    }
}

void update_game(game_data &game)
{
    update_player(game.player);

    if(game.pipe_data.size() < 10) // just keep replacing the last pipe, the player will only see roughly 3 at a time - Just to save memory
    {
        add_pipe_pair(game);
    }

    score_count(game);
        
    for (int i = 0; i < game.pipe_data.size(); i++)
    {
        update_pipe_set(game.pipe_data[i]);

        if(sprite_x(game.pipe_data[i].bottom_pipe) < - sprite_width(game.pipe_data[i].bottom_pipe))
        {
            remove_pipes(game, i);
        }
    }

    if (key_typed(SPACE_KEY) or mouse_clicked(LEFT_BUTTON))
    {
        if(game.player.dead == false and game.game_started == false)
        {
            game.game_started = true;
        }
    }

    if (game.game_started == true)
    {
        move_pipes(game, 1);
        freeze_gravity(game.player, false);
    }

    if(game.player.dead == true and game.game_started == false)
    {
        while(key_typed(SPACE_KEY) or mouse_clicked(LEFT_BUTTON))
        {
            process_events();
            game_over_next(game);
        }
    }
    
    check_collisions(game);
}

void draw_game(const player_data &player_to_draw, game_data &game)
{
    clear_screen(COLOR_BLACK);

    draw_background(game);

    draw_player(player_to_draw);

    refresh_screen(60);
}