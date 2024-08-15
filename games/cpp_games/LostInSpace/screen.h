#include "splashkit.h"
#include "button.h"
#include <memory>
#include <vector>

class Screen;

class ScreenState
{
protected:
    Screen *screen;
    string screen_state;

public:
    virtual ~ScreenState() {};

    void set_state(Screen *screen, string screen_state)
    {
        this->screen = screen;
        this->screen_state = screen_state;
    };

    string get_type()
    {
        return this->screen_state;
    };

    virtual void update() = 0;
};

class Screen
{
private:
    ScreenState *state;

public:
    key_code input_key = F_KEY;
    key_code pause_key = H_KEY;
    bool menu_completed = false;

    Screen(ScreenState *state) : state(nullptr)
    {
        this->change_state(state, "Initial");
    };

    ~Screen()
    {
        delete state;
    };

    void change_state(ScreenState *new_state, string type)
    {
        if (this->state != nullptr)
            delete this->state;
        this->state = new_state;
        this->state->set_state(this, type);
    };

    void update()
    {
        this->state->update();
    };
};

class CompanyIntroScreen : public ScreenState
{
private:
    bool run_once = false;
    double alpha = 1.0;
    int screen_time = 5;

public:
    CompanyIntroScreen() {};

    ~CompanyIntroScreen() {};

    void update() override;
};

class TeamIntroScreen : public ScreenState
{
private:
    bool run_once = false;
    double alpha = 1.0;
    int screen_time = 7;

public:
    TeamIntroScreen() {};

    ~TeamIntroScreen() {};

    void update() override;
};

class MenuScreen : public ScreenState
{
private:
    bool run_once = false;
    vector<shared_ptr<Button>> menu_buttons;
    int offset = -10;
    int num_buttons = 2;
    int selection = 0;

public:
    MenuScreen() {};

    ~MenuScreen() {};

    void update() override;
};

class MenuComplete : public ScreenState
{
public:
    MenuComplete() {};

    ~MenuComplete() {};

    void update() override
    {
        this->screen->menu_completed = true;
    };
};

double fade_in(double alpha, double fade_length)
{
    if (alpha > 0)
        alpha -= (fade_length / 180);

    fill_rectangle(rgba_color(0.0, 0.0, 0.0, alpha), 0, 0, 1600, 896, option_to_screen());

    return alpha;
}

double fade_out(double alpha, double fade_length)
{
    if (alpha < 1)
        alpha += (fade_length / 150);

    fill_rectangle(rgba_color(0.0, 0.0, 0.0, alpha), 0, 0, 1600, 896, option_to_screen());

    return alpha;
}

double screen_effect(double alpha, int time_length, string timer_name, double fade_length)
{
    int time = timer_ticks(timer_name) / 1000;

    if (time < fade_length)
        alpha = fade_in(alpha, fade_length);
    else if (time > (time_length - fade_length))
        alpha = fade_out(alpha, fade_length);

    return alpha;
}

bool screen_timer(int time_length, string timer_name)
{
    if (!timer_started(timer_named(timer_name)))
        start_timer(timer_name);

    int time = timer_ticks(timer_name) / 1000;

    if (time > time_length)
    {
        stop_timer(timer_name);
        reset_timer(timer_name);
        return true;
    }
    else
        return false;
}

void draw_buttons(vector<shared_ptr<Button>> menu_buttons, int selection)
{
    for (int i = 0; i < menu_buttons.size(); i++)
    {
        if (menu_buttons[i]->get_id() == selection)
            menu_buttons[i]->set_selected(true);
        else
            menu_buttons[i]->set_selected(false);

        menu_buttons[i]->draw();
    }
}

int button_selection(int selection, int num_buttons)
{
    if (key_typed(UP_KEY) || key_typed(W_KEY))
    {
        selection -= 1;

        if (selection < 0)
            selection = 0;
    }
    if (key_typed(DOWN_KEY) || key_typed(S_KEY))
    {
        selection += 1;

        if (selection > num_buttons - 1)
            selection = num_buttons - 1;
    }

    return selection;
}

// what the menu buttons should be called
string get_button_text(int id)
{
    string text = "";

    switch (id)
    {
    case 1:
        text = "PLAY";
        break;
    case 2:
        text = "EXIT";
        break;
    default:
        break;
    }

    return text;
}

void MenuScreen::update()
{
    set_camera_x(0);
    set_camera_y(0);

    if (!run_once)
    {
        // stop_music();
        for (int i = 0; i < num_buttons; i++)
        {
            string text = get_button_text(i + 1);
            shared_ptr<Button> test(new Button(bitmap_named("Button"), offset, i, text));
            offset += 100;
            menu_buttons.push_back(test);
        }
        run_once = true;
    }

    if (!music_playing())
    {
        play_music("MenuMusic.mp3");
        set_music_volume(0.2f);
    }

    point_2d pt = screen_center();
    rectangle rc = screen_rectangle();

    clear_screen(COLOR_BLACK);
    draw_bitmap("MenuBg", 0, 0, option_to_screen());

    string audio_text = "Music by Charlie Axl Tebbutt";
    font screen_font = font_named("DefaultFont");
    int font_size = 10;

    // draw the audio text small and down the bottom, as he didn't contribute much lol
    draw_text(audio_text, COLOR_WHITE, screen_font, font_size,
              rc.width - text_width(audio_text, screen_font, font_size) - 15, // buffer of 20, position to the right side of the screen
              (pt.y - text_height(audio_text, screen_font, font_size) / 2) + 300 + text_height(audio_text, screen_font, font_size) * 2,
              // dont ask me how the above line works, i bsed it until i got a good position ahahah
              option_to_screen());

    bitmap title = bitmap_named("Title");
    drawing_options scale = option_scale_bmp(2, 2);
    draw_bitmap(title, pt.x - bitmap_width(title) / 2, 100, scale);

    draw_buttons(menu_buttons, selection);
    selection = button_selection(selection, num_buttons);

    if (key_typed(RETURN_KEY) || key_typed(screen->input_key))
    {
        switch (selection)
        {
        case 0:
        {
            play_sound_effect("Select");
            stop_music();

            // from here, start the game
            this->screen->change_state(new MenuComplete(), "MenuComplete");
        }
        break;
        case 1:
        {
            exit(0);
        }
        break;
        default:
            break;
        }
    }
}

void CompanyIntroScreen::update()
{
    point_2d pt = screen_center();
    clear_screen(COLOR_BLACK);

    bitmap title = bitmap_named("Company1");
    bitmap title2 = bitmap_named("Company2");
    font screen_font = font_named("DefaultFont");
    int font_size = 80;
    color font_color = COLOR_WHITE;
    string text = "Thoth Tech presents";

    draw_bitmap(title2, pt.x - bitmap_width(title2) / 2 + 5, pt.y - bitmap_height(title2) / 2 - 5, option_to_screen());
    draw_bitmap(title, pt.x - bitmap_width(title) / 2, pt.y - bitmap_height(title) / 2, option_to_screen());

    draw_text(text, COLOR_BROWN, screen_font, font_size, pt.x - text_width(text, screen_font, font_size) / 2 + 5, (pt.y - text_height(text, screen_font, font_size) / 2) + 200 - 5, option_to_screen());
    draw_text(text, font_color, screen_font, font_size, pt.x - text_width(text, screen_font, font_size) / 2, (pt.y - text_height(text, screen_font, font_size) / 2) + 200, option_to_screen());

    bool time_up = screen_timer(screen_time, "ScreenTimer");

    alpha = screen_effect(alpha, screen_time, "ScreenTimer", 2);

    if (time_up)
        this->screen->change_state(new TeamIntroScreen, "TeamIntro");

    if (key_typed(RETURN_KEY) || key_typed(screen->input_key))
    {
        stop_timer("ScreenTimer");
        reset_timer("ScreenTimer");
        this->screen->change_state(new TeamIntroScreen, "TeamIntro");
    }
}

void TeamIntroScreen::update()
{
    point_2d pt = screen_center();
    clear_screen(COLOR_BLACK);

    bitmap logo = bitmap_named("TeamLogo");
    font screen_font = font_named("DefaultFont");
    int font_size = 30;
    color font_color = COLOR_WHITE;
    string text = "Nathan Love";
    string text2 = "Natasha Jiang";
    string text3 = "Qanita Nadhirazka";

    draw_bitmap(logo, pt.x - bitmap_width(logo) / 2, pt.y - bitmap_height(logo) / 2 - 150, option_to_screen());

    draw_text(text, font_color, screen_font, font_size, pt.x - text_width(text, screen_font, font_size) / 2, (pt.y - text_height(text, screen_font, font_size) / 2) + 150, option_to_screen());
    draw_text(text2, font_color, screen_font, font_size, pt.x - text_width(text2, screen_font, font_size) / 2, (pt.y - text_height(text2, screen_font, font_size) / 2) + 150 + text_height(text2, screen_font, font_size) * 1, option_to_screen());
    draw_text(text3, font_color, screen_font, font_size, pt.x - text_width(text3, screen_font, font_size) / 2, (pt.y - text_height(text3, screen_font, font_size) / 2) + 150 + text_height(text3, screen_font, font_size) * 2, option_to_screen());

    bool time_up = screen_timer(screen_time, "ScreenTimer");

    alpha = screen_effect(alpha, screen_time, "ScreenTimer", 2);

    if (time_up)
        this->screen->change_state(new MenuScreen, "Menu");

    if (key_typed(RETURN_KEY) || key_typed(screen->input_key))
    {
        stop_timer("ScreenTimer");
        reset_timer("ScreenTimer");
        this->screen->change_state(new MenuScreen, "Menu");
    }
}