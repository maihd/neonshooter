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
        char*               data;                   // Point to LocalBuf() or heap allocated
        int                 capacity : 21;          // Max 2 MB
        int                 localBufSize : 10;      // Max 1023 bytes
        bool                isOwned : 1;            // Set when we have ownership of the pointed data (most common, unless using SetRef() method or StrRef constructor)

        static const char*  EmptyBuffer;

    public:
        inline char*        CString(void)           { return data;              }
        inline const char*  CString(void)     const { return data;              }
        inline bool         IsEmpty(void)     const { return data[0] == 0;      }
        inline int          GetLength(void)   const { return (int)strlen(data); }    // by design, allow user to write into the buffer at any time
        inline int          GetCapacity(void) const { return capacity;          }

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
        inline char&        operator[](int i)                    { return data[i]; }
        inline char         operator[](int i) const              { return data[i]; }
        //explicit operator const char*() const{ return data; }

        inline String();
        inline String(const char* rhs);
        inline String&      operator=(const char* rhs)              { Set(rhs); return *this; }
        inline bool         operator==(const char* rhs) const       { return strcmp(CString(), rhs) == 0; }

        inline String(const String& rhs);
        inline String&      operator=(const String& rhs)               { Set(rhs); return *this; }
        inline bool         operator==(const String& rhs) const        { return strcmp(CString(), rhs.CString()) == 0; }

        // Destructor for all variants
        inline ~String()
        {
            if (isOwned && !IsUsingLocalBuffer())
                free(data);
        }

    public:
        inline char*        LocalBuffer(void)              { return (char*)this + sizeof(String);               }
        inline const char*  LocalBuffer(void)        const { return (const char*)this + sizeof(String);         }
        inline bool         IsUsingLocalBuffer(void) const { return data == LocalBuffer() && localBufSize != 0; }

        // Constructor for StrXXX variants with local buffer
        String(int localBufSize)
        {
            assert(localBufSize < 1024);

            data = LocalBuffer();
            data[0] = '\0';
            capacity = localBufSize;
            localBufSize = localBufSize;
            isOwned = 1;
        }
    };

    void    String::Set(const char* src)
    {
        // We allow Set(NULL) or via = operator to Clear the string.
        if (src == NULL)
        {
            Clear();
            return;
        }
        int buf_len = (int)strlen(src)+1;
        if ((int)capacity < buf_len)
            ReserveDiscard(buf_len);
        memcpy(data, src, buf_len);
        isOwned = 1;
    }

    void    String::Set(const char* src, const char* src_end)
    {
        assert(src != NULL && src_end >= src);
        int buf_len = (int)(src_end-src)+1;
        if ((int)capacity < buf_len)
            ReserveDiscard(buf_len);
        memcpy(data, src, buf_len-1);
        data[buf_len-1] = 0;
        isOwned = 1;
    }

    void    String::Set(const String& src)
    {
        int buf_len = (int)strlen(src.CString())+1;
        if ((int)capacity < buf_len)
            ReserveDiscard(buf_len);
        memcpy(data, src.CString(), buf_len);
        isOwned = 1;
    }

    inline void String::SetRef(const char* src)
    {
        if (isOwned && !IsUsingLocalBuffer())
            free(data);
        data = src ? (char*)src : (char*)EmptyBuffer;
        capacity = 0;
        isOwned = 0;
    }

    String::String()
    {
        data = (char*)EmptyBuffer;      // Shared READ-ONLY initial buffer for 0 GetCapacity
        capacity = 0;
        localBufSize = 0;
        isOwned = 0;
    }

    String::String(const String& rhs)
    {
        data = (char*)EmptyBuffer;
        capacity = 0;
        localBufSize = 0;
        isOwned = 0;
        Set(rhs);
    }

    String::String(const char* rhs)
    {
        data = (char*)EmptyBuffer;
        capacity = 0;
        localBufSize = 0;
        isOwned = 0;
        Set(rhs);
    }
}