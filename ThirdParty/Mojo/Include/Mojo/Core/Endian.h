#pragma once

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
#   define LE_TO_NATIVE_16(x) (x)
#   define BE_TO_NATIVE_16(x) BITSWAP16(x)
#   define LE_TO_NATIVE_32(x) (x)
#   define BE_TO_NATIVE_32(x) BITSWAP32(x)
#   define LE_TO_NATIVE_64(x) (x)
#   define BE_TO_NATIVE_64(x) BITSWAP64(x)

#   define LE_TO_NATIVE_FLOAT(x) (x)
#   define BE_TO_NATIVE_FLOAT(x) SwapFloat(x)
#   define LE_TO_NATIVE_DOUBLE(x) (x)
#   define BE_TO_NATIVE_DOUBLE(x) SwapDouble(x)

#   define NATIVE_TO_LE_16(x) (x)
#   define NATIVE_TO_BE_16(x) BITSWAP16(x)
#   define NATIVE_TO_LE_32(x) (x)
#   define NATIVE_TO_BE_32(x) BITSWAP32(x)
#   define NATIVE_TO_LE_64(x) (x)
#   define NATIVE_TO_BE_64(x) BITSWAP64(x)

#   define NATIVE_TO_LE_FLOAT(x)  (x)
#   define NATIVE_TO_BE_FLOAT(x)  SwapFloat(x)
#   define NATIVE_TO_LE_DOUBLE(x) (x)
#   define NATIVE_TO_BE_DOUBLE(x) SwapDouble(x)
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

inline namespace Mojo
{
    inline float SwapFloat(float x)
    {
        union
        {
            float   f;
            int     u;
        } swapper;
        swapper.f = x;
        swapper.u = BITSWAP32(swapper.u);
        return swapper.f;
    }

    inline double SwapDouble(double x)
    {
        union
        {
            double     d;
            long long  u;
        } swapper;
        swapper.d = x;
        swapper.u = BITSWAP64(swapper.u);
        return swapper.d;
    }
}