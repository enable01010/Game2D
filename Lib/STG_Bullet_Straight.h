#pragma once
#include "stdafx.h"
class STG_Status;
class STG_Bullet_Straight :public STG_Bullet
{
public:
	void BulletAI(int i);
	void SetBullet(Vector2 pos, Vector2 dir, STG_Status* status = NULL, int timer = 300);
};

