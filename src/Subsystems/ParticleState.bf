namespace NeonShooter;

using Raylib;
using System;

using NeonShooter.Entity;

typealias ParticleManager = ParticleManager<ParticleState>;

public enum ParticleType
{
	None,
	Enemy,
	Bullet,
	IgnoreGravity
}

public struct ParticleState
{
	public Vector2 velocity;
	public ParticleType type;
	public float baseOrientation = 0;
	public float lengthMultiplier = 1;

	public this()
	{
		velocity = .Zero;
		type = .None;
	}

	public this(Vector2 velocity, ParticleType type)
	{
		this.velocity =	velocity;
		this.type = type;
	}

	public static void UpdateParticle(ref ParticleManager.Particle particle)
	{
		var vel = particle.state.velocity;
		particle.position += vel;
		particle.orientation = particle.state.baseOrientation + vel.ToAngle();

		let speed = vel.length;
		let alpha = Math.Min(1.0f, Math.Min(particle.percent * 2.0f, speed));
		particle.tint.a = (.)(alpha * 0xff);

		particle.scale.x = particle.state.lengthMultiplier * Math.Min(Math.Min(1.0f, 2.0f * speed + 0.1f), alpha);
		//particle.scale.y = particle.state.lengthMultiplier * Math.Min(Math.Min(1.0f, 2.0f * speed + 0.1f), alpha);

		// denormalized floats cause significant performance issues
		if (Math.Abs(vel.x) + Math.Abs(vel.y) < 0.00000000001f)
		{
			vel = Vector2.Zero;
		}

		vel *= 0.978f;		// particles gradually slow down

		var pos = particle.position;
		int width = (int)GameRoot.Size.width;
		int height = (int)GameRoot.Size.height;

		// collide with the edges of the screen
		if (pos.x < 0) 	vel.x = Math.Abs(vel.x); else if (pos.x > width)
			vel.x = -Math.Abs(vel.x);

		if (pos.y < 0) 	vel.y = Math.Abs(vel.y); else if (pos.y > height)
			vel.y = -Math.Abs(vel.y);

		if (particle.state.type != .IgnoreGravity)
		{
			for (let blackHole in EntityManager.BlackHoles)
			{
				var dPos = blackHole.position - pos;
				float distance = dPos.length;

				var n = dPos / distance;
				vel += 10000 * n / (distance * distance + 10000);

				// add tangential acceleration for nearby particles
				if (distance < 400)
					vel += 45 * Vector2(n.y, -n.x) / (distance + 100);
			}
		}

		particle.state.velocity = vel;
	}
}