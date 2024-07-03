using Raylib;
using System;
using System.Collections;

namespace NeonShooter
{
	struct Entity
	{
	    public bool        active;

	    public float       scale;
	    public float       rotation;
	    public Vector2     position;

	    public Vector2     velocity;
	    public float       movespeed;

	    public Color       color;
	    public float       radius;

	    public Texture     texture;
	}

	struct PointMass
	{
	    public Vector2     position;
	    public Vector2     velocity;
	    public Vector2     acceleration;
	    public float       invMass;
	    public float       damping;
	}

	struct Spring
	{
	    public PointMass*  p0;
	    public PointMass*  p1;

	    public float       targetLength;
	    public float       stiffness;
	    public float       damping;
	    public float       force;
	}

	struct WarpGrid
	{
	    public int                 	cols;
	    public int                 	rows; 

	    public List<Spring>   		springs;

	    public List<PointMass>    	points;
	    public List<PointMass>    	fixedPoints;
	}

	public class World
	{
		public WarpGrid            grid;

		public Entity              player;

		public FreeList<Entity>    bullets;
		public FreeList<Entity>    seekers;
		public FreeList<Entity>    wanderers;
		public FreeList<Entity>    blackHoles;

		public int                 seekerSpawnRate;
		public int                 wandererSpawnRate;
		public int                 blackHoleSpawnRate;

		public bool                oldFire;
		public float               fireTimer;
		public float               fireInterval;

		public float               spawnTimer;
		public float               spawnInterval;

		public bool                lock;
		public float               gameOverTimer;
		public Random 			   random;

		
		public this()
		{
			grid = NewWarpGrid(.(-GetScreenWidth() * 1.1f, -GetScreenHeight() * 1.1f, 2.2f * GetScreenWidth(), 2.2f * GetScreenHeight()), .(128.0f, 128.0f));

			player.active = true;
			player.color = .WHITE;
			player.position = Vector2(0.0f, 0.0f);
			player.rotation = 0.0f;
			player.scale = 1.0f;
			player.movespeed = 720.0f;
			player.texture = Assets.CacheTexture("Art/Player.png");
			player.radius = player.texture.width * 0.5f;

			seekerSpawnRate = 80;
			wandererSpawnRate = 60;
			blackHoleSpawnRate = 20;

			oldFire = false;
			fireTimer = 0.0f;
			fireInterval = 0.1f;

			spawnTimer = 0.0f;
			spawnInterval = 1.0f;

			lock = false;
			gameOverTimer = 0.5f;

			bullets = .(256);
			seekers = .(256);
			wanderers = .(256);
			blackHoles = .(256);

			random = new Random();
		}

		public ~this()
		{
			FreeWarpGrid(grid);

			bullets.Free();
			seekers.Free();
			wanderers.Free();
			blackHoles.Free();

			delete random;
		}

		public void Update(float horizontal, float vertical, Vector2 aimDir, bool fire, float dt)
		{
			// Update warp grid
			UpdateWarpGrid(grid, dt);
			//UpdateMeshGrid(&world->meshGrid, dt);

			if (gameOverTimer > 0.0f)
			{
			    gameOverTimer -= dt;
			    if (gameOverTimer <= 0.0f)
			    {
			        GameAudio.PlayMusic();
			    }
			    return;
			}

			// Update is in progress, locking the list
			lock = true;

			Vector2 moveDirection;
			if (horizontal == 0.0f && vertical == 0.0f)
			{
			    moveDirection = Vector2(0.0f, 0.0f);
			}
			else
			{
			    moveDirection = Vector2Normalize(Vector2(horizontal, vertical));
			}

			player.velocity = Vector2Lerp(player.velocity, moveDirection, 5.0f * dt);
			player = UpdateEntityWithBound(player, Vector2(GetScreenWidth(), GetScreenHeight()), dt);
			WarpGridApplyExplosiveForce(grid, 4.0f * player.movespeed, player.position, 50.0f, dt);
			if (Vector2LengthSq(player.velocity) > 0.1f && GetTime() % 0.025f <= 0.01f)
			{
			    float speed;
			    float angle = Math.Atan2(player.velocity.y, player.velocity.x);

			    Texture glow_tex = Assets.CacheTexture("Art/Laser.png");
			    Texture line_tex = Assets.CacheTexture("Art/Laser.png");

			    Vector2 vel = Vector2Scale(player.velocity, -0.25f * player.movespeed);
			    Vector2 pos = Vector2Add(player.position, Vector2Scale(player.velocity, -45.0f));
			    Vector2 nvel = Vector2Scale(Vector2(vel.y, -vel.x), 0.9f * Math.Sin((.)GetTime() * 10.0f));
			    float alpha = 0.7f;

			    //Vector2 mid_vel = vel;
			    //SpawnParticle(glow_tex, pos, Vector4Scale((Vector4) { 1.0f, 0.7f, 0.1f, 1.0f }, alpha), 0.4f, (Vector2) { 3.0f, 2.0f }, angle, mid_vel);
			    //SpawnParticle(line_tex, pos, Vector4Scale((Vector4) { 1.0f, 1.0f, 1.0f, 1.0f }, alpha), 0.4f, (Vector2) { 3.0f, 1.0f }, angle, mid_vel);

			    //speed = rand() % 101 / 100.0f * 40.0f;
			    //angle = rand() % 101 / 100.0f * 2.0f * PI;
			    //Vector2 side_vel1 = Vector2Add(vel, Vector2Add(nvel, Vector2Scale((Vector2) { cosf(angle), sinf(angle) }, speed)));
			    //SpawnParticle(glow_tex, pos, Vector4Scale((Vector4) { 0.8f, 0.2f, 0.1f, 1.0f }, alpha), 0.4f, (Vector2) { 3.0f, 2.0f }, angle, side_vel1);
			    //SpawnParticle(line_tex, pos, Vector4Scale((Vector4) { 1.0f, 1.0f, 1.0f, 1.0f }, alpha), 0.4f, (Vector2) { 3.0f, 1.0f }, angle, side_vel1);

			    //speed = rand() % 101 / 100.0f * 40.0f;
			    //angle = rand() % 101 / 100.0f * 2.0f * PI;
			    //Vector2 side_vel2 = Vector2Subtract(vel, Vector2Add(nvel, Vector2Scale((Vector2) { cosf(angle), sinf(angle) }, speed)));
			    //SpawnParticle(glow_tex, pos, Vector4Scale((Vector4) { 0.8f, 0.2f, 0.1f, 1.0f }, alpha), 0.4f, (Vector2) { 3.0f, 2.0f }, angle, side_vel2);
			    //SpawnParticle(line_tex, pos, Vector4Scale((Vector4) { 1.0f, 1.0f, 1.0f, 1.0f }, alpha), 0.4f, (Vector2) { 3.0f, 1.0f }, angle, side_vel2);
			}

			for (int i = 0, n = bullets.Count; i < n; i++)
			{
			    if (bullets.elements[i].active)
			    {
			        Entity bullet = UpdateEntity(bullets[i], dt);

			        WarpGridApplyExplosiveForce(grid, 4000.0f, bullet.position, 128.0f, dt);

			        if (bullet.position.x < -GetScreenWidth()
			            || bullet.position.x > GetScreenWidth()
			            || bullet.position.y < -GetScreenHeight()
			            || bullet.position.y > GetScreenHeight())
			        {
			            DestroyBullet(i, true);
			        }
			        else
			        {
			            bullets[i] = bullet;
			        }
			    }
			}

			for (int i = 0, n = seekers.Count; i < n; i++)
			{
			    ref Entity s = ref seekers[i];
			    if (s.active)
			    {
			        if (s.color.a < 255)
			        {
			            uint8 newValue = (.)Math.Max(255, s.color.a + dt * 255);
			            s.color.a = newValue;
			        }
			        else
			        {
			            //MeshGridApplyExplosiveForce(&world->meshGrid, 2.0f * s->movespeed, s->position, dt);
			            WarpGridApplyExplosiveForce(grid, 4.0f * s.movespeed, s.position, 30.0f, dt);

			            Vector2 dir = Vector2Normalize(Vector2Subtract(player.position, s.position));
			            Vector2 acl = Vector2Scale(dir, 10.0f * dt);
			            s.velocity = Vector2Normalize(Vector2Add(s.velocity, acl));
			            s = UpdateEntity(s, dt);
			        }
			    }
			}

			for (int i = 0, n = wanderers.Count; i < n; i++)
			{
			    ref Entity s = ref wanderers[i];
			    if (s.active)
			    {
			        if (s.color.a < 255)
			        {
			            uint8 newValue = (.)Math.Max(255, s.color.a + dt * 255);
			            s.color.a = newValue;
			        }
			        else
			        {
			            const int INTERPOLATIONS = 6;
			            float real_speed = s.movespeed / INTERPOLATIONS;

			            float direction = Math.Max(s.velocity.y, s.velocity.x);
			            for (int j = 0; j < INTERPOLATIONS; j++)
			            {
			                direction += (0.12f * (random.Next(100) / 100.0f) - 0.06f) * Math.PI_f;

			                if (s.position.x < -GetScreenWidth() || s.position.x > GetScreenWidth()
			                    || s.position.y < -GetScreenHeight() || s.position.y > GetScreenHeight())
			                {
			                    direction = Math.Atan2(-s.position.y, -s.position.x) + (1.0f * (random.Next(100) / 100.0f) - 0.5f) * Math.PI_f;
			                }

			                s.rotation = direction;
			                s.velocity = Vector2(Math.Cos(direction), Math.Sin(direction));
			                s.position = Vector2Add(s.position, Vector2Scale(s.velocity, real_speed * dt));

			                WarpGridApplyExplosiveForce(grid, 4.0f * s.movespeed, s.position, 30.0f, dt);
			            }
			        }
			    }
			}

			for (int i = 0, n = bullets.Count; i < n; i++)
			{
			    ref Entity b = ref bullets[i];

			    if (!b.active) continue;
			    for (int j = 0, m = seekers.Count; j < m; j++)
			    {
			        Entity* s = &seekers.elements[j];
			        if (!s.active || s.color.a < 255) continue;

			        if (Vector2Distance(b.position, s.position) <= b.radius + s.radius)
			        {
			            DestroyBullet(i, true);
			            DestroySeeker(j);
			            break;
			        }
			    }

			    if (!b.active) continue;
			    for (int j = 0, m = wanderers.Count; j < m; j++)
			    {
			        Entity* s = &wanderers.elements[j];
			        if (!s.active || s.color.a < 255) continue;

			        if (Vector2Distance(b.position, s.position) <= b.radius + s.radius)
			        {
			            DestroyBullet(i, true);
			            DestroyWanderer(j);
			            break;
			        }
			    }

			    if (!b.active) continue;
			    for (int j = 0, m = blackHoles.Count; j < m; j++)
			    {
			        Entity* s = &blackHoles.elements[j];
			        if (!s.active || s.color.a < 255) continue;

			        float d = Vector2Distance(b.position, s.position);
			        if (d <= b.radius + s.radius)
			        {
			            DestroyBullet(i, true);
			            DestroyBlackhole(j);
			            break;
			        }
			        else if (d <= b.radius + s.radius * 5.0f)
			        {
			            float r = b.radius + s.radius * 5.0f;
			            float t = (d - r) / r;
			            b.velocity = Vector2Normalize(Vector2Add(b.velocity, Vector2Scale(Vector2Normalize(Vector2Subtract(b.position, s.position)), 0.3f)));
			        }
			    }
			}

			for (int j = 0, m = seekers.Count; j < m; j++)
			{
			    Entity* s = &seekers.elements[j];
			    if (!s.active || s.color.a < 255) continue;

			    if (Vector2Distance(player.position, s.position) <= player.radius + s.radius)
			    {
			        OnGameOver();
			        break;
			    }
			}

			for (int j = 0, m = wanderers.Count; j < m; j++)
			{
			    Entity* s = &wanderers.elements[j];
			    if (!s.active || s.color.a < 255) continue;

			    if (Vector2Distance(player.position, s.position) <= player.radius + s.radius)
			    {
			        OnGameOver();
			        break;
			    }
			}

			for (int i = 0, n = blackHoles.Count; i < n; i++)
			{
			    Entity* s = &blackHoles.elements[i];
			    if (s.active)
			    {
			        /*Texture glow_tex = CacheTexture("Art/Glow.png");
			        Texture line_tex = CacheTexture("Art/Laser.png");

			        Vector4 color1 = (Vector4){ 0.3f, 0.8f, 0.4f, 1.0f };
			        Vector4 color2 = (Vector4){ 0.5f, 1.0f, 0.7f, 1.0f };

			        
			        if (GetFrameCount() % 3 == 0)
			        {
			            float speed = 16.0f * s->radius * (0.8f + (rand() % 101 / 100.0f) * 0.2f);
			            float angle = rand() % 101 / 100.0f * GetTime();
			            float value = 4.0f + rand() % 101 / 100.0f * 4.0f;
			            Vector2  vel = (Vector2){ cosf(angle) * speed, sinf(angle) * speed };
			            Vector2  pos = Vector2Add(Vector2Add(s->position, Vector2Scale((Vector2) { vel.y, -vel.x }, 0.4f)), (Vector2) { value, value });

			            Vector4  color = Vector4Add(color1, Vector4Scale(Vector4Subtract(color2, color1), ((rand() % 101) / 100.0f)));
			            SpawnParticle(glow_tex, pos, color, 4.0f, (Vector2) { 0.3f, 0.2f }, 0.0f, vel);
			            SpawnParticle(line_tex, pos, color, 4.0f, (Vector2) { 1.0f, 1.0f }, 0.0f, vel);
			        }

			        if (GetFrameCount() % 60 == 0)
			        {
			            Texture texture = CacheTexture("Art/Laser.png");

			            float hue1 = rand() % 101 / 100.0f * 6.0f;
			            float hue2 = fmodf(hue1 + (rand() % 101 / 100.0f * 2.0f), 6.0f);
			            Vector4  color1 = HSV(hue1, 0.5f, 1);
			            Vector4  color2 = HSV(hue2, 0.5f, 1);

			            for (int i = 0; i < 120.0f; i++)
			            {
			                float speed = 180.0f;
			                float angle = rand() % 101 / 100.0f * 2 * PI;
			                Vector2  vel = (Vector2){ cosf(angle) * speed, sinf(angle) * speed };
			                Vector2  pos = Vector2Add(s->position, vel);
			                Vector4  color = Vector4Add(color1, Vector4Scale(Vector4Subtract(color2, color1), ((rand() % 101) / 100.0f)));
			                SpawnParticle(texture, pos, color, 2.0f, (Vector2) { 1.0f, 1.0f }, 0.0f, (Vector2) { 0.0f, 0.0f });
			            }
			        }*/

			        if (s.color.a < 255)
			        {
			            uint8 newValue = (.)Math.Max(255, s.color.a + dt * 255);
			            s.color.a = newValue;
			        }
			        else
			        {
			            WarpGridApplyImplosiveForce(grid, 2000.0f, s.position, 1024.0f, dt);

			            if (UpdateBlackhole(s, &player))
			            {
			                OnGameOver();
			                break;
			            }

			            for (int j = 0, m = seekers.Count; j < m; j++)
			            {
			                Entity* other = &seekers.elements[j];
			                if (!other.active || other.color.a < 255) continue;

			                if (UpdateBlackhole(s, other))
			                {
			                    DestroySeeker(j);
			                    break;
			                }
			            }

			            for (int j = 0, m = wanderers.Count; j < m; j++)
			            {
			                Entity* other = &wanderers.elements[j];
			                if (!other.active || other.color.a < 255) continue;

			                if (UpdateBlackhole(s, other))
			                {
			                    DestroyWanderer(j);
			                    break;
			                }
			            }
			        }
			    }
			}

			// Update is done, unlock the list
			lock = false;

			// Fire bullet if requested
			if (!fire)
			{
			    oldFire = false;
			    fireTimer = 0.0f;

			    //GameAudioStopShoot();
			}
			else
			{
			    if (oldFire != fire)
			    {
			        oldFire = fire;
			        fireTimer = fireInterval;
			    }

			    fireTimer += dt;
			    if (fireTimer >= fireInterval)
			    {
			        fireTimer = 0;
			        FireBullets(aimDir);

			        GameAudio.PlayShoot();
			    }
			}

			// Spawn enemies
			spawnTimer += dt;
			if (spawnTimer >= spawnInterval)
			{
			    spawnTimer -= spawnInterval;

			    if (random.Next(100) < seekerSpawnRate) SpawnSeeker();
			    if (random.Next(100) < wandererSpawnRate) SpawnWanderer();
			    if (random.Next(100) < blackHoleSpawnRate) SpawnBlackhole();
			}
		}

		public void Render()
		{
			RenderWarpGrid(grid);

			if (gameOverTimer > 0)
			{
			    return;
			}

			RenderEntity(player);
			RenderEntities(bullets.elements);
			RenderEntities(seekers.elements);
			RenderEntities(wanderers.elements);
			RenderEntities(blackHoles.elements);
		}

		// Game


		void SpawnBullet(Vector2 pos, Vector2 vel)
		{
		    Texture texture = Assets.CacheTexture("Art/Bullet.png");
		    Entity entity;
			entity.active = true;
			entity.scale = 1.0f;
		    entity.rotation = Math.Atan2(vel.y, vel.x);
		    entity.position = pos;

			entity.velocity = vel;
		    entity.movespeed = 1280.0f;

		    entity.color = .WHITE;
		    entity.radius = texture.height * 0.5f;
			entity.texture = texture;

		    bullets.Add(entity);
		}

		void FireBullets(Vector2 aim_dir)
		{
		    float angle = Math.Atan2(aim_dir.y, aim_dir.x) + random.Next(100) / 100.0f * (Math.PI_f * 0.025f);
		    float offset = Math.PI_f * 0.1f;

			var aim_dir;
		    aim_dir = Vector2(Math.Cos(angle), Math.Sin(angle));

		    // First bullet
		    {
		        Vector2 vel = Vector2Normalize(aim_dir);
		        Vector2 pos = Vector2Add(player.position, Vector2Scale(Vector2(Math.Cos(angle + offset), Math.Sin(angle + offset)), (.)player.texture.width * 1.25f));
		        SpawnBullet(pos, vel);
		    }

		    // Second bullet
		    {
		        Vector2 vel = Vector2Normalize(aim_dir);
		        Vector2 pos = Vector2Add(player.position, Vector2Scale(Vector2(Math.Cos(angle - offset), Math.Sin(angle - offset)), (.)player.texture.width * 1.25f));
		        SpawnBullet(pos, vel);
		    }
		}

		Vector2 GetSpawnPosition()
		{
		    float min_distance_sqr = (GetScreenHeight() * 0.3f) * (GetScreenHeight() * 0.3f);

		    Vector2 pos;
		    repeat
		    {
		        float x = (2.0f * random.Next(100) / 100.0f - 1.0f) * 0.8f * GetScreenWidth();
		        float y = (2.0f * random.Next(100) / 100.0f - 1.0f) * 0.8f * GetScreenHeight();
		        pos = Vector2(x, y);
		    } while (Vector2DistanceSq(pos, player.position) < min_distance_sqr);

		    return pos;
		}

		void SpawnSeeker()
		{
		    GameAudio.PlaySpawn();

		    Vector2 pos = GetSpawnPosition();
		    Vector2 vel = Vector2Normalize(Vector2Subtract(player.position, pos));

		    Texture texture = Assets.CacheTexture("Art/Seeker.png");

		    Entity entity;
			entity.active = true;

			entity.scale = 1.0f;
			entity.rotation = Math.Atan2(vel.y, vel.x);
		    entity.position = pos;

		    entity.velocity = vel;
		    entity.movespeed = 360.0f;

		    entity.color = Fade(.WHITE, 0.0f);
		    entity.radius = texture.width * 0.5f;
		    entity.texture = texture;

		    seekers.Add(entity);
		}

		void SpawnWanderer()
		{
		    GameAudio.PlaySpawn();

		    Vector2 pos = GetSpawnPosition();
		    Vector2 vel = Vector2Normalize(Vector2Subtract(player.position, pos));

		    Texture texture = Assets.CacheTexture("Art/Wanderer.png");

			Entity entity;
			entity.active = true;

			entity.scale = 1.0f;
			entity.rotation = Math.Atan2(vel.y, vel.x);
			entity.position = pos;

			entity.velocity = vel;
			entity.movespeed = 240.0f;

			entity.color = Fade(.WHITE, 0.0f);
			entity.radius = texture.width * 0.5f;
			entity.texture = texture;

			wanderers.Add(entity);
		}

		void SpawnBlackhole()
		{
		    GameAudio.PlaySpawn();

		    Vector2 pos = GetSpawnPosition();
		    Vector2 vel = Vector2(0.0f, 0.0f);

		    Texture texture = Assets.CacheTexture("Art/Black Hole.png");

		    Entity entity;
			entity.active = true;

		    entity.scale = 1.0f;
			entity.rotation = Math.Atan2(vel.y, vel.x);
			entity.position = pos;
		    entity.velocity = vel;

		    entity.movespeed = 240.0f;

		    entity.color = Fade(.WHITE, 0.0f);

			entity.radius = texture.width * 0.5f;
			entity.texture = texture;

		    blackHoles.Add(entity);
		}

		void DestroyBullet(int index, bool explosion)
		{
		    bullets.elements[index].active = false;
		    bullets.Collect(index);

		    if (explosion)
		    {
		        const int PARTICLE_COUNT = 30;
		        Texture texture = Assets.CacheTexture("Art/Laser.png");

		        /*for (int i = 0; i < PARTICLE_COUNT; i++)
		        {
		            float speed = 640.0f * (0.2f + (rand() % 101 / 100.0f) * 0.8f);
		            float angle = rand() % 101 / 100.0f * 2 * PI;
		            Vector2  vel   = (Vector2){ cosf(angle) * speed, sinf(angle) * speed };
		            Vector2  pos   = world->bullets.elements[index].position;
		            Vector4  color = (Vector4){ 0.6f, 1.0f, 1.0f, 1.0f };

		            SpawnParticle(texture, pos, color, 1.0f, (Vector2) { 1.0f, 1.0f }, 0.0f, vel);
		        }*/
		    }
		}

		void DestroySeeker(int index)
		{
		    GameAudio.PlayExplosion();

		    seekers.elements[index].active = false;
		    seekers.Collect(index);

		    Texture texture = Assets.CacheTexture("Art/Laser.png");

		    /*float hue1 = rand() % 101 / 100.0f * 6.0f;
		    float hue2 = fmodf(hue1 + (rand() % 101 / 100.0f * 2.0f), 6.0f);
		    Vector4  color1 = HSV(hue1, 0.5f, 1);
		    Vector4  color2 = HSV(hue2, 0.5f, 1);*/

		    /*for (int i = 0; i < 120; i++)
		    {
		        float speed = 640.0f * (0.2f + (rand() % 101 / 100.0f) * 0.8f);
		        float angle = rand() % 101 / 100.0f * 2 * PI;
		        Vector2  vel = (Vector2){ cosf(angle) * speed, sinf(angle) * speed };
		        Vector2  pos = world->seekers.elements[index].position;
		        Vector4  color = Vector4Add(color1, Vector4Scale(Vector4Subtract(color2, color1), ((rand() % 101) / 100.0f)));

		        SpawnParticle(texture, pos, color, 1.0f, (Vector2){ 1.0f, 1.0f }, 0.0f, vel);
		    }*/
		}

		void DestroyWanderer(int index)
		{
		    GameAudio.PlayExplosion();

		    wanderers.elements[index].active = false;
		    wanderers.Collect(index);

		    Texture texture = Assets.CacheTexture("Art/Laser.png");

		    /*float hue1 = rand() % 101 / 100.0f * 6.0f;
		    float hue2 = fmodf(hue1 + (rand() % 101 / 100.0f * 2.0f), 6.0f);
		    Vector4  color1 = HSV(hue1, 0.5f, 1);
		    Vector4  color2 = HSV(hue2, 0.5f, 1);*/

		    /*for (int i = 0; i < 120; i++)
		    {
		        float speed = 640.0f * (0.2f + (rand() % 101 / 100.0f) * 0.8f);
		        float angle = rand() % 101 / 100.0f * 2 * PI;
		        Vector2  vel = (Vector2){ cosf(angle) * speed, sinf(angle) * speed };
		        Vector2  pos = world->wanderers.elements[index].position;
		        Vector4  color = Vector4Add(color1, Vector4Scale(Vector4Subtract(color2, color1), ((rand() % 101) / 100.0f)));

		        SpawnParticle(texture, pos, color, 1.0f, (Vector2){ 1.0f, 1.0f }, 0.0f, vel);
		    }*/
		}

		void DestroyBlackhole(int index)
		{
		    GameAudio.PlayExplosion();

		    blackHoles.elements[index].active = false;
		    blackHoles.Collect(index);

		    Texture texture = Assets.CacheTexture("Art/Laser.png");

		    /*float hue1 = rand() % 101 / 100.0f * 6.0f;
		    float hue2 = fmodf(hue1 + (rand() % 101 / 100.0f * 2.0f), 6.0f);
		    Vector4  color1 = HSV(hue1, 0.5f, 1);
		    Vector4  color2 = HSV(hue2, 0.5f, 1);*/

		    /*for (int i = 0; i < 120; i++)
		    {
		        float speed = 640.0f * (0.2f + (rand() % 101 / 100.0f) * 0.8f);
		        float angle = rand() % 101 / 100.0f * 2 * PI;
		        Vector2  vel = (Vector2){ cosf(angle) * speed, sinf(angle) * speed };
		        Vector2  pos = world->blackHoles.elements[index].position;
		        Vector4  color = Vector4Add(color1, Vector4Scale(Vector4Subtract(color2, color1), ((rand() % 101) / 100.0f)));

		        SpawnParticle(texture, pos, color, 1.0f, (Vector2){ 1.0f, 1.0f }, 0.0f, vel);
		    }*/
		}

		void OnGameOver()
		{
		    GameAudio.StopMusic();
		    GameAudio.PlayExplosion();

		    bullets.Clear();
		    seekers.Clear();
		    wanderers.Clear();
		    blackHoles.Clear();

		    gameOverTimer = 3.0f;

		    /*Texture texture = Assets.CacheTexture("Art/Laser.png");

		    float hue1 = rand() % 101 / 100.0f * 6.0f;
		    float hue2 = fmodf(hue1 + (rand() % 101 / 100.0f * 2.0f), 6.0f);
		    Vector4  color1 = HSV(hue1, 0.5f, 1);
		    Vector4  color2 = HSV(hue2, 0.5f, 1);

		    for (int i = 0; i < 1200; i++)
		    {
		        float speed = 10.0f * fmaxf((float)GetScreenWidth(), (float)GetScreenHeight()) * (0.6f + (rand() % 101 / 100.0f) * 0.4f);
		        float angle = rand() % 101 / 100.0f * 2 * PI;
		        Vector2  vel = (Vector2){ cosf(angle) * speed, sinf(angle) * speed };

		        Vector4  color = Vector4Add(color1, Vector4Scale(Vector4Subtract(color2, color1), ((rand() % 101) / 100.0f)));
		        SpawnParticle(texture, world->player.position, color, world->gameOverTimer, (Vector2) { 1.0f, 1.0f }, 0.0f, vel);
		    }*/

		    player.position = Vector2(0, 0);
		    player.velocity = Vector2(0, 0);
		    player.rotation = 0.0f;
		}

		bool UpdateBlackhole(Entity* blackhole, Entity* other)
		{
		    if (Vector2Distance(other.position, blackhole.position) <= other.radius + blackhole.radius)
		    {
		        return true;
		    }
		    else if (Vector2Distance(other.position, blackhole.position) <= other.radius + blackhole.radius * 10.0f)
		    {
		        Vector2 diff = Vector2Subtract(blackhole.position, other.position);
		        other.velocity = Vector2Add(other.velocity, Vector2Scale(Vector2Normalize(diff), Math.Lerp(1.0f, 0.0f, Vector2Length(diff) / (blackhole.radius * 10.0f))));
		        other.velocity = Vector2Normalize(other.velocity);
		    }

		    return false;
		}

		// Utils

		static float Vector2LengthSq(Vector2 v)
		{
		    return v.x * v.x + v.y * v.y;
		}

		static float Vector2DistanceSq(Vector2 a, Vector2 b)
		{
		    float dx = a.x - b.x;
		    float dy = a.y - b.y;
		    float d  = dx * dx + dy * dy;
		    return d;
		}

		static Vector2 Vector2CatmullRom(Vector2 v1, Vector2 v2, Vector2 v3, Vector2 v4, float amount)
		{
		    float squared = amount * amount;
		    float cubed = amount * squared;

		    float x = 0.5f * ((((2.0f * v2.x) + ((-v1.x + v3.x) * amount))
		        + (((((2.0f * v1.x) - (5.0f * v2.x)) + (4.0f * v3.x)) - v4.x) * squared))
		        + ((((-v1.x + (3.0f * v2.x)) - (3.0f * v3.x)) + v4.x) * cubed));

		    float y = 0.5f * ((((2.0f * v2.y) + ((-v1.y + v3.y) * amount))
		        + (((((2.0f * v1.y) - (5.0f * v2.y)) + (4.0f * v3.y)) - v4.y) * squared))
		        + ((((-v1.y + (3.0f * v2.y)) - (3.0f * v3.y)) + v4.y) * cubed));

		    return .(x, y);
		}

		static Vector4 Vector4Add(Vector4 v1, Vector4 v2)
		{
		    Vector4 result = .(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z, v1.w + v1.w);
		    return result;
		}

		static Vector4 Vector4Subtract(Vector4 v1, Vector4 v2)
		{
		    Vector4 result = .(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z, v1.w - v1.w);
		    return result;
		}

		static Vector4 Vector4Scale(Vector4 v, float scalar)
		{
		    Vector4 result = .(v.x * scalar, v.y * scalar, v.z * scalar, v.w * scalar);
		    return result;
		}

		// Entity

		static Vector4 HSV(float h, float s, float v)
		{
		    if (h == 0 && s == 0)
		        return Vector4(v, v, v, 1.0f);

		    float c = s * v;
		    float x = c * (1 - Math.Abs((h % 2) - 1));
		    float m = v - c;

		    if (h < 1)      return Vector4(c + m, x + m, m, 1.0f);
		    else if (h < 2) return Vector4(x + m, c + m, m, 1.0f);
		    else if (h < 3) return Vector4(m, c + m, x + m, 1.0f);
		    else if (h < 4) return Vector4(m, x + m, c + m, 1.0f);
		    else if (h < 5) return Vector4(x + m, m, c + m, 1.0f);
		    else            return Vector4(c + m, m, x + m, 1.0f);
		}

		static Entity UpdateEntity(Entity entity, float dt)
		{
		    Entity newEntity;
			{
		        newEntity.active = entity.active;

		        newEntity.scale = entity.scale;
		        newEntity.position = Vector2Add(entity.position, Vector2Scale(entity.velocity, entity.movespeed * dt));
		        newEntity.rotation = entity.velocity.x != 0.0f || entity.velocity.y != 0.0f ? Math.Atan2(entity.velocity.y, entity.velocity.x) : entity.rotation;

		        newEntity.velocity = entity.velocity;
		        newEntity.movespeed = entity.movespeed;

		        newEntity.color = entity.color;
		        newEntity.radius = entity.radius;

		        newEntity.texture = entity.texture;
		    }

			return newEntity;
		}

		static void UpdateEntities(Entity[] entities, float dt)
		{
		    for (int i = 0, n = entities.Count; i < n; i++)
		    {
		        entities[i] = UpdateEntity(entities[i], dt);
		    }
		}

		static Entity UpdateEntityWithBound(Entity entity, Vector2 bound, float dt)
		{
		    Vector2  pos = Vector2Add(entity.position, Vector2Scale(entity.velocity, entity.movespeed * dt));
		    float rad = entity.radius;

		    if (pos.x + rad > bound.x)
		    {
		        pos.x = bound.x - rad;
		    }
		    else if (pos.x - rad < -bound.x)
		    {
		        pos.x = rad - bound.x;
		    }

		    if (pos.y + rad > bound.y)
		    {
		        pos.y = bound.y - rad;
		    }
		    else if (pos.y - rad < -bound.y)
		    {
		        pos.y = rad - bound.y;
		    }

		    Entity newEntity = entity;
			newEntity.rotation = entity.velocity.x != 0.0f || entity.velocity.y != 0.0f ? Math.Atan2(entity.velocity.y, entity.velocity.x) : entity.rotation;
			newEntity.position = pos;
			return newEntity;
		}

		static void UpdateEntitiesWithBound(Entity[] entities, Vector2 bound, float dt)
		{
		    for (int i = 0, n = entities.Count; i < n; i++)
		    {
		        entities[i] = UpdateEntityWithBound(entities[i], bound, dt);
		    }
		}

		static void RenderEntity(Entity entity)
		{
		    if (entity.active)
		    {
		        //DrawTextureEx(entity.texture, entity.position, entity.rotation * RAD2DEG, entity.scale, entity.color
		        DrawTexturePro(
		            entity.texture, 
		            Rectangle(0, 0, entity.texture.width, entity.texture.height), 
		            Rectangle(entity.position.x, entity.position.y, entity.texture.width, entity.texture.height),
		            Vector2(entity.texture.width * 0.5f, entity.texture.height * 0.5f),
		            entity.rotation * RAD2DEG, 
		            entity.color
		        );
		    }
		}

		static void RenderEntities(List<Entity> entities)
		{
		    for (int i = 0, n = entities.Count; i < n; i++)
		    {
		        Entity entity = entities[i];
		        if (entity.active)
		        {
		            DrawTexturePro(
		                entity.texture, 
		                Rectangle(0, 0, entity.texture.width, entity.texture.height), 
		                Rectangle(entity.position.x, entity.position.y, entity.texture.width, entity.texture.height),
		                Vector2(entity.texture.width * 0.5f, entity.texture.height * 0.5f),
		                entity.rotation * RAD2DEG, 
		                entity.color
		            );
		            //DrawTextureEx(entity.texture, entity.position, entity.rotation * RAD2DEG, entity.scale, entity.color);
		        }
		    }
		}

		// Warp grid

		const float DEFAULT_POINT_DAMPING = 1.0f;

		static PointMass NewPointMass(Vector2 position, float invMass)
		{
		    PointMass pointMass;
			{
		        pointMass.position = position;
		        pointMass.velocity = Vector2(0);

		        pointMass.acceleration = Vector2(0); 
		        pointMass.invMass = invMass;
		        pointMass.damping = DEFAULT_POINT_DAMPING;
		    }
			return pointMass;
		}

		static void UpdatePointMass(PointMass* p, float timeStep)
		{
		    Vector2 velocity = Vector2Add(p.velocity, Vector2Scale(p.acceleration, timeStep));
		    Vector2 position = Vector2Add(p.position, Vector2Scale(velocity, timeStep));

		    if (Vector2LengthSq(velocity) < 0.001f * 0.001f)
		    {
		        velocity = .(0, 0);
		    }
		    else
		    {
		        velocity = Vector2Scale(velocity, Math.Max(0.0f, 1.0f - p.damping * timeStep));
		    }

		    Vector2 acceleration = p.acceleration;
		    if (Vector2LengthSq(acceleration) < 0.001f * 0.001f)
		    {
		        acceleration = .(0, 0);
		    }
		    else
		    {
		        acceleration = Vector2Scale(acceleration, Math.Max(0.0f, 1.0f - p.damping * timeStep));
		    }

		    p.position     = position;
		    p.velocity     = velocity;
		    p.acceleration = acceleration;
		    p.damping      = DEFAULT_POINT_DAMPING;
		}

		static void PointMassApplyForce(PointMass* p, Vector2 force, float timeStep)
		{
		    p.acceleration = Vector2Add(p.acceleration, Vector2Scale(force, p.invMass * timeStep));
		}

		static void PointMassIncreaseDamping(PointMass* p, float factor)
		{
		    p.damping = p.damping * factor;
		}

		static Spring NewSpring(PointMass* p0, PointMass* p1, float stiffness, float damping)
		{
		    Spring spring;
	        spring.p0 = p0;
	        spring.p1 = p1;

	        spring.targetLength = Vector2Distance(p0.position, p1.position) * 0.99f;
	        spring.stiffness = stiffness;
	        spring.damping = damping;
	        spring.force = 60.0f;
			return spring;
		}

		static void UpdateSpring(Spring* spring, float timeStep)
		{
		    Vector2 diff = Vector2Subtract(spring.p0.position, spring.p1.position);
		    float len = Vector2Length(diff);
		    if (len > spring.targetLength)
		    {
		        float changeRate = (len - spring.targetLength) / len;
		        Vector2 dvel = Vector2Subtract(spring.p1.velocity, spring.p0.velocity);
		        Vector2 force = Vector2Subtract(Vector2Scale(diff, changeRate * spring.stiffness), Vector2Scale(dvel, Math.Max(0.0f, 1.0f - spring.damping * timeStep)));

		        PointMassApplyForce(spring.p0, Vector2Scale(Vector2Negate(force), spring.force), timeStep);
		        PointMassApplyForce(spring.p1, Vector2Scale(force, spring.force), timeStep);
		    }
		}

		static WarpGrid NewWarpGrid(Rectangle bounds, Vector2 spacing)
		{
		    int cols = (int)(bounds.width / spacing.x) + 1;
		    int rows = (int)(bounds.height / spacing.y) + 1;

		    int pointCount = cols * rows;
		    let springs = new List<Spring>(4 * pointCount);
		    let points = new List<PointMass>(pointCount);
		    let fixedPoints = new List<PointMass>(pointCount);

		    if (points == null || springs == null || fixedPoints == null)
		    {
		        delete points;
		        delete springs;
		        delete fixedPoints;

		        return default;
		    }

		    points.Count = pointCount;
		    fixedPoints.Count = pointCount;

		    for (int i = 0; i < rows; i++)
		    {
		        for (int j = 0; j < cols; j++)
		        {
		            int  index = i * cols + j;
		            Vector2 position = .(bounds.x + j * spacing.x, bounds.y + i * spacing.y);
		            
		            points[index] = NewPointMass(position, 1.0f);
		            fixedPoints[index] = NewPointMass(position, 0.0f);

		            if ((i == 0) || (j == 0) || (i == rows - 1) || (j == cols - 1)) // anchor the border of the grid
		            {
		                springs.Add(NewSpring(&fixedPoints[index], &points[index], 0.1f, 5.0f));
		            }
		            else if ((i % 3 == 0) && (j % 3 == 0)) // loosely anchor 1/9th of the point masses
		            {
		                springs.Add(NewSpring(&fixedPoints[index], &points[index], 0.002f, 40.0f));
		            }

		            const float stiffness = 0.28f;
		            const float damping = 30.0f;
		            if (j > 0)
		            {
		                springs.Add(NewSpring(&points[i * cols + (j - 1)], &points[index], stiffness, damping));
		            }

		            if (i > 0)
		            {
		                springs.Add(NewSpring(&points[(i - 1) * cols + j], &points[index], stiffness, damping));
		            }
		        }
		    }

			WarpGrid warpGrid;
			warpGrid.cols = cols;
			warpGrid.rows = rows;
			warpGrid.springs = springs;
			warpGrid.points = points;
			warpGrid.fixedPoints = fixedPoints;
		    return warpGrid;
		}

		static WarpGrid FreeWarpGrid(WarpGrid grid)
		{
		    delete grid.points;
		    delete grid.springs;
		    delete grid.fixedPoints;

		    return default;
		}

		static void UpdateWarpGrid(WarpGrid grid, float timeStep)
		{
		    for (int i = 0, n = grid.springs.Count; i < n; i++)
		    {
		        UpdateSpring(&grid.springs[i], timeStep);
		    }

		    for (int i = 0, n = grid.points.Count; i < n; i++)
		    {
		        UpdatePointMass(&grid.points[i], timeStep);
		    }
		}

		static void RenderWarpGrid(WarpGrid grid)
		{
		    BeginBlendMode(.BLEND_ADDITIVE);

		    Color color = .(30, 30, 139, 156);   // dark blue

		    int cols = grid.cols;
		    int rows = grid.rows;
		    for (int i = 1; i < rows; i++)
		    {
		        for (int j = 1; j < cols; j++)
		        {
		            float horThickness = 2.0f;
		            float verThickness = 2.0f;

		            Vector2 current = grid.points[i * cols + j].position;
		            
		            Vector2 left = grid.points[i * cols + (j - 1)].position;
		            Vector2 up = grid.points[(i - 1) * cols + j].position;

		            Vector2 midUp = Vector2Scale(Vector2Add(current, up), 0.5f);
		            Vector2 midLeft = Vector2Scale(Vector2Add(current, left), 0.5f);

		            Vector2 upLeft = grid.points[(i - 1) * cols + (j - 1)].position;
		            //DrawLineEx(Vector2Scale(Vector2Add(upLeft, left), 0.5f), midLeft, horThickness, color);   // horizontal line

		            int j0 = Math.Max(j - 2, 0);
		            int j1 = Math.Min(j + 1, cols - 1);
		            Vector2 horMid = Vector2CatmullRom(grid.points[i * cols + j0].position, left, current, grid.points[i * cols + j1].position, 0.5f);
		            if (Vector2DistanceSq(horMid, midLeft) > 1.0f)
		            {
		                DrawLineEx(left, horMid, horThickness, color);
		                DrawLineEx(horMid, current, horThickness, color);
		                DrawLineEx(Vector2Scale(Vector2Add(upLeft, up), 0.5f), horMid, verThickness, color);   // vertical line
		            }
		            else
		            {
		                DrawLineEx(left, current, horThickness, color);
		                DrawLineEx(Vector2Scale(Vector2Add(upLeft, up), 0.5f), midLeft, verThickness, color);   // vertical line
		            }
		            
		            int i0 = Math.Max(i - 2, 0);
		            int i1 = Math.Min(i + 1, rows - 1);
		            Vector2 verMid = Vector2CatmullRom(grid.points[i0 * cols + j].position, up, current, grid.points[i1 * cols + j].position, 0.5f);
		            if (Vector2DistanceSq(verMid, midUp) > 1.0f)
		            {
		                DrawLineEx(up, verMid, verThickness, color);
		                DrawLineEx(verMid, current, verThickness, color);
		                DrawLineEx(Vector2Scale(Vector2Add(upLeft, left), 0.5f), verMid, horThickness, color);   // horizontal line
		            }
		            else
		            {
		                DrawLineEx(up, current, verThickness, color);
		                DrawLineEx(Vector2Scale(Vector2Add(upLeft, left), 0.5f), midUp, horThickness, color);   // horizontal line
		            }
		        }
		    }

		    EndBlendMode();
		}

		static void WarpGridApplyDirectedForce(WarpGrid grid, Vector2 force, Vector2 position, float radius, float timeStep)
		{
		    for (int i = 0, n = grid.points.Count; i < n; i++)
		    {
		        PointMass point = grid.points[i];
		        if (Vector2DistanceSq(position, point.position) < radius * radius)
		        {
		             PointMassApplyForce(&grid.points[i], Vector2Scale(force, 1.0f / (1.0f + Vector2DistanceSq(position, point.position))), timeStep);
		        }
		    }
		}

		static void WarpGridApplyImplosiveForce(WarpGrid grid, float force, Vector2 position, float radius, float timeStep)
		{
		    for (int i = 0, n = grid.points.Count; i < n; i++)
		    {
		        PointMass point = grid.points[i];
		        Vector2 diff = Vector2Subtract(position, point.position);
		        float distSq = Vector2LengthSq(diff);
		        if (distSq < radius * radius)
		        {
		            Vector2 appliedForce = Vector2Scale(diff, force * 50.0f / (1000.0f + distSq));

		            PointMassApplyForce(&grid.points[i], appliedForce, timeStep);
		            PointMassIncreaseDamping(&grid.points[i], 1.0f / 0.6f);
		        }
		    }
		}

		static void WarpGridApplyExplosiveForce(WarpGrid grid, float force, Vector2 position, float radius, float timeStep)
		{
		    for (int i = 0, n = grid.points.Count; i < n; i++)
		    {
		        PointMass point = grid.points[i];
		        Vector2 diff = Vector2Subtract(point.position, position);
		        float distSq = Vector2LengthSq(diff);
		        if (distSq < radius * radius)
		        {
		            Vector2 appliedForce = Vector2Scale(diff, force * 100.0f / (1000.0f + distSq));

		            PointMassApplyForce(&grid.points[i], appliedForce, timeStep);
		            PointMassIncreaseDamping(&grid.points[i], 1.0f / 0.6f);
		        }
		    }
		}
	}
}