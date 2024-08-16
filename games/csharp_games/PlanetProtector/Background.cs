using SplashKitSDK;

namespace PlanetProtector
{
    class Background
    {
        // Fields
        private Bitmap _front;
        private Bitmap _rear;
        private double _frontY;
        private double _rearY;
        private double _speed;

        // Constructor
        public Background()
        {
            _front = SplashKit.BitmapNamed("background_front");
            _rear = SplashKit.BitmapNamed("background_rear");

            _frontY = 0;
            _rearY = 0;

            _speed =0.8;
        }

        // Update the background
        public void Update()
        {
            _frontY += _speed * 2;
            _rearY += _speed;

            if (_frontY >= _front.Height)
            {
                _frontY = 0;
            }

            if (_rearY >= _rear.Height)
            {
                _rearY = 0;
            }
        }

        // Draw the background
        public void Draw()
        {
            SplashKit.DrawBitmap(_rear, 0, _rearY);
            SplashKit.DrawBitmap(_rear, 0, _rearY - _rear.Height);

            SplashKit.DrawBitmap(_front, 0, _frontY);
            SplashKit.DrawBitmap(_front, 0, _frontY - _front.Height);
        }
    }
}