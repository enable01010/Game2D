#pragma once
#include "stdafx.h"

class STG_Enemy;
class STG_Boss;
class STG_Unit;

class STG_Bomb
{

protected:
	VTN_MoveImage CutInImg;
	bool isUse = false;
	int Timer = 0;

public:
	void setIsUse(bool now) { isUse = now; }
	bool getIsUse() { return isUse; }

	/// <summary>
	/// �J�b�g�C���p�̉��o�p
	/// </summary>
	void UseCutIn();

	//�I�[�o�[���[�h
	virtual void Init(ID3D12Device* dev) {}
	virtual void UseBomb(Vector2 pos) {}
	virtual void Update(STG_Unit** unit) {}
	virtual void Draw(ID3D12GraphicsCommandList* cmdList) {}
	virtual void Destroy() {}

};

