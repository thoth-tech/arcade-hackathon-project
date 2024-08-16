using SplashKitSDK;

namespace PlanetProtector
{
    // Asteroid kind enum
    enum AsteroidKind
    {
        MERCURY,
        VENUS,
        MARS,
        JUPITER,
        NEPTUNE,
        PLUTO
    }

    public class Asteroid
    {
        // CONSTANTS
        const float ASTEROID_SPEED = 3.5f; // should be 0.5f for final game
        
        // int ASTEROID_KIND_COUNT = 6; // number of entries in the AsteroidKind enum

        // need to somehow pass this into the drawing of the sprite
        // can use public void Bitmap.DrawBitmap(double x, double y, DrawingOptions opts);
        // but that is drawing the bitmap, not drawing the sprite
        // DrawingOptions asteroidOptions = SplashKit.OptionRotateBmp(10);

        // FIELDS
        private Sprite _asteroidSprite;
        private bool hitPlayer = false;


        // CONSTRUCTOR
        public Asteroid(int x, int y)
        {
            // Randomly set a new asteroids sprite
            // _asteroidSprite = new Sprite(_AsteroidBitmap((AsteroidKind)SplashKit.Rnd(ASTEROID_KIND_COUNT)));
            _asteroidSprite = new Sprite(SplashKit.BitmapNamed("asteroid"));

            //Set asteroid x and y
            _asteroidSprite.X = (x - _asteroidSprite.Width) / 2;
            _asteroidSprite.Y = (y - _asteroidSprite.Height) / 2;

            //Set asteroid velocity and rotation
            _asteroidSprite.Dy = ASTEROID_SPEED;
        }

        // ACCESSORS
        // Read-only property to return the asteroid sprite
        public Sprite Sprite
        {
            get { return _asteroidSprite; }
        }

        public bool HitPlayer
        {
            get { return hitPlayer; }
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

        // Asteroid has hit player
        public void HitsPlayer()
        {
            hitPlayer = true;
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