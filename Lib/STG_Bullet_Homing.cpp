#include "Object.h"

void STG_Bullet_Homing::BulletAI(int i)
{
	if (Data[i].GetTargetHp()==NULL|| *Data[i].GetTargetHp() <= 0)
	{
		SearchNextTerget(i);
	}

	if (Data[i].GetTargetHp() != NULL)
	{
		Vector2 temp = *(Data[i].GetTargetPos()) - Data[i].GetPos();
		float magnitude = temp.magnitude();
		temp = temp / magnitude / BULLET_ASIST + Data[i].GetDir();
		magnitude = temp.magnitude();
		temp = temp / magnitude * Data[i].GetSpeed();
		Data[i].SetDir(temp);
	}
	Data[i].SetPos(Data[i].GetPos() + Data[i].GetDir());
}

void STG_Bullet_Homing::SetBullet(Vector2 pos, Vector2 dir, STG_Status* status, int timer)
{
	if (!STG_Player::getIsDMG())
	{
		for (int i = 0; i < COUNT; i++)
		{
			if (Data[i].GetTimer() <= 0)
			{
				Data[i].SetPos(pos);
				Data[i].SetDir(dir);
				Data[i].SetTimer(timer);
				Data[i].SetIsGraze(false);

				if (status == NULL)
				{
					Data[i].SetTargetPos(NULL);
					Data[i].SetTargetHp(NULL);
				}
				else {
					Data[i].SetTargetPos(status->GetPosAddress());
					Data[i].SetTargetHp(status->GetHpAddress());
				}
				Data[i].SetSpeed(5);
				return;
			}
		}
	}
}

void STG_Bullet_Homing::SearchNextTerget(int dataNum)
{
	for (int i = 0; i < GetCount(); i++)
	{
		if (Data[i].GetTargetHp() != NULL &&(*Data[i].GetTargetHp() > 0 && Data[i].GetTimer() > 0))
		{
			Data[dataNum].SetTargetHp(Data[i].GetTargetHp());
			Data[dataNum].SetTargetPos(Data[i].GetTargetPos());
			return;
		}
	}
	Data[dataNum].SetTargetPos(NULL);
	Data[dataNum].SetTargetHp(NULL);
}