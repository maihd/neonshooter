#pragma once

#include <Mojo/Graphics.h>

struct MojoModule;

namespace Mojo
{
    void    AddModule(MojoModule* module, const char* moduleName);
    void    RemoveModule(MojoModule* module, const char* moduleName);

    void    LoadDynamicModule(const char* path);
    void    UnloadDynamicModule(const char* path);

    int     Run(void);
}