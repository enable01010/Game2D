#pragma once
#include "stdafx.h"

enum class PICKUPCHARACTER {
	MainCharacter,
	Manager
};

/// <summary>
/// 色々なシーンを統一して管理するためのもの
/// </summary>
class BaseScene
{
protected:
	ID3D12Device* dev;
	ID3D12GraphicsCommandList* cmdList;

public:
	BaseScene(ID3D12Device* Dev, ID3D12GraphicsCommandList* CmdList) {
		dev = Dev;
		cmdList = CmdList;
	}
	virtual void Init() {};
	virtual bool Update() { return true; };// 終了するときtrueを返してください
	virtual void Draw() {};
	virtual void onDestroy() {};
	
};
