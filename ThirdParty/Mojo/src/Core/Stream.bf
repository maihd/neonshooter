using System;

namespace Mojo
{
    //struct StreamOperation;

    // Start point for stream's cursor move around 
    public enum SeekWhence
    {
        Set     = 0,
        End     = 2,
        Current = 1,
    }
    
    // Everything is a stream of data
    public abstract class Stream
    {
        // Set the cursor position of stream
        public abstract int Seek(int count, SeekWhence whence = SeekWhence.Current);

        // Tell the cursor position of stream
        public abstract int Tell();

        // Get size of stream
        public abstract int Size();

        // Read content of stream
        public abstract int Read(void* buffer, int length);

        // Write content to stream
        public abstract int Write(void* buffer, int length);

        // Read content of file at given path, with async progress
        //virtual StreamOperation* ReadAsync(void* buffer, int GetLength) = 0;

        // Write content to file at given path, with async progress
        //virtual StreamOperation* WriteAsync(const void* buffer, int GetLength) = 0;

        // Skip bytes
		[Inline]
        public int Skip(int count)
        {
            if (count > 0)
            {
                return Seek(count);
            }
            else
            {
                return Tell();
            }
        }

        // Read 8bit signed integer
        [Inline]
        public int8 ReadInt8()
        {
            int8 result = ?;
            return Read(&result, sizeof(int8)) == sizeof(int8) ? result : 0;
        }

        // Read 8bit unsigned integer
        [Inline]
        public uint8 ReadUint8()
        {
            uint8 result = ?;
            return Read(&result, sizeof(uint8)) == sizeof(uint8) ? result : 0;
        }

        // Read 16bit signed integer, and dont care about endian
        [Inline]
        public int16 ReadInt16()
        {
            int16 result = ?;
            return Read(&result, sizeof(int16)) == sizeof(int16) ? result : 0;
        }

        // Read 16bit unsigned integer, and dont care about endian
        [Inline]
        public uint16 ReadUint16()
        {
            uint16 result = ?;
            return Read(&result, sizeof(uint16)) == sizeof(uint16) ? result : 0;
        }
        
        // Read 32bit signed integer, and dont care about endian
        [Inline]
        public int32 ReadInt32()
        {
            int32 result = ?;
            return Read(&result, sizeof(int32)) == sizeof(int32) ? result : 0;
        }

        // Read 32bit unsigned integer, and dont care about endian
        [Inline]
        public uint32 ReadUint32()
        {
            uint32 result = ?;
            return Read(&result, sizeof(uint32)) == sizeof(uint32) ? result : 0;
        }
        
        // Read 64bit signed integer, and dont care about endian
        [Inline]
        public int64 ReadInt64()
        {
            int64 result = ?;
            return Read(&result, sizeof(int64)) == sizeof(int64) ? result : 0;
        }

        // Read 64bit unsigned integer, and dont care about endian
        [Inline]
        public uint64 ReadUint64()
        {
            uint64 result = ?;
            return Read(&result, sizeof(uint64)) == sizeof(uint64) ? result : 0;
        }

        // Read little endian 16bit signed integer
        [Inline]
        public int16 ReadInt16LE()
        {
            int16 result = ?;
            return Read(&result, sizeof(int16)) == sizeof(int16) ? Endian.FromLE16(result) : 0;
        }

        // Read big endian 16bit signed integer
        [Inline]
        public int16 ReadInt16BE()
        {
            int16 result = ?;
            return Read(&result, sizeof(int16)) == sizeof(int16) ? Endian.FromBE16(result) : 0;
        }

        // Read little endian 16bit unsigned integer
        [Inline]
        public uint16 ReadUint16LE()
        {
            uint16 result = ?;
            return Read(&result, sizeof(uint16)) == sizeof(uint16) ? Endian.FromLE16(result) : 0;
        }

        // Read big endian 16bit unsigned integer
        [Inline]
        public uint16 ReadUint16BE()
        {
            uint16 result = ?;
            return Read(&result, sizeof(uint16)) == sizeof(uint16) ? Endian.FromBE16(result) : 0;
        }
        
        // Read little endian 32bit signed integer
        [Inline]
        public int32 ReadInt32LE()
        {
            int32 result = ?;
            return Read(&result, sizeof(int32)) == sizeof(int32) ? Endian.FromLE32(result) : 0;
        }

        // Read big endian 32bit signed integer
        [Inline]
        public int32 ReadInt32BE()
        {
            int32 result = ?;
            return Read(&result, sizeof(int32)) == sizeof(int32) ? Endian.FromBE32(result) : 0;
        }

        // Read little endian 32bit unsigned integer
        [Inline]
        public uint32 ReadUint32LE()
        {
            uint32 result = ?;
            return Read(&result, sizeof(uint32)) == sizeof(uint32) ? Endian.FromLE32(result) : 0;
        }

        // Read big endian 32bit unsigned integer
        [Inline]
        public uint32 ReadUint32BE()
        {
            uint32 result = ?;
            return Read(&result, sizeof(uint32)) == sizeof(uint32) ? Endian.FromBE32(result) : 0;
        }
        
        // Read little endian 64bit signed integer
        [Inline]
        public int64 ReadInt64LE()
        {
            int64 result = ?;
            return Read(&result, sizeof(int64)) == sizeof(int64) ? Endian.FromLE64(result) : 0;
        }

        // Read big endian 64bit signed integer
        [Inline]
        public int64 ReadInt64BE()
        {
            int64 result = ?;
            return Read(&result, sizeof(int64)) == sizeof(int64) ? Endian.FromBE64(result) : 0;
        }

        // Read little endian 64bit unsigned integer
        [Inline]
        public uint64 ReadUint64LE()
        {
            uint64 result = ?;
            return Read(&result, sizeof(uint64)) == sizeof(uint64) ? Endian.FromLE64(result) : 0;
        }

        // Read big endian 64bit unsigned integer
        [Inline]
        public uint64 ReadUint64BE()
        {
            uint64 result = ?;
            return Read(&result, sizeof(uint64)) == sizeof(uint64) ? Endian.FromBE64(result) : 0;
        }

        // Read single-precision floating point number, dont care endianess
        [Inline]
        public float ReadFloat()
        {
            float result = ?;
            return Read(&result, sizeof(float)) == sizeof(float) ? result : 0.0f;
        }

        // Read double-precision floating point number, dont care endianess
        [Inline]
        public double ReadDouble()
        {
            double result = ?;
            return Read(&result, sizeof(double)) == sizeof(double) ? result : 0.0;
        }

        // Read little endian single-precision floating point number
        [Inline]
        public float ReadFloatLE()
        {
            float result = ?;
            return Read(&result, sizeof(float)) == sizeof(float) ? Endian.FromLE32(result) : 0.0f;
        }

        // Read little endian double-precision floating point number
        [Inline]
        public double ReadDoubleLE()
        {
            double result = ?;
            return Read(&result, sizeof(double)) == sizeof(double) ? Endian.FromLE64(result) : 0.0;
        }

        // Read big endian single-precision floating point number
        [Inline]
        public float ReadFloatBE()
        {
            float result = ?;
            return Read(&result, sizeof(float)) == sizeof(float) ? Endian.FromBE32(result) : 0.0f;
        }

        // Read big endian double-precision floating point number
        [Inline]
        public double ReadDoubleBE()
        {
            double result = ?;
            return Read(&result, sizeof(double)) == sizeof(double) ? Endian.FromBE64(result) : 0.0;
        }

        // Read 8bit signed integer
        [Inline]
        public bool WriteInt8(int8 x)
        {
            return Write(&x, sizeof(int8)) == sizeof(int8);
        }

        // Read 8bit unsigned integer
        [Inline]
        public bool WriteUint8(uint8 x)
        {
            return Write(&x, sizeof(uint8)) == sizeof(uint8);
        }

        // Read 16bit signed integer, and dont care about endian
        [Inline]
        public bool WriteInt16(int16 x)
        {
            return Write(&x, sizeof(int16)) == sizeof(int16);
        }

        // Read 16bit unsigned integer, and dont care about endian
        [Inline]
        public bool WriteUint16(uint16 x)
        {
            return Write(&x, sizeof(uint16)) == sizeof(uint16);
        }

        // Read 32bit signed integer, and dont care about endian
        [Inline]
        public bool WriteInt32(int32 x)
        {
            return Write(&x, sizeof(int32)) == sizeof(int32);
        }

        // Read 32bit unsigned integer, and dont care about endian
        [Inline]
        public bool WriteUint32(uint32 x)
        {
            return Write(&x, sizeof(uint32)) == sizeof(uint32);
        }

        // Read 64bit signed integer, and dont care about endian
        [Inline]
        public bool WriteInt64(int64 x)
        {
            return Write(&x, sizeof(int64)) == sizeof(int64);
        }

        // Read 64bit unsigned integer, and dont care about endian
        [Inline]
        public bool WriteUint64(uint64 x)
        {
            return Write(&x, sizeof(uint64)) == sizeof(uint64);
        }

        // Read little endian 16bit signed integer
        [Inline]
        public bool WriteInt16LE(int16 x)
        {
            let result = Endian.ToLE16(x);
            return Write(&result, sizeof(int16)) == sizeof(int16);
        }

        // Read big endian 16bit signed integer
        [Inline]
        public bool WriteInt16BE(int16 x)
        {
            let result = Endian.ToBE16(x);
            return Write(&result, sizeof(int16)) == sizeof(int16);
        }

        // Read little endian 16bit unsigned integer
        [Inline]
        public bool WriteUint16LE(uint16 x)
        {
            let result = Endian.ToLE16(x);
            return Write(&result, sizeof(int16)) == sizeof(int16);
        }

        // Read big endian 16bit unsigned integer
        [Inline]
        public bool WriteUint16BE(int16 x)
        {
            let result = Endian.ToBE16(x);
            return Write(&result, sizeof(int16)) == sizeof(int16);
        }

        // Read little endian 32bit signed integer
        [Inline]
        public bool WriteInt32LE(int32 x)
        {
            let result = Endian.ToLE32(x);
            return Write(&result, sizeof(int32)) == sizeof(int32);
        }

        // Read big endian 32bit signed integer
        [Inline]
        public bool WriteInt32BE(int32 x)
        {
            let result = Endian.ToBE32(x);
            return Write(&result, sizeof(int32)) == sizeof(int32);
        }

        // Read little endian 32bit unsigned integer
        [Inline]
        public bool WriteUint32LE(uint32 x)
        {
            let result = Endian.ToLE32(x);
            return Write(&result, sizeof(uint32)) == sizeof(uint32);
        }

        // Read big endian 32bit unsigned integer
        [Inline]
        public bool WriteUint32BE(uint32 x)
        {
            let result = Endian.ToBE32(x);
            return Write(&result, sizeof(uint32)) == sizeof(uint32);
        }

        // Read little endian 64bit signed integer
        [Inline]
        public bool WriteInt64LE(int64 x)
        {
            let result = Endian.ToLE64(x);
            return Write(&result, sizeof(int64)) == sizeof(int64);
        }

        // Read big endian 64bit signed integer
        [Inline]
        public bool WriteInt64BE(int64 x)
        {
            let result = Endian.ToBE64(x);
            return Write(&result, sizeof(int64)) == sizeof(int64);
        }

        // Read little endian 64bit unsigned integer
        [Inline]
        public bool WriteUint64LE(uint64 x)
        {
            let result = Endian.ToLE64(x);
            return Write(&result, sizeof(uint64)) == sizeof(uint64);
        }

        // Read big endian 64bit unsigned integer
        [Inline]
        public bool WriteUint64BE(uint64 x)
        {
            let result = Endian.ToBE64(x);
            return Write(&result, sizeof(uint64)) == sizeof(uint64);
        }

        // Read single-precision floating point number, dont care endianess
        [Inline]
        public bool WriteFloat(float x)
        {
            return Write(&x, sizeof(float)) == sizeof(float);
        }

        // Read double-precision floating point number, dont care endianess
        [Inline]
        public bool WriteDouble(double x)
        {
            return Write(&x, sizeof(double)) == sizeof(double);
        }

        // Read little endian single-precision floating point number
        [Inline]
        public bool WriteFloatLE(float x)
        {
            let result = Endian.ToLE32(x);
            return Write(&result, sizeof(float)) == sizeof(float);
        }

        // Read little endian double-precision floating point number
        [Inline]
        public bool WriteDoubleLE(double x)
        {
            let result = Endian.ToLE64(x);
            return Write(&result, sizeof(double)) == sizeof(double);
        }

        // Read big endian single-precision floating point number
        [Inline]
        public bool WriteFloatBE(float x)
        {
            let result = Endian.ToBE64(x);
            return Write(&result, sizeof(float)) == sizeof(float);
        }

        // Read big endian double-precision floating point number
        [Inline]
        public bool WriteDoubleBE(double x)
        {
            let result = Endian.ToBE64(x);
            return Write(&result, sizeof(double)) == sizeof(double);
        }
    }

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