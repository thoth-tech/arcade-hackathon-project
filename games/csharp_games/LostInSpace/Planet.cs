using SplashKitSDK;

namespace LostInSpace
{
    // Planet kind enum
    enum PlanetKind
    {
        MERCURY,
        VENUS,
        EARTH,
        MARS,
        JUPITER,
        SATURN,
        URANUS,
        NEPTUNE,
        PLUTO
    }

    public class Planet
    {
        // Constants
        const float PLANET_SPEED = 0.1f;
        const float PLANET_ROTATE = 0.05f;

        // Fields
        private Sprite _planetSprite;
        private bool _visited;

        // Constructor
        public Planet(int x, int y)
        {
            //Set visited property to false, by default
            _visited = false;

            //Randomly set a new planets sprite
            _planetSprite = new Sprite(_PlanetBitmap((PlanetKind)SplashKit.Rnd(9)));

            //Set planet x and y
            _planetSprite.X = (x - _planetSprite.Width) / 2;
            _planetSprite.Y = (y - _planetSprite.Height) / 2;

            //Set planet velocity and rotation
            _planetSprite.Dx = PLANET_SPEED;
            _planetSprite.Rotation = PLANET_ROTATE;
        }

        // Read-only property to return the planet sprite
        public Sprite Sprite
        {
            get { return _planetSprite; }
        }

        // Property for whether the planet has been visited or not
        public bool Visited
        {
            get { return _visited; }
            set { _visited = value; }
        }

        /**
        * -----------------------
        * Public Methods
        * -----------------------
        */

        // Draw the planet sprite and a circle around if visited
        public void Draw()
        {
            _planetSprite.Draw();

            if (_visited)
            {
                Circle c = _planetSprite.Circle();
                c.Radius += 10;
                SplashKit.DrawCircle(Color.BrightGreen, c);
            }
        }

        // Update the planet sprite
        public void Update()
        {
            _planetSprite.Update();
        }

        /**
        * -----------------------
        * Private Methods
        * -----------------------
        */

        // Return Bitmap from Planet kind
        private Bitmap _PlanetBitmap(PlanetKind kind)
        {
            switch (kind)
            {
                case PlanetKind.MERCURY:
                    return SplashKit.BitmapNamed("mercury");
                case PlanetKind.VENUS:
                    return SplashKit.BitmapNamed("venus");
                case PlanetKind.EARTH:
                    return SplashKit.BitmapNamed("earth");
                case PlanetKind.MARS:
                    return SplashKit.BitmapNamed("mars");
                case PlanetKind.JUPITER:
                    return SplashKit.BitmapNamed("jupiter");
                case PlanetKind.SATURN:
                    return SplashKit.BitmapNamed("saturn");
                case PlanetKind.URANUS:
                    return SplashKit.BitmapNamed("uranus");
                case PlanetKind.NEPTUNE:
                    return SplashKit.BitmapNamed("neptune");
                case PlanetKind.PLUTO:
                    return SplashKit.BitmapNamed("pluto");
                default:
                    return SplashKit.BitmapNamed("mercury");
            }
        }
    }
}