namespace NeonShooter.Entity;

using System;
using Raylib;

public abstract class Entity
{
	protected Texture image = default;
	protected Color color = .WHITE;

	protected append Random random = .();

	public Vector2 position;
	public Vector2 velocity;

	public float orientation;
	public float radius = 20.0f;
	public bool isExpired;

	public Vector2 size
	{
		get => image.id == 0 ? .Zero : .(image.width, image.height);
	}

	public Rectangle rect
	{
		get => .(0, 0, size.x, size.y);
	}

	public abstract void Update(float dt);

	public virtual void Draw()
	{
		DrawTexturePro(image, rect, .(position, size), size * 0.5f, orientation * RAD2DEG, color);
	}

	public void HandleCollision(Enemy other)
	{
		let d = position - other.position;
		velocity += 10.0f * d / (d.lengthSqr + 1);
	}
}