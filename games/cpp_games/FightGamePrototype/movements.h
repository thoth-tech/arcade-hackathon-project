#include "splashkit.h"

//Animation
void idle(sprite sprite)
{
    string direction = "";
    if(sprite_value(sprite, "facing_left") == 0)
            direction = "IdleLeft";
    else
            direction = "IdleRight";
    sprite_start_animation(sprite, direction);
}
//Animation
void animation_routine(sprite sprite, string animation_type)
{
    if(sprite_value(sprite, "keypress") != 1)
            sprite_start_animation(sprite, animation_type);
    sprite_set_value(sprite, "keypress", 1);
}

//Animation
void walk_routine(sprite sprite, float value)
{
    sprite_set_dx(sprite, value);
    if(sprite_value(sprite, "edge") != 0)
        sprite_set_dx(sprite, 0);
    sprite_set_value(sprite, "edge", 0);
}

//Input
void walk(bool left, sprite sprite)
{
    string direction = "";
    if(sprite_value(sprite, "facing_left") == 0)
        direction = "WalkLeft";
    else
        direction = "WalkRight";

    sprite_set_value(sprite, "walk", 1);
    animation_routine(sprite, direction);
    
    if(left)
        walk_routine(sprite, -4);
    else
        walk_routine(sprite, 4);
}

//Input
void crouch(sprite sprite)
{
    string direction = "";
    sprite_set_value(sprite, "crouch", 1);
    if(sprite_value(sprite, "facing_left") == 0)
        direction = "CrouchLeft";
    else
        direction = "CrouchRight";
    animation_routine(sprite, direction);
}

//Input
void jump(sprite sprite)
{
    string direction = "";
    sprite_set_value(sprite, "jumping", 1);
    if(sprite_value(sprite, "facing_left") == 0)
        direction = "JumpLeft";
    else
        direction = "JumpRight";
    sprite_start_animation(sprite, direction);
}

//Input
void punch(sprite sprite)
{
    string direction = "";
    sprite_set_value(sprite, "attacking", 1);
    if(sprite_value(sprite, "facing_left") == 0)
        direction = "PunchLeft";
    else
        direction = "PunchRight";
    sprite_start_animation(sprite, direction);
}

//Input
void kick(sprite sprite)
{
    string direction = "";
    sprite_set_value(sprite, "attacking", 1);
    if(sprite_value(sprite, "facing_left") == 0)
        direction = "KickLeft";
    else
        direction = "KickRight";
    sprite_start_animation(sprite, direction);
}

//Input
void block(sprite sprite)
{
    string direction = "";
    if(sprite_value(sprite, "facing_left") == 0)
        direction = "BlockLeft";
    else
        direction = "BlockRight";

    sprite_set_value(sprite, "block", 1);
    animation_routine(sprite, direction);
}

//Input
void press_release_routine(sprite sprite, string string)
{
    sprite_set_value(sprite, string, 0);
    sprite_set_value(sprite, "keypress", 0);
    sprite_set_dx(sprite, 0);
    if(sprite_value(sprite, "walk") != 1 && sprite_value(sprite, "crouch") != 1 && sprite_value(sprite, "block") != 1)
    {
        if(sprite_value(sprite, "facing_left") == 0)
            sprite_start_animation(sprite, "IdleLeft");
        else
            sprite_start_animation(sprite, "IdleRight");
    }
}

void reset_animation(sprite sprite, key_code right_key, key_code left_key,
    key_code jump_key, key_code crouch_key, key_code punch_key, key_code kick_key, key_code block_key)
{
    if(key_down(left_key))
        walk(true, sprite);
    else if(key_down(right_key))
        walk(false, sprite);
    else if(key_typed(jump_key))
        jump(sprite);
    else if(key_down(crouch_key))
        crouch(sprite);
    else if(key_typed(punch_key))
        punch(sprite);
    else if(key_typed(kick_key))
        kick(sprite);
    else if(key_down(block_key))
        block(sprite);
    else
        idle(sprite);
}

//Input
void end_jump(sprite sprite, key_code right_key, key_code left_key,
    key_code jump_key, key_code crouch_key, key_code punch_key, key_code kick_key, key_code block_key)
{
    sprite_set_value(sprite, "jumping", 0);
    sprite_set_dx(sprite, 0);
    sprite_set_dy(sprite, 0);
    press_release_routine(sprite, "walk");

    reset_animation(sprite, right_key, left_key, jump_key, crouch_key, punch_key, kick_key, block_key);
}



//Input
void end_hit(sprite sprite, key_code right_key, key_code left_key,
    key_code jump_key, key_code crouch_key, key_code punch_key, key_code kick_key, key_code block_key)
{
    sprite_set_value(sprite, "hit", 0);
    press_release_routine(sprite, "walk");

    reset_animation(sprite, right_key, left_key, jump_key, crouch_key, punch_key, kick_key, block_key);
}