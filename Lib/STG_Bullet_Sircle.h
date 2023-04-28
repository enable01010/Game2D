#pragma once
#include "stdafx.h"
class STG_Status;


class STG_Bullet_Sircle :public STG_Bullet
{
public:
	void BulletAI(int i);

	/// <summary>
	/// 弾1発発射
	/// </summary>
	/// <param name="pos"></param>
	/// <param name="dir">ｘを最初の傾き、ｙを回る向き（0＝時計、1＝反時計）に代用</param>
	/// <param name="status"></param>
	/// <param name="timer"></param>
	void SetBullet(Vector2 pos, Vector2 dir, STG_Status* status = NULL, int timer = 300);
};

