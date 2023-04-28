#pragma once
#include "stdafx.h"

enum class PICKUPCHARACTER {
	MainCharacter,
	Manager
};

/// <summary>
/// �F�X�ȃV�[���𓝈ꂵ�ĊǗ����邽�߂̂���
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
	virtual bool Update() { return true; };// �I������Ƃ�true��Ԃ��Ă�������
	virtual void Draw() {};
	virtual void onDestroy() {};
	
};
