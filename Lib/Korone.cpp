#include "Scene.h"

void Korone::Init() {

	//ゲームプレイエリアの設定
	background2.Init(dev, L"img\\ShootingScene\\background.png");
	background2.AddPos(Vector2(LEFT_SIDE_MAX,0));
	background3.Init(dev, L"img\\ShootingScene\\background.png");
	background3.AddPos(Vector2(RIGHT_SIDE_MAX, 0));

	//背景の設定
	for (int i = 0; i < 2; i++)
	{
		playArea[i].Init(dev, L"img\\ShootingScene\\space.png",Vector2(BACKGROUND_POS_X, BACKGROUND_GOAL_POS_Y), BACKGROUND_SPEED);
		playArea[i].SetPos(Vector2(BACKGROUND_POS_X, 0 + i * BACKGROUND_POSX_Y));
		playArea[i].SetScale(Vector2(BACKGROUND_SIZE, BACKGROUND_SIZE));
	}

	//ユニットの設定
	STG_Unit::SetUnitTypeCount((int)UnitType::MAX);

	//プレイヤーの設定
	switch (SelectScene::getPlayerNum())
	{
	case (int)PLAYER_TYPE::SIRAKAMI:
		Unit[(int)UnitType::PLAYER]->Init(dev, L"img\\Player\\fubuki_00.png",Unit_Side::PlAYER);
		break;

	case (int)PLAYER_TYPE::KUROUE:
		Unit[(int)UnitType::PLAYER]->Init(dev, L"img\\Player\\fubuki_00.png", Unit_Side::PlAYER);
		break;
	}

	Player->SetScale(Vector2(0.3f));

	//エネミーの設定
	Unit[(int)UnitType::HUBU]->Init(dev, L"img\\Enemy\\hubura.png",Unit_Side::ENEMY,Items);
	Hubu->SetScale(Vector2(HUBURS_SIZE, HUBURS_SIZE));
	Unit[(int)UnitType::SISI]->Init(dev, L"img\\Enemy\\sisi.png", Unit_Side::ENEMY,Items);
	Sisi->SetScale(Vector2(SISI_SIZE, SISI_SIZE));

	//ボスの設定
	Unit[(int)UnitType::MID_BOSS]->Init(dev, L"img\\Boss\\Korone.png");
	BossMid->SetPos(Vector2(0, -800));
	BossMid->SetScale(Vector2(0.1f));
	Unit[(int)UnitType::LAST_BOSS]->Init(dev, L"img\\Boss\\Korone.png");
	BossLast->SetScale(Vector2(0.1f));
	BossLast->SetPos(Vector2(0, -800));
	//
	BossMid->Spawn();
	//BossLast->Spawn();
	//

	//アイテムの設定
	Items[(int)ENEMY_DROP::HP].Init(dev, L"img\\Item\\HP.png", ENEMY_DROP::HP);
	//BOMB
	Items[(int)ENEMY_DROP::ATTACK].Init(dev, L"img\\Item\\Power.png", ENEMY_DROP::ATTACK);
	Items[(int)ENEMY_DROP::BIG_ATTACK].Init(dev, L"img\\Item\\BigPower.png", ENEMY_DROP::BIG_ATTACK);
	Items[(int)ENEMY_DROP::FULL_ATTACK].Init(dev, L"img\\Item\\FullPower.png", ENEMY_DROP::FULL_ATTACK);
	Items[(int)ENEMY_DROP::POINT].Init(dev, L"img\\Item\\Point.png", ENEMY_DROP::POINT);
	for (int i = 0; i < (int)ENEMY_DROP::NOTHING; i++)
	{
		Items[i].SetScale(Vector2(ITEM_SIZE, ITEM_SIZE));
	}

	//会話パート用
	text.Init(dev, "0123456789あれ～机の上に置いてた、おぎりがなーニヤかゆ知ら？こるよ＜ポン＞恨みを思れ！Enterまも。しつぐじゃんっ言う許さぞ","EasyMode_1");
	TalkWindow.Init(dev, L"img\\Text\\TextWindow.png");
	TalkWindow.SetColor(Color(0.5f));
	TalkWindow.SetScale(Vector2(0));
	TalkWindow.SetPos(Vector2(-100, 250));
	PlayerTalk[0].Init(dev, L"img\\Text\\HubukiTalk.png");
	PlayerTalk[0].SetColor(Color(0));
	PlayerTalk[0].SetPos(Vector2(-300, 100));
	EnemyTalk[0].Init(dev, L"img\\Text\\OkayuTalk.png");
	EnemyTalk[0].SetColor(Color(0));
	EnemyTalk[0].SetPos(Vector2(100, 100));

	//ゲームオーバー判定用
	GameOver.Init(dev, L"img\\ShootingScene\\ResultGameOver.png");
	GameOver.SetPos(Vector2(-100, 0));
	GameOver.SetColor(Color(0.7f));
	GameOver.SetScale(Vector2(0));
	//ゲームクリアー判定用
	GameClear.Init(dev, L"img\\ShootingScene\\ResultGameClear.png");
	GameClear.SetPos(Vector2(-100, 0));
	GameClear.SetColor(Color(0.7f));
	GameClear.SetScale(Vector2(0));
	//ゲームオーバークリアー共通
	Cursol.Init(dev, L"img\\ShootingScene\\Cursol.png");
	Cursol.SetPos(Vector2(-300, 30));
	Cursol.SetColor(Color(0));
	Cursol.SetScale(Vector2(0.5f));

	isSoundStart = false;
}

void Korone::Draw() {

	for (int i = 0; i < 2; i++)
	{
		playArea[i].Draw(cmdList);
	}

	for (int i = 0; i < (int)UnitType::MAX;i++)
	{
		Unit[i]->Draw(cmdList);
	}

	for (int i = 0; i < (int)ENEMY_DROP::NOTHING; i++)
	{
		if (i == (int)ENEMY_DROP::BOMB)
		{
			continue;
		}
		Items[i].Draw(cmdList);
	}

	background2.Draw(cmdList);
	background3.Draw(cmdList);

	for (int i = 0; i < (int)UnitType::MAX; i++)
	{
		Unit[i]->DrawStatus(cmdList);
	}

	if (isTalkStart && !isTalkEnd)
	{
		PlayerTalk[PlayerTalkNum].Draw(cmdList);
		EnemyTalk[EnemyTalkNum].Draw(cmdList);

		TalkWindow.Draw(cmdList);
		text.DrawStart(cmdList);
		text.Draw(cmdList, OutPutChar[0],Vector2(300,550),0.6f);
		text.Draw(cmdList, OutPutChar[1], Vector2(300, 600), 0.6f);
		text.Draw(cmdList, OutPutChar[2], Vector2(300, 650), 0.6f);
		if (isAllOutPut)
		{
			text.Draw(cmdList, "Enter", Vector2(700, 650), 0.5f);
		}
	}

	if (isGameOver)
	{
		GameOver.Draw(cmdList);
		text.DrawStart(cmdList);
		text.Draw(cmdList, TototalScore,Vector2(750,330),1.0f,false);
		Cursol.Draw(cmdList);
	}

	if (isGameClear)
	{
		GameClear.Draw(cmdList);
		text.DrawStart(cmdList);
		text.Draw(cmdList, TototalScore, Vector2(750, 330), 1.0f, false);

	}

}

bool Korone::Update() {
	if (!isSoundStart)
	{
		isSoundStart = true;
		SoundManager::Play(SOUNDMANAGER::Mogumogu);
	}


	if (!isGameOver && !isGameClear)
	{
		//スコアの追加
		if (!(isTalkStart && !isTalkEnd))
		{
			ScoreManager::Update();
		}

		//時間経過トリガーのイベント管理
		TimeControl();

		//背景の移動
		BackGroundMove();

		//オブジェクトのアップデート
		for (int i = 0; i < (int)UnitType::MAX; i++)
		{
			Unit[i]->Update(Unit);
		}

		for (int i = 0; i < (int)ENEMY_DROP::NOTHING; i++)
		{
			if (i == (int)ENEMY_DROP::BOMB)
			{
				continue;
			}
			Items[i].Update(Player);
		}

		if (Player->GetHp() < 0)
		{
			isGameOver = true;
		}
	}
	else if(isGameOver)
	{
		CheckGameOverAfter();
	}
	else if (isGameClear)
	{
		CheckGameClearAfter();
	}
	return true;
}

void Korone::onDestroy()
{
	background2.Destroy();
	background3.Destroy();
	for (int i = 0; i < 2; i++)
	{
		playArea[i].Destroy();
	}
	
	for (int i = 0; i < (int)UnitType::MAX; i++)
	{
		Unit[i]->Destroy();
	}

	SoundManager::stop(SOUNDMANAGER::Mogumogu);
}

void Korone::TimeControl()
{
	Timer += FrameRateManager::getDeltaTime();
	if (Timer > 100)
	{
		Timer -= 100;
		isSpawn = false;
		TimeCount++;
	}

	//
	//SpawnControl();
	//
	return;
}

void  Korone::BackGroundMove()
{
	for (int i = 0; i < 2; i++)
	{
		playArea[i].Update();
		if (playArea[i].getIsGoal())
		{
			playArea[i].SetPos(Vector2(BACKGROUND_POS_X, BACKGROUND_POSX_Y));
		}
	}
}

void Korone::SpawnControl()
{
	if (SpawnDataCount > NowSpawn)
	{
		SpawnEnemyData table = SpawnTable[NowStep][NowSpawn];
		if (table.spawnTime < TimeCount)
		{
			NowSpawn++;
			Enemy[(int)table.name]->SpawnEnemy(table);
		}
	}
	else if(CheckEnemyClear())
	{
		if (!isTalkStart)
		{
			TalkWaitTime++;
			if (TalkWaitTime > MaxTalkWaitTime)
			{
				isTalkStart = true;
			}
		}
		else
		{
			if (!isTalkEnd)
			{
				TalkPart();
			}
			else
			{
				if (!Boss[NowStep]->getIsSpawn())
				{
					Boss[NowStep]->Spawn();
				}
				if (Boss[NowStep]->getIsClear())
				{
					if (StepDataCount - 1 > NowStep)
					{
						NowStep++;
						NowSpawn = 0;
						TimeCount = 0;
						SpawnDataCount = sizeof(SpawnTable[NowStep]) / sizeof(SpawnEnemyData);
						TalkWaitTime = 0;
						isTalkStart = false;
						isTextEnd = false;
						isTalkEnd = false;
					}
					else
					{
						isGameClear = true;
					}
				}
			}
		}
	}
}

bool Korone::CheckEnemyClear()
{
	for (int i = 0; i < (int)EnemyType::MAX; i++)
	{
		if (!Enemy[i]->CheckEnemyClear())
		{
			return false;
		}
	}
	return true;
}

void Korone::TalkPart()
{
	if (!isTextEnd)
	{
		if (TalkWindow.GetScale() < 0.25f)
		{
			TalkWindow.AddScale(0.01f);
			EnemyTalk[0].AddColor(0.01f);
			PlayerTalk[0].AddColor(0.04f);
		}
		else
		{
			TextManagement();
		}
	}
	else
	{
		TalkWindow.AddScale(-0.01f);
		EnemyTalk[0].AddColor(-0.04f);
		PlayerTalk[0].AddColor(-0.04f);
		if (TalkWindow.GetScale() == 0 &&
			EnemyTalk[0].GetColorArufa() == 0 &&
			PlayerTalk[0].GetColorArufa() == 0)
		{
			isTalkEnd = true;
		}
	}
}

void Korone::TextManagement()
{
	if (!isAllOutPut) {

		FramerateCount++;
		if (FramerateCount >= TextSpeed) {

			FramerateCount = 0;
			TextOutPutCount += 2;

			SoundManager::GetInstance()->Play(0);

			int temp = 0;
			for (int i = StartNum; i < TextOutPutCount; i++) {

				if (TextList[NowStep][TextLineCount][i] == '*')
				{
					TextOutPutCount++;
					OutPutCharNum++;
					i++;
					StartNum = i;
					temp = 0;
				}

				OutPutChar[OutPutCharNum][temp] = TextList[NowStep][TextLineCount][i];

				if (OutPutChar[OutPutCharNum][temp] == '\0') 
				{
					isAllOutPut = true;
					break;
				}
				else if (OutPutChar[OutPutCharNum][temp] == '~')
				{
					TextLineCount = 0;
					TextOutPutCount = 0;
					isTextEnd = true;
					break;
				}
				temp++;
			}
		}
		if (InputManager::GetPush(INPUT_TYPE::enter))
		{
			TextOutPutCount = 100;
		}
	}
	else {
		CheckEnter();
	}
}

void Korone::CheckEnter()
{
	if (InputManager::GetPush(INPUT_TYPE::enter)) {

		OutPutCharNum = 0;
		StartNum = 0;
		for (int j = 0; j < 2; j++)
		{
			int i = 0;
			while (OutPutChar[j][i] != '\0')
			{
				OutPutChar[j][i] = '\0';
				i++;
			}
		}

		isAllOutPut = false;
		FramerateCount = 0;
		TextOutPutCount = 0;
		TextLineCount++;

		switch (TextLineCount)
		{
		case 1:
		case 3:
			PlayerTalk[0].SetColor(Color(0.25f));
			EnemyTalk[0].SetColor(Color(1));
			break;
		case 2:
		case 4:
			EnemyTalk[0].SetColor(Color(0.25f));
			PlayerTalk[0].SetColor(Color(1));
			break;
		}
	}

}

void Korone::CheckGameOverAfter()
{
	if (GameOver.GetScale() < 0.5f)
	{
		GameOver.AddScale(0.01f);
		Cursol.AddColor(0.01f);
	}
	else
	{
		sprintf_s(TototalScore, 256, "%d", ScoreManager::GetScore());
		if (InputManager::GetPush(INPUT_TYPE::down))
		{
			Cursol.SetPos(Vector2(-300, 100));
			cursolNum = 1;
		}
		else if (InputManager::GetPush(INPUT_TYPE::up))
		{
			Cursol.SetPos(Vector2(-300, 30));
			cursolNum = 0;
		}
		else if (InputManager::GetPush(INPUT_TYPE::z))
		{
			ScoreManager::ResetScore();
			switch (cursolNum)
			{
			case 0:
				Manager::ThreadLoadScene(new ShootingTitleScene(dev, cmdList));
				break;
			case 1:
				Manager::ThreadLoadScene(new Korone(dev, cmdList));
				break;
			}
		}
	}
}

void Korone::CheckGameClearAfter()
{
	if (GameClear.GetScale() < 0.5f)
	{
		GameClear.AddScale(0.01f);
		Cursol.AddColor(0.01f);
	}
	else
	{
		sprintf_s(TototalScore, 256, "%d", ScoreManager::GetScore());
		
		if (InputManager::GetPush(INPUT_TYPE::z))
		{
			//シーンによって行き先は違う
			Manager::ThreadLoadScene(new ShootingTitleScene(dev, cmdList));
		}
	}
}