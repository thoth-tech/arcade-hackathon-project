#include "splashkit.h"

//Collision testing
rectangle make_hitbox_test(sprite sprite, float x, float y, float height, float width)
{
    rectangle rect;
    rect.x = sprite_x(sprite) + x;
    rect.y = sprite_y(sprite) + y;
    rect.height = sprite_height(sprite) + height;
    rect.width = sprite_width(sprite) + width;

    return rect;
}

//Collision testing
void draw_hitboxes(rectangle rect1, rectangle rect2)
{
    draw_rectangle(COLOR_GREEN, rect1);
    draw_rectangle(COLOR_GREEN, rect2);
}

//Collision Testing
void test_collisions(sprite sprite1, sprite sprite2)
{
    string output2 = "";
    //Makes Standing hitbox
    //rectangle rect1 = make_hitbox(sprite1, 0, -48, 105, -30);
    //Makes Crouching hitbox
    //rectangle rect1 = make_hitbox(sprite1, 0, 35, 40, -30);
    //Makes attack hitbox facing right
    //rectangle rect1 = make_hitbox(sprite1, 50, -48, -30, -30);
    //Makes attack hitbox facing left
    //rectangle rect1 = make_hitbox(sprite1, -50, -48, -30, -30);
    rectangle rect1 = make_hitbox_test(sprite1, -50, -48, -30, -30);
    rectangle rect2 = make_hitbox_test(sprite2, 0, -48, 105, -30);


    bool rectangle_collision = sprite_rectangle_collision(sprite2, rect1);

    if(rectangle_collision)
        output2 = "Rectangle Collision Detected: True";
    else
        output2 = "Rectangle Collision Detected: False";

    draw_text(output2, COLOR_WHITE, 0, 50, 375, 90, option_to_screen());
    draw_hitboxes(rect1, rect2);
}

void double_ko(sprite sprite1, sprite sprite2)
{
    if(key_typed(Q_KEY))
    {
        sprite_set_value(sprite1,"hit",1);
        sprite_set_dx(sprite1, 0);
        sprite_set_value(sprite1,"life", 100);
        sprite_start_animation(sprite1, "HitLeft1");
        sprite_set_value(sprite2,"hit",1);
        sprite_set_dx(sprite2, 0);
        sprite_set_value(sprite2,"life", 100);
        sprite_start_animation(sprite2, "HitLeft1");
    }
}

void draw_screen_rectangles()
{
    double left_edge = camera_x();
    double right_edge = left_edge + screen_width();

    draw_rectangle(COLOR_WHITE,left_edge,camera_y(),150,screen_height());
    draw_rectangle(COLOR_WHITE,right_edge-150,camera_y(),150,screen_height());
    draw_rectangle(COLOR_WHITE,(left_edge + right_edge)/2 - 75,camera_y(),150,screen_height());
}