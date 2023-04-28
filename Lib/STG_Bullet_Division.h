#pragma once
#include "stdafx.h"
class STG_Status;


class STG_Bullet_Division :public STG_Bullet
{
public:
	/// <summary>
	/// ’e‚ÌUŒ‚ƒpƒ^[ƒ“
	/// </summary>
	/// <param name="i"></param>
	void BulletAI(int i);

	/// <summary>
	/// ’e1”­”­Ë
	/// </summary>
	/// <param name="pos"></param>
	/// <param name="dir"></param>
	/// <param name="status"></param>
	/// <param name="timer"></param>
	void SetBullet(Vector2 pos, Vector2 dir, STG_Status* status = NULL, int timer = 300);
};

