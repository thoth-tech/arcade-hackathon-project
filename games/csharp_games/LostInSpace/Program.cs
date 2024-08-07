﻿using SplashKitSDK;

namespace LostInSpace
{
    public class Program
    {
        public static void Main(string[] args)
        {
            // Load the game images, sounds, etc.
            SplashKit.LoadResourceBundle("game_bundle", "lost_in_space.txt");

            Window window = new Window("Lost In Space", 800, 700);

            // Create new Game object
            Game game = new Game(window);

            while (!SplashKit.QuitRequested())
            {
                // Handle input to adjust player movement
                SplashKit.ProcessEvents();
                game.HandleInput();

                // Perform movement and update the camera
                game.UpdateGame();

                // Draw everything
                game.DrawGame();

            }

            // Free memory
            SplashKit.FreeAllSprites();
            SplashKit.CloseAllWindows();
        }
    }
}
