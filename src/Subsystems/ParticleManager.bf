namespace NeonShooter;

using Raylib;

class ParticleManager<T>
{
	public struct Particle
	{
		public Texture2D texture;
		public Vector2 position;
		public float orientation;

		public Vector2 scale = .One;

		public Color tint = .WHITE;

		public float duration;
		public float percent = 1.0f;

		public T state;
	}

	struct CircularParticleArray
	{
		private int start;

		public int Start
		{
			get => start;
			set mut => start = value % list.Count;
		}

		public int Count;
		public int Capacity => list.Count;

		private Particle[] list;

		public ref Particle this[int i]
		{
			get => ref list[(start + i) % list.Count];
			set => list[(start + i)] = value;
		}

		public this(int capacity)
		{
			list = new Particle[capacity];
			start = 0;
			Count = 0;
		}

		public void Dispose() mut
		{
			delete list;

			list = null;
			start = 0;
			Count = 0;
		}
	}

	private function void(ref Particle) updateParticle;
	private CircularParticleArray particles ~ _.Dispose();

	public this(int capacity, function void(ref Particle) updateParticle)
	{
		this.particles = .(capacity);
		this.updateParticle = updateParticle;
	}

	public void CreateParticle(Texture texture, Vector2 position, Color tint, float duration, Vector2 scale, T state, float theta = 0)
	{
		Particle* particle = ?;
		if (particles.Count == particles.Capacity)
		{
			particle = &particles[0];
			particles.Start++;
		}
		else
		{
			particle = &particles[particles.Count++];
		}

		particle.texture = texture;
		particle.position = position;
		particle.tint = tint;
		particle.duration = duration;
		particle.percent = 1.0f;
		particle.scale = scale;
		particle.orientation = theta;
		particle.state = state;
	}

	public void Update()
	{
		int removalCount = 0;
		for (let i < particles.Count)
		{
			var particle = ref particles[i];
			updateParticle(ref particle);

			particle.percent -= 1.0f / particle.duration;
			Swap!(particles, i - removalCount, i);

			if (particle.percent < 0.0f)
			{
				removalCount++;
			}
		}

		particles.Count -= removalCount;
	}

	private static mixin Swap(CircularParticleArray list, int index1, int index2)
	{
		var temp = list[index1];
		list[index1] = list[index2];
		list[index2] = temp;
	}

	public void Draw()
	{
		BeginBlendMode(.BLEND_ADDITIVE);

		for (let i < particles.Count)
		{
			let particle = ref particles[i];

			let size = Vector2(particle.texture.width, particle.texture.height);
			DrawTexturePro(particle.texture, .(.Zero, size), .(particle.position, size * particle.scale), size * 0.5f, particle.orientation * RAD2DEG, particle.tint);
		}

		EndBlendMode();
	}
}