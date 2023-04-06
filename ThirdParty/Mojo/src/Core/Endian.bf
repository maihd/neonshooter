namespace Mojo;

using System;

public static class Endian
{
	/* Swap bytes in 16-bit value.  */
	[Inline]
	public static uint16 BitSwap16(uint16 x)
	{
		return (((x >> 8) & 0xff) | (((x) & 0xff) << 8));
	}

	/* Swap bytes in 32-bit value.  */
	[Inline]
	public static uint32 BitSwap32(uint32 x)
	{
		return ((((x) & 0xff000000u) >> 24) | (((x) & 0x00ff0000u) >>  8)
			 |  (((x) & 0x0000ff00u) <<  8) | (((x) & 0x000000ffu) << 24));
	}

	/* Swap bytes in 32-bit value.  */
	[Inline]
	public static uint64 BitSwap64(uint64 x)
	{
		return ((((x) & 0xff00000000000000UL) >> 56)
			 |  (((x) & 0x00ff000000000000UL) >> 40)
			 |  (((x) & 0x0000ff0000000000UL) >> 24)
			 |  (((x) & 0x000000ff00000000UL) >>  8)
			 |  (((x) & 0x00000000ff000000UL) <<  8)
			 |  (((x) & 0x0000000000ff0000UL) << 24)
			 |  (((x) & 0x000000000000ff00UL) << 40)
			 |  (((x) & 0x00000000000000ffUL) << 56));
	}

	[Inline]
	public static float SwapFloat(float x)
	{
	    let u = BitSwap32(BitConverter.Convert<float, uint32>(x));
	    return BitConverter.Convert<uint32, float>(u);
	}

	[Inline]
	public static double SwapDouble(double x)
	{
	    let u = BitSwap64(BitConverter.Convert<double, uint64>(x));
		return BitConverter.Convert<uint64, double>(u);
	}
	
	#if BF_PLATFORM_WINDOWS || BF_PLATFORM_LINUX
	public static T FromLE16<T>(T x)
	{
		return x;
	}

	public static T FromBE16<T>(T x)
	{
		let swapped = BitSwap16(BitConverter.Convert<T, uint16>(x));
		return BitConverter.Convert<uint16, T>(swapped);
	}

	public static T FromLE32<T>(T x)
	{
		return x;
	}

	public static T FromBE32<T>(T x)
	{
		let swapped = BitSwap32(BitConverter.Convert<T, uint32>(x));
		return BitConverter.Convert<uint32, T>(swapped);
	}

	public static T FromLE64<T>(T x)
	{
		return x;
	}

	public static T FromBE64<T>(T x)
	{
		let swapped = BitSwap64(BitConverter.Convert<T, uint64>(x));
		return BitConverter.Convert<uint64, T>(swapped);
	}

	public static T ToLE16<T>(T x)
	{
		return x;
	}

	public static T ToBE16<T>(T x)
	{
		let swapped = BitSwap16(BitConverter.Convert<T, uint16>(x));
		return BitConverter.Convert<uint16, T>(swapped);
	}

	public static T ToLE32<T>(T x)
	{
		return x;
	}

	public static T ToBE32<T>(T x)
	{
		let swapped = BitSwap32(BitConverter.Convert<T, uint32>(x));
		return BitConverter.Convert<uint32, T>(swapped);
	}

	public static T ToLE64<T>(T x)
	{
		return x;
	}

	public static T ToBE64<T>(T x)
	{
		let swapped = BitSwap64(BitConverter.Convert<T, uint64>(x));
		return BitConverter.Convert<uint64, T>(swapped);
	}
	#else
	#   define LE_TO_NATIVE_16(x) BITSWAP16(x)
	#   define BE_TO_NATIVE_16(x) (x)
	#   define LE_TO_NATIVE_32(x) BITSWAP32(x)
	#   define BE_TO_NATIVE_32(x) (x)
	#   define LE_TO_NATIVE_64(x) BITSWAP64(x)
	#   define BE_TO_NATIVE_64(x) (x)
	
	#   define LE_TO_NATIVE_FLOAT(x) SwapFloat(x)
	#   define BE_TO_NATIVE_FLOAT(x) (x)
	#   define LE_TO_NATIVE_DOUBLE(x) SwapDouble(x)
	#   define BE_TO_NATIVE_DOUBLE(x) (x)
	
	#   define NATIVE_TO_LE_16(x) BITSWAP16(x)
	#   define NATIVE_TO_BE_16(x) (x)
	#   define NATIVE_TO_LE_32(x) BITSWAP32(x)
	#   define NATIVE_TO_BE_32(x) (x)
	#   define NATIVE_TO_LE_64(x) BITSWAP64(x)
	#   define NATIVE_TO_BE_64(x) (x)
	
	#   define NATIVE_TO_LE_FLOAT(x) SwapFloat(x)
	#   define NATIVE_TO_BE_FLOAT(x) (x)
	#   define NATIVE_TO_LE_DOUBLE(x) SwapDouble(x)
	#   define NATIVE_TO_BE_DOUBLE(x) (x)
	#endif
}