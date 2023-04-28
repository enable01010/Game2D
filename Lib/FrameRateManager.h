#pragma once
#include "stdafx.h"

class FrameRateManager
{
	static int oldTime;
	static int deltaTime;
	static int flameCount;
	static char* FPS;
	TextManager text;

public:

	void Init(ID3D12Device* dev);
	void Draw(ID3D12GraphicsCommandList* cmdList);
	static void Update();
	static int getDeltaTime() { return 8/*deltaTime*/; }
};

