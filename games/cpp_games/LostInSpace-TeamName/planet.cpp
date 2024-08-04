#include "splashkit.h"
#include "planet.h"

#define PLANET_SPEED 0.1
#define PLANET_ROTATE 0.05

bitmap planet_bitmap(planet_kind kind)
{
    switch (kind)
    {
        case MERCURY:
            return bitmap_named("mercury");
        case VENUS:
            return bitmap_named("venus");
        case EARTH:
            return bitmap_named("earth");
        case MARS:
            return bitmap_named("mars");
        case JUPITER:
            return bitmap_named("jupiter");
        case SATURN:
            return bitmap_named("saturn");
        case URANUS:
            return bitmap_named("uranus");
        case NEPTUNE:
            return bitmap_named("neptune");
        case PLUTO:
            return bitmap_named("pluto");
        default:
            return bitmap_named("mercury");
    }
}

planet_data new_planet(double x, double y)
{
    planet_data result;

    //Set visited property to false, by default
    result.visited = false;

    bitmap default_bitmap = planet_bitmap(MERCURY);

    //Randomly set a new planets sprite
    result.planet_sprite = create_sprite(planet_bitmap(static_cast<planet_kind>(rnd(9))));

    //Set planet x and y
    sprite_set_x(result.planet_sprite, x - sprite_width(result.planet_sprite)/2);
    sprite_set_y(result.planet_sprite, y - sprite_height(result.planet_sprite)/2);

    //Set planet velocity and rotation
    sprite_set_dx(result.planet_sprite, PLANET_SPEED);
    sprite_set_rotation(result.planet_sprite, PLANET_ROTATE);

    return result;
}

void draw_planet(const planet_data &planet)
{
    draw_sprite(planet.planet_sprite);

    if (planet.visited)
    {
        circle c = sprite_circle(planet.planet_sprite);
        c.radius += 10;
        draw_circle(COLOR_BRIGHT_GREEN, c);
    }
}

void update_planet(planet_data &planet_to_update)
{
    // Apply movement based on rotation and velocity in the sprite
    update_sprite(planet_to_update.planet_sprite);
}