namespace NeonShooter;

using System;
using Raylib;

using NeonShooter.Entity;

public static class EnemySpawner
{
	static Random random;
	static float inverseSpawnChance = 60;

	public static void Init()
	{
		random = new Random();
	}

	public static void Deinit()
	{
		DeleteAndNullify!(random);
	}

	public static void Update()
	{
		if (!PlayerShip.Instance.isDead && EntityManager.EntityCount < 200)
		{
			if (random.Next((int)inverseSpawnChance) == 0)
			{
				EntityManager.AddEntity(Enemy.CreateSeeker(GetSpawnPosition()));
				Audios.PlaySpawn();
			}

			if (random.Next((int)inverseSpawnChance) == 0)
			{
				EntityManager.AddEntity(Enemy.CreateWanderer(GetSpawnPosition()));
				Audios.PlaySpawn();
			}

			if (EntityManager.BlackHoleCount < 2 && random.Next((int)inverseSpawnChance) == 0)
			{
				EntityManager.AddEntity(new BlackHole(GetSpawnPosition()));
				Audios.PlaySpawn();
			}
		}
			
		// slowly increase the spawn rate as time progresses
		if (inverseSpawnChance > 20)
			inverseSpawnChance -= 0.005f;
	}

	private static Vector2 GetSpawnPosition()
	{
		Vector2 pos;

		repeat
		{
			pos = Vector2(random.Next((int)GameRoot.Size.width), random.Next((int)GameRoot.Size.height));
		} 
		while ((pos - PlayerShip.Instance.position).lengthSqr < 250 * 250);

		return pos;
	}

	public static void Reset()
	{
		inverseSpawnChance = 60;
	}
}