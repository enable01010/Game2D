#include "Scene.h"

int FrameRateManager::oldTime = 0;
int FrameRateManager::deltaTime = 0;
int FrameRateManager::flameCount = 0;
char* FrameRateManager::FPS = NULL;


void FrameRateManager::Init(ID3D12Device* dev)
{
	srand((unsigned int)time(NULL));
	FPS = new char[256];
	oldTime = timeGetTime();
	text.Init(dev, "0123456789fps", "FrameRateManager");
}

void FrameRateManager::Draw(ID3D12GraphicsCommandList* cmdList)
{
	text.DrawStart(cmdList);
	text.Draw(cmdList, FPS, Vector2(1150, 700), 0.5f);
}

void FrameRateManager::Update()
{
	int newTime = timeGetTime();
	deltaTime = newTime - oldTime;
	int sleepTime = 8 - deltaTime;
	if (sleepTime > 0) 
	{
		Sleep(sleepTime);
		deltaTime = 8;
	}
	else if (sleepTime < -16)
	{
		deltaTime = 32;
	}
	oldTime = timeGetTime();

	flameCount++;
	if (flameCount == 25)
	{
		flameCount = 0;
		if (deltaTime == 8)
		{
			FPS = new char[256]{ "120fps" };
		}
		else 
		{
			int fps = 1000 / deltaTime;
			sprintf_s(FPS, 256, "%dfps", fps);
		}
	}
}