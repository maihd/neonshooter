#include <Mojo/Core/CmdArgs.h>

#include <string.h>

namespace Mojo
{
    const char* CmdArgs::NextOf(const char* arg)
    {
        int index = this->IndexOf(arg);
        if (index > -1 && index < MaxArgs - 1)
        {
            return argv[index + 1];
        }

        return NULL;
    }

    int CmdArgs::IndexOf(const char* arg)
    {
        for (int i = 0, n = argc; i < n; i++)
        {
            if (strcmp(argv[i], arg) == 0)
            {
                return i;
            }
        }

        return -1;
    }

    bool CmdArgs::Append(const char* arg)
    {
        if (argc < MaxArgs)
        {
            if (argc <= 0)
            {
                argc = 1;
                argv[0] = strcpy(buffer, arg);
            }
            else
            {
                char* last = (char*)argv[argc - 1];
                while (*last++);

                argv[argc] = strcpy(last, arg);
            }
            
            return true;
        }
        else
        {
            return false;
        }
    }

    bool CmdArgs::Tokenize(const char* args)
    {
        return false;
    }
}