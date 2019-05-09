#pragma once

#include <stdint.h>

/* Swap bytes in 16-bit value.  */
#define BITSWAP16(x) (((((x) >> 8) & 0xff) | (((x) & 0xff) << 8)))

/* Swap bytes in 32-bit value.  */
#define BITSWAP32(x)        					                \
    ((((x) & 0xff000000u) >> 24) | (((x) & 0x00ff0000u) >> 8)	\
    | (((x) & 0x0000ff00u) << 8) | (((x) & 0x000000ffu) << 24))

/* Swap bytes in 64-bit value.  */
#define BITSWAP64(x)			                \
    ((((x) & 0xff00000000000000ull) >> 56)	    \
    | (((x) & 0x00ff000000000000ull) >> 40)	    \
    | (((x) & 0x0000ff0000000000ull) >> 24)	    \
    | (((x) & 0x000000ff00000000ull) >> 8)	    \
    | (((x) & 0x00000000ff000000ull) << 8)	    \
    | (((x) & 0x0000000000ff0000ull) << 24)	    \
    | (((x) & 0x000000000000ff00ull) << 40)	    \
    | (((x) & 0x00000000000000ffull) << 56))

#if defined(_WIN32) || defined(_ANDROID)
#define LE_TO_NATIVE_16(x) (x)
#define BE_TO_NATIVE_16(x) BITSWAP16(x)
#define LE_TO_NATIVE_32(x) (x)
#define BE_TO_NATIVE_32(x) BITSWAP32(x)
#define LE_TO_NATIVE_64(x) (x)
#define BE_TO_NATIVE_64(x) BITSWAP64(x)
#else
#define LE_TO_NATIVE_16(x) BITSWAP16(x)
#define BE_TO_NATIVE_16(x) (x)
#define LE_TO_NATIVE_32(x) BITSWAP32(x)
#define BE_TO_NATIVE_32(x) (x)
#define LE_TO_NATIVE_64(x) BITSWAP64(x)
#define BE_TO_NATIVE_64(x) (x)
#endif

inline namespace Mojo
{
    // Start point for stream's cursor move around 
    enum struct SeekWhence
    {
        Set     = 0,
        End     = 2,
        Current = 1,
    };
    
    // Everything is a stream of data
    struct Stream
    {
        // Set the cursor position of stream
        virtual int Seek(int count, SeekWhence whence = SeekWhence::Current) = 0;

        // Tell the cursor position of stream
        virtual int Tell(void) = 0;

        // Get size of stream
        virtual int Size(void) = 0;

        // Read content of stream
        virtual int Read(void* buffer, int length) = 0;

        // Write content to stream
        virtual int Write(const void* buffer, int length) = 0;

        // Skip bytes
        inline int Skip(int count)
        {
            if (count > 0)
            {
                return this->Seek(count);
            }
            else
            {
                return this->Tell();
            }
        }

        // Read 16bit signed integer, and dont care about endian
        inline int16_t ReadInt16(void)
        {
            int16_t result;
            return this->Read(&result, sizeof(result)) == sizeof(result) ? result : 0;
        }

        // Read 16bit unsigned integer, and dont care about endian
        inline uint16_t ReadUint16(void)
        {
            uint16_t result;
            return this->Read(&result, sizeof(result)) == sizeof(result) ? result : 0;
        }
        
        // Read 32bit signed integer, and dont care about endian
        inline int32_t ReadInt32(void)
        {
            int32_t result;
            return this->Read(&result, sizeof(result)) == sizeof(result) ? result : 0;
        }

        // Read 32bit unsigned integer, and dont care about endian
        inline uint32_t ReadUint32(void)
        {
            uint32_t result;
            return this->Read(&result, sizeof(result)) == sizeof(result) ? result : 0;
        }
        
        // Read 64bit signed integer, and dont care about endian
        inline int64_t ReadInt64(void)
        {
            int64_t result;
            return this->Read(&result, sizeof(result)) == sizeof(result) ? result : 0;
        }

        // Read 64bit unsigned integer, and dont care about endian
        inline uint64_t ReadUint64(void)
        {
            uint64_t result;
            return this->Read(&result, sizeof(result)) == sizeof(result) ? result : 0;
        }

        // Read little endian 16bit signed integer
        inline int16_t ReadInt16LE(void)
        {
            int16_t result;
            return this->Read(&result, sizeof(result)) == sizeof(result) ? LE_TO_NATIVE_16(result) : 0;
        }

        // Read big endian 16bit signed integer
        inline int16_t ReadInt16BE(void)
        {
            int16_t result;
            return this->Read(&result, sizeof(result)) == sizeof(result) ? BE_TO_NATIVE_16(result) : 0;
        }

        // Read little endian 16bit unsigned integer
        inline uint16_t ReadUint16LE(void)
        {
            uint16_t result;
            return this->Read(&result, sizeof(result)) == sizeof(result) ? LE_TO_NATIVE_16(result) : 0;
        }

        // Read big endian 16bit unsigned integer
        inline uint16_t ReadUint16BE(void)
        {
            uint16_t result;
            return this->Read(&result, sizeof(result)) == sizeof(result) ? BE_TO_NATIVE_16(result) : 0;
        }
        
        // Read little endian 32bit signed integer
        inline int32_t ReadInt32LE(void)
        {
            int32_t result;
            return this->Read(&result, sizeof(result)) == sizeof(result) ? LE_TO_NATIVE_32(result) : 0;
        }

        // Read big endian 32bit signed integer
        inline int32_t ReadInt32BE(void)
        {
            int32_t result;
            return this->Read(&result, sizeof(result)) == sizeof(result) ? BE_TO_NATIVE_32(result) : 0;
        }

        // Read little endian 32bit unsigned integer
        inline uint32_t ReadUint32LE(void)
        {
            uint32_t result;
            return this->Read(&result, sizeof(result)) == sizeof(result) ? LE_TO_NATIVE_32(result) : 0;
        }

        // Read big endian 32bit unsigned integer
        inline uint32_t ReadUint32BE(void)
        {
            uint32_t result;
            return this->Read(&result, sizeof(result)) == sizeof(result) ? BE_TO_NATIVE_32(result) : 0;
        }
        
        // Read little endian 64bit signed integer
        inline int64_t ReadInt64LE(void)
        {
            int64_t result;
            return this->Read(&result, sizeof(result)) == sizeof(result) ? LE_TO_NATIVE_64(result) : 0;
        }

        // Read big endian 64bit signed integer
        inline int64_t ReadInt64BE(void)
        {
            int64_t result;
            return this->Read(&result, sizeof(result)) == sizeof(result) ? BE_TO_NATIVE_64(result) : 0;
        }

        // Read little endian 64bit unsigned integer
        inline uint64_t ReadUint64LE(void)
        {
            uint64_t result;
            return this->Read(&result, sizeof(result)) == sizeof(result) ? LE_TO_NATIVE_64(result) : 0;
        }

        // Read big endian 64bit unsigned integer
        inline uint64_t ReadUint64BE(void)
        {
            uint64_t result;
            return this->Read(&result, sizeof(result)) == sizeof(result) ? BE_TO_NATIVE_64(result) : 0;
        }
    };
}