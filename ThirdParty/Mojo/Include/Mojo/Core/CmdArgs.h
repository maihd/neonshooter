#pragma once

inline namespace Mojo
{
    struct CmdArgs
    {
        constexpr static int MaxArgs       = 64;
        constexpr static int MaxBufferSize = 2048;

        int         argc;
        const char* argv[MaxArgs];
        char        buffer[MaxBufferSize];

        inline const char* operator[](int i) const
        {
            return !(i < 0 || i >= argc) ? argv[i] : "";
        }

        constexpr CmdArgs(void)
            : argc(0)
            , argv()
            , buffer()
        {
        }

        inline CmdArgs(const char* args)
            : argc(0)
            , argv()
            , buffer()
        {
            this->Tokenize(args);
        }

        inline CmdArgs(int argc, const char* argv[])
            : argc(0)
            , argv()
            , buffer()
        {
            for (int i = 0; i < argc; i++)
            {
                this->Append(argv[i]);
            }
        }

        inline void Clear(void)
        {
            argc = 0;
        }

        const char* NextOf(const char* arg);
        int         IndexOf(const char* arg);

        bool Append(const char* arg);
        bool Tokenize(const char* args);
    };
}