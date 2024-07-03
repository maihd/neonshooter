namespace NeonShooter.Entity;

using System.Linq;
using System.Collections;

public class EntityManager
{
	static List<Entity> entities;
	static List<Entity> addedEntities;

	static List<Enemy> enemies;
	static List<Bullet> bullets;
	static List<BlackHole> blackHoles;

	static bool isUpdating;

	public static int EntityCount => entities.Count;
	public static int BlackHoleCount => blackHoles.Count;

	public static List<BlackHole> BlackHoles => blackHoles;

	public static void Init()
	{
		entities = new List<Entity>();
		addedEntities = new List<Entity>();

		enemies = new List<Enemy>();
		bullets = new List<Bullet>();
		blackHoles = new List<BlackHole>();
	}

	public static void Deinit()
	{
		DeleteContainerAndItems!(entities);
		DeleteContainerAndItems!(addedEntities);

		DeleteAndNullify!(enemies);
		DeleteAndNullify!(bullets);
		DeleteAndNullify!(blackHoles);

		entities = null;
		addedEntities = null;

		isUpdating = false;
	}

	public static void AddEntity(Entity entity)
	{
		if (isUpdating)
		{
			addedEntities.Add(entity);
		}
		else
		{
			AddEntityInternal(entity);
		}
	}

	private static void AddEntityInternal(Entity entity)
	{
		entities.Add(entity);

		if (entity is Bullet)
		{
			bullets.Add(entity as Bullet);
		}
		else if (entity is Enemy)
		{
			enemies.Add(entity as Enemy);
		}
		else if (entity is BlackHole)
		{
			blackHoles.Add(entity as BlackHole);
		}
	}


	public static void RemoveEntity(Entity entity)
	{
		if (isUpdating)
		{
			entity.isExpired = true;
		}
		else
		{
			entities.Remove(entity);
		}
	}

	public static void Update(float dt)
	{
		// Handle entities update

		isUpdating = true;

		HandleCollisions();

		for (let entity in entities)
		{
			entity.Update(dt);
		}

		isUpdating = false;

		// Find and grouping entities

		let newBullets = bullets.Where((x) => !x.isExpired).ToList(.. new .());
		let newEnemies = enemies.Where((x) => !x.isExpired).ToList(.. new .());
		let newBlackHoles = blackHoles.Where((x) => !x.isExpired).ToList(.. new .());

		delete bullets;
		delete enemies;
		delete blackHoles;

		bullets = newBullets;
		enemies = newEnemies;
		blackHoles = newBlackHoles;

		// Handle add/remove entities

		for (let entity in addedEntities)
		{
			AddEntityInternal(entity);
		}

		addedEntities.Clear();

		for (let entity in entities)
		{
			if (entity.isExpired)
			{
				@entity.Remove();
				delete entity;
			}
		}
	}

	public static void Draw()
	{
		for (let entity in entities)
		{
			entity.Draw();
		}
	}

	private static bool IsColliding(Entity a, Entity b)
	{
		float radius = a.radius + b.radius;
		return !a.isExpired && !b.isExpired && (a.position - b.position).lengthSqr < radius * radius;
	}

	private static void HandleCollisions()
	{
		// Handle collisions between enemies
		for (let i < enemies.Count)
		{
			for (let j in i+1..<enemies.Count)
			{
				let a = enemies[i];
				let b = enemies[j];
				if (IsColliding(a, b))
				{
					a.HandleCollision(b);
					b.HandleCollision(a);
				}
			}
		}

		// Handle collisions between bullets and enemies
		for (let bullet in bullets)
		{
			for (let enemy in enemies)
			{
				if (enemy.isActive && IsColliding(bullet, enemy))
				{
					enemy.WasShot();
					bullet.isExpired = true;
				}
			}
		}

		// Handle collisions between player and enemies
		for (let enemy in enemies)
		{
			if (enemy.isActive && IsColliding(PlayerShip.Instance, enemy))
			{
				KillPlayer();
				break;
			}
		}

		// Handle collisions between blackholes and entities
		for (let blackHole in blackHoles)
		{
			for (let enemy in enemies)
			{
				if (enemy.isActive && IsColliding(enemy, blackHole))
				{
					enemy.WasShot();
				}
			}

			for (let bullet in bullets)
			{
				if (IsColliding(bullet, blackHole))
				{
					blackHole.WasShot();
					bullet.isExpired = true;
				}
			}

			if (IsColliding(PlayerShip.Instance, blackHole))
			{
				KillPlayer();
				break;
			}
		}
	}

	public static void KillPlayer()
	{
		PlayerShip.Instance.Kill();

		for (let x in enemies)
		{
			x.WasShot();
		}

		for (let x in bullets)
		{
			x.isExpired = true;
		}

		for (let x in blackHoles)
		{
			x.Kill();
		}

		enemies.Clear();
		bullets.Clear();
		blackHoles.Clear();
	}

	public static void GetNearbyEntities(Raylib.Vector2 position, float radius, List<Entity> outEntities)
	{
		for (let entity in entities)
		{
			if ((position - entity.position).lengthSqr < radius * radius)
			{
				outEntities.Add(entity);
			}
		}
	}
}