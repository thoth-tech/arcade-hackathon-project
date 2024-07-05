#include "splashkit.h"
#include "character.h"
#include "time.h"
#include "background.h"
#include "camera.h"
#include "collision.h"
#include "test_functions.h"

class GameScreen
{
    private:
        bool game_over;
        bool pre_game = true;
        bool victory = false;
        int round = 1;
        double round_alpha = 0.1;
        double fight_alpha = 0.1;
        double fade_alpha = 1.0;
        double round_over_timer = 0.0;
        string round_end_text = "";
        Background background;
        Time round_timer;
        Character player1;
        Character player2;

    public:
        GameScreen(Character player1, Character player2, Background background)
        {
            this->game_over = game_screen_init();
            cout << "Game Screen Resources Loaded" << endl; 
            this->player1 = player1;
            this->player2 = player2;
            this->background = background;
        };
        ~GameScreen()
        {
            cout << "Game Screen Resources freed" << endl; 
        };

        void update()
        {
            if(pre_game)
            {
                pre_game_loop();
            }
            else if(!game_over)
            {
                game_loop();
            }
            else if (game_over == true && !victory)
            {
                round_end_loop();
            }
            else if(victory)
            {
                victory_routine();
            }
        };

        void pre_game_loop()
        {
            clear_screen(COLOR_GRAY);
            background.draw_background();
            player1.sprite_update();
            player2.sprite_update();
            round_timer.stop_time();
            round_timer.update();
            pre_game_show_text();
            process_events();
            refresh_screen(60);
        };

        void pre_game_show_text()
        {
            if(round_alpha < 3)
            {
                round_alpha += 0.02;
                color text_color = rgba_color(255.0,255.0,255.0,round_alpha);
                draw_text("Round " + to_string(round), text_color, "normalFont", 65, 285, 130, option_to_screen());
            }
            if(round_alpha >= 3)
            {
                if(fight_alpha < 2.5)
                {
                    fight_alpha += 0.02;
                    color text_color = rgba_color(255.0,255.0,255.0,fight_alpha);
                    draw_text("Fight", text_color, "normalFont", 65, 310, 130, option_to_screen());
                }
                else if(fight_alpha > 2.5 && fight_alpha < 3)
                {
                    fight_alpha += 0.02;
                    if(fade_alpha > 0)
                        fade_alpha -= 0.02;
                    color text_color = rgba_color(255.0,0.0,0.0,fade_alpha);
                    draw_text("Fight", text_color, "normalFont", 65, 310, 130, option_to_screen());
                }
                else
                {
                    pre_game = false;
                    round_timer.start_time();
                }
            }
        };

        void game_loop()
        {
            //Draw Screen
            clear_screen(COLOR_GRAY);
            background.draw_background();

            //Player Inputs
            player1.get_input();
            player2.get_input();

            //Test player collision
            test_standing_collision(player1.get_sprite(), player2.get_sprite());
            test_attack_collisions(player1.get_sprite(), player2.get_sprite());
            test_attack_collisions(player2.get_sprite(), player1.get_sprite());

            game_over_test();
        
            //Update Sprites
            sprite_face_direction();
            player1.sprite_update();
            player2.sprite_update();

            //Update camera
            update_camera_position(player1.get_sprite(), player2.get_sprite());

            //Update timer
            round_timer.update();

            process_events();
            refresh_screen(60);
        }

        void game_over_test()
        {
            game_over = test_time_over();
            if(game_over)
                round_end_text = "Time Over";
            else
            {
                game_over = test_double_ko();
                if(game_over)
                {
                    round_end_text = "Double KO!!!";
                }
                else
                {
                    game_over = test_player_win(player1, player2);
                    if(game_over)
                    {
                        round_end_text = player1.get_name() + " Wins!";
                        player1.add_round_win();
                    }
                    else
                    {
                        game_over = test_player_win(player2, player1);
                        if(game_over)
                        {
                            round_end_text = player2.get_name() + " Wins!";
                            player2.add_round_win();
                        }
                    }
                }
            }
        };

        void round_end_loop()
        {
            clear_screen(COLOR_GRAY);
            background.draw_background();
            player1.game_over_update();
            player2.game_over_update();
            round_timer.update();
            draw_end_text();
            process_events();
            refresh_screen(60);

            //Reset Round
            if(round_over_timer > 5)
                reset_round();

            round_over_timer += 0.02;
        };

        void reset_round()
        {
            round_alpha = 0.1;
            fight_alpha = 0.1;
            fade_alpha = 1.0;
            pre_game = true;
            round += 1;
            game_over = game_screen_init();
            player1.reset();
            player2.reset();
            round_timer.reset();
            round_over_timer = 0;
            sprite_face_direction();
            if(player1.get_round_win() == 2 || player2.get_round_win() == 2)
            {
                victory = true;
                pre_game = false;
                game_over = true;
            }
        };

        void victory_routine()
        {
            //Change to victory screen
            clear_screen(COLOR_BLACK);
            draw_text("Victory Screen", COLOR_WHITE, "normalFont", 65, 150, 130, option_to_screen());
            process_events();
            refresh_screen(60);
        };

        void draw_end_text()
        {
            if(round_end_text == "Ryu Wins!")
                draw_text(round_end_text, COLOR_WHITE, "normalFont", 65, 235, 123, option_to_screen());
            else if(round_end_text == "Double KO!!!")
                draw_text(round_end_text, COLOR_WHITE, "normalFont", 65, 225, 123, option_to_screen());
            else if(round_end_text == "Time Over")
                draw_text(round_end_text, COLOR_WHITE, "normalFont", 65, 238, 123, option_to_screen());
        };

        bool game_screen_init()
        {
            bool game_over = false;
            set_camera_x(630);
            set_camera_y(200);
            return game_over;
        };

        void sprite_face_direction()
        {
            point_2d sprite1_center = center_point(player1.get_sprite());
            point_2d sprite2_center = center_point(player2.get_sprite());

            if(sprite1_center.x < sprite2_center.x)
            {
                sprite_set_value(player1.get_sprite(), "facing_left", 0);
                sprite_set_value(player2.get_sprite(), "facing_left", 1);
            }
            else
            {
                sprite_set_value(player1.get_sprite(), "facing_left", 1);
                sprite_set_value(player2.get_sprite(), "facing_left", 0);
            }
        };

        bool test_double_ko()
        {
            bool game_over = false;

            if(player1.get_lifebar().get_health() <= 0 && player2.get_lifebar().get_health() <= 0)
            {
                if(sprite_value(player1.get_sprite(), "facing_left") == 0)
                {
                    sprite_start_animation(player1.get_sprite(), "KoLeft");
                    sprite_start_animation(player2.get_sprite(), "KoRight");
                }
                else
                {
                    sprite_start_animation(player1.get_sprite(), "KoRight");
                    sprite_start_animation(player2.get_sprite(), "KoLeft");
                }
                game_over = true;
                round_timer.stop_time();
            }

            return game_over;
        };

        bool test_player_win(Character player1, Character player2)
        {
            bool game_over = false;

            if(player2.get_lifebar().get_health() <= 0)
            {
                sprite_start_animation(player1.get_sprite(), "Victory");
                if(sprite_value(player1.get_sprite(), "facing_left") == 0)
                    sprite_start_animation(player2.get_sprite(), "KoRight");
                else
                    sprite_start_animation(player2.get_sprite(), "KoLeft");
                game_over = true;
                round_timer.stop_time();
            }
            return game_over;
        };

        bool test_time_over()
        {
            bool game_over = false;

            if(round_timer.get_time() == "00")
            {
                sprite_start_animation(player1.get_sprite(), "Victory");
                sprite_start_animation(player2.get_sprite(), "Victory");
                game_over = true;
                round_timer.stop_time();
            }

            return game_over;
        }
};