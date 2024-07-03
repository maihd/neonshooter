using Raylib;
using System;
using System.Collections;

namespace NeonShooter
{
	public static class GameAudio
	{
		static Music        music;
		static List<Sound> 	shootSounds = null;
		static List<Sound> 	spawnSounds = null;
		static List<Sound> 	explosionSounds = null;

		public static Sound LoadSound(StringView fileName)
		{
			return Raylib.LoadSound(fileName.Ptr);
		}

		public static Music LoadMusicStream(StringView fileName)
		{
			return Raylib.LoadMusicStream(fileName.Ptr);
		}

		public static void Init()
		{
			shootSounds = new List<Sound>();
			spawnSounds = new List<Sound>();
			explosionSounds = new List<Sound>();

		    let musicStreamPath = Assets.GetAssetPath("Audios/Music.ogg");
		    music = LoadMusicStream(musicStreamPath);
			if (music.ctxType != .MUSIC_AUDIO_NONE)
			{
		    	StopMusicStream(music);
			}

		    shootSounds.Add(LoadSound(Assets.GetAssetPath("Audios/shoot-01.wav")));
		    shootSounds.Add(LoadSound(Assets.GetAssetPath("Audios/shoot-02.wav")));
		    shootSounds.Add(LoadSound(Assets.GetAssetPath("Audios/shoot-03.wav")));
		    shootSounds.Add(LoadSound(Assets.GetAssetPath("Audios/shoot-04.wav")));

		    spawnSounds.Add(LoadSound(Assets.GetAssetPath("Audios/spawn-01.wav")));
		    spawnSounds.Add(LoadSound(Assets.GetAssetPath("Audios/spawn-02.wav")));
		    spawnSounds.Add(LoadSound(Assets.GetAssetPath("Audios/spawn-03.wav")));
		    spawnSounds.Add(LoadSound(Assets.GetAssetPath("Audios/spawn-04.wav")));
		    spawnSounds.Add(LoadSound(Assets.GetAssetPath("Audios/spawn-05.wav")));
		    spawnSounds.Add(LoadSound(Assets.GetAssetPath("Audios/spawn-06.wav")));
		    spawnSounds.Add(LoadSound(Assets.GetAssetPath("Audios/spawn-07.wav")));
		    spawnSounds.Add(LoadSound(Assets.GetAssetPath("Audios/spawn-08.wav")));

		    explosionSounds.Add(LoadSound(Assets.GetAssetPath("Audios/explosion-01.wav")));
		    explosionSounds.Add(LoadSound(Assets.GetAssetPath("Audios/explosion-02.wav")));
		    explosionSounds.Add(LoadSound(Assets.GetAssetPath("Audios/explosion-03.wav")));
		    explosionSounds.Add(LoadSound(Assets.GetAssetPath("Audios/explosion-04.wav")));
		    explosionSounds.Add(LoadSound(Assets.GetAssetPath("Audios/explosion-05.wav")));
		    explosionSounds.Add(LoadSound(Assets.GetAssetPath("Audios/explosion-06.wav")));
		    explosionSounds.Add(LoadSound(Assets.GetAssetPath("Audios/explosion-07.wav")));
		    explosionSounds.Add(LoadSound(Assets.GetAssetPath("Audios/explosion-08.wav")));
		}

		public static void Release()
		{
			StopShoot();
			StopSpawn();
			StopExplosion();

		    for (let sound in shootSounds)
		    {
		        UnloadSound(sound);
		    }

		    for (let sound in spawnSounds)
			{
			    UnloadSound(sound);
			}

		    for (let sound in explosionSounds)
			{
			    UnloadSound(sound);
			}

			delete shootSounds;
			delete spawnSounds;
			delete explosionSounds;
			
			if (music.ctxType != .MUSIC_AUDIO_NONE)
			{
		    	UnloadMusicStream(music);
		    	music = default;
			}
		}

		public static void Update()
		{
			if (music.ctxType != .MUSIC_AUDIO_NONE)
			{
		    	UpdateMusicStream(music);
			}
		}

		public static void PlayMusic()
		{
			if (music.ctxType != .MUSIC_AUDIO_NONE)
			{
				music.looping = true;
			    SetMusicVolume(music, 0.5f);
			    PlayMusicStream(music);
			}
		}

		public static void StopMusic()
		{
			if (music.ctxType != .MUSIC_AUDIO_NONE)
			{
		    	StopMusicStream(music);
			}
		}

		public static void PlayShoot()
		{
		    int count = shootSounds.Count;
		    if (count > 0)
		    {
		        Sound sound = shootSounds[GetRandomValue(0, (.)count - 1)];
		        SetSoundVolume(sound, 0.2f);
		        PlaySound(sound);
		    }
		}

		public static void StopShoot()
		{
		    for (int i = 0, n = shootSounds.Count; i < n; i++)
		    {
		        StopSound(shootSounds[i]);
		    }
		}

		public static void PlayExplosion()
		{
		    int count = explosionSounds.Count;
		    if (count > 0)
		    {
		        Sound sound = explosionSounds[GetRandomValue(0, (.)count - 1)];
		        SetSoundVolume(sound, 0.4f);
		        PlaySound(sound);
		    }
		}

		public static void StopExplosion()
		{
		    for (int i = 0, n = explosionSounds.Count; i < n; i++)
		    {
		        StopSound(explosionSounds[i]);
		    }
		}

		public static void PlaySpawn()
		{
		    int count = spawnSounds.Count;
		    if (count > 0)
		    {
		        Sound sound = spawnSounds[GetRandomValue(0, (.)count - 1)];
		        SetSoundVolume(sound, 0.3f);
		        PlaySound(sound);
		    }
		}

		public static void StopSpawn()
		{
		    for (int i = 0, n = spawnSounds.Count; i < n; i++)
		    {
		        StopSound(spawnSounds[i]);
		    }
		}
	}
}