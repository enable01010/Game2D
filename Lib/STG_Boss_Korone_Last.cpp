#include "Object.h"
#include "Scene.h"

LRESULT STG_Boss_Korone_Last::Init(ID3D12Device* dev, const wchar_t* fileName, Unit_Side mySide, VTN_Item* item, bool add)
{
	LRESULT ret = STG_Boss::Init(dev, fileName, mySide, item, add);
	Hp = 2000;
	MaxHP = 2000;

	CutInImg[0].Init(dev, L"img\\Boss\\Skill_1.png", Vector2(100, 100));
	CutInImg[1].Init(dev, L"img\\Boss\\Skill_2.png", Vector2(100, 100));
	CutInImg[2].Init(dev, L"img\\Boss\\Skill_3.png", Vector2(100, 100));
	for (int i = 0; i < 3; i++)
	{
		CutInImg[i].SetPos(Vector2(400, 100));
		CutInImg[i].SetColor(Color(0.5f));
		CutInImg[i].SetScale(Vector2(0.25f));
	}

	return ret;
}

void STG_Boss_Korone_Last::Draw(ID3D12GraphicsCommandList* cmdList)
{
	STG_Boss::Draw(cmdList);
	for (int i = 0; i < 3; i++)
	{
		if (isSkill[i])
		{
			CutInImg[i].Draw(cmdList);
		}
	}
}

void STG_Boss_Korone_Last::DrawStatus(ID3D12GraphicsCommandList* cmdList)
{
	STG_Boss::DrawStatus(cmdList);
}

void STG_Boss_Korone_Last::BossAI(STG_Unit** unit) {

	switch (State)
	{
	case BOSS_STATE::DOWN_FIRST:
		Pos += MoveDir[1];
		if (Pos.y >= -200)
		{
			BossRnadPatarn();
		}
		break;
	case BOSS_STATE::LEFT_LASRE:
		if (!isEntry)
		{
			isEntry = true;
			AttackTime = 200;
		}
		if (AttackTime % 10 == 0 && AttackTime < 150)
		{
			int RMD_X = (rand() % 200) - 200;
			int RMD_Y = (rand() % 400) - 100;
			Vector2 targetPos = Vector2(RMD_X, RMD_Y);
			Charge.SetBeam(Vector2(-300, -200), targetPos);
		}
		if (AttackTime == 0)
		{
			WaitState(BOSS_STATE::RIGHT_LASER, 60);
		}
		AttackTime--;
		break;
	case BOSS_STATE::RIGHT_LASER:
		if (!isEntry)
		{
			isEntry = true;
			AttackTime = 200;
		}
		if (AttackTime % 10 == 0 && AttackTime < 150)
		{
			int RMD_X = (rand() % 200) - 200;
			int RMD_Y = (rand() % 400) - 100;
			Vector2 targetPos = Vector2(RMD_X, RMD_Y);
			Charge.SetBeam(Vector2(100, -200), targetPos);
		}
		if (AttackTime == 0)
		{
			WaitState(BOSS_STATE::DOUBLE_LASRE, 60);
		}
		AttackTime--;
		break;
	case BOSS_STATE::DOUBLE_LASRE:
		if (!isEntry)
		{
			isEntry = true;
			AttackTime = 200;
		}
		if (AttackTime % 10 == 0 && AttackTime < 150)
		{
			int RMD_X = (rand() % 200) - 200;
			int RMD_Y = (rand() % 400) - 100;
			Vector2 targetPos = Vector2(RMD_X, RMD_Y);
			if (AttackTime % 4 == 0)
			{
				Charge.SetBeam(Vector2(-300, -200), targetPos);
			}
			else
			{
				Charge.SetBeam(Vector2(100, -200), targetPos);
			}

		}
		if (AttackTime == 0)
		{
			BossRnadPatarn();
		}
		AttackTime--;
		break;
	case BOSS_STATE::RAND_BULLET:
		if (!isEntry)
		{
			isEntry = true;
			AttackTime = 200;
		}
		if (AttackTime % 5)
		{
			Bullet[(int)BulletType::YELLOW]->SetBullet(Pos + Vector2(cos(AttackTime) * 10, sin(AttackTime) * 10), Vector2(cos(AttackTime) * 3, sin(AttackTime) * 3));
		}
		if (AttackTime == 0)
		{
			BossRnadPatarn();
		}
		AttackTime--;
		break;
	case BOSS_STATE::FAN_BULLET:
		if (!isEntry)
		{
			isEntry = true;
			AttackTime = 360;
		}
		if (AttackTime % 10 == 0)
		{
			Bullet[(int)BulletType::BIG]->SetBullet(Pos, Vector2(-4, 2));
			Bullet[(int)BulletType::BIG]->SetBullet(Pos, Vector2(4, 2));
		}
		if (AttackTime % 2 == 0)
		{
			float temp = AttackTime * 3.14f / 180 * 4;
			if (sin(temp) > 0)
			{
				temp += 1.57f;
				Bullet[(int)BulletType::RED]->SetBullet(Pos + Vector2(-30, 0), Vector2(sin(temp) * 2, abs(cos(temp) * 2)));
			}
			else
			{
				temp += 1.57f;
				Bullet[(int)BulletType::BULE]->SetBullet(Pos + Vector2(30, 0), Vector2(sin(temp) * 2, abs(cos(temp) * 2)));
			}
		}
		if (AttackTime == 0)
		{
			BossRnadPatarn();
		}
		AttackTime--;
		break;
	case BOSS_STATE::CIRCLE_BULLET:
		if (!isEntry)
		{
			isEntry = true;
			AttackTime = 100;
		}
		if (AttackTime % 20 == 0)
		{
			for (int i = 0; i < 12; i++)
			{

				Bullet[(int)BulletType::CIRCLE]->SetBullet(Pos, Vector2(0 + (AttackTime / 10) + 30 * i, 0), NULL, 1000);
				Bullet[(int)BulletType::CIRCLE]->SetBullet(Pos, Vector2(0 + (AttackTime / 10) + 30 * i, 1), NULL, 1000);

			}
		}
		if (AttackTime == 0)
		{
			switch (CircleNum)
			{
			case 0:
				WaitState(BOSS_STATE::CIRCLE_BULLET_MOVE_ONE, 0);
				break;
			case 1:
				WaitState(BOSS_STATE::CIRCLE_BULLET_MOVE_TWO, 0);
				break;
			case 2:
				WaitState(BOSS_STATE::CiRCLE_BULLET_THREE, 0);
				break;
			}
		}
		AttackTime--;
		break;
	case BOSS_STATE::CIRCLE_BULLET_MOVE_ONE:
		if (!isEntry)
		{
			isEntry = true;
			AttackTime = 100;
		}
		Pos += Vector2(-2, -1);
		if (AttackTime == 0)
		{
			CircleNum = 1;
			WaitState(BOSS_STATE::CIRCLE_BULLET, 60);
		}
		AttackTime--;
		break;
	case BOSS_STATE::CIRCLE_BULLET_MOVE_TWO:
		if (!isEntry)
		{
			isEntry = true;
			AttackTime = 100;
		}
		Pos += Vector2(4, 0);
		if (AttackTime == 0)
		{
			CircleNum = 2;
			WaitState(BOSS_STATE::CIRCLE_BULLET, 60);
		}
		AttackTime--;
		break;
	case BOSS_STATE::CiRCLE_BULLET_THREE:
		if (!isEntry)
		{
			isEntry = true;
			AttackTime = 100;
		}
		Pos += Vector2(-2, 1);
		if (AttackTime == 0)
		{
			CircleNum = 0;
			BossRnadPatarn();
		}
		AttackTime--;
		break;
	case BOSS_STATE::CIRCLE_ONLY:
		if (!isEntry)
		{
			isEntry = true;
			AttackTime = 200;
		}
		if (AttackTime % 20 == 0)
		{
			for (int i = 0; i < 12; i++)
			{

				Bullet[(int)BulletType::CIRCLE]->SetBullet(Pos, Vector2(0 + (AttackTime / 10) + 30 * i, 0), NULL, 1000);
				Bullet[(int)BulletType::CIRCLE]->SetBullet(Pos, Vector2(0 + (AttackTime / 10) + 30 * i, 1), NULL, 1000);

			}
		}
		if (AttackTime == 0)
		{
			BossRnadPatarn();
		}
		AttackTime--;
		break;
	}

}

void STG_Boss_Korone_Last::BossDestroy()
{
	VTN_Image::Destroy();
}

void STG_Boss_Korone_Last::BossRnadPatarn()
{
	int RMD = rand() % 5;
	switch (RMD)
	{
	case 0:
		WaitState(BOSS_STATE::LEFT_LASRE, 150);
		break;
	case 1:
		WaitState(BOSS_STATE::RAND_BULLET, 150);
		break;
	case 2:
		WaitState(BOSS_STATE::FAN_BULLET, 150);
		break;
	case 3:
		WaitState(BOSS_STATE::CIRCLE_BULLET, 150);
		break;
	case 4:
		WaitState(BOSS_STATE::CIRCLE_ONLY, 150);
		break;
	}
}

void STG_Boss_Korone_Last::TimeOutAi(STG_Unit** unit)
{
	if (!isTimeOutStart)
	{
		isTimeOutStart = true;
		WaitState(BOSS_STATE::TIME_OUT_FIRST, 0);
		for (int i = 0; i < (int)BulletType::MAX; i++)
		{
			Bullet[i]->BulletDelete();
		}
	}

	for (int i = 0; i < 3; i++)
	{
		if (isSkill[i])
		{
			CutInImg[i].Update();
			if (CutInImg[i].getIsGoal())
			{
				SkillTimer++;
				if (SkillTimer > 150)
				{
					CutInImg[i].AddColor(-0.01f);
					CutInImg[i].AddScale(0.01f);
				}
			}
		}
	}

	switch (State)
	{
	case BOSS_STATE::TIME_OUT_FIRST:
		if (!isEntry)
		{
			isEntry = true;
			FirstPosDir = (Vector2(-100, -200) - Pos) / 100;
			AttackTime = 100;
		}
		Pos += FirstPosDir;
		if (AttackTime == 0)
		{
			BossRnadPatarn_Skiil();
		}
		AttackTime--;
		break;
	case BOSS_STATE::おかゆの背負った罪はお前の罪である:
		if (!isEntry)
		{
			isSkill[0] = true;
			CutInImg[0].SetColor(Color(0.5f));
			isEntry = true;
			AttackTime = 1200;
		}
		//移動
		if (AttackTime > 1100)
		{
			Pos += Vector2(-2, -1);
		}
		else if (AttackTime < 1050 && AttackTime > 950)
		{
			Pos += Vector2(4, 0);
		}
		else if (AttackTime < 900 && AttackTime > 800)
		{
			Pos += Vector2(-2, 1);
		}
		else if (AttackTime < 750 && AttackTime > 650)
		{
			Pos += Vector2(-2, -1);
		}
		else if (AttackTime < 600 && AttackTime > 500)
		{
			Pos += Vector2(0, 6);
		}
		else if (AttackTime < 450 && AttackTime > 350)
		{
			Pos += Vector2(4, 0);
		}
		else if (AttackTime < 300 && AttackTime > 200)
		{
			Pos += Vector2(0, -6);
		}
		else if (AttackTime < 150 && AttackTime > 50)
		{
			Pos += Vector2(-2, 1);
		}

		//ボスの攻撃
		if (AttackTime % 100 == 0)
		{
			for (int i = 0; i < 360; i += 30)
			{
				float radian = i * 3.14f * 0.0055555f;
				Bullet[(int)BulletType::YELLOW]->SetBullet(Pos + Vector2(cos(radian) * 10, sin(radian) * 10), Vector2(cos(radian) * 2, sin(radian) * 2));
			}
		}

		//横槍弾幕
		if (AttackTime % 100 == 0)
		{
			for (int i = 0; i < 360; i += 30)
			{
				float radian = i * 3.14f * 0.0055555f;
				Bullet[(int)BulletType::RED]->SetBullet(Vector2(-400, -300 + AttackTime / 2) + Vector2(cos(radian) * 5, sin(radian) * 5), Vector2(cos(radian), sin(radian)), NULL, 1000);
			}
			for (int i = 0; i < 360; i += 30)
			{
				float radian = i * 3.14f * 0.0055555f;
				Bullet[(int)BulletType::BULE]->SetBullet(Vector2(200, -300 + AttackTime / 2) + Vector2(cos(radian) * 5, sin(radian) * 5), Vector2(cos(radian), sin(radian)), NULL, 1000);
			}
		}
		if (AttackTime == 0)
		{
			BossRnadPatarn_Skiil();
		}
		AttackTime--;
		break;

	case BOSS_STATE::こつこつギャンブルで稼ぐ:
		if (!isEntry)
		{
			isSkill[1] = true;
			CutInImg[1].SetColor(Color(0.5f));
			isEntry = true;
			AttackTime = 1260;
		}
		if (AttackTime % 10 == 0)
		{
			float temp = ((1260 - AttackTime) % 360) * 3.14f / 180 * 2;
			Bullet[(int)BulletType::WAIT_ROCKON]->SetBullet(Vector2(-100, 0) + Vector2(sin(temp), -cos(temp)) * 300, Vector2(), unit[0]->GetUnitStatus(), 1000);
		}
		if (AttackTime == 0)
		{
			BossRnadPatarn_Skiil();
		}
		AttackTime--;
		break;

	case BOSS_STATE::リスナーは養分なんじゃコラァァ！:
		if (!isEntry)
		{
			isSkill[2] = true;
			CutInImg[2].SetColor(Color(0.5f));
			isEntry = true;
			AttackTime = 1200;
		}

		//移動関係
		if (AttackTime < 850 && AttackTime >= 800)
		{
			Pos += Vector2(-4, -2);
		}
		else if (AttackTime < 450 && AttackTime >= 400)
		{
			Pos += Vector2(8, 0);
		}
		else if (AttackTime < 50 && AttackTime >= 0)
		{
			Pos += Vector2(-4, 2);
		}

		if (AttackTime > 600)
		{
			if (AttackTime % 50 == 0)
			{
				for (int i = 0; i < 20; i++)
				{
					Bullet[(int)BulletType::RED]->SetBullet(Vector2(-400, -360 + 18 * i), Vector2(3, 0));
					Bullet[(int)BulletType::RED]->SetBullet(Vector2(200, 360 + 18 * -i), Vector2(-3, 0));
				}
			}
		}
		else
		{
			if (AttackTime % 100 == 0)
			{
				for (int i = 0; i < 20; i++)
				{
					Bullet[(int)BulletType::RED]->SetBullet(Vector2(-400, -360 + 18 * i), Vector2(2, 0));
					Bullet[(int)BulletType::RED]->SetBullet(Vector2(200, 360 + 18 * -i), Vector2(-2, 0));
				}
			}
			else if (AttackTime % 50 == 0)
			{
				for (int i = 0; i < 20; i++)
				{
					Bullet[(int)BulletType::BULE]->SetBullet(Vector2(-400 + 15 * i, 300), Vector2(0, -2));
					Bullet[(int)BulletType::BULE]->SetBullet(Vector2(200 + 15 * -i, -300), Vector2(0, 2));
				}
			}
		}

		if (AttackTime == 0)
		{
			BossRnadPatarn_Skiil();
		}
		AttackTime--;
		break;

	}

	if (AttackTime == 220)
	{
		SoundManager::Play(SOUNDMANAGER::CountDown);
	}
}

void STG_Boss_Korone_Last::BossRnadPatarn_Skiil()
{
	isSkill[0] = false;
	isSkill[1] = false;
	isSkill[2] = false;
	int RMD = rand() % 3;
	switch (RMD)
	{
	case 0:
		WaitState(BOSS_STATE::おかゆの背負った罪はお前の罪である, 150);
		break;
	case 1:
		WaitState(BOSS_STATE::こつこつギャンブルで稼ぐ, 150);
		break;
	case 2:
		WaitState(BOSS_STATE::リスナーは養分なんじゃコラァァ！, 150);
		break;
	}

	for (int i = 0; i < 3; i++)
	{
		CutInImg[i].SetScale(Vector2(0.25f));
		CutInImg[i].SetPos(Vector2(400, 100));
	}
	SkillTimer = 0;

	for (int i = 0; i < (int)BulletType::MAX; i++)
	{
		Bullet[i]->BulletDelete();
	}
}