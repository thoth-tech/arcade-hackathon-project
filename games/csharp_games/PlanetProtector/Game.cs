using SplashKitSDK;
using System.Collections.Generic;
using System.Linq;
using System.Security.Cryptography.X509Certificates;

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
                _player.Sprite.Y = (_gameWindow.Height - _player.Sprite.Height) - 100;
            }
        }

        // Draw the Heads Up Display
        private void _DrawHud(double currentTime)
        {

            SplashKit.FillRectangle(SplashKit.RGBAColor(0, 0, 0, 0), 0, 0, 800, 50);

            // Calculate and draw score
            double score = ((double)_player.Score + currentTime); // calculate
            string scoreString = score.ToString(); // convert to string
            // if there is more than one decimal place, remove any after the first
            if (scoreString.IndexOf('.') != -1)
            {
                scoreString = scoreString.Substring(0, scoreString.IndexOf('.') + 2);
            }

            int[] scorePosition = [_gameWindow.Width / 2 - 70, 8];
            _gameWindow.DrawText($"SCORE: {scoreString}", Color.White, "VT323", 30, scorePosition[0], scorePosition[1]);

            // draw health
            Bitmap fullHeart = SplashKit.BitmapNamed("heart_full");
            Bitmap emptyHeart = SplashKit.BitmapNamed("heart_empty");
            int middle = _gameWindow.Width / 2 - 25;
            int space = 60;
            int[] healthX = [middle - space * 2, middle - space, middle, middle + space, middle + space * 2];
            int healthY = 50;
            bool[] health = _player.Health;
            for (int i = 0; i < 5; i++)
            {
                if (health[i])
                {
                    _gameWindow.DrawBitmap(fullHeart, healthX[i], healthY);
                }
                else
                {
                    _gameWindow.DrawBitmap(emptyHeart, healthX[i], healthY);
                }
            }
        }

        // Check for player collision with asteroid
        private void _CheckCollisions() // this also updates the score when a asteroid is
        {
            if (!_gameOver)
            {
                foreach (Asteroid asteroid in _asteroids.ToList())
                {
                    // check player-asteroid collisions
                    if (
                        SplashKit.SpriteCollision(_player.Sprite, asteroid.Sprite)
                        && !asteroid.HitPlayer
                    )
                    {
                        asteroid.HitsPlayer();
                        _player.ReduceHealth();
                        if (!_player.IsAlive())
                        {
                            _gameOver = true;
                        }
                        else
                        {
                            SplashKit.FreeSprite(asteroid.Sprite);
                            _asteroids.Remove(asteroid);
                        }
                        // player.DeductHealth(10); // method to be added to player class

                        // return;
                    }
                    
                    // need to call _player.DestroyAsteroid within here to increase score
                    // Check asteroid-bullet collisions
                    foreach (Bullet bullet in _player.Bullets.ToList())
                    {
                        if (SplashKit.SpriteCollision(bullet.Sprite, asteroid.Sprite))
                        {
                            _player.DestroyAsteroid();  // increase the score
                            _asteroids.Remove(asteroid); // remove the asteroid
                            _player.Bullets.Remove(bullet); // remove the bullet
                            SplashKit.FreeSprite(asteroid.Sprite); // free asteroid sprite memory
                            SplashKit.FreeSprite(bullet.Sprite); // free bullet sprite memory
                            break;
                        }
                    }
                }
            }
        }

        // Spawn asteroids
        private void _SpawnAsteroids()
        {
            /* 
                want to adjust this so it spawns more randomly. 
                e.g. after 1s it's possible to spawn again, but the likelihood gets increased until it's certain at about 5s
             */
            if (_asteroidTimer.Ticks > 1000 || _asteroids.Count == 0) // spawn an asteroid every 10 seconds or if there are no asteroids   
            {
                _asteroidTimer.Reset();
                // not sure why but _gameWindow.Width is 800, which should be the entire width of the screen but is only half
                int newAsteroidX = SplashKit.Rnd(_gameWindow.Width * 2);
                _asteroids.Add(new Asteroid(newAsteroidX, -10));
            }
        }

        // Delete asteroids
        private void _DeleteAsteroids()
        {
            foreach (Asteroid asteroid in _asteroids.ToList())
            {
                if (asteroid.Sprite.Y > _gameWindow.Height)
                {
                    _asteroids.Remove(asteroid);
                    SplashKit.FreeSprite(asteroid.Sprite);
                }
            }
        }

        /**
        * -----------------------
        * Public Methods
        * -----------------------
        */

        public void HandleInput()
        {
            if (!_gameOver)
            {
                _player.HandleInput();
            }
        }

        // Draw the game
        public void DrawGame()
        {
            if (!_gameOver)
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

                // current game time in X.x seconds
                double currentTime = _gameTimer.Ticks / 1000.0;
                _DrawHud(currentTime);
            }

            else
            {
                _gameWindow.DrawText("GAME OVER!", Color.White, 370, 350, SplashKit.OptionToScreen());
            }

            // Refresh screen
            _gameWindow.Refresh(60);
        }

        // Update the game data
        public void UpdateGame()
        {
            if (!_gameOver)
            {
                _player.Update(_gameWindow);

                foreach (Asteroid asteroid in _asteroids)
                {
                    asteroid.Update();
                }

                _SpawnAsteroids();

                _CheckCollisions();

                _DeleteAsteroids();

                if (_gameTimer.Ticks > LEVEL_TIME)
                {
                    // _gameOver = true;  // for now, never game over
                    _gameTimer.Stop();
                }
            }

        }
    }
}