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

	//------------------------------------------------------------------------------------
	// Audio processing function
	//------------------------------------------------------------------------------------
	static void ProcessAudio(void *buffer, uint32 frames)
	{
	    float *samples = (float *)buffer;   // Samples internally stored as <float>s
	    float average = 0.0f;               // Temporary average volume

	    for (uint32 frame = 0; frame < frames; frame++)
	    {
	        float* left = &samples[frame * 2 + 0];
			float* right = &samples[frame * 2 + 1];

	        *left = Math.Pow(Math.Abs(*left), exponent) * ( (*left < 0.0f)? -1.0f : 1.0f );
	        *right = Math.Pow(Math.Abs(*right), exponent) * ( (*right < 0.0f)? -1.0f : 1.0f );

	        average += Math.Abs(*left) / frames;   // accumulating average volume
	        average += Math.Abs(*right) / frames;
	    }

	    // Moving history to the left
	    for (int i = 0; i < 399; i++) averageVolume[i] = averageVolume[i + 1];

	    averageVolume[399] = average;         // Adding last average value
	}

	[CLink]
	static extern void AttachAudioMixedProcessor(function void(void*, uint32) processor);

	public static void Load()
	{
		rand = new Random();

		AttachAudioMixedProcessor(=> ProcessAudio);

		SetMasterVolume(0.5f);

		//Music = LoadMusicStream("Assets/Audios/Music.mp3\0");

		var music = ref Music;
		music.looping = true;
		//PlayMusicStream(music);

		// These linq expressions are just a fancy way loading all sounds of each category into an array.
		explosions = Enumerable.Range(1, 8).Select((x) => LoadSound((scope $"Assets/Audios/explosion-0{x}.wav\0").Ptr)).ToList(..new .());
		shots = Enumerable.Range(1, 4).Select((x) => LoadSound((scope $"Assets/Audios/shoot-0{x}.wav\0").Ptr)).ToList(..new .());
		spawns = Enumerable.Range(1, 8).Select((x) => LoadSound((scope $"Assets/Audios/spawn-0{x}.wav\0").Ptr)).ToList(..new .());
	}

	public static void Unload()
	{
		//StopMusicStream(Music);
		//UnloadMusicStream(Music);

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
		//UpdateMusicStream(Music);
	}

	public static void PlayShot()
	{
		PlaySound(Shot);
	}

	public static void PlayExplosion()
	{
		PlaySound(Explosion);
	}

	public static void PlaySpawn()
	{
		PlaySound(Spawn);
	}
}