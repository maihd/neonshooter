#pragma once

#include <Mojo/Native/Input.h>

namespace Mojo
{
    namespace Input
    {
        void Setup(void);
        void Shutdown(void);

        void NewFrame(void);
        void EndFrame(void);

        void UpdateCharInput(int character);
        void UpdateCharInput(const char* string);

        void UpdateKey(KeyCode key, bool down);

        void UpdateMouse(MouseButton button, bool down);
        void UpdateMouseMove(float x, float y);
        void UpdateMouseWheel(float h, float v);
    }
}