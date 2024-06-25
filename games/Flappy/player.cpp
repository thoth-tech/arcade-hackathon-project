#include "splashkit.h"
#include "game.h"
#include "player.h"
#include "pipes.h"
#include "score.h"

/**
 * The ship bitmap function converts a ship kind into a 
 * bitmap that can be used.
 * 
 * Not exposed by the header.
 * 
 * @param kind  The kind of ship you want the bitmap of
 * @return      The bitmap matching this ship kind
 */
bitmap bird_bitmap(bird_type type)
{
    switch(type)
    {
    case UP_FLAP:
        return bitmap_named("bird-upflap");
    case DOWN_FLAP:
        return bitmap_named("bird-downflap");
    case MID_FLAP:
        return bitmap_named("bird-midflap");
    default:
        return bitmap_named("bird-midflap");
    }
}

player_data new_player()
{
    player_data result;

    bitmap player_bitmap = bird_bitmap(MID_FLAP);

    result.player_sprite = create_sprite(player_bitmap);

    sprite_add_layer(result.player_sprite, bird_bitmap(DOWN_FLAP), "DOWN_FLAP");
    sprite_add_layer(result.player_sprite, bird_bitmap(UP_FLAP), "UP_FLAP");

    sprite_hide_layer(result.player_sprite, DOWN_FLAP);
    sprite_hide_layer(result.player_sprite, UP_FLAP);

    sprite_set_x(result.player_sprite, (screen_width() / 2) - 19);
    sprite_set_y(result.player_sprite, (screen_height() / 2) - 37);

    return result;
}

void switch_bird_sprites(player_data &player, bird_type target, bird_type hide1, bird_type hide2)
{
    if (player.kind != target)
    {
        sprite_show_layer(player.player_sprite, static_cast<int>(target));
        sprite_hide_layer(player.player_sprite, static_cast<int>(hide1));
        sprite_hide_layer(player.player_sprite, static_cast<int>(hide2));
    }
}

void handle_input(player_data &player)
{
    if(player.dead == false)
    {
        if (key_typed(SPACE_KEY) or mouse_clicked(LEFT_BUTTON))
        {
            sprite_set_dy(player.player_sprite, -35);
            play_sound_effect("wing");
            switch_bird_sprites(player, UP_FLAP, DOWN_FLAP, MID_FLAP);
        }
    }
}

void draw_player(const player_data &player_to_draw)
{
    draw_sprite(player_to_draw.player_sprite);
}

void update_player(player_data &player_to_update)
{
    update_sprite(player_to_update.player_sprite);

    switch_bird_sprites(player_to_update, MID_FLAP, DOWN_FLAP, UP_FLAP);
}