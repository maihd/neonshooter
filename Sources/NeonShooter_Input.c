#include "NeonShooter_Input.h"

#include <raymath.h>

static float clampf(float value, float min, float max)
{
    const float res = value < min ? min : value;
    return res > max ? max : res;
}

// Calculate linear interpolation between two floats
static float lerpf(float start, float end, float amount)
{
    return start + amount * (end - start);
}

const static int gGamePadIndex = 0;
static bool gShooting = false;

bool Input_IsUsingGamepad(void)
{
    return IsGamepadAvailable(gGamePadIndex);
}

bool Input_IsShooting(void)
{
    return gShooting;
}

bool Input_IsBombPressed(void)
{
    return false;
}

Vector2 Input_GetAimDirection(Vector2 playerPosition)
{
    Vector2 aim;
    if (Input_IsUsingGamepad(gGamePadIndex))
    {
        float axis_right_x = GetGamepadAxisMovement(0, GAMEPAD_AXIS_RIGHT_X);
        float axis_right_y = GetGamepadAxisMovement(0, GAMEPAD_AXIS_RIGHT_Y);
        float x = fabsf(axis_right_x) > 0.1f ? axis_right_x : 0.0f;
        float y = fabsf(axis_right_y) > 0.1f ? axis_right_y : 0.0f;
        if (Vector2Length((Vector2){ x, y }) < 0.01f)
        {
            aim = (Vector2){0, 0};
        }
        else
        {
            gShooting = true;

            float cur_angle = atan2f(aim.y, aim.x);
            float aim_angle = atan2f(y, x);

            cur_angle = lerpf(cur_angle, aim_angle, 0.8f);
            aim = (Vector2){ cosf(cur_angle), sinf(cur_angle) };

            aim.x = lerpf(aim.x, x, 0.6f);
            aim.y = lerpf(aim.y, y, 0.6f);
        }
    }
    else
    {
        float mx = GetMouseX();
        float my = GetMouseY();
        gShooting = IsMouseButtonDown(MOUSE_LEFT_BUTTON);

        Vector2 clip = { 2.0f * mx / GetScreenWidth() - 1.0f, 2.0f * my / GetScreenHeight() - 1.0f };

        Vector2 mpos = { clip.x * GetScreenWidth(), clip.y * GetScreenHeight() };

        Vector2 taim = Vector2Normalize(Vector2Subtract(mpos, playerPosition));

        aim = Vector2Lerp(aim, taim, 0.8f);
    }

    return aim;
}

Vector2 Input_GetMoveDirection(void)
{
    const float LERP_RATE = 0.5f;

    float axisVertical    = 0.0f;
    float axisHorizontal  = 0.0f;

    // Moving by controller

    if (Input_IsUsingGamepad(gGamePadIndex))
    {
        float axisLeftX = GetGamepadAxisMovement(gGamePadIndex, GAMEPAD_AXIS_LEFT_X);
        float axisLeftY = GetGamepadAxisMovement(gGamePadIndex, GAMEPAD_AXIS_LEFT_Y);
        axisVertical = lerpf(axisVertical, fabsf(axisLeftY) > 0.05f ? axisLeftY : 0.0f, LERP_RATE);
        axisHorizontal = lerpf(axisHorizontal, fabsf(axisLeftX) > 0.05f ? axisLeftX : 0.0f, LERP_RATE);
    }
    // Move by keyboard
    else
    {
        if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP) || IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN))
        {
            if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP))
            {
                axisVertical = lerpf(axisVertical, -1.0f, LERP_RATE);
            }

            if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN))
            {
                axisVertical = lerpf(axisVertical, 1.0f, LERP_RATE);
            }
        }
        else
        {
            axisVertical = lerpf(axisVertical, 0.0f, LERP_RATE);
        }

        if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT))
        {
            if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT))
            {
                axisHorizontal = lerpf(axisHorizontal, -1.0f, LERP_RATE);
            }

            if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT))
            {
                axisHorizontal = lerpf(axisHorizontal, 1.0f, LERP_RATE);
            }
        }
        else
        {
            axisHorizontal = lerpf(axisHorizontal, 0.0f, LERP_RATE);
        }
    }

    // Normalize vecrtor
    Vector2 axes = { axisHorizontal, axisVertical };
    if (Vector2Length(axes) < 0.01f)
    {
        axes = (Vector2){ 0, 0 };
    }
    else
    {
        float magnitude = clampf(Vector2Length(axes), 0, 1);
        float angle = atan2f(axes.y, axes.x);

        axes = (Vector2){ cosf(angle) * magnitude, sinf(angle) * magnitude };
    }

    return axes;
}
