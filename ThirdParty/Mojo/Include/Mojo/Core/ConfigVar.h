#pragma once

#include <assert.h>

#define BIT(x) (1 << (x))

inline namespace Mojo
{
    namespace ConfigVarFlags
    {
        enum 
        {
            All         = -1,		// all flags
            Boolean     = 1 <<  0,	// variable is a boolean
            Integer     = 1 <<  1,	// variable is an integer
            Float       = 1 <<  2,	// variable is a float
            System      = 1 <<  3,	// system variable
            Renderer    = 1 <<  4,	// renderer variable
            Audio       = 1 <<  5,	// sound variable
            GUI         = 1 <<  6,	// gui variable
            Game        = 1 <<  7,	// game variable
            Tools       = 1 <<  8,	// tool variable
            ServerInfo  = 1 << 10,	// sent from servers, available to menu
            NetworkSync = 1 << 11,	// cvar is synced from the server to clients
            Static      = 1 << 12,	// statically declared, not user created
            Cheat       = 1 << 13,	// variable is considered a cheat
            NonCheat    = 1 << 14,	// variable is not considered a cheat
            Init        = 1 << 15,	// can only be set from the command-line
            ROM         = 1 << 16,	// display only, cannot be set by user at all
            Archive     = 1 << 17,	// set to cause it to be saved to a config file
            Modified    = 1 << 18,	// set when the variable is modified
        };
    }

    struct ConfigVar final
    {
    public:
        // Finds the ConfigVar with the given name.
        // Returns NULL if there is no ConfigVar with the given name.
        static ConfigVar* Find(const char* name);

    public:
        ConfigVar(const char* name, const char* description, int flags, int value);
        ConfigVar(const char* name, const char* description, int flags, int value, int min, int max);

        ConfigVar(const char* name, const char* description, int flags, float value);
        ConfigVar(const char* name, const char* description, int flags, float value, float min, float max);

        ConfigVar(const char* name, const char* description, int flags, const char* value);

    public:
        const char* name;				// name
        const char* description;		// description
        int         flags;				// ConfigVarFlags flags

        ConfigVar*  next;				// next statically declared cvar

        //union
        //{
        const char* asString;			// value

        struct
        {
            int     asInt;			    // int
            int     asIntMin;		    // minimum value
            int     asIntMax;		    // maximum value
        };

        struct
        {
            float   asFloat;		    // float 
            float   asFloatMin;			// minimum value
            float   asFloatMax;		    // maximum value
        };
        //};
    };
}