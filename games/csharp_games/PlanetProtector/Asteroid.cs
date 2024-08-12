using SplashKitSDK;

namespace PlanetProtector
{
    // Asteroid kind enum
    enum AsteroidKind
    {
        MERCURY,
        VENUS,
        EARTH,
        MARS,
        JUPITER,
        NEPTUNE,
        PLUTO
    }

    public class Asteroid
    {
        // Constants
        const float ASTEROID_SPEED = 0.1f;
        const float ASTEROID_ROTATE = 0.05f;

        // Fields
        private Sprite _asteroidSprite;

        // Constructor
        public Asteroid(int x, int y)
        {
            //Randomly set a new asteroids sprite
            _asteroidSprite = new Sprite(_AsteroidBitmap((AsteroidKind)SplashKit.Rnd(9)));

            //Set asteroid x and y
            _asteroidSprite.X = (x - _asteroidSprite.Width) / 2;
            _asteroidSprite.Y = (y - _asteroidSprite.Height) / 2;

            //Set asteroid velocity and rotation
            _asteroidSprite.Dx = ASTEROID_SPEED;
            _asteroidSprite.Rotation = ASTEROID_ROTATE;
        }

        // Read-only property to return the asteroid sprite
        public Sprite Sprite
        {
            get { return _asteroidSprite; }
        }

        /**
        * -----------------------
        * Public Methods
        * -----------------------
        */

        // Draw the asteroid sprite
        public void Draw()
        {
            _asteroidSprite.Draw();
        }

        // Update the asteroid sprite
        public void Update()
        {
            _asteroidSprite.Update();
        }

        /**
        * -----------------------
        * Private Methods
        * -----------------------
        */

        // Return Bitmap from Asteroid kind
        private Bitmap _AsteroidBitmap(AsteroidKind kind)
        {
            switch (kind)
            {
                case AsteroidKind.MERCURY:
                    return SplashKit.BitmapNamed("mercury");
                case AsteroidKind.VENUS:
                    return SplashKit.BitmapNamed("venus");
                case AsteroidKind.EARTH:
                    return SplashKit.BitmapNamed("earth");
                case AsteroidKind.MARS:
                    return SplashKit.BitmapNamed("mars");
                case AsteroidKind.JUPITER:
                    return SplashKit.BitmapNamed("jupiter");
                case AsteroidKind.NEPTUNE:
                    return SplashKit.BitmapNamed("neptune");
                case AsteroidKind.PLUTO:
                    return SplashKit.BitmapNamed("pluto");
                default:
                    return SplashKit.BitmapNamed("mercury");
            }
        }
    }
}