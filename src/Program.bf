namespace NeonShooter;

using Raylib;

class Program : RaylibApp
{
	public static void Main()
	{
		Self app = scope .();

 		app.title = "NeonShooter";
		app.width = 800;
		app.height = 600;
		app.Run();
	}

	public override void Init()
	{
		base.Init();
	}

	public override void Close()
	{
		base.Close();
	}

	public override void Update(float dt)
	{

	}

	public override void Draw()
	{
		ClearBackground(.BLACK);
	}
}