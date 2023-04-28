#include "Scene.h"

int ScoreManager::Score = 0;

void ScoreManager::Update()
{
	Score += FrameRateManager::getDeltaTime();
}

void ScoreManager::ResetScore()
{
	Score = 0;
}
