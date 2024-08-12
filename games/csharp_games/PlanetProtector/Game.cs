using SplashKitSDK;
using System.Collections.Generic;

namespace PlanetProtector
{
    public class Game
    {
        // Constants
        const double LEVEL_TIME = 12000.0;

        // Fields
        private Window _gameWindow;
        private Player _player;
        private List<Asteroid> _asteroids;
        private Timer _gameTimer;
        private bool _gameOver;
        private int _score;
        private Timer _asteroidTimer;

        // Constructor
        public Game(Window gameWindow)
        {
            // Initialise fields
            _gameWindow = gameWindow;
            _player = new Player(_gameWindow);
            _asteroids = new List<Asteroid>();
            _gameTimer = new Timer("GameTimer");
            _asteroidTimer = new Timer("AsteroidTimer");
            _gameOver = false;
            _score = 0;

            _SetupLevel(3000, 3000);

            _gameTimer.Reset();
            _gameTimer.Start();

            _asteroidTimer.Reset();
            _asteroidTimer.Start();
        }

        /**
        * -----------------------
        * Private Methods
        * -----------------------
        */

        // Set up the level data
        private void _SetupLevel(int width, int height)
        {
            if (!_gameOver)
            {
                // remove any existing asteroids if needed
                foreach (Asteroid asteroid in _asteroids)
                {
                    SplashKit.FreeSprite(asteroid.Sprite);
                }
                _asteroids.Clear();

                // Position in the centre of the initial screen
                // _player.Sprite.X = (width - _player.Sprite.Width) / 2; // Translate to world coords
                _player.Sprite.X = (_gameWindow.Width - _player.Sprite.Width) / 2;
                _player.Sprite.Y = (_gameWindow.Height  - _player.Sprite.Height) - 100;
            }
        }

        // Draw the Heads Up Display
        private void _DrawHud(Asteroid closestAsteroid, double timePercent)
        {
            int part_width = 300; // How much the bar is filled in (300 is max)

            SplashKit.FillRectangle(Color.Gray, 0, 0, 800, 50, SplashKit.OptionToScreen());

            float distance = _player.DistanceToAsteroid(closestAsteroid);

            // Draw score, location and distance to top left of the screen
            _gameWindow.DrawText($"SCORE: {_score}", Color.White, 0, 0, SplashKit.OptionToScreen());
            _gameWindow.DrawText("LOCATION: (" + ((int)_player.Sprite.CenterPoint.X).ToString() + ", " + ((int)_player.Sprite.CenterPoint.Y).ToString() + ")", Color.White, 0, 10, SplashKit.OptionToScreen());
            _gameWindow.DrawText($"DISTANCE: {distance}", Color.White, 0, 20, SplashKit.OptionToScreen());

            // Draw bar
            _gameWindow.DrawBitmap(SplashKit.BitmapNamed("empty"), 300, 0, SplashKit.OptionToScreen());
            _gameWindow.DrawBitmap(SplashKit.BitmapNamed("full"), 300, 0, SplashKit.OptionPartBmp(0, 0, part_width * timePercent, SplashKit.BitmapHeight("full"), SplashKit.OptionToScreen()));

            // Draw compass
            Vector2D direction;
            if (closestAsteroid != null)
            {
                direction = SplashKit.VectorMultiply(SplashKit.UnitVector(SplashKit.VectorFromTo(_player.Sprite, closestAsteroid.Sprite)), 15);
            }
            else
            {
                direction = new Vector2D(); // if there are no asteroids, don't draw anything
            }

            // draws compas to closest asteroid
            _gameWindow.DrawCircle(Color.White, 750, 20, 15, SplashKit.OptionToScreen());
            _gameWindow.DrawLine(Color.White, 750, 20, 750 + direction.Y, 20 + direction.Y, SplashKit.OptionToScreen());
        }

        // Check for player collision with asteroid
        private void _CheckCollisions() // this also updates the score when a asteroid is
        {
            if (!_gameOver)
            {
                foreach (Asteroid asteroid in _asteroids)
                {
                    if (SplashKit.SpriteCollision(_player.Sprite, asteroid.Sprite))
                    {
                        _score += 1000;
                        _gameTimer.Reset();
                        // return;
                    }
                }
            }
        }

        // Spawn asteroids
        private void _SpawnAsteroids()
        {
            if (_asteroidTimer.Ticks > 1000)
            {
                _asteroidTimer.Reset();
                int newAsteroidX = SplashKit.Rnd(_gameWindow.Width);
                newAsteroidX = 0;
                _asteroids.Add(new Asteroid(newAsteroidX, -100));
            }
        }

        /**
        * -----------------------
        * Public Methods
        * -----------------------
        */

        public void HandleInput()
        {
            _player.HandleInput();
        }

        // Draw the game
        public void DrawGame()
        {
            // Redraw everything
            _gameWindow.Clear(Color.Black);

            // Debug square in centre of map
            SplashKit.FillRectangle(Color.White, 400, 400, 10, 10);

            // Draw asteroids first, so player is in front
            foreach (Asteroid asteroid in _asteroids)
            {
                asteroid.Draw();
            }

            // Draw player
            _player.Draw();

            double timePercent = (LEVEL_TIME - _gameTimer.Ticks) / LEVEL_TIME;
            if (timePercent < 0)
            {
                timePercent = 0; // Ensures doesn't go into the negatives
            }

            Asteroid closestAsteroid = null;
            if(_asteroids.Count > 0) // if there are asteroids, get the closest one
            {
                closestAsteroid = _player.ClosestAsteroid(_asteroids);
            }

            _DrawHud(closestAsteroid, timePercent);

            if (_gameOver)
            {
                _gameWindow.DrawText("GAME OVER!", Color.White, 370, 350, SplashKit.OptionToScreen());
            }

            // Refresh screen
            _gameWindow.Refresh(60);
        }

        // Update the game data
        public void UpdateGame()
        {
            _player.Update(_gameWindow);

            foreach (Asteroid asteroid in _asteroids)
            {
                asteroid.Update();
            }

            _SpawnAsteroids();

            _CheckCollisions();

            if (_gameTimer.Ticks > LEVEL_TIME)
            {
                _gameOver = true;
                _gameTimer.Stop();
            }
        }
    }
}