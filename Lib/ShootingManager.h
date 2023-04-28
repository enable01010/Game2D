#pragma once
#include "stdafx.h"

class ShootingManager
{
	static int Stage;
	static bool isEscape;
public:
	static void SetStage(int val) { Stage = val; }
	static int GetStage() { return Stage; }
	static bool GetIsEscape() { return isEscape; }
	
	static void ChengeScene(ID3D12Device* Dev, ID3D12GraphicsCommandList* CmdList);
	static void Update();
};

