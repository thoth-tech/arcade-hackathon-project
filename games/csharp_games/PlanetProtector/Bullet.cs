using SplashKitSDK;

namespace PlanetProtector
{
    public class Bullet
    {
        // Fields
        private Sprite _bulletSprite;

        // Constructor
        public Bullet(double x, double y)
        {
            // Load the bullet bitmap and create the sprite
            _bulletSprite = SplashKit.CreateSprite(SplashKit.BitmapNamed("bullet"));
            _bulletSprite.X = (float)x;
            _bulletSprite.Y = (float)y;

            _bulletSprite.Rotation = 270;
        }

        // Read-only property to access the bullet sprite
        public Sprite Sprite
        {
            get
            {
                return _bulletSprite;
            }
        }

        // Update the bullet's position
        public void Update()
        {
            _bulletSprite.Y -= 10; // Move bullet up the screen
        }

        // Draw the bullet
        public void Draw()
        {
            _bulletSprite.Draw();
        }

        // Check if the bullet is off-screen
        public bool IsOffScreen(Window gameWindow)
        {
            return _bulletSprite.Y < -_bulletSprite.Height;
        }
    }
}
