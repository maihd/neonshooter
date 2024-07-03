namespace NeonShooter.Entity;

using Raylib;
using System;
using System.Collections;

class BlackHole : Entity
{
	private int hitpoints = 10;
	private float sprayAngle = 0;


	public this(Vector2 position)
	{
		this.image = Art.BlackHole;
		this.position = position;
		this.radius = image.width * 0.5f;
	}

	public void WasShot()
	{
		hitpoints--;
		if (hitpoints <= 0)
		{
			isExpired = true;
		}

		float hue = (float)((3 * GetTime()) % 6);
		Color color = Color.HSV(hue, 0.25f, 1);

		const int numParticles = 150;
		float startOffset = random.NextFloat(0, 2.0f * Math.PI_f / numParticles);

		for (int i = 0; i < numParticles; i++)
		{
			Vector2 sprayVel = .FromAngle(2.0f * Math.PI_f * i / numParticles + startOffset, random.NextFloat(8, 16));
			Vector2 pos = position + 2f * sprayVel;

			var state = ParticleState()
			{
				velocity = sprayVel,
				lengthMultiplier = 1,
				type = ParticleType.IgnoreGravity
			};

			GameRoot.ParticleManager.CreateParticle(Art.LineParticle, pos, color, 90, .(1.5f), state);
		}
	}

	public void Kill()
	{
		hitpoints = 0;
		WasShot();
	}

	public override void Update(float dt)
	{
		let entities = EntityManager.GetNearbyEntities(position, 250, ..scope .());
		for (let entity in entities)
		{
			if (entity is BlackHole)
			{
				continue;
			}

			if (entity is Enemy && !(entity as Enemy).isActive)
			{
				continue;
			}

			if (entity is Bullet)
			{
				entity.velocity += (entity.position - position).normalized * 0.3f;
			}
			else
			{
				let dpos = position - entity.position;
				let length = dpos.length;

				entity.velocity += dpos.normalized * Math.Lerp(2, 0, length / 250.0f);
			}
		}

		GameRoot.WarpGrid.ApplyImplosiveForce((float)Math.Sin(sprayAngle / 2) * 5 + 10, .(position.x, position.y, 0.0f), 50);

		// Particles

		// The black holes spray some orbiting particles. The spray toggles on and off every quarter second.
		if (((uint32)(GetTime() * 1000) / 250) % 2 == 0)
		{
			Vector2 sprayVel = .FromAngle(sprayAngle, random.NextFloat(12, 15));
			Color color = Color.HSV(5, 0.5f, 0.8f); 	// light purple

			Vector2 pos = position + 1.5f * Vector2(sprayVel.y, -sprayVel.x) + random.NextVector2(4, 8);

			var state = ParticleState()
			{
				velocity = sprayVel,
				lengthMultiplier = 1,
				type = ParticleType.Enemy
			};

			GameRoot.ParticleManager.CreateParticle(Art.LineParticle, pos, color, 190, .(1.5f), state);
		}


		// rotate the spray direction
		sprayAngle -= 2.0f * Math.PI_f / 50f;
	}

	public override void Draw()
	{
		let scale = 1.0f + 0.1f * (.)Math.Sin(10 * GetTime());
		DrawTexturePro(image, .(0, 0, image.width, image.height), .(position, .(image.width, image.height) * scale), scale * size * 0.5f, orientation, color);
	}
}