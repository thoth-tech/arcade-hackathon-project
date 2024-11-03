#include "splashkit.h"
#include "player.h"
#include <cmath>
#include <iostream>
#include <sstream>
#include <iomanip>

/**
 * The ship bitmap function converts a ship kind into a
 * bitmap that can be used.
 *
 * Not exposed by the header.
 *
 * @param kind  The kind of ship you want the bitmap of
 * @return      The bitmap matching this ship kind
 */
bitmap ship_bitmap(ship_kind kind)
{
    switch (kind)
    {
    case GLIESE:
        return bitmap_named("gliese");
    case PEGASI:
        return bitmap_named("pegasi");
    case AQUARII:
    default:
        return bitmap_named("aquarii");
    }
}

player_data new_player()
{
    player_data result;
    bitmap default_bitmap = ship_bitmap(AQUARII);

    // Create the sprite with 3 layers - we can turn on and off based
    // on the ship kind selected
    result.player_sprite = create_sprite(default_bitmap);

    sprite_add_layer(result.player_sprite, ship_bitmap(GLIESE), "GLIESE");
    sprite_add_layer(result.player_sprite, ship_bitmap(PEGASI), "PEGASI");

    // Default to layer 0 = Aquarii so hide others
    sprite_hide_layer(result.player_sprite, 1);
    sprite_hide_layer(result.player_sprite, 2);

    result.kind = AQUARII;
    result.base_speed = PLAYER_SPEED; // Initialise base speed

    // Position in the centre of the initial screen
    // sprite_set_x(result.player_sprite, 400 - sprite_width(result.player_sprite)/2); //Subtracts half the player's width to x, centring
    // sprite_set_y(result.player_sprite, 400 - sprite_height(result.player_sprite)/2); //Subtracts half the player's height to y, centring

    return result;
}

void draw_player(const player_data &player_to_draw)
{
    draw_sprite(player_to_draw.player_sprite);
}

void update_player(player_data &player_to_update, double elapsed_time)
{
    // Apply movement based on rotation and velocity in the sprite
    update_sprite(player_to_update.player_sprite);

    // Test edge of screen boundaries to adjust the camera
    double left_edge = camera_x() + SCREEN_BORDER;
    double right_edge = left_edge + screen_width() - 2 * SCREEN_BORDER;
    double top_edge = camera_y() + SCREEN_BORDER;
    double bottom_edge = top_edge + screen_height() - 2 * SCREEN_BORDER;

    // Get the center of the player
    point_2d sprite_center = center_point(player_to_update.player_sprite);

    // Test if the player is outside the area and move the camera
    // the player will appear to stay still and everything else
    // will appear to move :)

    // Test top/bottom of screen
    if (sprite_center.y < top_edge)
    {
        move_camera_by(0, sprite_center.y - top_edge);
    }
    else if (sprite_center.y > bottom_edge)
    {
        move_camera_by(0, sprite_center.y - bottom_edge);
    }

    // Test left/right of screen
    if (sprite_center.x < left_edge)
    {
        move_camera_by(sprite_center.x - left_edge, 0);
    }
    else if (sprite_center.x > right_edge)
    {
        move_camera_by(sprite_center.x - right_edge, 0);
    }

    // Increase base speed over time
    float dx = sprite_dx(player_to_update.player_sprite);
    float speed_increase = elapsed_time * 0.0001;
    player_to_update.base_speed += speed_increase;
    sprite_set_dx(player_to_update.player_sprite, dx += speed_increase);
}

/**
 * Switch the ship kind - will adjust which layer is hidden/shown
 */
void player_switch_to_ship(player_data &player, ship_kind target)
{
    // only do this if there is a change
    if (player.kind != target)
    {
        // show then hide layers
        sprite_show_layer(player.player_sprite, static_cast<int>(target));
        sprite_hide_layer(player.player_sprite, static_cast<int>(player.kind));

        // remember what is currently shown
        player.kind = target;
    }
}

void initialize_player(player_data &player) 
{
    // Set initial velocity to move forward
    float initial_dx = PLAYER_SPEED; // Adjust PLAYER_SPEED as needed
    sprite_set_dx(player.player_sprite, initial_dx);
    
    // Facing upwards
    sprite_set_rotation(player.player_sprite, -90);

}

void handle_input(player_data &player)
{
    // Allow the player to switch ships
    if (key_typed(NUM_1_KEY))
        player_switch_to_ship(player, AQUARII);
    if (key_typed(NUM_2_KEY))
        player_switch_to_ship(player, GLIESE);
    if (key_typed(NUM_3_KEY))
        player_switch_to_ship(player, PEGASI);

    // Handle movement - rotating left/right and moving forward/back
    float dx = sprite_dx(player.player_sprite);
    float rotation = sprite_rotation(player.player_sprite);

    // Allow rotation with left/right keys
    if (key_down(LEFT_KEY))
        sprite_set_rotation(player.player_sprite, rotation - PLAYER_ROTATE_SPEED);
    if (key_down(RIGHT_KEY))
        sprite_set_rotation(player.player_sprite, rotation + PLAYER_ROTATE_SPEED);

    // Change acceleration with up/down keys
    // Increase speed with up/down keys - typed to give step increases
    if (key_typed(UP_KEY))
        sprite_set_dx(player.player_sprite, dx + PLAYER_SPEED);
    // Decrease speed with up/down keys - typed to give step decreaseds until reaching default speed
    if (key_typed(DOWN_KEY))
    {
        // Increase speed with up/down keys - typed to give step increases
        if (dx - PLAYER_SPEED >= player.base_speed)
            sprite_set_dx(player.player_sprite, dx - PLAYER_SPEED);
        else
            sprite_set_dx(player.player_sprite, player.base_speed);
    }
}

void draw_hud(const player_data &player, const planet_data &planet, double time_percent)
{
    int part_width = 300; // How much the bar is filled in (300 is max)

    // fill_rectangle(COLOR_GRAY, 0, 0, 800, 50, option_to_screen()); (i didn't like how this looked)

    float distance = distance_to_planet(player, planet);

    // Helper function to format floating-point numbers to 1 decimal place and return as string
    auto format_float = [](float value) -> std::string
    {
        std::ostringstream out;
        out << std::fixed << std::setprecision(1) << value;
        return out.str();
    };

    // Draw score, location and distance to top left of the screen
    draw_text("SCORE: " + to_string(player.score),
              COLOR_GOLD, 0, 0, option_to_screen());

    draw_text("LOCATION: " + point_to_string(center_point(player.player_sprite)),
              COLOR_WHITE, 0, 10, option_to_screen());

    draw_text("DISTANCE: " + to_string(distance_to_planet(player, planet)),
              COLOR_WHITE, 0, 20, option_to_screen());

    draw_text("BASE SPEED: " + to_string(player.base_speed),
              COLOR_WHITE, 0, 30, option_to_screen());

    if (sprite_dx(player.player_sprite) - player.base_speed > 1.4)
        draw_text("SPEED + " + format_float(sprite_dx(player.player_sprite) - player.base_speed),
                  COLOR_RED, 0, 40, option_to_screen());

    // Draw bar
    load_bitmap("orange_bar", "orange_bar.png");
    draw_bitmap("empty", 300, 0, option_to_screen());
    if (time_percent <= 0.3)
    {
        draw_bitmap("orange_bar", 300, 0, option_part_bmp(0, 0, part_width * time_percent, bitmap_height("orange_bar"), option_to_screen()));
    }

    else
    {
        draw_bitmap("full", 300, 0, option_part_bmp(0, 0, part_width * time_percent, bitmap_height("full"), option_to_screen()));
    }

    vector_2d direction;
    float rotation_angle;

    direction = vector_multiply(unit_vector(vector_from_to(player.player_sprite, planet.planet_sprite)), 30);
    rotation_angle = sprite_rotation(player.player_sprite);

    // Load bitmaps
    load_bitmap("compass", "Compass.png");
    load_bitmap("view", "Compass2.png");

    // draws compas to closest planet
    // draw_circle(COLOR_WHITE, 750, 20, 15, option_to_screen());
    draw_bitmap("compass", 720 - 23.5, 40 - 25.5, option_to_screen());
    // Direction of the spaceship
    draw_bitmap("view", 720 - 23.5, 40 - 25.5, option_rotate_bmp(rotation_angle, option_to_screen()));
    // draw_line(COLOR_WHITE, 720, 40, 720 + direction.x, 40 + direction.y, option_to_screen());

    // Draw triangle to represent direction
    // Calculate the points for the triangle
    point_2d tip = {720 + direction.x, 40 + direction.y};
    vector_2d perpendicular = {-direction.y, direction.x}; // Perpendicular vector to the direction
    perpendicular = unit_vector(perpendicular);            // Normalize the perpendicular vector

    // Adjust the size of the triangle as needed
    double triangle_base = 7.0;
    double triangle_height = 7.0;

    point_2d left = {tip.x + perpendicular.x * (triangle_base / 2) - direction.x * (triangle_height / 15),
                     tip.y + perpendicular.y * (triangle_base / 2) - direction.y * (triangle_height / 15)};
    point_2d right = {tip.x - perpendicular.x * (triangle_base / 2) - direction.x * (triangle_height / 15),
                      tip.y - perpendicular.y * (triangle_base / 2) - direction.y * (triangle_height / 15)};

    // Draw triangle
    fill_triangle(COLOR_YELLOW, tip.x, tip.y, left.x, left.y, right.x, right.y, option_to_screen());
}

double distance_to_planet(const player_data &player, const planet_data &planet)
{
    point_2d planet_loccation, player_loccation;

    planet_loccation = center_point(planet.planet_sprite);
    player_loccation = center_point(player.player_sprite);

    // Returns distance between two points
    return point_point_distance(player_loccation, planet_loccation);
}

int closest_planet_index(const player_data &player, const vector<planet_data> &planets)
{
    int result = -1;

    double closest_distance = 0;
    double planet_distance;

    for (int i = 0; i < planets.size(); i++)
    {
        // Check if visited (if we even need to check its distance)
        if (!planets[i].visited)
        {
            planet_distance = distance_to_planet(player, planets[i]);
            // Check if closest
            if (result == -1 or planet_distance < closest_distance)
            {
                closest_distance = planet_distance;
                result = i;
            }
        }
    }

    return result; //-1 means no planet was found
}