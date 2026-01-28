namespace NeonShooter;

static class GameRoot
{
	public static Raylib.Vector2   	Size				{ get; private set; }
	public static Raylib.Rectangle 	Viewport 			{ get; private set; }
	

	public static WarpGrid 			WarpGrid 			{ get; private set; }
	public static ParticleManager 	ParticleManager 	{ get; private set; }


	public static void Init()
	{
		ParticleManager = new ParticleManager(1024 * 20, => ParticleState.UpdateParticle);

		WarpGrid = new WarpGrid(.(Raylib.GetScreenWidth() * -0.5f, Raylib.GetScreenHeight() * -0.5f, Raylib.GetScreenWidth() * 2.0f, Raylib.GetScreenHeight() * 2.0f), .(20.0f));
	}


	public static void Deinit()
	{
		DeleteAndNullify!(WarpGrid);
		DeleteAndNullify!(ParticleManager);
	}


	public static void Update()
	{
		Size = .(Raylib.GetScreenWidth(), Raylib.GetScreenHeight());
		Viewport = .(.Zero, Size);

		ParticleManager.Update();
		WarpGrid.Update();
	}


	public static void Draw()
	{
		ParticleManager.Draw();
	}
}