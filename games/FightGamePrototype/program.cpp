#include "splashkit.h"
#include "gamescreen.h"

int main()
{
    load_resource_bundle("bundle", "animation_bundle.txt");
    open_window("Test", 768, 488);
    set_camera_x(630);
    set_camera_y(200);
    CityBackground background;
    Ryu ryu_sprite(true, camera_x() + 150, camera_y() + 280, true, true);
    Ryu ryu_sprite2(false, camera_x() + 543, camera_y() + 280, false, true);
    GameScreen game(ryu_sprite, ryu_sprite2, background);

    while(!key_typed(ESCAPE_KEY))
    {
        game.update();
    }

    free_resource_bundle("bundle");
    return 0;
}