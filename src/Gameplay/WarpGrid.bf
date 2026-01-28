namespace NeonShooter;

using Raylib;
using System;
using System.Collections;

class WarpGrid
{
	struct PointMass
	{
		public Vector3 position;
		public Vector3 velocity = .Zero;
		public float inverseMass;

		private Vector3 acceleration = .Zero;
		private float damping = 0.98f;

		public this(Vector3 position, float invMass)
		{
			this.position = position;
			this.inverseMass = invMass;
		}

		public void ApplyForce(Vector3 force) mut
		{
			acceleration += force * inverseMass;
		}

		public void IncreaseDamping(float factor) mut
		{
			damping *= factor;
		}

		public void Update() mut
		{
			velocity += acceleration;
			position += velocity;
			acceleration = Vector3.Zero;

			if (velocity.LengthSqr() < 0.001f * 0.001f)
			{
				velocity = Vector3.Zero;
			}

			velocity *= damping;
			damping = 0.98f;
		}
	}

	private struct Spring
	{
		public PointMass* end1;
		public PointMass* end2;

		public float targetLength;
		public float stiffness;
		public float damping;

		public this(PointMass* end1, PointMass* end2, float stiffness, float damping)
		{
			this.end1 = end1;
			this.end2 = end2;
			this.stiffness = stiffness;
			this.damping = damping;
			this.targetLength = (end2.position - end1.position).Length() * 0.95f;
		}

		public void Update() mut
		{
			var x = end1.position - end2.position;

			let length = x.Length();
			if (length <= targetLength)
			{
				return;
			}

			x = (x / length) * (length - targetLength);
			let dv = end2.velocity - end1.velocity;
			let force = stiffness * x - dv * damping;

			end1.ApplyForce(-force);
			end2.ApplyForce(force);
		}
	}

	List<Spring> springs;
	PointMass[,] points, fixedPoints;


	public this(Rectangle rect, Vector2 spacing)
	{
		var springList = new List<Spring>();

		int numColumns = (int)(rect.width / spacing.x) + 1;
		int numRows = (int)(rect.height / spacing.y) + 1;

		// these fixed points will be used to anchor the grid to fixed positions on the screen
		points = new PointMass[numColumns, numRows];
		fixedPoints = new PointMass[numColumns, numRows];

		// create the point masses
		int column = 0, row = 0;
		for (float y = rect.y, bottom = rect.y + rect.height; y <= bottom; y += spacing.y)
		{
			for (float x = rect.x, right = rect.x + rect.width; x <= right; x += spacing.x)
			{
				points[column, row] = PointMass(Vector3(x, y, 0), 1);
				fixedPoints[column, row] = PointMass(Vector3(x, y, 0), 0);
				column++;
			}

			row++;
			column = 0;
		}

		// link the point masses with springs
		for (int y = 0; y < numRows; y++)
		{
			for (int x = 0; x < numColumns; x++) 
			{ 
				if (x == 0 || y == 0 || x == numColumns - 1 || y == numRows - 1)	// anchor the border of the grid
					springList.Add(Spring(&fixedPoints[x, y], &points[x, y], 0.1f, 0.1f)); 

				else if (x % 3 == 0 && y % 3 == 0)									// loosely anchor 1/9th of the point masses
					springList.Add(Spring(&fixedPoints[x, y], &points[x, y], 0.002f, 0.02f));

				const float stiffness = 0.28f; 
				const float damping = 0.06f; 

				if (x > 0)
					springList.Add(Spring(&points[x - 1, y], &points[x, y], stiffness, damping));

				if (y > 0)
					springList.Add(Spring(&points[x, y - 1], &points[x, y], stiffness, damping));
			}
		}

		springs = springList;
	}

	public ~this()
	{
		DeleteAndNullify!(springs);
		DeleteAndNullify!(points);
		DeleteAndNullify!(fixedPoints);
	}

	public void Update()
	{
		for (var spring in ref springs)
			spring.Update();

		for (var mass in ref points)
			mass.Update();
	}

	public void ApplyDirectedForce(Vector3 force, Vector3 position, float radius)
	{
		for (var mass in ref points)
		{
			float dist2 = (position - mass.position).LengthSqr();
			if (dist2 < radius * radius)
			{
				mass.ApplyForce(10 * force / (10 + Math.Sqrt(dist2)));
			}
		}
	}

	public void ApplyImplosiveForce(float force, Vector3 position, float radius)
	{
		for (var mass in ref points)
		{
			float dist2 = (position - mass.position).LengthSqr();
			if (dist2 < radius * radius)
			{
				mass.ApplyForce(10 * force * (position - mass.position) / (100 + dist2));
				mass.IncreaseDamping(0.6f);
			}
		}
	}

	public void ApplyExplosiveForce(float force, Vector3 position, float radius)
	{
		for (var mass in ref points)
		{
			float dist2 = (position - mass.position).LengthSqr();
			if (dist2 < radius * radius)
			{
				mass.ApplyForce(100 * force * (mass.position - position) / (10000 + dist2));
				mass.IncreaseDamping(0.6f);
			}
		}
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

	    return Vector2(x, y);
	}

	public void Draw()
	{
		int width = points.GetLength(0);
		int height = points.GetLength(1);
	
		Color color = Color(30, 30, 139, 85);	// dark blue
	
		for (int y = 1; y < height; y++)
		{
			for (int x = 1; x < width; x++)
			{
				Vector2 left = Vector2(), up = Vector2();
				Vector2 p = ToVec2(points[x, y].position);

				if (x > 1)
				{
					left = ToVec2(points[x - 1, y].position);
					float thickness = (y % 6) == 1 ? 2f : 1f;
					DrawLineEx(left, p, thickness, color);

					/*
					// use Catmull-Rom interpolation to help smooth bends in the grid
					int clampedX = Math.Min(x + 1, width - 1);
					Vector2 mid = Vector2CatmullRom(ToVec2(points[x - 2, y].position), left, p, ToVec2(points[clampedX, y].position), 0.5f);

					// If the grid is very straight here, draw a single straight line. Otherwise, draw lines to our
					// new interpolated midpoint
					if (Vector2.Distance(mid, (left + p) / 2) > 1)
					{
						DrawLineEx(left, mid, thickness, color);
						DrawLineEx(mid, p, thickness, color);
					}
					else
					{
						DrawLineEx(left, p, thickness, color);
					}
					*/
				}
	
				if (y > 1)
				{
					up = ToVec2(points[x, y - 1].position);
					float thickness = (x % 6) == 1 ? 2f : 1f;
					DrawLineEx(up, p, thickness, color);

					/*
					// use Catmull-Rom interpolation to help smooth bends in the grid
					int clampedY = Math.Min(y + 1, height - 1);
					Vector2 mid = Vector2CatmullRom(ToVec2(points[x, y - 2].position), up, p, ToVec2(points[x, clampedY].position), 0.5f);

					// If the grid is very straight here, draw a single straight line. Otherwise, draw lines to our
					// new interpolated midpoint
					if (Vector2.Distance(mid, (up + p) / 2) > 1)
					{
						DrawLineEx(up, mid, thickness, color);
						DrawLineEx(mid, p, thickness, color);
					}
					else
					{
						DrawLineEx(up, p, thickness, color);
					}
					*/
				}

				if (x > 1 && y > 1)
				{
					Vector2 upLeft = ToVec2(points[x - 1, y - 1].position);
					float thickness = 1f;

					DrawLineEx(0.5f * (upLeft + up), 0.5f * (left + p), thickness, color);
					DrawLineEx(0.5f * (upLeft + left), 0.5f * (up + p), thickness, color);
				}
			}
		}
	}

	public Vector2 ToVec2(Vector3 v3)
	{
		return .(v3.x, v3.y);
	}
}