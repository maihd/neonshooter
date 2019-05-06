#pragma once

#include <Mojo/Input.h>

inline namespace Mojo
{
    namespace Input
    {
        void NewFrame(void);
        void EndFrame(void);

        void UpdateCharInput(int character);
        void UpdateCharInput(const char* string);

        void UpdateKey(KeyCode key, bool down);

        void UpdateMouse(MouseButton button, bool down);
        void UpdateMouseMove(float x, float y);
        void UpdateMouseWheel(float h, float v);

        void UpdateGamepadAxis(int gamepadID, GamepadAxis axis, float value);
        void UpdateGamepadButton(int gamepadID, GamepadButton button, bool down);
    }
}