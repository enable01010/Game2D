#include "Object.h"
#include "Scene.h"

LRESULT STG_Boss::Init(ID3D12Device* dev, const wchar_t* fileName, Unit_Side mySide, VTN_Item* item, bool add)
{
	
	//ステータス関係
	LRESULT ret = VTN_Image::Init(dev, fileName);
	Hp = 500;
	MaxHP = Hp;
	Pos = Vector2(-100, -550);

	//残り時間表示用
	text.Init(dev,"0123456789","Boss");
	sprintf_s(TimerChar, 256, "%d", MicroTimer/1000);

	//HPゲージ
	HpGaze.Init(dev, L"img\\Boss\\BossHP.png",IMG_BASE_POSITION::LEFT_SENTER);
	HpGaze.SetPos(Vector2(-400,-350));
	HpGaze.SetScale(Vector2(HPGAZE_SIZE_X, HPGAZE_SIZE_Y));

	SetPosAddress(GetPos_Address());
	SetSizeAddress(GetSize_Address());
	SetRadius(Get_Radius());
	MySide = Unit_Side::ENEMY;
	Bullet = new STG_Bullet * [(int)BulletType::MAX]
	{
		new STG_Bullet_Straight(),
		new STG_Bullet_Straight(),
		new STG_Bullet_Straight(),
		new STG_Bullet_Straight(),
		new STG_Bullet_Sircle(),
		new STG_Bullet_Division(),
		new STB_Bullet_Wait_RockOn(),
		new STG_Bullet_Homing()
	};
	BulletTypeCount = (int)BulletType::MAX;
	Fx = new STG_Fx[(int)FxType::MAX]{ STG_Fx(),STG_Fx() };
	FxTypeCount = (int)FxType::MAX;
	GroupSide = mySide;
	InstanceCount = 1;

	//玉関係
	Bullet[(int)BulletType::YELLOW]->Init(dev, L"img\\Boss\\BulletYellow.png");
	Bullet[(int)BulletType::YELLOW]->SetScale(Vector2(BULLET_SIZE, BULLET_SIZE));
	Bullet[(int)BulletType::BULE]->Init(dev, L"img\\Boss\\BulletBule.png");
	Bullet[(int)BulletType::BULE]->SetScale(Vector2(BULLET_SIZE, BULLET_SIZE));
	Bullet[(int)BulletType::RED]->Init(dev, L"img\\Boss\\BulletRed.png");
	Bullet[(int)BulletType::RED]->SetScale(Vector2(BULLET_SIZE, BULLET_SIZE));
	Bullet[(int)BulletType::BIG]->Init(dev, L"img\\Boss\\BulletPurple.png");
	Bullet[(int)BulletType::BIG]->SetScale(Vector2(10, 10));
	Bullet[(int)BulletType::CIRCLE]->Init(dev, L"img\\Boss\\BulletYellow.png");
	Bullet[(int)BulletType::CIRCLE]->SetScale(Vector2(BULLET_SIZE, BULLET_SIZE));
	Bullet[(int)BulletType::DIVISION]->Init(dev, L"img\\Boss\\BulletYellow.png");
	Bullet[(int)BulletType::DIVISION]->SetScale(Vector2(BULLET_SIZE, BULLET_SIZE));
	Bullet[(int)BulletType::WAIT_ROCKON]->Init(dev, L"img\\Boss\\BulletYellow.png");
	Bullet[(int)BulletType::WAIT_ROCKON]->SetScale(Vector2(BULLET_SIZE, BULLET_SIZE));
	Bullet[(int)BulletType::HOMING]->Init(dev, L"img\\Boss\\BulletYellow.png");
	Bullet[(int)BulletType::HOMING]->SetScale(Vector2(BULLET_SIZE, BULLET_SIZE));
	//エフェクト
	Fx[(int)FxType::EXPLOAD].Init(dev, L"img\\Fx\\Expload.png", 0.5f, false);
	Fx[(int)FxType::PLAYER_HIT].Init(dev, L"img\\Fx\\Frash.png", PLAYER_HIT_FX_SIZE, false);

	//レーザー関係
	Charge.Init(dev, L"img\\shooting\\Beam.png", true);
	Charge.SetMySide(MySide);
	return ret;
}

void STG_Boss::Update(STG_Unit** unit) {
	if (isSpawn && !isClear)
	{
		if (!isExpload && !isDeath)TimerCount();
		if (!isExpload)
		{
			if (!isTimeOver)
			{
				if (WaitTime > 0)
				{
					WaitTime--;
				}
				else
				{
					BossAI(unit);
					StateTime--;
				}
			}
			else
			{
				if (WaitTime > 0)
				{
					WaitTime--;
				}
				else
				{
					TimeOutAi(unit);
				}
			}
		}
		else
		{
			Expload();
		}
		
		if (!isExpload)
		{
			int damage = CheckBulletHit(unit);
			if (damage > 0)
			{
				SoundManager::Play(SOUNDMANAGER::EnemyHit);
				Damage(damage);
			}
			BossHpGaze();
		}
		for (int i = 0; i < BulletTypeCount; i++)
		{
			Bullet[i]->Update();
		}

		//ビーム関係
		Charge.Update(unit);
	}
}

void STG_Boss::Draw(ID3D12GraphicsCommandList* cmdList) {
	if (isSpawn && !isClear)
	{
		if (!isDeath) {
			VTN_Image::Draw(cmdList);
		}
		for (int i = 0; i < BulletTypeCount; i++)
		{
			Bullet[i]->Draw(cmdList);
		}
		for (int i = 0; i < FxTypeCount; i++)
		{
			Fx[i].Draw(cmdList);
		}
		Charge.Draw(cmdList);
	}
}

void STG_Boss::DrawStatus(ID3D12GraphicsCommandList* cmdList)
{
	if (isSpawn && !isClear)
	{
		if (!isExpload && !isDeath)
		{
			HpGaze.Draw(cmdList);
			if (!isTimeOver)
			{
				text.DrawStart(cmdList);
				text.Draw(cmdList, TimerChar, Vector2(900, 30));
			}
		}
	}
}

void STG_Boss::UnitDestroy()
{
	VTN_Image::Destroy();
	text.Destroy();
	HpGaze.Destroy();
	Charge.Destroy();

	BossDestroy();
}

void STG_Boss::Damage(int val){
	if (isSpawn)
	{
		STG_Unit_Single::Damage(val);
		if (Hp <= 0) {
			State = 0;
			isExpload = true;
			StateTime = 200;
		}
	}
}

bool STG_Boss::NextState(int state, int nextTime) {
	if (StateTime <= 0) {
		State = state;
		StateTime = nextTime;
		return true;
	}
	return false;
}

void STG_Boss::WaitState(int nextState, int waitTime)
{
	isEntry = false;
	isExit = false;
	State = nextState;
	WaitTime = waitTime;
}

void STG_Boss::TimerCount()
{
	if (!isTimeOver)
	{
		MicroTimer -= FrameRateManager::getDeltaTime();
		int Seconds = MicroTimer / 1000;
		sprintf_s(TimerChar, 256, "%d", Seconds);
		if (Seconds == 3)
		{
			SoundManager::Play(SOUNDMANAGER::CountDown);
		}
		if (Seconds == 0)
		{
			isTimeOver = true;
		}
	}
}

void STG_Boss::BossHpGaze()
{
	if (!isHpGazeFull)
	{
		FirstHpGazeSize++;
		HpGaze.SetScale(Vector2(HPGAZE_SIZE_X * Hp / MaxHP * FirstHpGazeSize / FirstHpGazeSizeMax , HPGAZE_SIZE_Y));
		if (FirstHpGazeSize == FirstHpGazeSizeMax)
		{
			isHpGazeFull = true;
		}
	}
	else
	{
		if (Hp > 0)
		{
			HpGaze.SetScale(Vector2(HPGAZE_SIZE_X * Hp / MaxHP, HPGAZE_SIZE_Y));
		}
		else
		{
			HpGaze.SetScale(Vector2(0, HPGAZE_SIZE_Y));
		}
	}
}

void STG_Boss::Spawn()
{
	Pos = Vector2(-100, -550);
	Hp = MaxHP;
	isSpawn = true;
}

void STG_Boss::Expload()
{
	Fx[(int)FxType::EXPLOAD].SetFx(Pos + Vector2(100 - rand() % 200, 100 - rand() % 200));
	if (StateTime <= 0) {
		isDeath = true;
		isClear = true;
	}
	StateTime--;
}
