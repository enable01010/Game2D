#pragma once
#include "stdafx.h"
class STG_Status;

class STG_Bullet_Homing :public STG_Bullet
{
	void SearchNextTerget(int dataNum);
	const int BULLET_ASIST = 10;
public:
	void BulletAI(int i);
	void SetBullet(Vector2 pos, Vector2 dir, STG_Status* status = NULL, int timer = 300);

};

