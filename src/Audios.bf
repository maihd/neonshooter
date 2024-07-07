namespace NeonShooter;

using Raylib;

using System;
using System.Linq;
using System.Collections;

static class Audios
{
	private static Music Music;

	private static Random rand;

	// return a random explosion sound

	private static List<Sound> explosions;
	public static Sound Explosion { get { return explosions[rand.Next(explosions.Count)]; } }

	private static List<Sound> shots;
	public static Sound Shot { get { return shots[rand.Next(shots.Count)]; } }

	private static List<Sound> spawns;
	public static Sound Spawn { get { return spawns[rand.Next(spawns.Count)]; } }

	static float exponent = 1.0f;                 	// Audio exponentiation value
	static float[400] averageVolume = default;   	// Average volume history

	[CLink]
	static extern void AttachAudioMixedProcessor(function void(void*, uint32) processor);

	public static void Load()
	{
		rand = new Random();

		Music = LoadMusicStream("Assets/Audios/Music.mp3");
		Music.looping = true;
		PlayMusicStream(Music);

		// These linq expressions are just a fancy way loading all sounds of each category into an array.
		explosions = Enumerable.Range(1, 8).Select((x) => LoadSound((scope $"Assets/Audios/explosion-0{x}.wav\0").Ptr)).ToList(..new .());
		shots = Enumerable.Range(1, 4).Select((x) => LoadSound((scope $"Assets/Audios/shoot-0{x}.wav\0").Ptr)).ToList(..new .());
		spawns = Enumerable.Range(1, 8).Select((x) => LoadSound((scope $"Assets/Audios/spawn-0{x}.wav\0").Ptr)).ToList(..new .());
	}

	public static void Unload()
	{
		StopMusicStream(Music);
		UnloadMusicStream(Music);

		Music = default;

		mixin UnloadAllsAndDelete(List<Sound> sounds)
		{
			for (let sound in sounds)
			{
				UnloadSound(sound);
			}

			delete sounds;
			sounds = null;
		}

		UnloadAllsAndDelete!(explosions);
		UnloadAllsAndDelete!(shots);
		UnloadAllsAndDelete!(spawns);

		DeleteAndNullify!(rand);
	}

	public static void Update()
	{
		UpdateMusicStream(Music);
	}

	public static void PlayShot()
	{
		let sound = Shot;
		SetSoundVolume(sound, 0.5f);
		PlaySound(sound);
	}

	public static void PlayExplosion()
	{
		let sound = Explosion;
		SetSoundVolume(sound, 0.5f);
		PlaySound(sound);
	}

	public static void PlaySpawn()
	{
		let sound = Spawn;
		SetSoundVolume(sound, 0.5f);
		PlaySound(sound);
	}
}