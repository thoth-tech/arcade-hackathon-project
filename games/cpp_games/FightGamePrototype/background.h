#include "splashkit.h"
#include <iostream>
using namespace std;

class Background
{
    protected:
        string bundlename;
        int layers;
    
    public:
        Background(){};
        virtual ~Background()
        {
            if(has_resource_bundle(bundlename))
            {
                free_resource_bundle(bundlename);
                cout << "Background Resources Free: " << bundlename << endl; 
            }
        };

        virtual void draw_background()
        {
            if(layers == 3)
                draw_3_layers();
        };

        virtual void draw_3_layers()
        {
            point_2d cam = camera_position();
            draw_bitmap("farground",(cam.x - 100) - (0.5*(cam.x)),100,option_scale_bmp(1,1));
            draw_bitmap("background",(cam.x - 100) - (0.8*(cam.x)),0,option_scale_bmp(1,0.8));
            draw_bitmap("foreground",(cam.x + 400) - (1.6*(cam.x)),0,option_scale_bmp(1.5,1));
        };
};

class CityBackground : public Background
{
    public:
        CityBackground()
        {
            this->layers = 3;
            this->bundlename = "citybackground";
            if(!has_resource_bundle(bundlename))
            {
                load_resource_bundle(bundlename, "background_city.txt");
                cout << "Background Loaded: " << bundlename << endl; 
            }
            cout << "Background Resources Loaded: " << bundlename << endl;
        };
};