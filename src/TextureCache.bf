namespace NeonShooter;

using Mojo;
using System;
using System.Collections;

public static struct TextureCache
{
    static Dictionary<uint32, Texture> _textures;

    static uint32 HashString(StringView path)
    {
        var h 		= (uint32)0;
        let length 	= (uint32)path.Length;
        var key 	= (uint8*)path.Ptr;

        if (length > 3)
        {
            var key_x4 = (uint32*)key;
            var i = length >> 2;

            repeat
            {
                uint32 k = *key_x4++;
                k *= 0xcc9e2d51;
                k = (k << 15) | (k >> 17);
                k *= 0x1b873593;
                h ^= k;
                h = (h << 13) | (h >> 19);
                h = (h * 5) + 0xe6546b64;
            }
			while (--i > 0);

            key = (uint8*)key_x4;
        }

        if (length & 3 != 0)
        {
            int i = length & 3;
            uint32 k = 0;
            key = &key[i - 1];

            repeat
			{
                k <<= 8;
                k |= *key--;
            }
			while (--i > 0);

            k *= 0xcc9e2d51;
            k = (k << 15) | (k >> 17);
            k *= 0x1b873593;
            h ^= k;
        }

        h ^= length;
        h ^= h >> 16;
        h *= 0x85ebca6b;
        h ^= h >> 13;
        h *= 0xc2b2ae35;
        h ^= h >> 16;

        return h;
    }

    public static Texture LoadTexture(StringView path)
    {
        uint32 hash = HashString(path);

        Texture texture;
        if (_textures.TryGetValue(hash, out texture))
        {
            return texture;
        }

        int32 w, h, n;
        void* pixels = Assets.LoadImage(path, out w, out h, out n);
        if (pixels != null)
        {
            texture = new Texture();

            texture.Wrap = .Repeat;
            texture.Filter = .Linear;
            texture.UpdatePixels(w, h, n == 4 ? .RGBA : .RGB, pixels);

            Assets.FreeImage(pixels);
            _textures.Add(hash, texture);
        }

        return texture; 
    }
}