#include "splashkit.h"
#include "player.h"
#include "planet.h"
#include "screen.h"
#include "lost_in_space.h"
#include <memory>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 700

/**
 * Load the game images, sounds, etc.
 */
void load_resources()
{
    load_resource_bundle("game_bundle", "lost_in_space.txt");
    load_resource_bundle("menu_bundle", "menu_bundle.txt");
}

/**
 * Entry point.
 *
 * Manages the initialisation of data, the event loop, and quitting.
 */

int main()
{
    load_resources();
    open_window("Lost In Space", SCREEN_WIDTH, SCREEN_HEIGHT);

    create_timer("ScreenTimer");

    game_data game = new_game();

    bool menu_do_once = false;

    // holds the screen data
    shared_ptr<Screen> screen; // new "smart" c++ pointer that removes itself when no longer in use - avoid memory leaks

    shared_ptr<Screen> normal_screen(new Screen(new CompanyIntroScreen));
    screen = normal_screen;

    // added ability to quit with escape, was just nice to have to correspond to the other game
    while (!quit_requested() && !key_typed(ESCAPE_KEY))
    {
        // Handle input to adjust player movement
        process_events();

        // if the game is in a menu state, we continue in the menu state until the menu
        // tells us that it no longer needs to exist
        // we can remove references to it and the smart pointer will clear it up
        if (game.state == MENU)
        {
            screen->update(); // this starts the screen

            if (screen->menu_completed == true)
            {
                start_game(game);
                screen = nullptr; // should trigger the destructor (smart ptr), which will trigger the state destructor
            }
        }
        else if (game.state == PLAY) // if play, work as normal
        {
            handle_input(game.player);

            // Perform movement and update the camera
            update_game(game);

            // draw everything
            draw_game(game);
        }
        else if (game.state == END)
        { // really awful to have the code here but its 4:33 in the morning and i just want this done
            // but was much easier to paste the code, considering i dont want buttons or anything like that
            // could be its own function but it is now 5:09 in the morning and at the moment, the only thing that
            // matters is sleep.
            set_camera_x(0);
            set_camera_y(0);
            clear_screen(COLOR_BLACK);
            draw_bitmap("MenuBg", 0, 0, option_to_screen());

            if (!music_playing())
            {
                play_music("MenuMusic.mp3");
                set_music_volume(0.2f);
            }

            // set up for the text to draw the players final score.
            point_2d pt = screen_center();
            string score_text = "Your final score: " + to_string(game.player.score);
            font screen_font = font_named("DefaultFont");
            int font_size = 30;
            color font_color = COLOR_WHITE_SMOKE;

            // qanita's gameover bitmap
            bitmap title = bitmap_named("GameOver");
            drawing_options scale = option_scale_bmp(1, 1);
            draw_bitmap(title, pt.x - bitmap_width(title) / 2, 100, scale);

            draw_text(score_text, font_color, screen_font, font_size, pt.x - text_width(score_text, screen_font, font_size) / 2, (pt.y - text_height(score_text, screen_font, font_size) / 2) + 150, option_to_screen());
        }

        refresh_screen(60);
    }

    free_resource_bundle("game_bundle");
    // free_resource_bundle("menu_bundle");
    // do not uncomment the above line, everything will immediately go to hell
    free_all_timers();

    return 0;
}