#pragma once

#include <Mojo/Graphics.h>

namespace Mojo
{
    struct Engine
    {
        virtual bool Setup(void)    = 0;
        virtual void Shutdown(void) = 0;

        virtual void Render(void)   = 0;
        virtual void Update(void)   = 0;
    };

    int RunEngine(Engine* engine);
}