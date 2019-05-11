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

    public:
                                // Never use the default constructor.
                                //ConfigVar(void) { assert(decltype(*this) != decltype(ConfigVar)); }

                                // Always use one of the following constructors.
                                //ConfigVar(const char *name, const char *value, int flags, const char *description,
                                //        argCompletion_t valueCompletion = NULL );
                                //ConfigVar( const char *name, const char *value, int flags, const char *description,
                                //        float valueMin, float valueMax, argCompletion_t valueCompletion = NULL );
                                //ConfigVar( const char *name, const char *value, int flags, const char *description,
                                //        const char **valueStrings, argCompletion_t valueCompletion = NULL );

                                ConfigVar(const char *name, const char *value, int flags, const char *description);
                                ConfigVar(const char *name, const char *value, int flags, const char *description,
                                        float valueMin, float valueMax);
                                ConfigVar(const char *name, const char *value, int flags, const char *description,
                                        const char **valueStrings);

        virtual					~ConfigVar(void) {}

        inline const char*      GetName()           const { return internalVar->name;           }
        inline int              GetFlags()          const { return internalVar->flags;          }
        inline const char*      GetDescription()    const { return internalVar->description;    }
        inline float            GetMinValue()       const { return internalVar->valueMin;       }
        inline float            GetMaxValue()       const { return internalVar->valueMax;       }
        inline const char**     GetValueStrings()   const { return valueStrings;                }

        //inline argCompletion_t			GetValueCompletion() const { return valueCompletion; }

        bool					IsModified() const { return ( internalVar->flags & ConfigVarFlags::Modified ) != 0; }
        void					SetModified() { internalVar->flags |= ConfigVarFlags::Modified; }
        void					ClearModified() { internalVar->flags &= ~ConfigVarFlags::Modified; }

        const char *			GetDefaultString() const { return internalVar->InternalGetResetString(); }
        const char *			GetString() const { return internalVar->value; }
        bool					GetBool() const { return ( internalVar->integerValue != 0 ); }
        int						GetInteger() const { return internalVar->integerValue; }
        float					GetFloat() const { return internalVar->floatValue; }

        void					SetString( const char *value ) { internalVar->InternalSetString( value ); }
        void					SetBool( const bool value ) { internalVar->InternalSetBool( value ); }
        void					SetInteger( const int value ) { internalVar->InternalSetInteger( value ); }
        void					SetFloat( const float value ) { internalVar->InternalSetFloat( value ); }

        void					SetInternalVar( ConfigVar *cvar ) { internalVar = cvar; }

        static void				RegisterStaticVars();

    private:
        //void					Init( const char *name, const char *value, int flags, const char *description,
        //                                float valueMin, float valueMax, const char **valueStrings, argCompletion_t valueCompletion );
        void					Init(const char *name, const char *value, int flags, const char *description,
                                    float valueMin, float valueMax, const char **valueStrings);

        void			        InternalSetString( const char *newValue ) {}
        void			        InternalSetBool( const bool newValue ) {}
        void			        InternalSetInteger( const int newValue ) {}
        void			        InternalSetFloat( const float newValue ) {}
        
        const char* 	        InternalGetResetString() const { return value; }

    private:
        const char*             name;					// name
        const char*             value;					// value
        const char*             description;			// description
        int                     flags;					// CVAR_? flags
        float                   valueMin;				// minimum value
        float                   valueMax;				// maximum value
        const char**            valueStrings;			// valid value strings
        //argCompletion_t	    		valueCompletion;		// value auto-completion function
        int                     integerValue;			// atoi( string )
        float                   floatValue;				// atof( value )
        ConfigVar*              internalVar;			// internal cvar
        ConfigVar*              next;					// next statically declared cvar

    private:
        static ConfigVar*       staticVars;
    };

    //inline ConfigVar::ConfigVar( const char *name, const char *value, int flags, const char *description,
    //                            argCompletion_t valueCompletion ) {
    //    if ( !valueCompletion && ( flags & Boolean ) ) {
    //        valueCompletion = idCmdSystem::ArgCompletion_Boolean;
    //    }
    //    Init( name, value, flags, description, 1, -1, NULL, valueCompletion );
    //}
//
    //inline ConfigVar::ConfigVar( const char *name, const char *value, int flags, const char *description,
    //                            float valueMin, float valueMax, argCompletion_t valueCompletion ) {
    //    Init( name, value, flags, description, valueMin, valueMax, NULL, valueCompletion );
    //}
//
    //inline ConfigVar::ConfigVar( const char *name, const char *value, int flags, const char *description,
    //                            const char **valueStrings, argCompletion_t valueCompletion ) {
    //    Init( name, value, flags, description, 1, -1, valueStrings, valueCompletion );
    //}

    inline ConfigVar::ConfigVar(const char *name, const char *value, int flags, const char *description) 
    {
        this->Init(name, value, flags, description, 1, -1, NULL);
    }

    inline ConfigVar::ConfigVar(const char *name, const char *value, int flags, const char *description,
                                float valueMin, float valueMax) 
    {
        this->Init(name, value, flags, description, valueMin, valueMax, NULL);
    }

    inline ConfigVar::ConfigVar(const char *name, const char *value, int flags, const char *description,
                                const char **valueStrings) 
    {
        this->Init(name, value, flags, description, 1, -1, valueStrings);
    }

    namespace ConfigVarSystem 
    {
        void			Init(void);
        void			Shutdown(void);
        bool			IsInitialized(void);

        // Registers a ConfigVar.
        void			Register(ConfigVar* cvar);

        // Finds the ConfigVar with the given name.
        // Returns NULL if there is no ConfigVar with the given name.
        ConfigVar *		Find(const char* name);

        // Sets the value of a ConfigVar by name.
        void			SetCVarString(const char *name, const char *value, int flags = 0 );
        void			SetCVarBool(const char *name, const bool value, int flags = 0 );
        void			SetCVarInteger(const char *name, const int value, int flags = 0 );
        void			SetCVarFloat(const char *name, const float value, int flags = 0 );

        // Gets the value of a ConfigVar by name.
        const char *	GetCVarString(const char *name);
        bool			GetCVarBool(const char *name);
        int				GetCVarInteger(const char *name);
        float			GetCVarFloat(const char *name);

        // Called by the command system when argv(0) doesn't match a known command.
        // Returns true if argv(0) is a variable reference and prints or changes the ConfigVar.
        //bool			Command(const idCmdArgs &args);

        // Command and argument completion using callback for each valid string.
        //void			CommandCompletion(void(*callback)( const char *s));
        //void			ArgCompletion(const char *cmdString, void(*callback)(const char *s)) = 0;

        // Sets/gets/clears modified flags that tell what kind of CVars have changed.
        void			SetModifiedFlags(int flags);
        int				GetModifiedFlags(void);
        void			ClearModifiedFlags(int flags);

        // Resets variables with one of the given flags set.
        void			ResetFlaggedVariables(int flags);

        // Removes auto-completion from the flagged variables.
        void			RemoveFlaggedAutoCompletion(int flags);

        // Writes variables with one of the given flags set to the given file.
        //void			WriteFlaggedVariables(int flags, const char *setCmd, idFile *f);

        // Moves CVars to and from dictionaries.
        //void			MoveCVarsToDict(int flags, idDict & dict, bool onlyModified = false);
        //void			SetCVarsFromDict(const idDict &dict);
    };


    /*
    ===============================================================================
        ConfigVar Registration
        Each DLL using CVars has to declare a private copy of the static variable
        ConfigVar::staticVars like this: ConfigVar * ConfigVar::staticVars = NULL;
        Furthermore ConfigVar::RegisterStaticVars() has to be called after the
        cvarSystem pointer is set when the DLL is first initialized.
    ===============================================================================
    */

    //inline void ConfigVar::Init( const char *name, const char *value, int flags, const char *description,
    //                            float valueMin, float valueMax, const char **valueStrings, argCompletion_t valueCompletion ) {
    //    this->name = name;
    //    this->value = value;
    //    this->flags = flags;
    //    this->description = description;
    //    this->flags = flags | Static;
    //    this->valueMin = valueMin;
    //    this->valueMax = valueMax;
    //    this->valueStrings = valueStrings;
    //    this->valueCompletion = valueCompletion;
    //    this->integerValue = 0;
    //    this->floatValue = 0.0f;
    //    this->internalVar = this;
    //    if ( staticVars != (ConfigVar *)0xFFFFFFFF ) {
    //        this->next = staticVars;
    //        staticVars = this;
    //    } else {
    //        cvarSystem->Register( this );
    //    }
    //}

    inline void ConfigVar::Init(const char *name, const char *value, int flags, const char *description,
                                float valueMin, float valueMax, const char **valueStrings) 
    {
        this->name          = name;
        this->value         = value;
        this->flags         = flags;
        this->description   = description;
        this->flags         = flags | ConfigVarFlags::Static;
        this->valueMin      = valueMin;
        this->valueMax      = valueMax;
        this->valueStrings  = valueStrings;
        this->integerValue  = 0;
        this->floatValue    = 0.0f;
        this->internalVar   = this;

        if (staticVars != (ConfigVar*)0xFFFFFFFF) 
        {
            this->next = staticVars;
            staticVars = this;
        } 
        else 
        {
            ConfigVarSystem::Register(this);
        }
    }

    inline void ConfigVar::RegisterStaticVars(void) 
    {
        if (staticVars != (ConfigVar*)0xFFFFFFFF) 
        {
            for (ConfigVar *cvar = staticVars; cvar; cvar = cvar->next) 
            {
                ConfigVarSystem::Register(cvar);
            }

            staticVars = (ConfigVar*)0xFFFFFFFF;
        }
    }
}