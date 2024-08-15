using SplashKitSDK;
using System.Collections.Generic;
using System.Linq;

namespace PlanetProtector
{
    // Ship kind enum
    public enum ShipKind
    {
        AQUARII,
        GLIESE,
        PEGASI
    }

    public class Player
    {
        // Constants
        const float PLAYER_SPEED = 9.5f;

        // Fields
        private Sprite _playerSprite;
        private ShipKind _kind;
        private List<Bullet> _bullets;
        private int _score;
        private bool[] _health;

        // Constructor
        public Player(Window gameWindow)
        {
            // Default Ship kind
            _kind = ShipKind.AQUARII;

            // Create the sprite with 3 layers - we can turn on and off based
            // on the ship kind selected
            _playerSprite = SplashKit.CreateSprite(_ShipBitmap(_kind));
            _playerSprite.AddLayer(_ShipBitmap(ShipKind.GLIESE), "GLIESE");
            _playerSprite.AddLayer(_ShipBitmap(ShipKind.PEGASI), "PEGASI");

            // Default to layer 0 = Aquarii so hide others
            _playerSprite.HideLayer(1);
            _playerSprite.HideLayer(2);
            _playerSprite.Rotation = 270;
            _bullets = new List<Bullet>();

            _health = [true, true, true, false, false]; // set to all true for build, falses are for testing
        }

        // Read-only property to return the player sprite
        public Sprite Sprite
        {
            get
            {
                return _playerSprite;
            }
        }

        public int Score
        {
            get
            {
                return _score;
            }
        }

        public bool[] Health
        {
            get
            {
                return _health;
            }
        }

        /**
        * -----------------------
        * Public Methods
        * -----------------------
        */

        // Handle the user inputs for the player movement
        public void HandleInput()
        {
            // Allow the player to switch ships
            if (SplashKit.KeyTyped(KeyCode.Num1Key))
                _SwitchShip(ShipKind.AQUARII);
            if (SplashKit.KeyTyped(KeyCode.Num2Key))
                _SwitchShip(ShipKind.GLIESE);
            if (SplashKit.KeyTyped(KeyCode.Num3Key))
                _SwitchShip(ShipKind.PEGASI);

            // Handle movement - moving left/right 
            float dx = _playerSprite.X;

            // Allow for movement on the x axis with border restrictions
            if (
                (SplashKit.KeyDown(KeyCode.LeftKey) || SplashKit.KeyDown(KeyCode.AKey))
                && _playerSprite.X > -10
            )
            {
                _playerSprite.X = dx - PLAYER_SPEED; // move left
            }

            if (
                (SplashKit.KeyDown(KeyCode.RightKey) || SplashKit.KeyDown(KeyCode.DKey))
                && _playerSprite.X < 740
            )
            {
                _playerSprite.X = dx + PLAYER_SPEED; // move right
            }
            
            if (SplashKit.KeyTyped(KeyCode.SpaceKey))
            {
                Shoot();
            }

        }

        // Draw the player sprite
        public void Draw()
        {
            _playerSprite.Draw();
            foreach (Bullet bullet in _bullets)
            {
                bullet.Draw();
            }
        }

        // Update the player sprite and camera movement
        public void Update(Window gameWindow)
        {
            // Apply movement based on rotation and velocity in the sprite
            _playerSprite.Update();
            // Update bullets and remove any that are off-screen
            _bullets = _bullets.Where(bullet => !bullet.IsOffScreen(gameWindow)).ToList();
            foreach (Bullet bullet in _bullets)
            {
                bullet.Update();
            }
        }

        // Calculate the distance to asteroid
        public float DistanceToAsteroid(Asteroid asteroid)
        {
            if (asteroid == null) // at start of game there are no asteroids
                return 0;

            // Returns distance between two points
            return SplashKit.PointPointDistance(_playerSprite.CenterPoint, asteroid.Sprite.CenterPoint);
        }

        public void DestroyAsteroid()
        {
            _score += 10;
        }

        // Return the closest asteroid
        public Asteroid ClosestAsteroid(List<Asteroid> asteroids)
        {
            Asteroid result = null;

            double closest_distance = 0;
            double asteroid_distance;

            foreach (Asteroid asteroid in asteroids)
            {
                asteroid_distance = DistanceToAsteroid(asteroid);
                if (result == null || asteroid_distance < closest_distance)
                {
                    closest_distance = asteroid_distance;
                    result = asteroid;
                }
            }

            return result;
        }

        /**
        * -----------------------
        * Private Methods
        * -----------------------
        */

        // Return Bitmap from Ship kind
        private Bitmap _ShipBitmap(ShipKind kind)
        {
            switch (kind)
            {
                case ShipKind.GLIESE:
                    return SplashKit.BitmapNamed("gliese");
                case ShipKind.PEGASI:
                    return SplashKit.BitmapNamed("pegasi");
                case ShipKind.AQUARII:
                default:
                    return SplashKit.BitmapNamed("aquarii");
            }
        }

        // Switch the player's sprite layer based on the ShipKind
        private void _SwitchShip(ShipKind target)
        {
            // only do this if there is a change
            if (_kind != target)
            {
                // show then hide layers
                _playerSprite.ShowLayer((int)(target));
                _playerSprite.HideLayer((int)(_kind));

                // remember what is currently shown
                _kind = target;
            }
        }

        private void Shoot()
        {
            // Create a new bullet and add it to the list
            Bullet bullet = new Bullet(_playerSprite.X + _playerSprite.Width / 2, _playerSprite.Y);
            _bullets.Add(bullet);
        }
    }
}