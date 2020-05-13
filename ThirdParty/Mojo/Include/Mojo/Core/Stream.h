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
        inline I8 ReadInt8(void)
        {
            I8 result;
            return this->Read(&result, sizeof(result)) == sizeof(result) ? result : 0;
        }

        // Read 8bit unsigned integer
        inline U8 ReadUint8(void)
        {
            U8 result;
            return this->Read(&result, sizeof(result)) == sizeof(result) ? result : 0;
        }

        // Read 16bit signed integer, and dont care about endian
        inline I16 ReadInt16(void)
        {
            I16 result;
            return this->Read(&result, sizeof(result)) == sizeof(result) ? result : 0;
        }

        // Read 16bit unsigned integer, and dont care about endian
        inline U16 ReadUint16(void)
        {
            U16 result;
            return this->Read(&result, sizeof(result)) == sizeof(result) ? result : 0;
        }
        
        // Read 32bit signed integer, and dont care about endian
        inline I32 ReadInt32(void)
        {
            I32 result;
            return this->Read(&result, sizeof(result)) == sizeof(result) ? result : 0;
        }

        // Read 32bit unsigned integer, and dont care about endian
        inline U32 ReadUint32(void)
        {
            U32 result;
            return this->Read(&result, sizeof(result)) == sizeof(result) ? result : 0;
        }
        
        // Read 64bit signed integer, and dont care about endian
        inline I64 ReadInt64(void)
        {
            I64 result;
            return this->Read(&result, sizeof(result)) == sizeof(result) ? result : 0;
        }

        // Read 64bit unsigned integer, and dont care about endian
        inline U64 ReadUint64(void)
        {
            U64 result;
            return this->Read(&result, sizeof(result)) == sizeof(result) ? result : 0;
        }

        // Read little endian 16bit signed integer
        inline I16 ReadInt16LE(void)
        {
            I16 result;
            return this->Read(&result, sizeof(result)) == sizeof(result) ? LE_TO_NATIVE_16(result) : 0;
        }

        // Read big endian 16bit signed integer
        inline I16 ReadInt16BE(void)
        {
            I16 result;
            return this->Read(&result, sizeof(result)) == sizeof(result) ? BE_TO_NATIVE_16(result) : 0;
        }

        // Read little endian 16bit unsigned integer
        inline U16 ReadUint16LE(void)
        {
            U16 result;
            return this->Read(&result, sizeof(result)) == sizeof(result) ? LE_TO_NATIVE_16(result) : 0;
        }

        // Read big endian 16bit unsigned integer
        inline U16 ReadUint16BE(void)
        {
            U16 result;
            return this->Read(&result, sizeof(result)) == sizeof(result) ? BE_TO_NATIVE_16(result) : 0;
        }
        
        // Read little endian 32bit signed integer
        inline I32 ReadInt32LE(void)
        {
            I32 result;
            return this->Read(&result, sizeof(result)) == sizeof(result) ? LE_TO_NATIVE_32(result) : 0;
        }

        // Read big endian 32bit signed integer
        inline I32 ReadInt32BE(void)
        {
            I32 result;
            return this->Read(&result, sizeof(result)) == sizeof(result) ? BE_TO_NATIVE_32(result) : 0;
        }

        // Read little endian 32bit unsigned integer
        inline U32 ReadUint32LE(void)
        {
            U32 result;
            return this->Read(&result, sizeof(result)) == sizeof(result) ? LE_TO_NATIVE_32(result) : 0;
        }

        // Read big endian 32bit unsigned integer
        inline U32 ReadUint32BE(void)
        {
            U32 result;
            return this->Read(&result, sizeof(result)) == sizeof(result) ? BE_TO_NATIVE_32(result) : 0;
        }
        
        // Read little endian 64bit signed integer
        inline I64 ReadInt64LE(void)
        {
            I64 result;
            return this->Read(&result, sizeof(result)) == sizeof(result) ? LE_TO_NATIVE_64(result) : 0;
        }

        // Read big endian 64bit signed integer
        inline I64 ReadInt64BE(void)
        {
            I64 result;
            return this->Read(&result, sizeof(result)) == sizeof(result) ? BE_TO_NATIVE_64(result) : 0;
        }

        // Read little endian 64bit unsigned integer
        inline U64 ReadUint64LE(void)
        {
            U64 result;
            return this->Read(&result, sizeof(result)) == sizeof(result) ? LE_TO_NATIVE_64(result) : 0;
        }

        // Read big endian 64bit unsigned integer
        inline U64 ReadUint64BE(void)
        {
            U64 result;
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
        inline bool WriteInt8(I8 x)
        {
            return this->Write(&x, sizeof(x)) == sizeof(x);
        }

        // Read 8bit unsigned integer
        inline bool WriteUint8(U8 x)
        {
            return this->Write(&x, sizeof(x)) == sizeof(x);
        }

        // Read 16bit signed integer, and dont care about endian
        inline bool WriteInt16(I16 x)
        {
            return this->Write(&x, sizeof(x)) == sizeof(x);
        }

        // Read 16bit unsigned integer, and dont care about endian
        inline bool WriteUint16(U16 x)
        {
            return this->Write(&x, sizeof(x)) == sizeof(x);
        }

        // Read 32bit signed integer, and dont care about endian
        inline bool WriteInt32(I32 x)
        {
            return this->Write(&x, sizeof(x)) == sizeof(x);
        }

        // Read 32bit unsigned integer, and dont care about endian
        inline bool WriteUint32(U32 x)
        {
            return this->Write(&x, sizeof(x)) == sizeof(x);
        }

        // Read 64bit signed integer, and dont care about endian
        inline bool WriteInt64(I64 x)
        {
            return this->Write(&x, sizeof(x)) == sizeof(x);
        }

        // Read 64bit unsigned integer, and dont care about endian
        inline bool WriteUint64(U64 x)
        {
            return this->Write(&x, sizeof(x)) == sizeof(x);
        }

        // Read little endian 16bit signed integer
        inline bool WriteInt16LE(I16 x)
        {
            I16 result = NATIVE_TO_LE_16(x);
            return this->Write(&result, sizeof(result)) == sizeof(result);
        }

        // Read big endian 16bit signed integer
        inline bool WriteInt16BE(I16 x)
        {
            I16 result = NATIVE_TO_BE_16(x);
            return this->Write(&result, sizeof(result)) == sizeof(result);
        }

        // Read little endian 16bit unsigned integer
        inline bool WriteUint16LE(U16 x)
        {
            U16 result = NATIVE_TO_LE_16(x);
            return this->Write(&result, sizeof(result)) == sizeof(result);
        }

        // Read big endian 16bit unsigned integer
        inline bool WriteUint16BE(U16 x)
        {
            U16 result = NATIVE_TO_BE_16(x);
            return this->Write(&result, sizeof(result)) == sizeof(result);
        }

        // Read little endian 32bit signed integer
        inline bool WriteInt32LE(I32 x)
        {
            I32 result = NATIVE_TO_LE_32(x);
            return this->Write(&result, sizeof(result)) == sizeof(result);
        }

        // Read big endian 32bit signed integer
        inline bool WriteInt32BE(I32 x)
        {
            I32 result = NATIVE_TO_BE_32(x);
            return this->Write(&result, sizeof(result)) == sizeof(result);
        }

        // Read little endian 32bit unsigned integer
        inline bool WriteUint32LE(U32 x)
        {
            U32 result = NATIVE_TO_LE_32(x);
            return this->Write(&result, sizeof(result)) == sizeof(result);
        }

        // Read big endian 32bit unsigned integer
        inline bool WriteUint32BE(U32 x)
        {
            U32 result = NATIVE_TO_BE_32(x);
            return this->Write(&result, sizeof(result)) == sizeof(result);
        }

        // Read little endian 64bit signed integer
        inline bool WriteInt64LE(I64 x)
        {
            I64 result = NATIVE_TO_LE_64(x);
            return this->Write(&result, sizeof(result)) == sizeof(result);
        }

        // Read big endian 64bit signed integer
        inline bool WriteInt64BE(I64 x)
        {
            I64 result = NATIVE_TO_BE_64(x);
            return this->Write(&result, sizeof(result)) == sizeof(result);
        }

        // Read little endian 64bit unsigned integer
        inline bool WriteUint64LE(U64 x)
        {
            U64 result = NATIVE_TO_LE_64(x);
            return this->Write(&result, sizeof(result)) == sizeof(result);
        }

        // Read big endian 64bit unsigned integer
        inline bool WriteUint64BE(U64 x)
        {
            U64 result = NATIVE_TO_BE_64(x);
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
