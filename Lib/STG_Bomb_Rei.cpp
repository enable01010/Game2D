#include "Object.h"
#include "Scene.h"

void STG_Bomb_Rei::Init(ID3D12Device* dev)
{
	CutInImg.Init(dev, L"img\\Bomb\\ReiCutIn.png", Vector2(-300, 100));
	CutInImg.SetPos(Vector2(-600, 100));
	CutInImg.SetScale(Vector2(0.4f));
	CutInImg.SetColor(Color(0.5f));

	Beam.Init(dev, L"img\\Bomb\\Beam.png",IMG_BASE_POSITION::SENTER_DOWN);
	Beam.SetColor(Color(0.7f));

	Charge.Init(dev, L"img\\Bomb\\Fx_Particle.png",1);

}

void STG_Bomb_Rei::Update(STG_Unit** unit)
{
	if (isUse)
	{
		CutInImg.Update();
		PosUpdate(unit[0]->GetUnitStatus()->GetUnitPos());
		UseCutIn();
		TimeManager(unit);
	}
}

void STG_Bomb_Rei::UseBomb(Vector2 pos)
{
	isUse = true;

	Beam.SetScale(Vector2(1));

	//時間関係
	Timer = 0;

	//カットインの初期化
	CutInImg.SetPos(Vector2(-600, 100));
	CutInImg.SetColor(Color(0.5f));
	CutInImg.SetScale(Vector2(0.25f));
	SoundManager::Play(SOUNDMANAGER::Charge);
}

void STG_Bomb_Rei::Draw(ID3D12GraphicsCommandList* cmdList)
{
	if (isUse)
	{
		CutInImg.Draw(cmdList);
		if (Timer > ChargeTime)
		{
			Beam.Draw(cmdList);
		}
		Charge.Draw(cmdList);
	}
}

void STG_Bomb_Rei::Destroy()
{

}

void STG_Bomb_Rei::PosUpdate(Vector2 pos)
{
	Beam.SetPos(pos);
	Charge.SetPos(pos);
}

void STG_Bomb_Rei::TimeManager(STG_Unit** unit)
{
	Timer += FrameRateManager::getDeltaTime();
	if (Timer < ChargeTime)
	{
		//チャージ関連
		float r = (rand() % 10000) * 0.001f;
		float size = 50.0f;
		Vector2 dir = Vector2(sin(r) * size, cos(r) * size);
		Charge.SetFx(dir, dir * 0.05f);
	}
	else if (Timer < MaxTime)
	{
		if (Timer <= ChargeTime + 8)
		{
			SoundManager::stop(SOUNDMANAGER::Charge);
			SoundManager::Play(SOUNDMANAGER::ChargeFire);
			SoundManager::Play(SOUNDMANAGER::ForgetBeam);
		}
		float rate = 1.0f * Timer / MaxTime;
		Beam.SetScale(Vector2(0.4f * rate, 10 * rate * rate));
		HitCheck(unit);
	}
	else
	{
		isUse = false;
	}
}

void STG_Bomb_Rei::HitCheck(STG_Unit** unit)
{
	int unitCount = STG_Unit::GetUnitTypeCount();
	for (int i = 0; i < unitCount; i++)
	{
		if (unit[i]->GetGroupSide() == Unit_Side::PlAYER)
		{
			continue;
		}

		int enemyCount = unit[i]->GetInstanceCount();
		for (int j = 0; j < enemyCount; j++)
		{
			STG_Status* unitStatus = unit[i]->GetUnitStatus(j);
			if (unitStatus->GetHp() <= 0)
			{
				continue;
			}

			if (unitStatus->GetUnitPos().x + sqrt(unitStatus->GetRadius()) * unitStatus->GetSizeAddress()->x > Beam.GetPos().x - sqrt(Beam.Get_Radius()) * Beam.GetSize_Address()->x &&
				unitStatus->GetUnitPos().x - sqrt(unitStatus->GetRadius()) * unitStatus->GetSizeAddress()->x < Beam.GetPos().x + sqrt(Beam.Get_Radius()) * Beam.GetSize_Address()->x)
			{
				if (Beam.GetPos().y > unitStatus->GetUnitPos().y && unitStatus->GetUnitPos().y > -500)
				{
					unitStatus->Damage(ATTACK);
				}
			}
		}
	}
}