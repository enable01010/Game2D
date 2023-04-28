#include "Object.h"

void STG_Bullet_Straight::BulletAI(int i)
{
	Data[i].SetPos(Data[i].GetPos() + Data[i].GetDir());
}

void STG_Bullet_Straight::SetBullet(Vector2 pos, Vector2 dir, STG_Status* status, int timer)
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
				return;
			}
		}
	}
}