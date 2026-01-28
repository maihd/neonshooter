namespace NeonShooter;

using Raylib;

public static class Art
{
	public static Texture2D Player 			{ get; private set; }
	public static Texture2D Seeker 			{ get; private set; }
	public static Texture2D Wanderer 		{ get; private set; }
	public static Texture2D Bullet 			{ get; private set; }
	public static Texture2D BlackHole		{ get; private set; }
	public static Texture2D Pointer 		{ get; private set; }
	public static Texture2D LineParticle 	{ get; private set; }
	public static Texture2D GlowParticle 	{ get; private set; }
	public static Texture2D Pixel 			{ get; private set; }

	public static Font		Font			{ get; private set; }

	public static void Load()
	{
		Player 			= Texture.Load("Assets/Art/Player.png");
		Seeker 			= Texture.Load("Assets/Art/Seeker.png");
		Wanderer 		= Texture.Load("Assets/Art/Wanderer.png");
		Bullet 			= Texture.Load("Assets/Art/Bullet.png");
		BlackHole   	= Texture.Load("Assets/Art/Black Hole.png");
		Pointer 		= Texture.Load("Assets/Art/Pointer.png");
		LineParticle 	= Texture.Load("Assets/Art/Laser.png");
		GlowParticle 	= Texture.Load("Assets/Art/Glow.png");

		Pixel			= LoadTextureFromImage(GenImageColor(1, 1, .WHITE));

		Font			= LoadFontEx("Assets/Fonts/NovaSquare-Regular.ttf", 32, null, 0);
	}

	public static void Unload()
	{
		UnloadFont(Font);
		Font = default;

		Player.Unload();
		Seeker.Unload();
		Wanderer.Unload();
		Bullet.Unload();
		BlackHole.Unload();
		Pointer.Unload();
		LineParticle.Unload();
		GlowParticle.Unload();

		Pixel.Unload();
	}
}