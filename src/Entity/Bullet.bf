namespace NeonShooter.Entity;

using Raylib;

class Bullet : Entity
{
	public this(Vector2 position, Vector2 velocity)
	{
		this.image = Art.Bullet;
		this.position = position;
		this.velocity = velocity;
		this.orientation = velocity.ToAngle();
	}

	public override void Update(float dt)
	{
		if (velocity.lengthSqr > 0)
		{
			orientation = velocity.ToAngle();
		}

		position += velocity;

		if (!GameRoot.Viewport.Contains(position))
		{
			isExpired = true;

			// Particles
			for (int i = 0; i < 30; i++)
			{
				GameRoot.ParticleManager.CreateParticle(Art.LineParticle, position, .SKYBLUE, 50, .(1.0f),
					ParticleState() { velocity = random.NextVector2(0, 9), type = ParticleType.Bullet, lengthMultiplier = 1 });
			}
		}

		GameRoot.WarpGrid.ApplyExplosiveForce(0.5f * velocity.length, .(position.x, position.y, 0.0f), 80);
	}
}