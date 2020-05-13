#include <Mojo/Core/String.h>

#include <stdio.h>

// On some platform vsnprintf() takes va_list by reference and modifies it.
// va_copy is the 'correct' way to copy a va_list but Visual Studio prior to 2013 doesn't have it.
#ifndef va_copy
#define va_copy(dest, src) (dest = src)
#endif

namespace Mojo
{
    // Static IsEmpty buffer we can point to for IsEmpty strings
    // Pointing to a literal increases the like-hood of getting a crash if someone attempts to write in the IsEmpty string buffer. 
    const char* const String::EmptyBuffer = (char*)"\0NULL";

    // Clear 
    void String::Clear()
    {
        if (isOwned && !IsUsingLocalBuffer())
        {
            free(buffer);
        }
        
        if (localBufSize)
        {
            buffer = GetLocalBuffer();
            buffer[0] = '\0';
            capacity = localBufSize;
            isOwned = 1;
        }
        else
        {
            buffer = (char*)EmptyBuffer;
            capacity = 0;
            isOwned = 0;
        }
    }

    // Reserve memory, preserving the current of the buffer
    void String::Reserve(int newCapacity)
    {
        if (newCapacity <= capacity)
        {
            return;
        }

        char* newData;
        if (newCapacity < localBufSize)
        {
            // Disowned -> LocalBuf
            newData = this->GetLocalBuffer();
            newCapacity = localBufSize;
        }
        else
        {
            // Disowned or LocalBuf -> Heap
            newData = (char*)malloc(newCapacity * sizeof(char));
        }
        strcpy(newData, buffer);

        if (isOwned && !this->IsUsingLocalBuffer())
        {
            free(buffer);
        }

        buffer = newData;
        capacity = newCapacity;
    }

    // Reserve memory, discarding the current of the buffer (if we expect to be fully rewritten)
    void String::ReserveDiscard(int newCapacity)
    {
        if (newCapacity <= capacity)
        {
            return;
        }

        if (isOwned && !this->IsUsingLocalBuffer())
        {
            free(buffer);
        }

        if (newCapacity < localBufSize)
        {
            // Disowned -> LocalBuf
            buffer = this->GetLocalBuffer();
            capacity = localBufSize;
        }
        else
        {
            // Disowned or LocalBuf -> Heap
            buffer = (char*)malloc(newCapacity * sizeof(char));
            capacity = newCapacity;
        }
    }

    void String::ShrinkToFit()
    {
        if (!isOwned || this->IsUsingLocalBuffer())
        {
            return;
        }

        int newCapacity = this->GetLength() + 1;
        if (capacity <= newCapacity)
        {
            return;
        }

        char* newData = (char*)malloc(newCapacity * sizeof(char));
        memcpy(newData, buffer, newCapacity);
        free(buffer);

        buffer = newData;
        capacity = newCapacity;
    }

    // FIXME: merge SetFormatArgs() and AppendFormatArgs()?
    int String::SetFormatArgs(const char* fmt, va_list args)
    {
        // Needed for portability on platforms where va_list are passed by reference and modified by functions
        va_list args2;
        va_copy(args2, args);

        // MSVC returns -1 on overflow when writing, which forces us to do two passes
        // FIXME-OPT: Find a way around that.
#ifdef _MSC_VER
        int len = vsnprintf(NULL, 0, fmt, args);
        assert(len >= 0);

        if (capacity < len + 1)
        {
            this->ReserveDiscard(len + 1);
        }

        len = vsnprintf(buffer, len + 1, fmt, args2);
#else
    // First try
        int len = vsnprintf(isOwned ? data : NULL, isOwned ? capacity : 0, fmt, args);
        assert(len >= 0);

        if (capacity < len + 1)
        {
            this->ReserveDiscard(len + 1);
            len = vsnprintf(data, len + 1, fmt, args2);
        }
#endif

        isOwned = 1;
        return len;
    }

    int String::SetFormat(const char* fmt, ...)
    {
        va_list args;
        va_start(args, fmt);
        int len = this->SetFormatArgs(fmt, args);
        va_end(args);
        return len;
    }

    int String::SetFormatArgsNoGrow(const char* fmt, va_list args)
    {
        int w = vsnprintf(buffer, capacity, fmt, args);
        buffer[capacity - 1] = 0;
        isOwned = 1;

        return (w == -1) ? capacity - 1 : w;
    }

    int String::SetFormatNoGrow(const char* fmt, ...)
    {
        va_list args;
        va_start(args, fmt);
        int len = SetFormatArgsNoGrow(fmt, args);
        va_end(args);
        return len;
    }

    int String::AppendFrom(int idx, char c)
    {
        int add_len = 1;
        if (capacity < idx + add_len + 1)
        {
            Reserve(idx + add_len + 1);
        }

        buffer[idx] = c;
        buffer[idx + add_len] = 0;
        isOwned = 1;
        return add_len;
    }

    int String::AppendFrom(int idx, const char* s, const char* s_end)
    {
        if (!s_end)
        {
            s_end = s + strlen(s);
        }
        
        int add_len = (int)(s_end - s);
        if (capacity < idx + add_len + 1)
        {
            Reserve(idx + add_len + 1);
        }
        
        memcpy(buffer + idx, (const void*)s, add_len);
        buffer[idx + add_len] = 0; // Our source data isn't necessarily zero-terminated
        isOwned = 1;
        return add_len;
    }

    // FIXME: merge SetFormatArgs() and AppendFormatArgs()?
    int String::AppendFromFormatArgs(int idx, const char* fmt, va_list args)
    {
        // Needed for portability on platforms where va_list are passed by reference and modified by functions
        va_list args2;
        va_copy(args2, args);

        // MSVC returns -1 on overflow when writing, which forces us to do two passes
        // FIXME-OPT: Find a way around that.
#ifdef _MSC_VER
        int add_len = vsnprintf(NULL, 0, fmt, args);
        assert(add_len >= 0);

        if (capacity < idx + add_len + 1)
        {
            Reserve(idx + add_len + 1);
        }
        
        add_len = vsnprintf(buffer + idx, add_len + 1, fmt, args2);
#else
    // First try
        int add_len = vsnprintf(isOwned ? data + idx : NULL, isOwned ? capacity - idx : 0, fmt, args);
        assert(add_len >= 0);

        if (capacity < idx + add_len + 1)
        {
            Reserve(idx + add_len + 1);
            add_len = vsnprintf(data + idx, add_len + 1, fmt, args2);
        }
#endif

        isOwned = 1;
        return add_len;
    }

    int String::AppendFromFormat(int idx, const char* fmt, ...)
    {
        va_list args;
        va_start(args, fmt);
        int len = AppendFromFormatArgs(idx, fmt, args);
        va_end(args);
        return len;
    }

    int String::Append(char c)
    {
        int cur_len = GetLength();
        return AppendFrom(cur_len, c);
    }

    int String::Append(const char* s, const char* end)
    {
        int cur_len = GetLength();
        return AppendFrom(cur_len, s, end);
    }

    int String::AppendFormatArgs(const char* fmt, va_list args)
    {
        int cur_len = GetLength();
        return AppendFromFormatArgs(cur_len, fmt, args);
    }

    int String::AppendFormat(const char* fmt, ...)
    {
        va_list args;
        va_start(args, fmt);
        int len = AppendFormatArgs(fmt, args);
        va_end(args);
        return len;
    }
}
