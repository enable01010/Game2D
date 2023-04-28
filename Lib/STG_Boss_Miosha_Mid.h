#pragma once
#include "stdafx.h"

class STG_Boss_Miosha_Mid :public STG_Boss
{
	enum BOSS_STATE
	{
		DOWN_FIRST,
		RIGHT,
		LEFT,
		DOWN_SECOND,
		ATTACK_ONE,
		ATTACK_TWO,
		BEAM,
		DOWN_THIRD
	};

	Vector2 MoveDir[4] = {
		Vector2(3,0),
		Vector2(0,3),
		Vector2(-3,0),
		Vector2(0,-3)
	};

	//RIGHTÅQLEFTÇÃà⁄ìÆä÷åW
	Vector2 RIGHT_FIRST_POS = Vector2(-550, 0);
	Vector2 LEFT_FIRST_POS = Vector2(450, 0);

	//ATTACKä÷åW
	int AttackLoopCount = 0;

	//ã ä÷åW
	int AttackTime = 0;
	int BulletDirection = 0;
	int BULLET_DELAY = 10;
	int BulletDelayTime = 0;

	int MoveDirCount = sizeof(MoveDir) / sizeof(Vector2);

	void BossAI(STG_Unit** unit);

	void TimeOutAi(STG_Unit** unit);
public:

	void BossDestroy();
	void Damage(int val);

};
