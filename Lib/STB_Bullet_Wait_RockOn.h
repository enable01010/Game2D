#pragma once
#include "stdafx.h"
class STG_Status;

class STB_Bullet_Wait_RockOn :public STG_Bullet
{
	void BulletAI(int i);

	/// <summary>
	/// �e1������ TargetPos���T�C�Y�ɑ�p�@Soeed���o�ߎ��Ԍv���̂��߂Ɏg�p
	/// </summary>
	/// <param name="pos"></param>
	/// <param name="dir"></param>
	/// <param name="status"></param>
	/// <param name="timer"></param>
	void SetBullet(Vector2 pos, Vector2 dir, STG_Status* status = NULL, int timer = 300);

	void Draw(ID3D12GraphicsCommandList* cmdList);
};

