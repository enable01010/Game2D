#include "Object.h"

void STG_Bullet_Sircle::BulletAI(int i)
{
	float time = 1000 - Data[i].GetTimer();
	float mag = time / 180 * 2;
	float seta = time * 3.14 / 180 / 5 + (Data[i].GetDir().x *3.14/180);
	if (Data[i].GetDir().y == 0)
	{
		Data[i].SetPos(Vector2(Data[i].GetTargetPos()->x + sin(seta) * mag * 100, Data[i].GetTargetPos()->y + cos(seta) * mag * 100));
	}
	else
	{
		Data[i].SetPos(Vector2(Data[i].GetTargetPos()->x - sin(seta) * mag * 100, Data[i].GetTargetPos()->y + cos(seta) * mag * 100));
	}
}	

void STG_Bullet_Sircle::SetBullet(Vector2 pos, Vector2 dir, STG_Status* status, int timer)
{
	if (!STG_Player::getIsDMG())
	{
		for (int i = 0; i < COUNT; i++)
		{
			if (Data[i].GetTimer() <= 0)
			{
				//Dirのｘを最初の傾きyを時計回り、反時計周りの判定に使用
				Data[i].SetPos(pos);
				Data[i].SetTargetPos(new Vector2(pos.x, pos.y));
				Data[i].SetDir(dir);
				Data[i].SetTimer(timer);
				Data[i].SetIsGraze(false);
				return;
			}
		}
	}
}