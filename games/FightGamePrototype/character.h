#include "splashkit.h"
#include "lifebar.h"
#include "playerinput.h"
#include <iostream>
using namespace std;

class Character
{
    protected:
        Lifebar lifebar;
        sprite sprite_bmp;
        string name;
        point_2d start_position;
        string bundlename;
        bool facing_left;
        Player_Input input;
        bool player1;
        bool human_player;
        int round_win = 0;

    public:
        Character(){};
        virtual ~Character()
        {
            if(has_resource_bundle(bundlename))
            {
                free_resource_bundle(bundlename);
                cout << "Character Resources Free: " << bundlename << endl; 
            }
        };

        virtual void sprite_setup(bool facing_left)
        {
            sprite_set_position(sprite_bmp, start_position);
            sprite_set_scale(sprite_bmp, 2.8);

            sprite_add_value(sprite_bmp, "walk", 0);
            sprite_add_value(sprite_bmp, "crouch", 0);
            sprite_add_value(sprite_bmp, "keypress", 0);
            sprite_add_value(sprite_bmp, "edge", 0);
            sprite_add_value(sprite_bmp, "jumping", 0);
            sprite_add_value(sprite_bmp, "attacking", 0);
            sprite_add_value(sprite_bmp, "block", 0);
            sprite_add_value(sprite_bmp, "hit", 0);
            sprite_add_value(sprite_bmp, "life", 0);
            
            if(facing_left)
            {
                sprite_start_animation(sprite_bmp, "IdleLeft");
                sprite_add_value(sprite_bmp, "facing_left", 0);
            }
            else
            {
                sprite_start_animation(sprite_bmp, "IdleRight");
                sprite_add_value(sprite_bmp, "facing_left", 1);
            }
        };

        virtual void sprite_update()
        {
            //possibly make this in a separate function
            draw_sprite(sprite_bmp);
            if(sprite_value(sprite_bmp, "walk") != 1 && sprite_animation_has_ended(sprite_bmp))
            {
                if(sprite_value(sprite_bmp, "facing_left") == 0)
                    sprite_start_animation(sprite_bmp, "IdleLeft");
                else
                    sprite_start_animation(sprite_bmp, "IdleRight");
            }
            if(sprite_animation_has_ended(sprite_bmp))
                sprite_replay_animation(sprite_bmp);
            update_sprite(sprite_bmp);

            lifebar.damage(sprite_value(sprite_bmp,"life"));
            sprite_set_value(sprite_bmp,"life", 0);
            lifebar.draw();
            draw_round_win();
        };

        virtual void get_input()
        {
            input.check_input();
        };

        virtual void game_over_update()
        {
            draw_sprite(sprite_bmp);
            update_sprite(sprite_bmp);
            draw_round_win();
            lifebar.draw();
        };

        virtual sprite get_sprite()
        {
            return this->sprite_bmp;
        };

        virtual Lifebar get_lifebar()
        {
            return lifebar;
        };

        virtual void reset()
        {
            sprite_setup(facing_left);
            lifebar.reset();
        };

        void set_up_input()
        {
            //Change input based on player vs player or player vs comp
            if(player1)
            {
                Player_Input input(sprite_bmp, RIGHT_KEY, LEFT_KEY, UP_KEY, DOWN_KEY, A_KEY, S_KEY, D_KEY);
                this->input = input;
            }
            else
            {
                Player_Input input(sprite_bmp, K_KEY, H_KEY, U_KEY, J_KEY, B_KEY, N_KEY, M_KEY);
                this->input = input;
            }
        };

        string get_name()
        {
            return this->name;
        };

        void add_round_win()
        {
            this->round_win += 1;
        };

        int get_round_win()
        {
            return this->round_win;
        };

        void draw_round_win()
        {
            if(player1 && round_win >= 1)
                draw_bitmap("coin",320,45,option_to_screen());
            else if(!player1 && round_win >= 1)
                draw_bitmap("coin",425,45,option_to_screen());
        };
};

class Ryu : public Character
{
    public:
        Ryu(bool facing_left, double start_x, double start_y, bool player1, bool human_player)
        {
            this->facing_left = facing_left;
            this->bundlename = "ryu";
            this->player1 = player1;
            this->human_player = human_player;
            if(!has_resource_bundle(bundlename))
            {
                load_resource_bundle(bundlename, "fighter_ryu.txt");
                cout << "Character Loaded: " << bundlename << endl; 
            }

            this->name = "Ryu";
            sprite_bmp = create_sprite("RyuBmp", "RyuTest");
            start_position.x = start_x;
            start_position.y = start_y;
            set_up_input();
            this->input = input;

            Lifebar lifebar(facing_left,name);
            this->lifebar = lifebar;

            sprite_setup(facing_left);
        };
};