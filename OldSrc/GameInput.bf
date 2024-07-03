using Raylib;
using System;

namespace NeonShooter
{
	public static class GameInput
	{
		const int GamepadIndex = 0;
		static bool gShooting = false;

		public static bool IsUsingGamepad()
		{
			return IsGamepadAvailable(GamepadIndex);
		}

		public static bool IsShooting()
		{
			return gShooting;
		}

		public static bool IsBombPressed()
		{
			if (IsUsingGamepad())
			{
			    return IsGamepadButtonDown(GamepadIndex, .GAMEPAD_BUTTON_LEFT_TRIGGER_1)
			        || IsGamepadButtonDown(GamepadIndex, .GAMEPAD_BUTTON_RIGHT_TRIGGER_1);
			}

			return IsKeyDown(.KEY_SPACE);
		}

		public static Vector2 GetAimDirection(Vector2 playerPosition)
		{
			static Vector2 aim = .(0, 0);
			if (IsUsingGamepad())
			{
			    float axisRightX = GetGamepadAxisMovement(0, .GAMEPAD_AXIS_RIGHT_X);
			    float axisRightY = GetGamepadAxisMovement(0, .GAMEPAD_AXIS_RIGHT_Y);
			    float x = Math.Abs(axisRightX) > 0.1f ? axisRightX : 0.0f;
			    float y = Math.Abs(axisRightY) > 0.1f ? axisRightY : 0.0f;
			    if (Vector2Length(.(x, y)) < 0.01f)
			    {
			        aim = .(0, 0);
			    }
			    else
			    {
			        gShooting = true;

			        float cur_angle = Math.Atan2(aim.y, aim.x);
			        float aim_angle = Math.Atan2(y, x);

			        cur_angle = Math.Lerp(cur_angle, aim_angle, 0.8f);
			        aim = .(Math.Cos(cur_angle), Math.Sin(cur_angle));

			        aim.x = Math.Lerp(aim.x, x, 0.6f);
			        aim.y = Math.Lerp(aim.y, y, 0.6f);
			    }
			}
			else
			{
			    float mx = GetMouseX();
			    float my = GetMouseY();
			    gShooting = IsMouseButtonDown(.MOUSE_LEFT_BUTTON);

			    Vector2 clip = .(2.0f * mx / GetScreenWidth() - 1.0f, 2.0f * my / GetScreenHeight() - 1.0f);

			    Vector2 mpos = .(clip.x * GetScreenWidth(), clip.y * GetScreenHeight());

			    Vector2 taim = Vector2Normalize(Vector2Subtract(mpos, playerPosition));

			    aim = Vector2Lerp(aim, taim, 0.8f);
			}

			return aim;
		}

		public static Vector2 GetMoveDirection()
		{
			const float LERP_RATE = 0.5f;

			float axisVertical    = 0.0f;
			float axisHorizontal  = 0.0f;

			// Moving by controller

			if (IsUsingGamepad())
			{
			    float axisLeftX = GetGamepadAxisMovement(GamepadIndex, .GAMEPAD_AXIS_LEFT_X);
			    float axisLeftY = GetGamepadAxisMovement(GamepadIndex, .GAMEPAD_AXIS_LEFT_Y);
			    axisVertical = Math.Lerp(axisVertical, Math.Abs(axisLeftY) > 0.05f ? axisLeftY : 0.0f, LERP_RATE);
			    axisHorizontal = Math.Lerp(axisHorizontal, Math.Abs(axisLeftX) > 0.05f ? axisLeftX : 0.0f, LERP_RATE);
			}
			// Move by keyboard
			else
			{
			    if (IsKeyDown(.KEY_W) || IsKeyDown(.KEY_UP) || IsKeyDown(.KEY_S) || IsKeyDown(.KEY_DOWN))
			    {
			        if (IsKeyDown(.KEY_W) || IsKeyDown(.KEY_UP))
			        {
			            axisVertical = Math.Lerp(axisVertical, -1.0f, LERP_RATE);
			        }

			        if (IsKeyDown(.KEY_S) || IsKeyDown(.KEY_DOWN))
			        {
			            axisVertical = Math.Lerp(axisVertical, 1.0f, LERP_RATE);
			        }
			    }
			    else
			    {
			        axisVertical = Math.Lerp(axisVertical, 0.0f, LERP_RATE);
			    }

			    if (IsKeyDown(.KEY_A) || IsKeyDown(.KEY_LEFT) || IsKeyDown(.KEY_D) || IsKeyDown(.KEY_RIGHT))
			    {
			        if (IsKeyDown(.KEY_A) || IsKeyDown(.KEY_LEFT))
			        {
			            axisHorizontal = Math.Lerp(axisHorizontal, -1.0f, LERP_RATE);
			        }

			        if (IsKeyDown(.KEY_D) || IsKeyDown(.KEY_RIGHT))
			        {
			            axisHorizontal = Math.Lerp(axisHorizontal, 1.0f, LERP_RATE);
			        }
			    }
			    else
			    {
			        axisHorizontal = Math.Lerp(axisHorizontal, 0.0f, LERP_RATE);
			    }
			}

			// Normalize vecrtor
			Vector2 axes = .(axisHorizontal, axisVertical);
			if (Vector2Length(axes) < 0.01f)
			{
			    axes = .(0, 0);
			}
			else
			{
			    float magnitude = Math.Clamp(Vector2Length(axes), 0, 1);
			    float angle = Math.Atan2(axes.y, axes.x);

			    axes = .(Math.Cos(angle) * magnitude, Math.Sin(angle) * magnitude);
			}

			return axes;
		}
	}
}