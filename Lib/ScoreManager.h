#pragma once
class ScoreManager
{
	static int Score;
	
public:
	static void AddScore(int num) { Score += num; }
	static int GetScore() { return Score; }
	static void Update();
	static void ResetScore();
};

