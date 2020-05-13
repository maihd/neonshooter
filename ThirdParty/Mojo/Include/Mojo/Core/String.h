#pragma once

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>   // for va_list

inline namespace Mojo
{
    // Dynamic string
    // Footprint is 8-bytes (32-bits arch) or 16-bytes (64-bits arch)
    struct String
    {
        char*               buffer;                 // Point to LocalBuf() or heap allocated
        int                 capacity : 21;          // Max 2 MB
        int                 localBufSize : 10;      // Max 1023 bytes
        bool                isOwned : 1;            // Set when we have ownership of the pointed data (most common, unless using SetRef() method or StrRef constructor)

    public: // Constants
        static const char* const EmptyBuffer;

    public:
        inline char*        GetCString(void)        { return buffer;              }
        inline const char*  GetCString(void)  const { return buffer;              }
        inline bool         IsEmpty(void)     const { return buffer[0] == 0;      }
        inline int          GetLength(void)   const { return (int)strlen(buffer); }    // by design, allow user to write into the buffer at any time
        inline int          GetCapacity(void) const { return capacity;            }

        inline void         Set(const char* src);
        inline void         Set(const char* src, const char* end);
        inline void         Set(const String& src);

        inline void         SetRef(const char* src);
        int                 SetFormat(const char* fmt, ...);
        int                 SetFormatArgs(const char* fmt, va_list args);
        int                 SetFormatNoGrow(const char* fmt, ...);
        int                 SetFormatArgsNoGrow(const char* fmt, va_list args);

        int                 Append(char c);
        int                 Append(const char* src, const char* end = NULL);
        int                 AppendFormat(const char* fmt, ...);
        int                 AppendFormatArgs(const char* fmt, va_list args);

        int                 AppendFrom(int idx, char c);
        int                 AppendFrom(int idx, const char* src, const char* end = NULL);		// If you know the string GetLength or want to Append from a certain point
        int                 AppendFromFormat(int idx, const char* fmt, ...);
        int                 AppendFromFormatArgs(int idx, const char* fmt, va_list args);

        void                Clear(void);
        void                ShrinkToFit(void);
        void                Reserve(int capacity);
        void                ReserveDiscard(int capacity);

    public:
        inline char&        operator[](int i)                    { return buffer[i]; }
        inline char         operator[](int i) const              { return buffer[i]; }
        //explicit operator const char*() const{ return data; }

        inline              String();
        inline              String(const char* rhs);
        inline String&      operator=(const char* rhs)           { this->Set(rhs); return *this;            }
        inline bool         operator==(const char* rhs) const    { return strcmp(GetCString(), rhs) == 0;   }

        inline              String(const String& rhs);
        inline String&      operator=(const String& rhs)         { this->Set(rhs); return *this;                       }
        inline bool         operator==(const String& rhs) const  { return strcmp(GetCString(), rhs.GetCString()) == 0; }

        // Destructor for all variants
        inline ~String()
        {
            if (this->isOwned && !this->IsUsingLocalBuffer())
            {
                free(this->buffer);
            }

            // Reset fields
            this->buffer = (char*)EmptyBuffer;
            this->capacity = 0;
            this->localBufSize = 0;
            this->isOwned = false;
        }

    public: // Working with local buffer
        inline char*        GetLocalBuffer(void)           { return (char*)this + sizeof(String);                           }
        inline const char*  GetLocalBuffer(void)     const { return (const char*)this + sizeof(String);                     }
        inline bool         IsUsingLocalBuffer(void) const { return buffer == this->GetLocalBuffer() && localBufSize != 0;  }

        // Constructor for StrXXX variants with local buffer
        String(int localBufSize)
        {
            assert(localBufSize < 1024);

            this->buffer        = this->GetLocalBuffer();
            this->buffer[0]     = '\0';
            this->capacity      = localBufSize;
            this->localBufSize  = localBufSize;
            this->isOwned       = 1;
        }
    };

    void String::Set(const char* src)
    {
        // We allow Set(NULL) or via = operator to Clear the string.
        if (src == NULL)
        {
            this->Clear();
            return;
        }

        int buflen = (int)strlen(src) + 1;
        if (this->capacity < buflen)
        {
            this->ReserveDiscard(buflen);
        }
        memcpy(this->buffer, src, buflen);
        this->isOwned = true;
    }

    void String::Set(const char* src, const char* srcEnd)
    {
        assert(src != NULL && srcEnd >= src);

        int buflen = (int)(srcEnd - src) + 1;
        if (this->capacity < buflen)
        {
            this->ReserveDiscard(buflen);
        }

        memcpy(this->buffer, src, buflen - 1);
        this->buffer[buflen - 1] = 0;
        this->isOwned = true;
    }

    void String::Set(const String& src)
    {
        int buffer = src.GetLength() + 1;
        if (this->capacity < buffer)
        {
            this->ReserveDiscard(buffer);
        }

        memcpy(this->buffer, src.GetCString(), buffer);
        this->isOwned = true;
    }

    void String::SetRef(const char* src)
    {
        if (this->isOwned && !this->IsUsingLocalBuffer())
        {
            free(this->buffer);
        }

        this->buffer = src ? (char*)src : (char*)EmptyBuffer;
        this->capacity = 0;
        this->isOwned = false;
    }

    String::String()
    {
        this->buffer = (char*)EmptyBuffer;      // Shared READ-ONLY initial buffer for 0 GetCapacity
        this->capacity = 0;
        this->localBufSize = 0;
        this->isOwned = false;
    }

    String::String(const String& rhs)
        : String()
    {
        this->Set(rhs);
    }

    String::String(const char* rhs)
        : String()
    {
        this->Set(rhs);
    }
}