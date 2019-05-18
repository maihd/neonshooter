#pragma once

#include <Mojo/Core/Types.h>
#include <Mojo/Core/Endian.h>

inline namespace Mojo
{
    //struct StreamOperation;

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

        // Read content of file at given path, with async progress
        //virtual StreamOperation* ReadAsync(void* buffer, int GetLength) = 0;

        // Write content to file at given path, with async progress
        //virtual StreamOperation* WriteAsync(const void* buffer, int GetLength) = 0;

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

        // Read 8bit signed integer
        inline int8 ReadInt8(void)
        {
            int8 result;
            return this->Read(&result, sizeof(result)) == sizeof(result) ? result : 0;
        }

        // Read 8bit unsigned integer
        inline uint8 ReadUint8(void)
        {
            uint8 result;
            return this->Read(&result, sizeof(result)) == sizeof(result) ? result : 0;
        }

        // Read 16bit signed integer, and dont care about endian
        inline int16 ReadInt16(void)
        {
            int16 result;
            return this->Read(&result, sizeof(result)) == sizeof(result) ? result : 0;
        }

        // Read 16bit unsigned integer, and dont care about endian
        inline uint16 ReadUint16(void)
        {
            uint16 result;
            return this->Read(&result, sizeof(result)) == sizeof(result) ? result : 0;
        }
        
        // Read 32bit signed integer, and dont care about endian
        inline int32 ReadInt32(void)
        {
            int32 result;
            return this->Read(&result, sizeof(result)) == sizeof(result) ? result : 0;
        }

        // Read 32bit unsigned integer, and dont care about endian
        inline uint32 ReadUint32(void)
        {
            uint32 result;
            return this->Read(&result, sizeof(result)) == sizeof(result) ? result : 0;
        }
        
        // Read 64bit signed integer, and dont care about endian
        inline int64 ReadInt64(void)
        {
            int64 result;
            return this->Read(&result, sizeof(result)) == sizeof(result) ? result : 0;
        }

        // Read 64bit unsigned integer, and dont care about endian
        inline uint64 ReadUint64(void)
        {
            uint64 result;
            return this->Read(&result, sizeof(result)) == sizeof(result) ? result : 0;
        }

        // Read little endian 16bit signed integer
        inline int16 ReadInt16LE(void)
        {
            int16 result;
            return this->Read(&result, sizeof(result)) == sizeof(result) ? LE_TO_NATIVE_16(result) : 0;
        }

        // Read big endian 16bit signed integer
        inline int16 ReadInt16BE(void)
        {
            int16 result;
            return this->Read(&result, sizeof(result)) == sizeof(result) ? BE_TO_NATIVE_16(result) : 0;
        }

        // Read little endian 16bit unsigned integer
        inline uint16 ReadUint16LE(void)
        {
            uint16 result;
            return this->Read(&result, sizeof(result)) == sizeof(result) ? LE_TO_NATIVE_16(result) : 0;
        }

        // Read big endian 16bit unsigned integer
        inline uint16 ReadUint16BE(void)
        {
            uint16 result;
            return this->Read(&result, sizeof(result)) == sizeof(result) ? BE_TO_NATIVE_16(result) : 0;
        }
        
        // Read little endian 32bit signed integer
        inline int32 ReadInt32LE(void)
        {
            int32 result;
            return this->Read(&result, sizeof(result)) == sizeof(result) ? LE_TO_NATIVE_32(result) : 0;
        }

        // Read big endian 32bit signed integer
        inline int32 ReadInt32BE(void)
        {
            int32 result;
            return this->Read(&result, sizeof(result)) == sizeof(result) ? BE_TO_NATIVE_32(result) : 0;
        }

        // Read little endian 32bit unsigned integer
        inline uint32 ReadUint32LE(void)
        {
            uint32 result;
            return this->Read(&result, sizeof(result)) == sizeof(result) ? LE_TO_NATIVE_32(result) : 0;
        }

        // Read big endian 32bit unsigned integer
        inline uint32 ReadUint32BE(void)
        {
            uint32 result;
            return this->Read(&result, sizeof(result)) == sizeof(result) ? BE_TO_NATIVE_32(result) : 0;
        }
        
        // Read little endian 64bit signed integer
        inline int64 ReadInt64LE(void)
        {
            int64 result;
            return this->Read(&result, sizeof(result)) == sizeof(result) ? LE_TO_NATIVE_64(result) : 0;
        }

        // Read big endian 64bit signed integer
        inline int64 ReadInt64BE(void)
        {
            int64 result;
            return this->Read(&result, sizeof(result)) == sizeof(result) ? BE_TO_NATIVE_64(result) : 0;
        }

        // Read little endian 64bit unsigned integer
        inline uint64 ReadUint64LE(void)
        {
            uint64 result;
            return this->Read(&result, sizeof(result)) == sizeof(result) ? LE_TO_NATIVE_64(result) : 0;
        }

        // Read big endian 64bit unsigned integer
        inline uint64 ReadUint64BE(void)
        {
            uint64 result;
            return this->Read(&result, sizeof(result)) == sizeof(result) ? BE_TO_NATIVE_64(result) : 0;
        }

        // Read single-precision floating point number, dont care endianess
        inline float ReadFloat(void)
        {
            float result;
            return this->Read(&result, sizeof(result)) == sizeof(result) ? result : 0.0f;
        }

        // Read double-precision floating point number, dont care endianess
        inline double ReadDouble(void)
        {
            double result;
            return this->Read(&result, sizeof(result)) == sizeof(result) ? result : 0.0;
        }

        // Read little endian single-precision floating point number
        inline float ReadFloatLE(void)
        {
            float result;
            return this->Read(&result, sizeof(result)) == sizeof(result) ? LE_TO_NATIVE_FLOAT(result) : 0.0f;
        }

        // Read little endian double-precision floating point number
        inline double ReadDoubleLE(void)
        {
            double result;
            return this->Read(&result, sizeof(result)) == sizeof(result) ? LE_TO_NATIVE_DOUBLE(result) : 0.0;
        }

        // Read big endian single-precision floating point number
        inline float ReadFloatBE(void)
        {
            float result;
            return this->Read(&result, sizeof(result)) == sizeof(result) ? BE_TO_NATIVE_FLOAT(result) : 0.0f;
        }

        // Read big endian double-precision floating point number
        inline double ReadDoubleBE(void)
        {
            double result;
            return this->Read(&result, sizeof(result)) == sizeof(result) ? BE_TO_NATIVE_DOUBLE(result) : 0.0;
        }

        // Read 8bit signed integer
        inline bool WriteInt8(int8 x)
        {
            return this->Write(&x, sizeof(x)) == sizeof(x);
        }

        // Read 8bit unsigned integer
        inline bool WriteUint8(uint8 x)
        {
            return this->Write(&x, sizeof(x)) == sizeof(x);
        }

        // Read 16bit signed integer, and dont care about endian
        inline bool WriteInt16(int16 x)
        {
            return this->Write(&x, sizeof(x)) == sizeof(x);
        }

        // Read 16bit unsigned integer, and dont care about endian
        inline bool WriteUint16(uint16 x)
        {
            return this->Write(&x, sizeof(x)) == sizeof(x);
        }

        // Read 32bit signed integer, and dont care about endian
        inline bool WriteInt32(int32 x)
        {
            return this->Write(&x, sizeof(x)) == sizeof(x);
        }

        // Read 32bit unsigned integer, and dont care about endian
        inline bool WriteUint32(uint32 x)
        {
            return this->Write(&x, sizeof(x)) == sizeof(x);
        }

        // Read 64bit signed integer, and dont care about endian
        inline bool WriteInt64(int64 x)
        {
            return this->Write(&x, sizeof(x)) == sizeof(x);
        }

        // Read 64bit unsigned integer, and dont care about endian
        inline bool WriteUint64(uint64 x)
        {
            return this->Write(&x, sizeof(x)) == sizeof(x);
        }

        // Read little endian 16bit signed integer
        inline bool WriteInt16LE(int16 x)
        {
            int16 result = NATIVE_TO_LE_16(x);
            return this->Write(&result, sizeof(result)) == sizeof(result);
        }

        // Read big endian 16bit signed integer
        inline bool WriteInt16BE(int16 x)
        {
            int16 result = NATIVE_TO_BE_16(x);
            return this->Write(&result, sizeof(result)) == sizeof(result);
        }

        // Read little endian 16bit unsigned integer
        inline bool WriteUint16LE(uint16 x)
        {
            uint16 result = NATIVE_TO_LE_16(x);
            return this->Write(&result, sizeof(result)) == sizeof(result);
        }

        // Read big endian 16bit unsigned integer
        inline bool WriteUint16BE(uint16 x)
        {
            uint16 result = NATIVE_TO_BE_16(x);
            return this->Write(&result, sizeof(result)) == sizeof(result);
        }

        // Read little endian 32bit signed integer
        inline bool WriteInt32LE(int32 x)
        {
            int32 result = NATIVE_TO_LE_32(x);
            return this->Write(&result, sizeof(result)) == sizeof(result);
        }

        // Read big endian 32bit signed integer
        inline bool WriteInt32BE(int32 x)
        {
            int32 result = NATIVE_TO_BE_32(x);
            return this->Write(&result, sizeof(result)) == sizeof(result);
        }

        // Read little endian 32bit unsigned integer
        inline bool WriteUint32LE(uint32 x)
        {
            uint32 result = NATIVE_TO_LE_32(x);
            return this->Write(&result, sizeof(result)) == sizeof(result);
        }

        // Read big endian 32bit unsigned integer
        inline bool WriteUint32BE(uint32 x)
        {
            uint32 result = NATIVE_TO_BE_32(x);
            return this->Write(&result, sizeof(result)) == sizeof(result);
        }

        // Read little endian 64bit signed integer
        inline bool WriteInt64LE(int64 x)
        {
            int64 result = NATIVE_TO_LE_64(x);
            return this->Write(&result, sizeof(result)) == sizeof(result);
        }

        // Read big endian 64bit signed integer
        inline bool WriteInt64BE(int64 x)
        {
            int64 result = NATIVE_TO_BE_64(x);
            return this->Write(&result, sizeof(result)) == sizeof(result);
        }

        // Read little endian 64bit unsigned integer
        inline bool WriteUint64LE(uint64 x)
        {
            uint64 result = NATIVE_TO_LE_64(x);
            return this->Write(&result, sizeof(result)) == sizeof(result);
        }

        // Read big endian 64bit unsigned integer
        inline bool WriteUint64BE(uint64 x)
        {
            uint64 result = NATIVE_TO_BE_64(x);
            return this->Write(&result, sizeof(result)) == sizeof(result);
        }

        // Read single-precision floating point number, dont care endianess
        inline bool WriteFloat(float x)
        {
            return this->Write(&x, sizeof(x)) == sizeof(x);
        }

        // Read double-precision floating point number, dont care endianess
        inline bool WriteDouble(double x)
        {
            return this->Write(&x, sizeof(x)) == sizeof(x);
        }

        // Read little endian single-precision floating point number
        inline bool WriteFloatLE(float x)
        {
            float result = NATIVE_TO_LE_FLOAT(x);
            return this->Write(&result, sizeof(result)) == sizeof(result);
        }

        // Read little endian double-precision floating point number
        inline bool WriteDoubleLE(double x)
        {
            double result = NATIVE_TO_LE_DOUBLE(x);
            return this->Write(&result, sizeof(result)) == sizeof(result);
        }

        // Read big endian single-precision floating point number
        inline bool WriteFloatBE(float x)
        {
            float result = NATIVE_TO_BE_FLOAT(x);
            return this->Write(&result, sizeof(result)) == sizeof(result);
        }

        // Read big endian double-precision floating point number
        inline bool WriteDoubleBE(double x)
        {
            double result = NATIVE_TO_BE_DOUBLE(x);
            return this->Write(&result, sizeof(result)) == sizeof(result);
        }
    };

    // Previewing
    //struct StreamOperation
    //{
    //    Stream* stream;
    //
    //    bool isDone;
    //    bool isSuccess;
    //
    //    //int   const priority;
    //    float progress;
    //
    //    int         GetLength;
    //    const void* buffer;
    //
    //    StreamOperation(void)
    //        : stream(0)
    //        , isDone(false)
    //        , isSuccess(false)
    //        , progress(0.0f)
    //        , GetLength(0)
    //        , buffer(0)
    //    {
    //    }
    //
    //    void Wait(void);
    //};
}