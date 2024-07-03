namespace NeonShooter.Entity;

using System;
using Raylib;

public class PlayerShip : Entity
{
	private static Self sInstance;
	public static Self Instance
	{
		get
		{
			if (sInstance == null)
			{
				sInstance = new Self();
			}

			return sInstance;
		}
	}

	public static void DestroyInstance()
	{
		EntityManager.RemoveEntity(sInstance);
		DeleteAndNullify!(sInstance);
	}

	const int CooldownFrames = 6;

	private int cooldownRemaining = 0;
	private int framesUntilRespawn = 0;


	public bool isDead => framesUntilRespawn > 0;


	private this()
	{
		image = Art.Player;
		position = .(GetScreenWidth() * 0.5f, GetScreenHeight() * 0.5f);
		radius = 10.0f;
	}

	public override void Update(float dt)
	{
		if (isDead)
		{
			framesUntilRespawn--;
			if (framesUntilRespawn == 0)
			{
				GameRoot.WarpGrid.ApplyDirectedForce(Vector3(0, 0, 5000), .(position.x, position.y, 0.0f), 50);
			}

			return;
		}

		const float speed = 8.0f;
		velocity = Vector2.Lerp(velocity, Input.GetMoveDirection() * speed, 15.0f * dt);
		position += velocity;
		position = Vector2Clamp(position, size * 0.5f, .(GetScreenWidth(), GetScreenHeight()) - size * 0.5f);

		if (velocity.lengthSqr > 0)
		{
			orientation = velocity.ToAngle();
		}

		let aim = Input.GetAimDirection();
		if (aim.lengthSqr > 0 && cooldownRemaining <= 0)
		{
			cooldownRemaining = CooldownFrames;

			let aimAngle = aim.ToAngle();

			let randomSpeed = random.NextFloat(-0.04f, 0.04f) + random.NextFloat(-0.04f, 0.04f);
			let velocity = Vector2.FromAngle(aimAngle + randomSpeed, 11.0f);

			let offset0 = Vector2.Rotate(.(25, -8), aimAngle * RAD2DEG);
			EntityManager.AddEntity(new Bullet(position + offset0, velocity));

			let offset1 = Vector2.Rotate(.(25,  8), aimAngle * RAD2DEG);
			EntityManager.AddEntity(new Bullet(position + offset1, velocity));

			Audios.PlayShot();
		}
		else
		{
			cooldownRemaining--;
		}

		MakeExhaustFire();
	}

	public override void Draw()
	{
		if (!isDead)
		{
			base.Draw();
		}
	}

	public void Kill()
	{
		Audios.PlayExplosion();
		PlayerStatus.RemoveLife();
		framesUntilRespawn = 180;

		Color yellow = Color(0.8f, 0.8f, 0.4f);
		for (int i = 0; i < 1200; i++)
		{
			float speed = 18f * (1f - 1 / random.NextFloat(1f, 10f));
			Color color = Color.Lerp(.WHITE, yellow, random.NextFloat(0, 1));

			var state = ParticleState()
			{
				velocity = random.NextVector2(speed, speed),
				type = ParticleType.None,
				lengthMultiplier = 1
			};

			GameRoot.ParticleManager.CreateParticle(Art.LineParticle, position, color, 190, .(1.5f), state);

		}

		position = .(GetScreenWidth() * 0.5f, GetScreenHeight() * 0.5f);

		GameRoot.WarpGrid.ApplyDirectedForce(Vector3(0, 0, 5000), .(position.x, position.y, 0.0f), 50);
	}

	private void MakeExhaustFire()
	{
		if (velocity.lengthSqr > 0.1f)
		{

			// set up some variables

			double t = GetTime();

			// The primary velocity of the particles is 3 pixels/frame in the direction opposite to which the ship is travelling.

			Vector2 baseVel = velocity.normalized * -3;

			// Calculate the sideways velocity for the two side streams. The direction is perpendicular to the ship's velocity and the

			// magnitude varies sinusoidally.

			Vector2 perpVel = Vector2(baseVel.y, -baseVel.x) * (0.6f * (float)Math.Sin(t * 10));

			const float alpha = 0.7f;

			Color whiteColor = Color(.WHITE, alpha);
			Color sideColor = Color(200, 38, 9, (uint8)(alpha * 255));	// deep red
			Color midColor = Color(255, 187, 30, (uint8)(alpha * 255));	// orange-yellow

			Vector2 pos = position + Vector2.Rotate(Vector2(-25, 0), velocity.angle * RAD2DEG);	// position of the ship's exhaust pipe.

			// middle particle stream

			Vector2 velMid = baseVel + random.NextVector2(0, 1);

			GameRoot.ParticleManager.CreateParticle(Art.LineParticle, pos, whiteColor, 60f, Vector2(0.5f, 1),

				ParticleState(velMid, ParticleType.Enemy));

			GameRoot.ParticleManager.CreateParticle(Art.GlowParticle, pos, midColor, 60f, Vector2(0.5f, 1),

				ParticleState(velMid, ParticleType.Enemy));

			// side particle streams

			Vector2 vel1 = baseVel + perpVel + random.NextVector2(0, 0.3f);

			Vector2 vel2 = baseVel - perpVel + random.NextVector2(0, 0.3f);

			GameRoot.ParticleManager.CreateParticle(Art.LineParticle, pos, whiteColor, 60f, Vector2(0.5f, 1),

				ParticleState(vel1, ParticleType.Enemy));

			GameRoot.ParticleManager.CreateParticle(Art.LineParticle, pos, whiteColor, 60f, Vector2(0.5f, 1),

				ParticleState(vel2, ParticleType.Enemy));

			GameRoot.ParticleManager.CreateParticle(Art.GlowParticle, pos, sideColor, 60f, Vector2(0.5f, 1),

				ParticleState(vel1, ParticleType.Enemy));

			GameRoot.ParticleManager.CreateParticle(Art.GlowParticle, pos, sideColor, 60f, Vector2(0.5f, 1),

				ParticleState(vel2, ParticleType.Enemy));

		}

	}

}