using SplashKitSDK;
using System.Collections.Generic;

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
        const float PLAYER_SPEED = 1.5f;
        const int PLAYER_ROTATE_SPEED = 3;
        const int SCREEN_BORDER = 100;

        // Fields
        private Sprite _playerSprite;
        private ShipKind _kind;

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
        }

        // Read-only property to return the player sprite
        public Sprite Sprite
        {
            get
            {
                return _playerSprite;
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

            // Handle movement - rotating left/right and moving forward/back
            float dx = _playerSprite.Dx;
            float rotation = _playerSprite.Rotation;

            // Allow rotation with left/right keys
            if (SplashKit.KeyDown(KeyCode.LeftKey))
                _playerSprite.Rotation = rotation - PLAYER_ROTATE_SPEED;
            if (SplashKit.KeyDown(KeyCode.RightKey))
                _playerSprite.Rotation = rotation + PLAYER_ROTATE_SPEED;

            // Increase speed with up/down keys - typed to give step increases
            if (SplashKit.KeyTyped(KeyCode.DownKey))
                _playerSprite.Dx = dx - PLAYER_SPEED;
            if (SplashKit.KeyTyped(KeyCode.UpKey))
                _playerSprite.Dx = dx + PLAYER_SPEED;
        }

        // Draw the player sprite
        public void Draw()
        {
            _playerSprite.Draw();
        }

        // Update the player sprite and camera movement
        public void Update(Window gameWindow)
        {
            // Apply movement based on rotation and velocity in the sprite
            _playerSprite.Update();

            // Test edge of screen boundaries to adjust the camera
            double left_edge = SplashKit.CameraX() + SCREEN_BORDER;
            double right_edge = left_edge + gameWindow.Width - 2 * SCREEN_BORDER;
            double top_edge = SplashKit.CameraY() + SCREEN_BORDER;
            double bottom_edge = top_edge + gameWindow.Height - 2 * SCREEN_BORDER;

            // Get the center of the player
            Point2D sprite_center = _playerSprite.CenterPoint;

            // Test if the player is outside the area and move the camera
            // the player will appear to stay still and everything else
            // will appear to move :)

            // Test top/bottom of screen
            if (sprite_center.Y < top_edge)
            {
                SplashKit.MoveCameraBy(0, sprite_center.Y - top_edge);
            }
            else if (sprite_center.Y > bottom_edge)
            {
                SplashKit.MoveCameraBy(0, sprite_center.Y - bottom_edge);
            }

            // Test left/right of screen
            if (sprite_center.X < left_edge)
            {
                SplashKit.MoveCameraBy(sprite_center.X - left_edge, 0);
            }
            else if (sprite_center.X > right_edge)
            {
                SplashKit.MoveCameraBy(sprite_center.X - right_edge, 0);
            }
        }

        // Calculate the distance to asteroid
        public float DistanceToAsteroid(Asteroid asteroid)
        {
            // Returns distance between two points
            return SplashKit.PointPointDistance(_playerSprite.CenterPoint, asteroid.Sprite.CenterPoint);
        }

        // Return the closest asteroid
        public Asteroid ClosestAsteroid(List<Asteroid> asteroids)
        {
            Asteroid result = null;

            double closest_distance = 0;
            double asteroid_distance;

            foreach (Asteroid asteroid in asteroids)
            {
                if (!asteroid.Visited)
                {
                    asteroid_distance = DistanceToAsteroid(asteroid);
                    if (result == null || asteroid_distance < closest_distance)
                    {
                        closest_distance = asteroid_distance;
                        result = asteroid;
                    }
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
    }
}