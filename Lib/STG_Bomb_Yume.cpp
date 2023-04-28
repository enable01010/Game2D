#include "Object.h"
#include "Scene.h"

void STG_Bomb_Yume::Init(ID3D12Device* dev)
{
	BombFX[(int)Bomb_Direction::Up].Init(dev, L"img\\Bomb\\Red.png");
	BombFX[(int)Bomb_Direction::Down].Init(dev, L"img\\Bomb\\Red.png");
	BombFX[(int)Bomb_Direction::Left].Init(dev,L"img\\Bomb\\Blue.png");
	BombFX[(int)Bomb_Direction::Right].Init(dev, L"img\\Bomb\\Blue.png");

	CutInImg.Init(dev, L"img\\Bomb\\YumeCutIn.png",Vector2(-300,100));
	CutInImg.SetPos(Vector2(-600, 100));
	CutInImg.SetScale(Vector2(0.4f));
	CutInImg.SetColor(Color(0.5f));
}

void STG_Bomb_Yume::UseBomb(Vector2 pos)
{
	//画像関係
	ImgSize = FIRST_IMG_SIZE;
	isUse = true;
	for (int i = 0; i < (int)Bomb_Direction::Max; i++)
	{
		BombFX[i].SetPos(pos + FIRST_POS[i]);
		BombFX[i].SetColor(Color(1,1,1, IMG_ALUFA));
		BombFX[i].SetScale(Vector2(ImgSize, ImgSize));
	}

	//時間関係
	Timer = 0;
	waitTime = 0;

	//ボムの判定用
	BombArea = FIRST_BOMB_AREA;
	UsePosition = pos;

	//カットインの初期化
	CutInImg.SetPos(Vector2(-600, 100));
	CutInImg.SetColor(Color(0.5f));
	CutInImg.SetScale(Vector2(0.25f));

	SoundManager::Play(SOUNDMANAGER::Charge);
	SoundManager::Play(SOUNDMANAGER::AreYouLooking);
}

void STG_Bomb_Yume::Update(STG_Unit** unit)
{
	if (isUse)
	{
		CheckArea(unit);
		TimeCount();
		BombAreaUp();
		UseCutIn();
		CutInImg.Update();
	}
}

void STG_Bomb_Yume::Draw(ID3D12GraphicsCommandList* cmdList)
{
	if (isUse)
	{
		for (int i = 0; i < (int)Bomb_Direction::Max; i++)
		{
			BombFX[i].Draw(cmdList);
		}
		CutInImg.Draw(cmdList);
	}
}

void STG_Bomb_Yume::Destroy()
{
	for (int i = 0; i < (int)Bomb_Direction::Max; i++)
	{
		BombFX[i].Destroy();
	}
	CutInImg.Destroy();
}

void STG_Bomb_Yume::TimeCount()
{
	Timer += FrameRateManager::getDeltaTime();
	if (Timer >= BOMB_TIME)
	{
		isUse = false;
	}
}

void STG_Bomb_Yume::CheckArea(STG_Unit** unit)
{
	//検知エリア
	int upSide = UsePosition.y - BombArea;
	int downSide = UsePosition.y + BombArea;
	int leftSide = UsePosition.x - BombArea;
	int rightSide = UsePosition.x + BombArea;

	//雑魚用
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

			if ((unitStatus->GetUnitPos().y > upSide && unitStatus->GetUnitPos().y < downSide)
				|| (unitStatus->GetUnitPos().x > leftSide && unitStatus->GetUnitPos().x < rightSide))
			{
				unitStatus->Damage(ATTACK);
			}
		}
	}
}

void STG_Bomb_Yume::BombAreaUp()
{
	
	if (ImgSize < 20.0f)
	{
		ImgSize += 1.0f;
		BombFX[(int)Bomb_Direction::Up].SetScale(Vector2(ImgSize, FIRST_IMG_SIZE));
		BombFX[(int)Bomb_Direction::Down].SetScale(Vector2(ImgSize, FIRST_IMG_SIZE));
		BombFX[(int)Bomb_Direction::Left].SetScale(Vector2(FIRST_IMG_SIZE, ImgSize));
		BombFX[(int)Bomb_Direction::Right].SetScale(Vector2(FIRST_IMG_SIZE, ImgSize));
	}
	else if (waitTime < 100)
	{
		waitTime++;
	}
	else
	{
		if (waitTime == 100)
		{
			SoundManager::stop(SOUNDMANAGER::Charge);
			SoundManager::Play(SOUNDMANAGER::ChargeFire);
			waitTime++;
		}
		BombArea += 20;
		BombFX[(int)Bomb_Direction::Up].AddPos(Vector2(0,-20));
		BombFX[(int)Bomb_Direction::Down].AddPos(Vector2(0,20));
		BombFX[(int)Bomb_Direction::Left].AddPos(Vector2(-20,0));
		BombFX[(int)Bomb_Direction::Right].AddPos(Vector2(20,0));
		if (BombArea > 1200)
		{
			setIsUse(false);
		}
	}
}

