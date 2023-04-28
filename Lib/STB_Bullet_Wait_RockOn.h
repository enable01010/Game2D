#pragma once
#include "stdafx.h"
class STG_Status;

class STB_Bullet_Wait_RockOn :public STG_Bullet
{
	void BulletAI(int i);

	/// <summary>
	/// 弾1発発射 TargetPosをサイズに代用　Soeedを経過時間計測のために使用
	/// </summary>
	/// <param name="pos"></param>
	/// <param name="dir"></param>
	/// <param name="status"></param>
	/// <param name="timer"></param>
	void SetBullet(Vector2 pos, Vector2 dir, STG_Status* status = NULL, int timer = 300);

	void Draw(ID3D12GraphicsCommandList* cmdList);
};

