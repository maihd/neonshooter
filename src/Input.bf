namespace NeonShooter;

using Raylib;
using NeonShooter.Entity;

public static class Input
{
	private static Vector2 lastMousePosition = .Zero;
	private static bool isAimingWithMouse = false;

	public static Vector2 MousePosition => GetMousePosition();
	public static bool IsAimingWithMouse => isAimingWithMouse;

	public static void Update()
	{
		var found = false;
		for (let key in KeyboardKey[?](.KEY_LEFT, .KEY_RIGHT, .KEY_UP, .KEY_DOWN))
		{
			if (IsKeyDown(key))
			{
				found = true;
				break;
			}
		}

		if (!found)
		{
			if (GetGamepadAxisMovement(0, .GAMEPAD_AXIS_RIGHT_X) > 0 || GetGamepadAxisMovement(0, .GAMEPAD_AXIS_RIGHT_Y) > 0)
			{
				found = true;
			}
		}
		
		if (found)
		{
			isAimingWithMouse = false;
		}
		else if (lastMousePosition != MousePosition)
		{
			isAimingWithMouse = true;
		}

		lastMousePosition = MousePosition;
	}

	public static bool WasKeyPressed(KeyboardKey key)
	{
		return IsKeyPressed(key);
	}

	public static bool WasButtonPressed(GamepadButton button)
	{
		return IsGamepadButtonPressed(0, button);
	}

	public static Vector2 GetMoveDirection()
 	{
		 Vector2 direction = .(GetGamepadAxisMovement(0, .GAMEPAD_AXIS_LEFT_X), GetGamepadAxisMovement(0, .GAMEPAD_AXIS_LEFT_Y));
		 //direction.y *= -1;	// invert the y-axis

		 if (IsKeyDown(.KEY_A)) direction.x -= 1;
		 if (IsKeyDown(.KEY_D)) direction.x += 1;

		 if (IsKeyDown(.KEY_W)) direction.y -= 1;
		 if (IsKeyDown(.KEY_S)) direction.y += 1;

		 // Clamp the length of the vector to a maximum of 1.

		 if (direction.lengthSqr > 1)
		 {
			 direction.Normalize();
		 }

		 return direction;
	}

	public static Vector2 GetAimDirection()
 	{
		 if (isAimingWithMouse)
		 {
			 return GetMouseAimDirection();
		 }

		 Vector2 direction = .(GetGamepadAxisMovement(0, .GAMEPAD_AXIS_RIGHT_X), GetGamepadAxisMovement(0, .GAMEPAD_AXIS_RIGHT_Y));
		 //direction.y *= -1;	// invert the y-axis

		 if (IsKeyDown(.KEY_LEFT)) direction.x -= 1;
		 if (IsKeyDown(.KEY_RIGHT)) direction.x += 1;

		 if (IsKeyDown(.KEY_UP)) direction.y -= 1;
		 if (IsKeyDown(.KEY_DOWN)) direction.y += 1;

		 // Clamp the length of the vector to a maximum of 1.

		 if (direction.lengthSqr > 1)
		 {
			 direction.Normalize();
		 }

		 return direction;
	}

	public static Vector2 GetMouseAimDirection()
	{
		Vector2 direction = MousePosition - PlayerShip.Instance.position;
		return direction.normalized;
	}

	public static bool WasBombButtonPressed()
	{
		return WasButtonPressed(.GAMEPAD_BUTTON_LEFT_TRIGGER_1)
			|| WasButtonPressed(.GAMEPAD_BUTTON_RIGHT_TRIGGER_1)
			|| WasKeyPressed(.KEY_SPACE);
	}
}