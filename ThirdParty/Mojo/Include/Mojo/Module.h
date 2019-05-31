#pragma once

#if _WIN32
#   define MOJO_MODULE_EXPORT __declspec(dllexport)
#elif defined(__GNUC__)
#   define MOJO_MODULE_EXPORT __attribute__((visible("default")))
#else
#   define MOJO_MODULE_EXPORT
#endif

#ifdef __cplusplus
#   define MOJO_MODULE extern "C" MOJO_MODULE_EXPORT
#else
#   define MOJO_MODULE MOJO_MODULE_EXPORT
#endif

#ifndef __cplusplus
typedef unsigned char bool;
enum { true = 1, false = 0, };
#endif

typedef struct MojoModule
{
    void (*Update)(Module* system);
    void (*FixedUpdate)(Module* system);
    void (*FinalUpdate)(Module* system);
} MojoModule;

MOJO_MODULE bool MojoModule_Setup(MojoModule* module);
MOJO_MODULE void MojoModule_Shutdown(MojoModule* module);