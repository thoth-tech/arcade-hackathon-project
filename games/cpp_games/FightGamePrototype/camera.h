#include "splashkit.h"

#define SCREEN_BORDER 30
#define SCREEN_LIMIT_LEFT 0
#define SCREEN_RIGHT_LIMIT 1900

//Camera
void reject_boundry(sprite sprite, float border, float value)
{
    sprite_set_value(sprite, "edge", 1);
    sprite_set_x(sprite, border + value);
}

//Camera
void test_left_edge(sprite sprite1, sprite sprite2, double left_edge, double right_edge)
{
    if (sprite_x(sprite1) < left_edge && (sprite_value(sprite1, "walk") == 1 || sprite_value(sprite1, "jumping") == 1))
    {
        move_camera_by(sprite_x(sprite1) - left_edge, 0);
        if(sprite_x(sprite2) > right_edge)
            reject_boundry(sprite2, right_edge, -1);
    }
}

//Camera
void test_right_edge(sprite sprite1, sprite sprite2, double left_edge, double right_edge)
{
    if (sprite_x(sprite1) > right_edge && (sprite_value(sprite1, "walk") == 1 || sprite_value(sprite1, "jumping") == 1))
    {
        move_camera_by(sprite_x(sprite1) - right_edge, 0);
        if(sprite_x(sprite2) < left_edge)
            reject_boundry(sprite2, left_edge, 1);
    }
}

//Camera
void test_left_boundry(sprite sprite1, sprite sprite2, double right_edge)
{
    if(sprite_x(sprite1) < SCREEN_LIMIT_LEFT)
    {
        reject_boundry(sprite1, SCREEN_LIMIT_LEFT, 1);
        if(sprite_x(sprite2) > right_edge)
            reject_boundry(sprite2, right_edge, -1);
    }
}

//Camera
void test_right_boundry(sprite sprite1, sprite sprite2, double left_edge)
{
    if(sprite_x(sprite1) > SCREEN_RIGHT_LIMIT)
    {
        reject_boundry(sprite1, SCREEN_RIGHT_LIMIT, -1);
        if(sprite_x(sprite2) < left_edge)
            reject_boundry(sprite2, left_edge, 1);
    }
}

//Camera
void update_camera_position(sprite sprite1, sprite sprite2)
{
    // Test edge of screen boundaries to adjust the camera
    double left_edge = camera_x() + SCREEN_BORDER;
    double right_edge = left_edge + screen_width() - 2 * SCREEN_BORDER - 100;

    test_left_edge(sprite1, sprite2, left_edge, right_edge);
    test_left_edge(sprite2, sprite1, left_edge, right_edge);
    test_right_edge(sprite1, sprite2, left_edge, right_edge);
    test_right_edge(sprite2, sprite1, left_edge, right_edge);
    test_left_boundry(sprite1, sprite2, right_edge);
    test_left_boundry(sprite2, sprite1, right_edge);
    test_right_boundry(sprite1, sprite2, left_edge);
    test_right_boundry(sprite2, sprite1, left_edge);
}