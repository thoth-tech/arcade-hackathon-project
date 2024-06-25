#include "splashkit.h"

class Time
{
    private:
        unsigned int time_ticks;
        int starting_time = 99;
        string time = "99";
        string name = "timer";
        color text_color = COLOR_WHITE;
    public:
        Time()
        {
            create_timer(name);
            start_timer(name);
        };

        void stop_time()
        {
            pause_timer(name);
        };

        void start_time()
        {
            start_timer(name);
        }

        void update()
        {
            time_ticks = timer_ticks(name);
            
            if(std::stoi(time) > 9)
            {
                if(std::stoi(time) == 10)
                    text_color = COLOR_YELLOW;
                time = std::to_string(starting_time - time_ticks/1000);
                draw_text(time, text_color, "normalFont", 35, 368, 45, option_to_screen());
            }
            else if(std::stoi(time) > 0)
            {
                if(std::stoi(time) % 2 == 0)
                    text_color = COLOR_YELLOW;
                else
                    text_color = COLOR_RED;
                
                time = std::to_string(starting_time - time_ticks/1000);
                time = "0" + time;
                draw_text(time, text_color, "normalFont", 35, 368, 45, option_to_screen());
            }
            else if(std::stoi(time) == 0)
            {
                text_color = COLOR_RED;
                time = "0";
                stop_timer(name);
                draw_text("00", text_color, "normalFont", 35, 368, 45, option_to_screen());
            }
        }

        string get_time()
        {
            return this->time;
        }

        void reset()
        {
            text_color = COLOR_WHITE;
            starting_time = 99;
            time = "99";
            reset_timer(name);
            start_timer(name);
        }
};