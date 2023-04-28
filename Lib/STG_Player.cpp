#include "Object.h"
#include "Scene.h"

bool STG_Player::isDMG = false;

LRESULT STG_Player::Init(ID3D12Device* dev, const wchar_t* fileName, Unit_Side mySide, VTN_Item* item, bool add)
{
	//画像の処理
	LRESULT ret = VTN_Image::Init(dev, fileName);
	Pos.x = START_POS_X;
	Pos.y = START_POS_Y;

	//最初は無敵状態から始まるため
	isDMG = true;

	//白上用の初期ステータス設定用
	if (SelectScene::getPlayerNum() == (int)PLAYER_TYPE::SIRAKAMI)
	{
		Hp = START_HP_SIRAKAMI;
		BombCount = START_BOMB_SIRAKAMI;
		Speed = START_SPEED_SIRAKAMI;
	}
	//黒上用の初期ステータス設定用
	else if (SelectScene::getPlayerNum() == (int)PLAYER_TYPE::KUROUE)
	{
		Hp = START_HP_KUROUE;
		BombCount = START_BOMB_KUROUE;
		Speed = START_SPEED_KUROUE;
	}

	//ボムの設定
	switch (SelectScene::getHudaNum())
	{
	case 0:
		Bomb = new STG_Bomb_Yume();
		Bomb->Init(dev);
		break;
	case 1:
		Bomb = new STG_Bomb_Rei();
		Bomb->Init(dev);
		break;
	case 2:
	case 3:
		Bomb = new STG_Bomb_Yume();
		Bomb->Init(dev);
		break;
	}

	//ステータスの項目
	Score.Init(dev, L"img\\Player\\Score.png");
	Score.SetPos(Vector2(FIRST_STATUS_POS_X , FIRST_STATUS_POS_Y - MAIN_SPACE));
	Score.SetScale(Vector2(0.2f));

	Status[(int)PLAYER_STATUS::HP].Init(dev, L"img\\Player\\Player.png");
	Status[(int)PLAYER_STATUS::BOMB].Init(dev, L"img\\Player\\Bomb.png");
	Status[(int)PLAYER_STATUS::ATK].Init(dev, L"img\\Player\\Power.png");
	Status[(int)PLAYER_STATUS::GRAZE].Init(dev, L"img\\Player\\Graze.png");
	Status[(int)PLAYER_STATUS::POINT].Init(dev, L"img\\Player\\Point.png");

	//ステータスの現在値
	Life.Init(dev, L"img\\Player\\HP.png", Vector2(LIFE_POS_X, LIFE_POS_Y));
	Life.SetScale(Vector2(LIFE_IMG_SIZE, LIFE_IMG_SIZE));
	BombStar.Init(dev, L"img\\Player\\BombCount.png", Vector2(BOMB_POS_X, BOMB_POS_Y));
	BombStar.SetScale(Vector2(BOMB_IMG_SIZE, BOMB_IMG_SIZE));
	int space = 0;
	for (int i = 0; i < (int)PLAYER_STATUS::MAX; i++)
	{
		if (i == 2) space = SUB_SPACE;
		Status[i].SetPos(Vector2(FIRST_STATUS_POS_X, FIRST_STATUS_POS_Y + i * MAIN_SPACE + space));
		Status[i].SetScale(Vector2(STATUS_STR_IMG_SIZE, STATUS_STR_IMG_SIZE));
	}
	text.Init(dev, "0123456789", "PlayerStatus");
	sprintf_s(textChar[(int)PLAYER_STATUS::ATK], 256, "%d", ATK);
	sprintf_s(textChar[(int)PLAYER_STATUS::GRAZE], 256, "%d", Graze);
	sprintf_s(textChar[(int)PLAYER_STATUS::POINT], 256, "%d", Point);

	SetPosAddress(GetPos_Address());
	SetSizeAddress(GetSize_Address());
	SetRadius(Get_Radius());
	MySide = mySide;
	Bullet = new STG_Bullet * [(int)BulletType::MAX]{ new STG_Bullet_Straight(),new STG_Bullet_Homing() };
	BulletTypeCount = (int)BulletType::MAX;
	Fx = new STG_Fx[(int)FxType::MAX]{ STG_Fx() };
	FxTypeCount = (int)FxType::MAX;
	GroupSide = Unit_Side::PlAYER;
	InstanceCount = 1;

	//モーション用
	for (int i = 0; i < 4; i++)
	{
		wchar_t adress[256];
		swprintf_s(adress,256,L"img\\Player\\fubuki_0%d.png",i);
		Motion[i].Init(dev, adress);
		Motion[i].SetPos(Pos);
	}

	//攻撃用
	Bullet[(int)BulletType::STRAIGHT]->Init(dev, L"img\\Bullet\\RedBullet.png");
	Bullet[(int)BulletType::STRAIGHT]->SetScale(Vector2(2));
	Bullet[(int)BulletType::HOMING]->Init(dev, L"img\\Bullet\\BuleBullet.png");
	Bullet[(int)BulletType::HOMING]->SetScale(Vector2(2));

	//FX
	Fx[(int)FxType::HITFX].Init(dev, L"img\\Fx\\Frash.png", HITFX_IMG_SIZE, false);
	return ret;
}

void STG_Player::Draw(ID3D12GraphicsCommandList* cmdList) {

	if (Hp >= 0) {
		MotionNum = (MotionNum + 1) % 40;
		Motion[MotionNum / 10].Draw(cmdList);
		for (int i = 0; i < BulletTypeCount;i++)
		{
			Bullet[i]->Draw(cmdList);
		}
		Bomb->Draw(cmdList);
	}

	for (int i = 0; i < (int)FxType::MAX; i++)
	{
		Fx[i].Draw(cmdList);
	}
}

void STG_Player::DrawStatus(ID3D12GraphicsCommandList* cmdList)
{
	Score.Draw(cmdList);
	for (int i = 0; i < (int)PLAYER_STATUS::MAX; i++)
	{
		Status[i].Draw(cmdList);
	}
	char temp[256];
	sprintf_s(temp, 256, "%d", ScoreManager::GetScore());
	text.DrawStart(cmdList);
	text.Draw(cmdList, temp, Vector2(1250, 95),1,false);
	text.Draw(cmdList, textChar[(int)PLAYER_STATUS::ATK], Vector2(1100, 325));
	text.Draw(cmdList, textChar[(int)PLAYER_STATUS::GRAZE], Vector2(1100, 375));
	text.Draw(cmdList, textChar[(int)PLAYER_STATUS::POINT], Vector2(1100, 425));
	Life.Draw(cmdList, Hp);
	BombStar.Draw(cmdList, BombCount);
}

void STG_Player::Update(STG_Unit** unit) {

	if (Hp >= 0) {
		CheckGraze(unit);

		//弾の攻撃を受けたか
		if (CheckBulletHit(unit) > 0)
		{
			Hit();
		}
		//敵に直接当たっているか
		else if (CheckUnitHit(unit) > 0)
		{
			Hit();
		}

		CheckInput(unit);
		CheckInvincible(unit);
		CheckBomb(unit);

		for (int i = 0; i < BulletTypeCount; i++)
		{
			Bullet[i]->Update();
		}
	
		Bomb->Update(unit);

		for (int i = 0; i < 4; i++)
		{
			Motion[i].SetPos(Pos);
		}
	}
}

void STG_Player::CheckGraze(STG_Unit** unit)
{
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
				if (bullet->GetPos().x - GetPos().x < PLAYER_GRAZE_AREA &&
					bullet->GetPos().x - GetPos().x > -PLAYER_GRAZE_AREA &&
					bullet->GetPos().y - GetPos().y < PLAYER_GRAZE_AREA &&
					bullet->GetPos().y - GetPos().y > -PLAYER_GRAZE_AREA
					)
				{
					if (!bullet->GetIsGraze())
					{
						ScoreManager::AddScore(3000);
						GrazeHit();
						bullet->SetIsGraze(true);
						SoundManager::Play(SOUNDMANAGER::GrazeHit);
					}
				}
			}
		}
	}
}

void STG_Player::UnitDestroy()
{
	VTN_Image::Destroy();

	Life.Destroy();
	BombStar.Destroy();
	for (int i = 0; i < (int)PLAYER_STATUS::MAX; i++)
	{
		Status[i].Destroy();
	}
	text.Destroy();

	Bomb->Destroy();

	for (int i = 0; i < 4; i++)
	{
		Motion[i].Destroy();
	}
}

void STG_Player::CheckInput(STG_Unit** unit)
{
	Vector2 firstPos = Vector2(Pos.x,Pos.y);
	//移動
	if (InputManager::GetKey(INPUT_TYPE::up)) {
		AddPos(Vector2(0, -1 * Speed * FrameRateManager::getDeltaTime()));
		if (Pos.y < PLAY_WINDOW_DOWN_AREA)
		{
			Pos.y = PLAY_WINDOW_DOWN_AREA;
		}
	}
	if (InputManager::GetKey(INPUT_TYPE::down)) {
		AddPos(Vector2(0, 1 * Speed * FrameRateManager::getDeltaTime()));
		if (Pos.y > PLAY_WINDOW_UP_AREA)
		{
			Pos.y = PLAY_WINDOW_UP_AREA;
		}
	}
	if (InputManager::GetKey(INPUT_TYPE::right)) {
		AddPos(Vector2(1 * Speed * FrameRateManager::getDeltaTime(), 0));
		if (Pos.x > PLAY_WINDOW_RIGHT_AREA)
		{
			Pos.x = PLAY_WINDOW_RIGHT_AREA;
		}
	}
	if (InputManager::GetKey(INPUT_TYPE::left)) {
		AddPos(Vector2(-1 * Speed * FrameRateManager::getDeltaTime(), 0));
		if (Pos.x < PLAY_WINDOW_LEFT_AREA)
		{
			Pos.x = PLAY_WINDOW_LEFT_AREA;
		}
	}
	Vector2 movement = Pos - firstPos;
	if (movement.x > 0)
	{
		for (int i = 0; i < 4; i++)
		{
			Motion[i].SetRotation(15);
		}
	}
	else if (movement.x < 0)
	{
		for (int i = 0; i < 4; i++)
		{
			Motion[i].SetRotation(345);
		}
	}
	else
	{
		for (int i = 0; i < 4; i++)
		{
			Motion[i].SetRotation(0);
		}
	}

	//攻撃
	if (InputManager::GetTime(INPUT_TYPE::z) % COOL_FLAME == 1) {
		//一番近い敵を選出
		STG_Status* nearUnit = NULL;
		float NearMagnitude = 1300*1300;
		int unitTypeCount = unit[0]->GetUnitTypeCount();
		for (int i = 0; i < unitTypeCount; i++)
		{
			if (GroupSide == unit[i]->GetGroupSide())
			{
				continue;
			}
			int InstanceCount = unit[i]->GetInstanceCount();
			for (int j = 0; j < InstanceCount; j++)
			{
				STG_Status* unitData = unit[i]->GetUnitStatus(j);
				if (unitData->GetHp() > 0)
				{
					Vector2 distance = Pos - unitData->GetUnitPos();
					float magnitude = distance.magnitude();
					if (magnitude < NearMagnitude)
					{
						NearMagnitude = magnitude;
						nearUnit = unitData;
					}
				}
			
			}
		}
		
		//弾を発射する関係
		SoundManager::Play(SOUNDMANAGER::PlayerBulletShoot);
		bool temporary = isDMG;
		isDMG = false;
		if (ATK < 1)
		{
			Bullet[(int)BulletType::STRAIGHT]->SetBullet(Vector2(Pos.x, Pos.y), Vector2(WAY_STRAT_X, WAY_STRAT_Y));
		}
		else if (ATK < 3)
		{
			Bullet[(int)BulletType::STRAIGHT]->SetBullet(Vector2(Pos.x, Pos.y), Vector2(WAY_STRAT_X, WAY_STRAT_Y));
			if (InputManager::GetTime(INPUT_TYPE::z) % (COOL_FLAME * 2) == 1)
			{
				Bullet[(int)BulletType::HOMING]->SetBullet(Vector2(Pos.x + POS_INTERVAL_HOMING, Pos.y), Vector2(WAY_SLANT_RIGHT_X, WAY_STRAT_Y), nearUnit);
				Bullet[(int)BulletType::HOMING]->SetBullet(Vector2(Pos.x - POS_INTERVAL_HOMING, Pos.y), Vector2(WAY_SLANT_LEFT_X, WAY_STRAT_Y), nearUnit);
			}
		}
		else /*if (ATK < 5)*/
		{
			Bullet[(int)BulletType::STRAIGHT]->SetBullet(Vector2(Pos.x, Pos.y), Vector2(WAY_STRAT_X, WAY_STRAT_Y));
			Bullet[(int)BulletType::STRAIGHT]->SetBullet(Vector2(Pos.x - POS_INTERVAL_STRAIGHT, Pos.y), Vector2(WAY_SLANT_LEFT_X, WAY_STRAT_Y));
			Bullet[(int)BulletType::STRAIGHT]->SetBullet(Vector2(Pos.x + POS_INTERVAL_STRAIGHT, Pos.y), Vector2(WAY_SLANT_RIGHT_X, WAY_STRAT_Y));
			if (InputManager::GetTime(INPUT_TYPE::z) % (COOL_FLAME * 2) == 1)
			{
				Bullet[(int)BulletType::HOMING]->SetBullet(Vector2(Pos.x + POS_INTERVAL_HOMING, Pos.y), Vector2(WAY_SLANT_RIGHT_X, WAY_STRAT_Y), nearUnit);
				Bullet[(int)BulletType::HOMING]->SetBullet(Vector2(Pos.x - POS_INTERVAL_HOMING, Pos.y), Vector2(WAY_SLANT_LEFT_X, WAY_STRAT_Y), nearUnit);
			}
		}
		isDMG = temporary;
	}
}

void STG_Player::CheckInvincible(STG_Unit** unit)
{
	if (isDMG)
	{
		InvincibleCount += FrameRateManager::getDeltaTime();
		if (InvincibleCount > HIT_FX_DELAY)
		{
			InvincibleCount = 0;
			isDMG = false;
			PlayerArufaUp = false;
			for (int i = 0; i < 4; i++)
			{
				Motion[i].SetColor(Color(1));
			}
		}
		else if (PlayerArufaUp)
		{
			for (int i = 0; i < 4; i++)
			{
				Motion[i].AddColor(0.05f);
			}
			if (Motion[0].GetColorArufa() == 1)
			{
				PlayerArufaUp = !PlayerArufaUp;
			}
		}
		else
		{
			for (int i = 0; i < 4; i++)
			{
				Motion[i].AddColor(-0.05f);
			}
			if (Motion[0].GetColorArufa() == 0)
			{
				PlayerArufaUp = !PlayerArufaUp;
			}
		}
		if (InvincibleCount > DMG_DELAY_TIME)
		{
			EnemyBulletDelete(unit);
		}
	}
}

void STG_Player::CheckBomb(STG_Unit** unit)
{
	if (InputManager::GetKey(INPUT_TYPE::x))
	{
		if (!Bomb->getIsUse() && BombCount > 0)
		{
			Bomb->UseBomb(Pos);
			BombCount--;
			isDMG = true;

			EnemyBulletDelete(unit);
		}
	}
}

void STG_Player::EnemyBulletDelete(STG_Unit** unit)
{
	int unitCount = STG_Unit::GetUnitTypeCount();
	for (int i = 0; i < unitCount; i++)
	{
		if (GroupSide == unit[i]->GetGroupSide())
		{
			continue;
		}
		int BulletTypeCount = unit[i]->GetBulletTypeCount();
		for (int j = 0; j < BulletTypeCount; j++)
		{
			unit[i]->GetBullet()[j]->BulletDelete();
		}
	}
}

void STG_Player::Hit()
{
	if (!isDMG)
	{
		SoundManager::Play(SOUNDMANAGER::PlayerHit);
		Fx[(int)FxType::HITFX].SetFx(Pos);
		isDMG = true;
		Hp--;
	}
}

void STG_Player::GrazeHit()
{
	Graze++;
	sprintf_s(textChar[(int)PLAYER_STATUS::GRAZE], 256, "%d", Graze);
	//音
	//FX
}

void STG_Player::getItem(ENEMY_DROP drop)
{
	switch (drop)
	{
	case ENEMY_DROP::HP:
		Hp++;
		break;

	case ENEMY_DROP::BOMB:
		BombCount++;
		break;

	case ENEMY_DROP::ATTACK:
		SoundManager::Play(SOUNDMANAGER::PowerItemGet);
		ATK++;
		sprintf_s(textChar[(int)PLAYER_STATUS::ATK], 256, "%d", ATK);
		break;

	case ENEMY_DROP::POINT:
		SoundManager::Play(SOUNDMANAGER::PointItemGet);
		Point++;
		ScoreManager::AddScore(10000);
		sprintf_s(textChar[(int)PLAYER_STATUS::POINT], 256, "%d", Point);
		break;

	}
}

void STG_Player::Damage(int val)
{
	if (!isDMG)
	{
		SoundManager::Play(SOUNDMANAGER::PlayerHit);
		Hp -= val;
		Fx[(int)FxType::HITFX].SetFx(Pos);
		isDMG = true;
	}
}