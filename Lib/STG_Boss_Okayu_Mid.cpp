#include "Object.h"
#include "Scene.h"

void STG_Boss_Okayu_Mid::BossAI(STG_Unit** unit)
{
	switch (State)
	{
	case BOSS_STATE::DOWN_FIRST:
		Pos += MoveDir[1];
		BulletDelayTime++;
		if (BulletDelayTime >= BULLET_DELAY)
		{
			Bullet[(int)BulletType::YELLOW]->SetBullet(Pos, Vector2(0, 5));
			BulletDelayTime = 0;
		}
		
		if (Pos.y >= 450)
		{
			int RMD = (rand() % 2) + 1;
			WaitState(RMD,60);
		}
		break;
	case BOSS_STATE::RIGHT:
		if (!isEntry)
		{
			isEntry = true;
			Pos = RIGHT_FIRST_POS;
		}
		Pos += MoveDir[0];
		BulletDelayTime++;
		if (BulletDelayTime >= BULLET_DELAY)
		{
			Bullet[(int)BulletType::YELLOW]->SetBullet(Pos, Vector2(0, 5));
			BulletDelayTime = 0;
		}
		if (Pos.x >= 450)
		{
			WaitState(BOSS_STATE::DOWN_SECOND, 60);
		}
		break;
	case BOSS_STATE::LEFT:
		if (!isEntry)
		{
			isEntry = true;
			Pos = LEFT_FIRST_POS;
		}
		Pos += MoveDir[2];
		BulletDelayTime++;
		if (BulletDelayTime >= BULLET_DELAY)
		{
			Bullet[(int)BulletType::YELLOW]->SetBullet(Pos, Vector2(0, 5));
			BulletDelayTime = 0;
		}
		if (Pos.x <= -550)
		{
			WaitState(BOSS_STATE::DOWN_SECOND, 60);
		}
		break;
	case BOSS_STATE::DOWN_SECOND:
		if (!isEntry)
		{
			Pos = Vector2(-100,-450);
			isEntry = true;
		}
		Pos += MoveDir[1];
		if (Pos.y >= -100)
		{
			WaitState(BOSS_STATE::ATTACK_ONE, 60);
		}
		break;
	case BOSS_STATE::ATTACK_ONE:
		if (!isEntry)
		{
			isEntry = true;
			AttackTime = 100;
		}
		if (AttackTime % 10 == 0)
		{
			for (int i = 0; i < 360; i += 10)
			{
				float radian = i * 3.14f * 0.0055555f;
				Bullet[(int)BulletType::YELLOW]->SetBullet(Pos + Vector2(cos(radian) * 10, sin(radian) * 10), Vector2(cos(radian) * 3, sin(radian) * 3));
			}
		}
		if (AttackTime == 0)
		{
			WaitState(BOSS_STATE::ATTACK_TWO, 60);
		}
		AttackTime--;
		break;
	case BOSS_STATE::ATTACK_TWO:
		if (!isEntry)
		{
			isEntry = true;
			AttackTime = 100;
			BulletDirection = 0;
		}
		if (AttackTime % 10 == 0)
		{
			for (int i = BulletDirection; i < BulletDirection + 360; i += 20)
			{
				float radian = i * 3.14f * 0.0055555f;
				Bullet[(int)BulletType::YELLOW]->SetBullet(Pos + Vector2(cos(radian) * 10, sin(radian) * 10), Vector2(cos(radian) * 3, sin(radian) * 3));
			}
			BulletDirection += 3;
		}
		if (AttackTime == 0)
		{

			WaitState(BOSS_STATE::BEAM, 60);
			
		}
		AttackTime--;
		break;

	case BOSS_STATE::BEAM:
		if (!isEntry)
		{
			isEntry = true;
			AttackTime = 150;
		}
		if (AttackTime % 10 == 0)
		{
			Vector2 targetPos = unit[0]->GetUnitStatus()->GetUnitPos();
			int temp = AttackTime / 10;
			switch (temp)
			{
			case 0:
			case 5:
				Charge.SetBeam(Vector2(Pos.x - 60,Pos.y), targetPos);
				break;
			case 1:
			case 6:
				Charge.SetBeam(Vector2(Pos.x - 30, Pos.y + 30), targetPos);
				break;
			case 2:
			case 7:
				Charge.SetBeam(Vector2(Pos.x , Pos.y + 60), targetPos);
				break;
			case 3:
			case 8:
				Charge.SetBeam(Vector2(Pos.x + 30, Pos.y + 30), targetPos);
				break;
			case 4:
			case 9:
				Charge.SetBeam(Vector2(Pos.x + 60, Pos.y), targetPos);
				break;
			}
			
		}
		if (AttackTime == 0)
		{
			if (AttackLoopCount < 1)
			{
				AttackLoopCount++;
				WaitState(BOSS_STATE::ATTACK_ONE, 60);
			}
			else
			{
				AttackLoopCount = 0;
				WaitState(BOSS_STATE::DOWN_THIRD, 60);
			}
		}
		AttackTime--;
		break;
	case BOSS_STATE::DOWN_THIRD:
		Pos += MoveDir[1];
		if (Pos.y >= 450)
		{
			int RMD = (rand() % 2) + 1;
			WaitState(RMD, 60);
		}
		break;

	}
}

void STG_Boss_Okayu_Mid::BossDestroy()
{

}

void STG_Boss_Okayu_Mid::Damage(int val)
{
	STG_Unit_Single::Damage(val);
	if (Hp <= 0) {
		isTimeOver = true;
		ScoreManager::AddScore(50000);
	}
}

void STG_Boss_Okayu_Mid::TimeOutAi(STG_Unit** unit)
{
	Pos += Vector2(0, -3);
	if (Pos.y <= -450) {
		isClear = true;
	}
}