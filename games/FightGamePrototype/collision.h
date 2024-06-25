#include "splashkit.h"

#define SCREEN_BORDER 30

//Collision
rectangle make_hitbox(sprite sprite, float x, float y, float height, float width)
{
    rectangle rect;
    rect.x = sprite_x(sprite) + x;
    rect.y = sprite_y(sprite) + y;
    rect.height = sprite_height(sprite) + height;
    rect.width = sprite_width(sprite) + width;

    return rect;
}

//Collision
void standing_collision_routine(sprite sprite1, sprite sprite2)
{
    double left_edge = camera_x() + SCREEN_BORDER;
    double right_edge = left_edge + screen_width() - 2 * SCREEN_BORDER - 100;
    if(sprite_value(sprite1, "facing_left") == 0)
    {
        if(sprite_x(sprite2) < right_edge)
        {
            sprite_set_x(sprite1, sprite_x(sprite1) - 1);
            sprite_set_x(sprite2, sprite_x(sprite2) + 1);
        }
        else
        {
            sprite_set_x(sprite2, right_edge - 5);
            sprite_set_x(sprite1, sprite_x(sprite2) - 80);
        }
    }
    else
    {
        if(sprite_x(sprite2) > left_edge)
        {
            sprite_set_x(sprite1, sprite_x(sprite1) + 1);
            sprite_set_x(sprite2, sprite_x(sprite2) - 1);
        }
        else
        {
            sprite_set_x(sprite2, left_edge + 5);
            sprite_set_x(sprite1, sprite_x(sprite2) + 80);
        }
    }
}

//Collision
void test_standing_collision(sprite sprite1, sprite sprite2)
{
    rectangle rect1 = make_hitbox(sprite1, 0, -48, 105, -30);
    bool rectangle_collision = sprite_rectangle_collision(sprite2, rect1);

    if(sprite_value(sprite1, "jumping") == 0 && sprite_value(sprite2, "jumping") == 0)
    {
        if(rectangle_collision)
        {
            sprite_set_dx(sprite1, 0);
            sprite_set_dx(sprite2, 0);
            standing_collision_routine(sprite1, sprite2);
            standing_collision_routine(sprite2, sprite1);
        }
    }
}

//Collision
void attack_collision_routine(sprite sprite,rectangle rect, string animation, bool facing_left)
{
    bool rectangle_collision = sprite_rectangle_collision(sprite, rect);
    if(rectangle_collision)
    {
        sprite_set_value(sprite,"hit",1);
        sprite_set_dx(sprite, 0);
        if(facing_left)
            sprite_set_x(sprite, sprite_x(sprite) - 12);
        else
            sprite_set_x(sprite, sprite_x(sprite) + 12);
        sprite_set_value(sprite,"life", 10);
        sprite_start_animation(sprite, animation);
    }
}

//Collision
void test_attack_collisions(sprite sprite1, sprite sprite2)
{
    if(sprite_value(sprite1, "attacking") == 1 && sprite_value(sprite2, "hit") != 1 && sprite_value(sprite2, "jumping") != 1)
    {
        if(sprite_value(sprite1, "facing_left") == 1)
        {
            rectangle rect1 = make_hitbox(sprite1, -50, -48, -30, -30);
            int random = rand() % 2;
            if(random == 0)
                attack_collision_routine(sprite2, rect1, "HitLeft1", true);
            else
                attack_collision_routine(sprite2, rect1, "HitLeft2", true);
        }
        else
        {
            rectangle rect1 = make_hitbox(sprite1, 50, -48, -30, -30);
            int random = rand() % 2;
            if(random == 0)
                attack_collision_routine(sprite2, rect1, "HitRight1", false);
            else
                attack_collision_routine(sprite2, rect1, "HitRight2", false);
        }
    }
}