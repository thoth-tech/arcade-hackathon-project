#include "splashkit.h"
#include <iostream>
using namespace std;

class Lifebar
{
    private:
        double health = 100;
        double y_position = 10;
        bool left;
        double portrait_x_pos;
        double bar_x_pos;
        double fill_width;
        double name_pos;
        color bar_color;
        color bar_color2;
        string name;

    public:
        Lifebar(){};
        Lifebar(bool left, string name)
        {
            this->left = left;
            this->name = name;
        };
        ~Lifebar(){};

        void damage(double damage)
        {
            health -= damage;
        }

        void set_health()
        {
            if(health > 0)
                if(left)
                    fill_width = (health/100 * 276);
                else
                    fill_width = 276 - (health/100 * 276);
            else
                if(left)
                    fill_width = 0;
                else 
                    fill_width = 276;
        }

        double get_health()
        {
            return this->health;
        }

        void set_attributes(double portrait_x_pos, double bar_x_pos, color color1, color color2, double name_pos)
        {
            this->portrait_x_pos = portrait_x_pos;
            this->bar_x_pos = bar_x_pos;
            this->bar_color = color1;
            this->bar_color2 = color2;
            this->name_pos = name_pos;
        }

        void draw()
        {
            if(left)
                set_attributes(10, 90, COLOR_RED, COLOR_YELLOW, 95);
            else
            {
                if(name == "Ryu")
                    name = "                                         RYU";
                set_attributes(678, 394, COLOR_YELLOW, COLOR_RED, 395);
            }
            set_health();
            fill_rectangle(COLOR_BLACK, portrait_x_pos, y_position, 80, 80, option_to_screen());
            fill_rectangle(COLOR_LIGHT_GRAY, bar_x_pos, y_position, 284, 30, option_to_screen());
            fill_rectangle(bar_color, (bar_x_pos + 4), (y_position + 4), 276, 22, option_to_screen());
            fill_rectangle(bar_color2, (bar_x_pos + 4), (y_position + 4), fill_width, 22, option_to_screen());
            draw_text(name, COLOR_WHITE, "normalFont", 18, name_pos, 45, option_to_screen());
        }

        void reset()
        {
            this->health = 100;
        }
};