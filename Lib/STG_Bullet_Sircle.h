#pragma once
#include "stdafx.h"
class STG_Status;


class STG_Bullet_Sircle :public STG_Bullet
{
public:
	void BulletAI(int i);

	/// <summary>
	/// �e1������
	/// </summary>
	/// <param name="pos"></param>
	/// <param name="dir">�����ŏ��̌X���A�����������i0�����v�A1�������v�j�ɑ�p</param>
	/// <param name="status"></param>
	/// <param name="timer"></param>
	void SetBullet(Vector2 pos, Vector2 dir, STG_Status* status = NULL, int timer = 300);
};

