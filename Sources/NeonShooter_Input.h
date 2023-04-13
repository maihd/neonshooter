#pragma once

#include <raylib.h>
#include <stdbool.h>

// void Input_Update(void);

bool    Input_IsUsingGamepad(void);

bool    Input_IsShooting(void);
bool    Input_IsBombPressed(void);

Vector2 Input_GetAimDirection(Vector2 playerPosition);
Vector2 Input_GetMoveDirection(void);
