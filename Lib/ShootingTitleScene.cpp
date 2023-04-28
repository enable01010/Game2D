#include "Scene.h"


void ShootingTitleScene::Init() {
	SoundManager::Play(SOUNDMANAGER::Fanfarle);

	//背景
	Background.Init(dev, L"img\\Title\\Title.png");
	Background.SetScale(Vector2(BACKGROUND_IMG_SIZE, BACKGROUND_IMG_SIZE));

	//タイトルの文字関係
	Title[(int)TITLE_STR::SIRA].Init(dev, L"img\\Title\\sira.png", Vector2(TITLE_FIRST_POS_X, TITLE_SIRA_GOAL_POS_Y));
	Title[(int)TITLE_STR::KAMI].Init(dev, L"img\\Title\\kami.png", Vector2(TITLE_FIRST_POS_X, TITLE_KAMI_GOAL_POS_Y));
	Title[(int)TITLE_STR::KO].Init(dev, L"img\\Title\\kou.png", Vector2(TITLE_FIRST_POS_X, TITLE_KO_GOAL_POS_Y));
	Title[(int)TITLE_STR::MA].Init(dev, L"img\\Title\\ma.png", Vector2(TITLE_FIRST_POS_X, TITLE_MA_GOAL_POS_Y));
	Title[(int)TITLE_STR::KYO].Init(dev, L"img\\Title\\kyo.png", Vector2(TITLE_FIRST_POS_X, TITLE_KYO_GOAL_POS_Y));
	Title[(int)TITLE_STR::ENGLISH].Init(dev, L"img\\Title\\english.png", Vector2(TITLE_FIRST_POS_X, TITLE_ENGLISH_GOAL_POS_Y));
	for (int i = 0; i < (int)TITLE_STR::MAX; i++) {
		Title[i].SetScale(Vector2(TITLE_IMG_SIZE, TITLE_IMG_SIZE));
		TitleMove[i] = false;
	}
	Title[(int)TITLE_STR::SIRA].SetPos(Vector2(TITLE_SIRA_FIRST_POS_X, TITLE_SIRA_FIRST_POS_Y));
	Title[(int)TITLE_STR::KAMI].SetPos(Vector2(TITLE_KAMI_FIRST_POS_X, TITLE_KAMI_FIRST_POS_Y));
	Title[(int)TITLE_STR::KO].SetPos(Vector2(TITLE_KO_FIRST_POS_X, TITLE_KO_FIRST_POS_Y));
	Title[(int)TITLE_STR::MA].SetPos(Vector2(TITLE_MA_FIRST_POS_X, TITLE_MA_FIRST_POS_Y));
	Title[(int)TITLE_STR::KYO].SetPos(Vector2(TITLE_KYO_FIRST_POS_X, TITLE_KYO_FIRST_POS_Y));
	Title[(int)TITLE_STR::ENGLISH].SetPos(Vector2(TITLE_ENGLISH_FIRST_POS_X, TITLE_ENGLISH_FIRST_POS_Y));
	
	//モードセレクトの関係
	modeSelect[(int)MODESELECT_STR::START].Init(dev, L"img\\Title\\Start_w.png", Vector2());
	modeSelect[(int)MODESELECT_STR::QUIT].Init(dev, L"img\\Title\\Quit_w.png", Vector2());

	modeSelectRed[(int)MODESELECT_STR::START].Init(dev, L"img\\Title\\Start.png", Vector2());
	modeSelectRed[(int)MODESELECT_STR::QUIT].Init(dev, L"img\\Title\\Quit.png", Vector2());
	
	for (int i = 0; i < (int)MODESELECT_STR::MAX; i++) {
		modeSelect[i].SetPos(Vector2(FIRST_MODEDELECT_FIRST_POS_X + MODESELECT_INTERVAL_X * i, FIRST_MODEDELECT_FIRST_POS_Y + MODESELECT_INTERVAL_Y * i));
		modeSelect[i].setGoalPos(Vector2(FIRST_MODEDELECT_FIRST_POS_X + MODESELECT_INTERVAL_X * i, FIRST_MODEDELECT_FIRST_POS_Y + MODESELECT_INTERVAL_Y * i));
		modeSelect[i].SetScale(Vector2(MODESELECT_IMG_SIZE, MODESELECT_IMG_SIZE));
		modeSelectRed[i].SetPos(Vector2(FIRST_MODEDELECTRED_FIRST_POS_X + MODESELECTRED_INTERVAL_X * i, FIRST_MODEDELECTRED_FIRST_POS_Y + MODESELECTRED_INTERVAL_Y * i));
		modeSelectRed[i].setGoalPos(Vector2(FIRST_MODEDELECTRED_FIRST_POS_X + MODESELECTRED_INTERVAL_X * i, FIRST_MODEDELECTRED_FIRST_POS_Y + MODESELECTRED_INTERVAL_Y * i));
		modeSelectRed[i].SetScale(Vector2(MODESELECTRED_IMG_SIZE, MODESELECTRED_IMG_SIZE));
	}

	//SoundManager::Play((int)SOUND_NAME::FANFARRE);
}

void ShootingTitleScene::Draw(){

	Background.Draw(cmdList);

	for (int i = 0; i < (int)TITLE_STR::MAX; i++)
	{
		Title[i].Draw(cmdList);
	}

	for (int i = 0; i < (int)MODESELECT_STR::MAX; i++)
	{
		if (i == SelectNum) {
			modeSelectRed[i].Draw(cmdList);
		}
		else {
			modeSelect[i].Draw(cmdList);
		}
	}
}

bool ShootingTitleScene::Update() {

	//オブジェクトのアップデート
	for (int i = 0; i < (int)TITLE_STR::MAX; i++)
	{
		Title[i].Update();
		if (Title[i].getIsGoal())
		{
			TitleMove[i] = true;
		}
	}
	for (int i = 0; i < (int)MODESELECT_STR::MAX; i++)
	{
		modeSelect[i].Update();
		modeSelectRed[i].Update();
	}
	//紅の傾き
	if (Title[5].getIsGoal())
	{
		if (Degree < 55)
		{
			Degree += 2;
			Title[2].SetRotation(Degree);
		}
	
	}

	//入力受付
	if (InputManager::GetPush(INPUT_TYPE::z))
	{
		if (ZCount < 2)
		{
			ZCount++;
			ChengeZCount();
		}
		SoundManager::Play((int)SOUND_NAME::CLICK);
	}
	else if (InputManager::GetPush(INPUT_TYPE::x))
	{
		if (ZCount > 0)
		{
			ZCount--;
			ChengeZCount();
		}
		SoundManager::Play((int)SOUND_NAME::CLICK);
	}

	//シーンの進捗度によって変わる処理（Zボタンが押された回数）
	if (ZCount == 0)
	{

	}
	else if (ZCount == 1) {
		if (InputManager::GetPush(INPUT_TYPE::up) || (InputManager::GetTime(INPUT_TYPE::up) % INPUT_SPEED == 1 && InputManager::GetTime(INPUT_TYPE::up) >= INPUT_DELAY_TIME))
		{
			SoundManager::Play((int)SOUND_NAME::CLICK);
			if (SelectNum == 0) {
				SelectNum = (int)MODESELECT_STR::MAX - 1;
			}
			else {
				SelectNum--;
			}
		}
		else if (InputManager::GetPush(INPUT_TYPE::down) || (InputManager::GetTime(INPUT_TYPE::down) % INPUT_SPEED == 1 && InputManager::GetTime(INPUT_TYPE::down) >= INPUT_DELAY_TIME))
		{
			SoundManager::Play((int)SOUND_NAME::CLICK);
			if (SelectNum == (int)MODESELECT_STR::MAX - 1) {
				SelectNum = 0;
			}
			else {
				SelectNum++;
			}
		}
	}
	return !isGameEnd;
}

void ShootingTitleScene::ChengeZCount()
{

	switch (ZCount)
	{
	case 0: 
		Title[(int)TITLE_STR::SIRA].setGoalPos(Vector2(TITLE_FIRST_POS_X, TITLE_SIRA_GOAL_POS_Y));
		Title[(int)TITLE_STR::KAMI].setGoalPos(Vector2(TITLE_FIRST_POS_X, TITLE_KAMI_GOAL_POS_Y));
		Title[(int)TITLE_STR::KO].setGoalPos(Vector2(TITLE_FIRST_POS_X, TITLE_KO_GOAL_POS_Y));
		Title[(int)TITLE_STR::MA].setGoalPos(Vector2(TITLE_FIRST_POS_X, TITLE_MA_GOAL_POS_Y));
		Title[(int)TITLE_STR::KYO].setGoalPos(Vector2(TITLE_FIRST_POS_X, TITLE_KYO_GOAL_POS_Y));
		Title[(int)TITLE_STR::ENGLISH].setGoalPos(Vector2(TITLE_FIRST_POS_X, TITLE_ENGLISH_GOAL_POS_Y));

		for (int i = 0; i < (int)MODESELECT_STR::MAX; i++)
		{
			modeSelect[i].setGoalPos(Vector2(FIRST_MODEDELECT_FIRST_POS_X + i * MODESELECT_INTERVAL_X, FIRST_MODEDELECT_FIRST_POS_Y + MODESELECT_INTERVAL_Y * i));
			modeSelectRed[i].setGoalPos(Vector2(FIRST_MODEDELECTRED_FIRST_POS_X + i * MODESELECTRED_INTERVAL_X, 0 + MODESELECTRED_INTERVAL_Y * i));
		}

		break;
	case 1:
		for (int i = 0; i < (int)TITLE_STR::MAX; i++)
		{
			TitleMove[i] = true;
		}
		Title[(int)TITLE_STR::SIRA].setGoalPos(Vector2(TITLE_SECOND_POS_X, TITLE_SIRA_GOAL_POS_Y));
		Title[(int)TITLE_STR::KAMI].setGoalPos(Vector2(TITLE_SECOND_POS_X, TITLE_KAMI_GOAL_POS_Y));
		Title[(int)TITLE_STR::KO].setGoalPos(Vector2(TITLE_SECOND_POS_X, TITLE_KO_GOAL_POS_Y));
		Title[(int)TITLE_STR::MA].setGoalPos(Vector2(TITLE_SECOND_POS_X, TITLE_MA_GOAL_POS_Y));
		Title[(int)TITLE_STR::KYO].setGoalPos(Vector2(TITLE_SECOND_POS_X, TITLE_KYO_GOAL_POS_Y));
		Title[(int)TITLE_STR::ENGLISH].setGoalPos(Vector2(TITLE_SECOND_POS_X, TITLE_ENGLISH_GOAL_POS_Y));

		for (int i = 0; i < (int)MODESELECT_STR::MAX; i++)
		{
			modeSelect[i].setGoalPos(Vector2(FIRST_MODEDELECT_SECOND_POS_X + i * MODESELECT_INTERVAL_X, FIRST_MODEDELECT_FIRST_POS_Y + MODESELECT_INTERVAL_Y * i));
			modeSelectRed[i].setGoalPos(Vector2(FIRST_MODEDELECTRED_SECOND_POS_X + i * MODESELECTRED_INTERVAL_X, FIRST_MODEDELECTRED_FIRST_POS_Y + MODESELECTRED_INTERVAL_Y * i));
		}
		
		break;
	case 2:

		Title[(int)TITLE_STR::SIRA].setGoalPos(Vector2(TITLE_SIRA_FINAL_POS_X, TITLE_SIRA_FINAL_POS_Y));
		Title[(int)TITLE_STR::KAMI].setGoalPos(Vector2(TITLE_KAMI_FINAL_POS_X, TITLE_KAMI_FINAL_POS_Y));
		Title[(int)TITLE_STR::KO].setGoalPos(Vector2(TITLE_KO_FINAL_POS_X, TITLE_KO_FINAL_POS_Y));
		Title[(int)TITLE_STR::MA].setGoalPos(Vector2(TITLE_MA_FINAL_POS_X, TITLE_MA_FINAL_POS_Y));
		Title[(int)TITLE_STR::KYO].setGoalPos(Vector2(TITLE_KYO_FINAL_POS_X, TITLE_KYO_FINAL_POS_Y));
		Title[(int)TITLE_STR::ENGLISH].setGoalPos(Vector2(TITLE_ENGLISH_FINAL_POS_X, TITLE_ENGLISH_FINAL_POS_Y));

		for (int i = 0; i < (int)MODESELECT_STR::MAX; i++)
		{
			modeSelect[i].setGoalPos(Vector2(FIRST_MODEDELECT_FIRST_POS_X + i * MODESELECT_INTERVAL_X, FIRST_MODEDELECT_FIRST_POS_Y + MODESELECT_INTERVAL_Y * i));
			modeSelectRed[i].setGoalPos(Vector2(FIRST_MODEDELECTRED_FIRST_POS_X + i * MODESELECTRED_INTERVAL_X, 0 + MODESELECTRED_INTERVAL_Y * i));
		}
		switch (SelectNum)
		{
		case (int)MODESELECT_STR::QUIT:
			isGameEnd = true;
			break;
		default:
			Manager::ThreadLoadScene(new SelectScene(dev, cmdList));
			break;
		}
	}
	
}

void ShootingTitleScene::onDestroy() {
	Background.Destroy();
	for (int i = 0; i < (int)TITLE_STR::MAX; i++)
	{
		Title[i].Destroy();
	}
	for (int i = 0; i < (int)MODESELECT_STR::MAX; i++)
	{
		modeSelect[i].Destroy();
		modeSelectRed[i].Destroy();
	}
}
