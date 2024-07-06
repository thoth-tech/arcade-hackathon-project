using SplashKitSDK;
using System.Collections.Generic;

namespace LostInSpace
{
    public class Game
    {
        // Constants
        const double LEVEL_TIME = 12000.0;

        // Fields
        private Window _gameWindow;
        private Player _player;
        private Planet _planet;
        private List<Planet> _planets;
        private Timer _gameTimer;
        private bool _gameOver;
        private int _score;

        // Constructor
        public Game(Window gameWindow)
        {
            // Initialise fields
            _gameWindow = gameWindow;
            _player = new Player(_gameWindow);
            _planet = new Planet(0, 0);
            _planets = new List<Planet>();
            _gameTimer = new Timer("GameTimer");
            _gameOver = false;
            _score = 0;

            _SetupLevel(3000, 3000, 10);

            _gameTimer.Reset();
            _gameTimer.Start();
        }

        /**
        * -----------------------
        * Private Methods
        * -----------------------
        */

        // Set up the level data
        private void _SetupLevel(int width, int height, int numPlanets)
        {
            if (!_gameOver)
            {
                foreach (Planet planet in _planets)
                {
                    SplashKit.FreeSprite(planet.Sprite);
                }
                _planets.Clear();

                if (numPlanets <= 0)
                {
                    numPlanets = 1;
                }

                // Position in the centre of the initial screen
                // _player.Sprite.X = (width - _player.Sprite.Width) / 2; // Translate to world coords
                _player.Sprite.X = (_gameWindow.Width - _player.Sprite.Width) / 2;
                _player.Sprite.Y = (_gameWindow.Height - _player.Sprite.Height) / 2;

                // Add planets
                for (int i = 0; i < numPlanets; i++)
                {
                    _planets.Add(new Planet(SplashKit.Rnd(width), SplashKit.Rnd(height)));
                }
            }
        }

        // Draw the Heads Up Display
        private void _DrawHud(Planet closestPlanet, double timePercent)
        {
            int part_width = 300; // How much the bar is filled in (300 is max)

            SplashKit.FillRectangle(Color.Gray, 0, 0, 800, 50, SplashKit.OptionToScreen());

            float distance = _player.DistanceToPlanet(closestPlanet);

            // Draw score, location and distance to top left of the screen
            _gameWindow.DrawText($"SCORE: {_score}", Color.White, 0, 0, SplashKit.OptionToScreen());
            _gameWindow.DrawText("LOCATION: (" + ((int)_player.Sprite.CenterPoint.X).ToString() + ", " + ((int)_player.Sprite.CenterPoint.Y).ToString() + ")", Color.White, 0, 10, SplashKit.OptionToScreen());
            _gameWindow.DrawText($"DISTANCE: {distance}", Color.White, 0, 20, SplashKit.OptionToScreen());

            // Draw bar
            _gameWindow.DrawBitmap(SplashKit.BitmapNamed("empty"), 300, 0, SplashKit.OptionToScreen());
            _gameWindow.DrawBitmap(SplashKit.BitmapNamed("full"), 300, 0, SplashKit.OptionPartBmp(0, 0, part_width * timePercent, SplashKit.BitmapHeight("full"), SplashKit.OptionToScreen()));

            Vector2D direction = SplashKit.VectorMultiply(SplashKit.UnitVector(SplashKit.VectorFromTo(_player.Sprite, closestPlanet.Sprite)), 15);

            // draws compas to closest planet
            _gameWindow.DrawCircle(Color.White, 750, 20, 15, SplashKit.OptionToScreen());
            _gameWindow.DrawLine(Color.White, 750, 20, 750 + direction.Y, 20 + direction.Y, SplashKit.OptionToScreen());
        }

        // Check for player collision with planet
        private void _CheckCollisions() // this also updates the score when a planet is
        {
            if (!_gameOver)
            {
                foreach (Planet planet in _planets)
                {
                    if (!planet.Visited && SplashKit.SpriteCollision(_player.Sprite, planet.Sprite))
                    {
                        planet.Visited = true;
                        _score += 1000;
                        _gameTimer.Reset();
                        // return;
                    }
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
            _player.HandleInput();
        }

        // Draw the game
        public void DrawGame()
        {
            // Redraw everything
            _gameWindow.Clear(Color.Black);

            // Debug square in centre of map
            SplashKit.FillRectangle(Color.White, 400, 400, 10, 10);

            // Draw planets first, so player is in front
            foreach (Planet planet in _planets)
            {
                planet.Draw();
            }

            // Draw player
            _player.Draw();

            double timePercent = (LEVEL_TIME - _gameTimer.Ticks) / LEVEL_TIME;
            if (timePercent < 0)
            {
                timePercent = 0; // Ensures doesn't go into the negatives
            }

            Planet closestPlanet = _player.ClosestPlanet(_planets);
            // Are there any planets left?
            if (closestPlanet != null)
            {
                // Draw hud
                _DrawHud(closestPlanet, timePercent);
            }
            else
            {
                _DrawHud(_planets[0], timePercent);
            }

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

            foreach (Planet planet in _planets)
            {
                planet.Update();
            }

            _CheckCollisions();

            if (_gameTimer.Ticks > LEVEL_TIME)
            {
                _gameOver = true;
                _gameTimer.Stop();
            }

            // New level upon no more planets
            if (_player.ClosestPlanet(_planets) == null)
            {
                _SetupLevel(3000, 3000, _planets.Count - 1);
            }
        }
    }
}