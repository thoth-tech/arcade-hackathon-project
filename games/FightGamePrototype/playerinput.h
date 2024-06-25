#include "splashkit.h"
#include "movements.h"

class Player_Input
{
    private:
        sprite player;
        key_code right_key;
        key_code left_key;
        key_code jump_key;
        key_code crouch_key;
        key_code punch_key;
        key_code kick_key;
        key_code block_key;

    public:
        Player_Input(){};
        Player_Input(sprite player, key_code right_key, key_code left_key,
            key_code jump_key, key_code crouch_key, key_code punch_key, key_code kick_key, key_code block_key)
        {
            this->player = player;
            this->right_key = right_key;
            this->left_key = left_key;
            this->jump_key = jump_key;
            this->crouch_key = crouch_key;
            this->punch_key = punch_key;
            this->kick_key = kick_key;
            this->block_key = block_key;
        };
        ~Player_Input(){};

        void check_input()
        {
            if(sprite_value(player, "jumping") == 0 && sprite_value(player, "attacking") == 0 && sprite_value(player, "hit") == 0)
            {
                //Key Presses
                if(sprite_value(player, "crouch") != 1 && sprite_value(player, "block") != 1)
                {
                    if(key_down(right_key))
                        walk(false, player);
                    else if(key_down(left_key))
                        walk(true, player);
                    if(key_typed(jump_key))
                        jump(player);
                }

                if(sprite_value(player, "crouch") != 1 && sprite_value(player, "walk") != 1)
                {
                    if(key_down(block_key))
                        block(player);
                }

                if(sprite_value(player, "walk") != 1 && sprite_value(player, "block") != 1)
                {
                    if(key_down(crouch_key) && sprite_value(player, "walk") != 1)
                        crouch(player);
                }

                if(sprite_value(player, "walk") != 1 && sprite_value(player, "crouch") != 1 && sprite_value(player, "block") != 1)
                {
                    if(key_typed(punch_key))
                        punch(player);
                    if(key_typed(kick_key))
                        kick(player);
                }
                
                //Key Releases
                if((key_released(right_key) || key_released(left_key)))
                    press_release_routine(player, "walk");
                if(key_released(crouch_key))
                    press_release_routine(player, "crouch");
                if(key_released(block_key))
                    press_release_routine(player, "block");
            }
            else
            {
                if(sprite_animation_has_ended(player))
                {
                    if(sprite_value(player, "attacking") == 1)
                        sprite_set_value(player, "attacking", 0);
                    else if(sprite_value(player, "jumping") == 1)
                        end_jump(player, right_key, left_key, jump_key, crouch_key, punch_key, kick_key, block_key);
                    else if(sprite_value(player, "hit") == 1)
                        end_hit(player, right_key, left_key, jump_key, crouch_key, punch_key, kick_key, block_key);
                }
            }
        }
};