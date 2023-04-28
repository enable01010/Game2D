#include "Object.h"
#include "Scene.h"

int STG_Unit::UnitTypeCount = 0;

int STG_Status::CheckBulletHit(STG_Unit** unit)
{
	int DMG = 0;
	//全キャラチェック
	int unitCount = STG_Unit::GetUnitTypeCount();
	for (int i = 0; i < unitCount; i++)
	{
		if (MySide == unit[i]->GetGroupSide())
		{
			continue;
		}
		
		//バレットが当たってるかの判定
		int BulletTypeCount = unit[i]->GetBulletTypeCount();
		for (int j = 0; j < BulletTypeCount; j++)
		{
			int BulletCount = unit[i]->GetBullet()[j]->GetCount();
			for (int k = 0; k < BulletCount; k++)
			{
				STG_BulletData* bullet = unit[i]->GetBullet()[j]->GetData(k);
				if (bullet->GetTimer() <= 0)
				{
					continue;
				}

				if (CheckHit(bullet->GetPos(), unit[i]->GetBullet()[j]->Get_Radius(), unit[i]->GetBullet()[j]->GetScale()))
				{
					DMG++;
					bullet->SetTimer(0);
				}

			}
		}
	}
	return DMG;
}

int STG_Status::CheckUnitHit(STG_Unit** unit)
{
	int DMG = 0;
	int unitTypeCount = unit[0]->GetUnitTypeCount();
	for (int i = 0; i < unitTypeCount; i++)
	{
		if (MySide == unit[i]->GetGroupSide())
		{
			continue;
		}

		int InstanceCount = unit[i]->GetInstanceCount();
		for (int j = 0; j < InstanceCount; j++)
		{
			STG_Status* unitStatus = unit[i]->GetUnitStatus(j);
			if (unitStatus->GetHp() <= 0)
			{
				continue;
			}

			if (CheckHit(unitStatus->GetUnitPos(), unitStatus->GetRadius(), (unitStatus->GetUnitSize().x + unitStatus->GetUnitSize().y) * 0.5f))
			{
				DMG++;
			}

		}
	}
	return DMG;
}

bool STG_Status::CheckHit(Vector2 pos,int magnitude,float size)
{
	bool isHit = false;

	Vector2 dir = *PosAddress - pos;
	float len = dir.sqrtMagnitude();

	if (len < magnitude*size*size + ((SizeAddress->x + SizeAddress->y) * 0.5f) * ((SizeAddress->x + SizeAddress->y) * 0.5f) * Radius)isHit = true;
	return isHit;
}

void STG_Unit::Destroy()
{
	UnitDestroy();
	for (int i = 0; i < BulletTypeCount; i++)
	{
		Bullet[i]->Destroy();
	}

	for (int i = 0; i < FxTypeCount; i++)
	{
		Fx[i].Destroy();
	}
}