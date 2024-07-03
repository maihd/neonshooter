namespace NeonShooter.Entity;

using System;
using System.Collections;

using Raylib;
using minicoro;
using System.Collections;

class Enemy : Entity
{
	private int timeUntilStart = 60;
	private append List<Coroutine> behaviours = .() ~ ClearAndDeleteItems!(_);

	public bool isActive => timeUntilStart <= 0;

	public static Enemy CreateSeeker(Vector2 position)
	{
		var enemy = new Enemy(Art.Seeker, position);
		enemy.AddBehaviour(enemy.FollowPlayer());
		return enemy;
	}

	public static Enemy CreateWanderer(Vector2 position)
	{
		var enemy = new Enemy(Art.Wanderer, position);
		enemy.AddBehaviour(enemy.MoveRandomly());
		return enemy;
	}

	public this(Texture image, Vector2 position)
	{
		this.image = image;
		this.position = position;
		this.radius = image.width * 0.5f;
		this.color = .(0, 0, 0, 0);
	}


	public override void Update(float dt)
	{
		if (isActive)
		{
			ApplyBehaviours();
		}
		else
		{
			timeUntilStart -= 1;
			color = .WHITE * (1 - (timeUntilStart / 60.0f));
		}

		position += velocity;
		position = Vector2Clamp(position, size * 0.5f, GameRoot.Size - size * 0.5f);

		velocity *= 0.8f;
	}

	public void WasShot()
	{
		isExpired = true;
		Audios.PlayExplosion();

		// Particles

		float hue1 = random.NextFloat(0, 6);
		float hue2 = (hue1 + random.NextFloat(0, 2)) % 6f;

		Color color1 = Color.HSV(hue1, 0.5f, 1);
		Color color2 = Color.HSV(hue2, 0.5f, 1);

		for (int i = 0; i < 120; i++)
		{
			float speed = 18f * (1f - 1 / random.NextFloat(1f, 10f));

			var state = ParticleState()
			{
				velocity = random.NextVector2(speed, speed),
				type = ParticleType.Enemy,
				lengthMultiplier = 1f	
			};

			Color color = Color.Lerp(color1, color2, random.NextFloat(0, 1));
			GameRoot.ParticleManager.CreateParticle(Art.LineParticle, position, color, 190, .(1.5f), state);
		}
	}

	private void AddBehaviour(Coroutine behaviour)
	{
		behaviours.Add(behaviour);
	}

	private void ApplyBehaviours()
	{
		for (let behaviour in behaviours)
		{
			behaviour.Resume();
			if (!behaviour.IsRunning)
			{
				@behaviour.Remove();
				delete behaviour;
			}
		}

	}

	Coroutine FollowPlayer(float acceleration = 1f)
	{
		let coroutine = new Coroutine(new () => {
			while (true)
			{
				velocity += Vector2.FromAngle((PlayerShip.Instance.position - position).angle, acceleration);

				if (velocity != Vector2.Zero)
				{
					orientation = velocity.ToAngle();
				}

				Coroutine.Yield();
			}
		});

		return coroutine;
	}

	Coroutine MoveInSquare()
	{
		const int FramesPerSide = 30;
		return new Coroutine(new () => {
			for (let i < FramesPerSide)
			{
				velocity = .Right;
				Coroutine.Yield();
			}

			for (let i < FramesPerSide)
			{
				velocity = .Up;
				Coroutine.Yield();
			}

			for (let i < FramesPerSide)
			{
				velocity = .Left;
				Coroutine.Yield();
			}

			for (let i < FramesPerSide)
			{
				velocity = .Down;
				Coroutine.Yield();
			}
		});
	}

	Coroutine MoveRandomly()
	{
		return new Coroutine(new () => {
			var direction = random.NextFloat(0, Math.PI_f * 2.0f);

			while (true)
			{
				direction += random.NextFloat(-0.1f, 0.1f);
				direction = direction % 360.0f;

				for (int i = 0; i < 6; i++)
				{
					velocity += .FromAngle(direction, 0.4f);
					orientation -= 0.05f;

					var bounds = GameRoot.Viewport;
					bounds.width -= image.width;
					bounds.height -= image.height;

					// if the enemy is outside the bounds, make it move away from the edge
					if (!bounds.Contains(position))
					{
						direction = (GameRoot.Size / 2 - position).ToAngle() + random.NextFloat(-Math.PI_f * 0.5f, Math.PI_f * 0.5f);
					}

					Coroutine.Yield();
				}
			}
		});
	}

}