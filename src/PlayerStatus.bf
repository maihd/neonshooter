namespace NeonShooter;

using System;
using System.IO;
using NeonShooter.Entity;

public static class PlayerStatus
{
	// amount of time it takes, in seconds, for a multiplier to expire.
	private const float multiplierExpiryTime = 0.8f;
	private const int maxMultiplier = 20;

	public static int Lives { get; private set; }
	public static int Score { get; private set; }
	public static int HighScore { get; private set; }
	public static int Multiplier { get; private set; }

	private static float multiplierTimeLeft;	// time until the current multiplier expires
	private static int scoreForExtraLife;		// score required to gain an extra life

	public static bool IsGameOver => Lives == 0;

	// Static constructor

	static this()
	{
		HighScore = LoadHighScore();
		Reset();
	}

	public static void Reset()
	{
		if (Score > HighScore)
		{
			SaveHighScore(Score);
		}

		Score = 0;
		Multiplier = 1;
		Lives = 4;
		scoreForExtraLife = 2000;
		multiplierTimeLeft = 0;
	}

	public static void Update()
	{
		if (Multiplier > 1)
		{
			// update the multiplier timer
			if ((multiplierTimeLeft -= (float)Raylib.GetFrameTime()) <= 0)
			{
				multiplierTimeLeft = multiplierExpiryTime;
				ResetMultiplier();
			}
		}
	}

	public static void AddPoints(int basePoints)
	{
		if (PlayerShip.Instance.isDead)
			return;

		Score += basePoints * Multiplier;

		while (Score >= scoreForExtraLife)
		{
			scoreForExtraLife += 2000;
			Lives++;
		}
	}

	public static void IncreaseMultiplier()
	{
		if (PlayerShip.Instance.isDead)
			return;

		multiplierTimeLeft = multiplierExpiryTime;
		if (Multiplier < maxMultiplier)
			Multiplier++;
	}

	public static void ResetMultiplier()
	{
		Multiplier = 1;
	}

	public static void RemoveLife()
	{
		Lives--;
	}

	private const StringView highScoreFilename = "highscore.txt";
	
	// Return the saved high score if possible and return 0 otherwise
	private static int LoadHighScore() 
	{
		let text = scope String();
		if (File.ReadAllText(highScoreFilename, text) case .Ok)
		{
			return int.Parse(text) case .Ok(let score) ? score : 0;
		}

		return 0;
	}

	private static void SaveHighScore(int score)
	{
		File.WriteAllText(highScoreFilename, score.ToString(..scope .()));
	}
}