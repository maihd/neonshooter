using Raylib;
using System;
using System.Collections;

namespace NeonShooter
{
	public static class Assets
	{
		static Dictionary<int, Texture> cachedTextures;

		public static StringView GetAssetPath(StringView target)
		{
			const char8* ASSET_PATH = "Assets";

			let finalPath = TextFormat("%s/%s", ASSET_PATH, target.Ptr);
			return .(finalPath);
		}

		public static void InitCacheTextures()
		{
			cachedTextures = new Dictionary<int, Texture>();
		}

		public static void ClearCacheTextures()
		{
			for (let (_, texture) in cachedTextures)
			{
				UnloadTexture(texture);
			}
			delete cachedTextures;
		}

		public static Texture CacheTexture(StringView path)
		{
			let finalPath = GetAssetPath(path);
			let targetHash = StringView(finalPath).GetHashCode();

			if (cachedTextures.TryGetValue(targetHash, var texture))
			{
				return texture;
			}

			texture = LoadTexture(finalPath.Ptr);
			if (texture.id != 0)
			{
				cachedTextures.Add(targetHash, texture);
			}

			return texture;
		}
	}
}