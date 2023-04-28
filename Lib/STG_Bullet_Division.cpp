#include "Object.h"

void STG_Bullet_Division::BulletAI(int i)
{
	Data[i].SetPos(Data[i].GetPos() + Data[i].GetDir());
	if (Data[i].GetTimer() % 50 == 1 && Data[i].GetTimer() > 950)
	{
		Vector2 dir = Data[i].GetDir();
		Vector2 dir2 = Vector2(dir.x - sin(dir.y*2), dir.y + cos(dir.x*2));
		dir = Vector2(dir.x + sin(dir.y*2),dir.y + cos(dir.x*2));
		 
		SetBullet(Data[i].GetPos(),dir,NULL,Data[i].GetTimer() - 1);
		SetBullet(Data[i].GetPos(), dir2, NULL, Data[i].GetTimer() - 1);
	}
}

void STG_Bullet_Division::SetBullet(Vector2 pos, Vector2 dir, STG_Status* status, int timer)
{
	if (!STG_Player::getIsDMG())
	{
		for (int i = 0; i < COUNT; i++)
		{
			if (Data[i].GetTimer() <= 0)
			{
				//Dirのｘを最初の傾きyを時計回り、反時計周りの判定に使用
				Data[i].SetPos(pos);
				Data[i].SetDir(dir);
				Data[i].SetTimer(timer);
				Data[i].SetIsGraze(false);
				return;
			}
		}
	}
}