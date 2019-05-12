#include <Mojo/Core/ConfigVar.h>

#include <string.h>

namespace Mojo
{
    static ConfigVar* rootVars = 0;
    static ConfigVar* lastVars = 0;

    // Finds the ConfigVar with the given name.
    // Returns NULL if there is no ConfigVar with the given name.
    ConfigVar* ConfigVar::Find(const char* name)
    {
        for (ConfigVar* var = rootVars; var; var = var->next)
        {
            if (stricmp(name, var->name) == 0)
            {
                return var;
            }
        }

        return 0;
    }

    ConfigVar::ConfigVar(const char *name, const char *description, int flags, int value)
        : name(name)
        , description(description)
        , flags(flags)
        , next(0)
        , asInt(value)
        , asIntMin(value)
        , asIntMax(value)
    {
        ConfigVar* other = Find(name);
        if (other)
        {
            other->asInt = value;
        }
        else
        {
            if (lastVars)
            {
                lastVars = lastVars->next = this;
            }
            else
            {
                rootVars = lastVars = this;
            }
        }
    }

    ConfigVar::ConfigVar(const char *name, const char *description, int flags, int value, int min, int max)
        : name(name)
        , description(description)
        , flags(flags)
        , next(0)
        , asInt(value)
        , asIntMin(min)
        , asIntMax(max)
    {
        ConfigVar* other = Find(name);
        if (other)
        {
            other->asInt = value;
            other->asIntMin = min;
            other->asIntMax = max;
        }
        else
        {
            if (lastVars)
            {
                lastVars = lastVars->next = this;
            }
            else
            {
                rootVars = lastVars = this;
            }
        }
    }

    ConfigVar::ConfigVar(const char *name, const char *description, int flags, float value)
        : name(name)
        , description(description)
        , flags(flags)
        , next(0)
        , asFloat(value)
        , asFloatMin(value)
        , asFloatMax(value)
    {
        ConfigVar* other = Find(name);
        if (other)
        {
            other->asFloat = value;
        }
        else
        {
            if (lastVars)
            {
                lastVars = lastVars->next = this;
            }
            else
            {
                rootVars = lastVars = this;
            }
        }
    }
    
    ConfigVar::ConfigVar(const char *name, const char *description, int flags, float value, float min, float max)
        : name(name)
        , description(description)
        , flags(flags)
        , next(0)
        , asFloat(value)
        , asFloatMin(min)
        , asFloatMax(max)
    {
        ConfigVar* other = Find(name);
        if (other)
        {
            other->asFloat = value;
            other->asFloatMin = min;
            other->asFloatMax = max;
        }
        else
        {
            if (lastVars)
            {
                lastVars = lastVars->next = this;
            }
            else
            {
                rootVars = lastVars = this;
            }
        }
    }

    ConfigVar::ConfigVar(const char *name, const char *description, int flags, const char* value)
        : name(name)
        , description(description)
        , flags(flags)
        , next(0)
        , asString(value)
    {
        ConfigVar* other = Find(name);
        if (other)
        {
            other->asString = value;
        }
        else
        {
            if (lastVars)
            {
                lastVars = lastVars->next = this;
            }
            else
            {
                rootVars = lastVars = this;
            }
        }
    }
}