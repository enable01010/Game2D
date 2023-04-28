#include "Scene.h"

int SelectScene::PlayerNum = 0;
int SelectScene::HudaNum = 0;

void SelectScene::Init() 
{
	//背景
	background.Init(dev, L"img\\Select\\background.png");
	background.SetScale(Vector2(BACKGROUND_IMG_SIZE, BACKGROUND_IMG_SIZE));
	
	//モード選択
	selectMode.Init(dev, L"img\\Select\\modeSelect.png", Vector2(SELECTMODE_SECOND_POS_X, SELECTMODE_SECOND_POS_Y));
	selectMode.SetScale(Vector2(SELECTMODE_IMG_SIZE, SELECTMODE_IMG_SIZE));
	selectMode.SetPos(Vector2(SELECTMODE_FIRST_POS_X, SELECTMODE_FIRST_POS_Y));
	Modes[(int)MODE_TYPE::EASY].Init(dev, L"img\\Select\\easy.png", Vector2());
	Modes[(int)MODE_TYPE::NORMAL].Init(dev, L"img\\Select\\normal.png", Vector2());
	Modes[(int)MODE_TYPE::HARD].Init(dev, L"img\\Select\\hard.png", Vector2());
	Modes[(int)MODE_TYPE::LUNATIC].Init(dev, L"img\\Select\\Lunatic.png", Vector2());

	for (int i = 0; i < (int)MODE_TYPE::MAX; i++) 
	{
		Modes[i].SetScale(Vector2(MODES_IMG_SIZE, MODES_IMG_SIZE));
		Modes[i].SetPos(Vector2(FIRST_MODES_POS_X, FIRST_MODES_POS_Y + MODES_INTERVAL_Y * i));
	}
	ChangeModeNum();
	
	//キャラクター選択
	selectPlayer.Init(dev, L"img\\Select\\playerSelect.png", Vector2(SELECTPLAYER_FIRST_POS_X, SELECTPALYER_FIRST_POS_Y));
	selectPlayer.SetScale(Vector2(SELECTPLAYER_IMG_SIZE, SELECTPLAYER_IMG_SIZE));
	player[(int)CHARACTER_TYPE::SIRAKAMI].Init(dev, L"img\\Select\\siro.png", Vector2(PLAYER_FIRST_POS_X, PLAYER_FIRST_POS_Y));
	player[(int)CHARACTER_TYPE::KUROUE].Init(dev, L"img\\Select\\kuro.png", Vector2(PLAYER_FIRST_POS_X, PLAYER_FIRST_POS_Y));

	selectPlayer.SetPos(Vector2(SELECTPLAYER_FIRST_POS_X, SELECTPALYER_FIRST_POS_Y));
	for (int i = 0; i < (int)CHARACTER_TYPE::MAX; i++)
	{
		player[i].SetPos(Vector2(PLAYER_FIRST_POS_X, PLAYER_FIRST_POS_Y));
		player[i].SetScale(Vector2(PLAYER_IMG_SIZE, PLAYER_IMG_SIZE));
	}

	//スキル選択
	selectHuda.Init(dev, L"img\\Select\\hudaSelect.png", Vector2(SELECTHUDA_FIRSTPOS_X, SELECTHUDA_FIRSTPOS_Y));
	selectHuda.SetScale(Vector2(SELECTHUDA_IMG_SIZE, SELECTHUDA_IMG_SIZE));
	huda[(int)SKILL_TYPE::REI].Init(dev, L"img\\Select\\rei.png", Vector2());
	huda[(int)SKILL_TYPE::YUME].Init(dev, L"img\\Select\\yume.png", Vector2());
	huda[(int)SKILL_TYPE::MA].Init(dev, L"img\\Select\\ma.png", Vector2());
	huda[(int)SKILL_TYPE::KOI].Init(dev, L"img\\Select\\koi.png", Vector2());
	
	selectHuda.SetPos(Vector2(SELECTHUDA_FIRSTPOS_X, SELECTHUDA_FIRSTPOS_Y));
	for (int i = 0; i < (int)SKILL_TYPE::MAX; i++)
	{
		if (i % (int)CHARACTER_TYPE::MAX == 0) {
			huda[i].SetPos(Vector2(HUDA_FIRST_POS_X, FIRST_HUDA_FIRST_POS_Y));
		}
		else
		{
			huda[i].SetPos(Vector2(HUDA_FIRST_POS_X, SECOND_HUDA_FIRST_POS_Y));
			huda[i].SetColor(Color(1,1,1, HUDA_IMG_ARUFA));
		}
		huda[i].SetScale(Vector2(HUDA_IMG_SIZE, HUDA_IMG_SIZE));
		huda[i].setGoalPos(Vector2(HUDA_FIRST_POS_X, SECOND_HUDA_FIRST_POS_Y));
	}
}

void SelectScene::Draw() 
{
	background.Draw(cmdList);

	selectMode.Draw(cmdList);
	for (int i = 0; i < (int)MODE_TYPE::MAX; i++)
	{
		Modes[i].Draw(cmdList);
	}

	selectPlayer.Draw(cmdList);
	player[PlayerNum].Draw(cmdList);

	selectHuda.Draw(cmdList);
	for (int i = 0; i < (int)SKILL_TYPE::MAX; i++)
	{
		huda[i].Draw(cmdList);
	}

		
}

bool SelectScene::Update() 
{
	//オブジェクトアップデート
	selectMode.Update();
	for (int i = 0; i < (int)MODE_TYPE::MAX; i++)
	{
		Modes[i].Update();
	}
	selectPlayer.Update();
	for (int i = 0; i < (int)CHARACTER_TYPE::MAX;i++)
	{
		player[i].Update();
	}
	selectHuda.Update();
	for (int i = 0; i < (int)SKILL_TYPE::MAX; i++)
	{
		huda[i].Update();
	}

	//入力関係
	if (InputManager::GetPush(INPUT_TYPE::z))
	{
		if (!(PlayerNum == 1 && ZCount == 1 )&& ModeNum != 3)
		{
			ZCount++;
			ChangeZCount();
		}
		SoundManager::Play((int)SOUND_NAME::CLICK);
	}
	else if (InputManager::GetPush(INPUT_TYPE::x))
	{
		ZCount--;
		ChangeZCount();
		SoundManager::Play((int)SOUND_NAME::CLICK);
	}

	//シーンの進捗度によって変わる処理
	switch (ZCount)
	{
	case 0:

		//入力関係
		if (InputManager::GetPush(INPUT_TYPE::down) || (InputManager::GetTime(INPUT_TYPE::down) % 10 == 1 && InputManager::GetTime(INPUT_TYPE::down) >= 100))
		{
			ModeNum++;
			if (ModeNum > (int)MODE_TYPE::MAX - 1)
			{
				ModeNum = 0;
			}
			ChangeModeNum();
			SoundManager::Play((int)SOUND_NAME::CLICK);
		}
		else if (InputManager::GetPush(INPUT_TYPE::up) || (InputManager::GetTime(INPUT_TYPE::up) % 10 == 1 && InputManager::GetTime(INPUT_TYPE::up) >= 100))
		{
			ModeNum--;
			if (ModeNum < 0)
			{
				ModeNum = (int)MODE_TYPE::MAX - 1;
			}
			ChangeModeNum();
			SoundManager::Play((int)SOUND_NAME::CLICK);
		}
		break;

	case 1:
		//入力関係
		if (InputManager::GetPush(INPUT_TYPE::right))
		{
			PlayerNum++;
			if (PlayerNum > (int)CHARACTER_TYPE::MAX - 1)
			{
				PlayerNum = 0;
			}
			SoundManager::Play((int)SOUND_NAME::CLICK);
		}
		else if (InputManager::GetPush(INPUT_TYPE::left))
		{
			PlayerNum--;
			if (PlayerNum < 0)
			{
				PlayerNum = (int)CHARACTER_TYPE::MAX - 1;
			}
			SoundManager::Play((int)SOUND_NAME::CLICK);
		}
		break;

	case 2:
		//入力関係
		if (InputManager::GetPush(INPUT_TYPE::up) || InputManager::GetPush(INPUT_TYPE::down))
		{
			huda[HudaNum + PlayerNum * 2].SetColor(Color(1, 1, 1, 0.5f));
			HudaNum = (HudaNum + 1) % 2;
			huda[HudaNum + PlayerNum * 2].SetColor(Color(1,1,1,1.0f));
			SoundManager::Play((int)SOUND_NAME::CLICK);
		}
	}
	return true;
}

void SelectScene::onDestroy() 
{
	background.Destroy();
	selectMode.Destroy();
	for (int i = 0; i < (int)MODE_TYPE::MAX; i++)
	{
		Modes[i].Destroy();
	}
	selectPlayer.Destroy();
	for (int i = 0; i < (int)CHARACTER_TYPE::MAX; i++)
	{
		player[i].Destroy();
	}
	selectHuda.Destroy();
	for (int i = 0; i < (int)SKILL_TYPE::MAX; i++)
	{
		huda[i].Destroy();
	}
}

void SelectScene::ChangeModeNum()
{
	for (int i = 0; i < (int)MODE_TYPE::MAX; i++)
	{
		if (ModeNum == i)
		{
			Modes[i].setGoalPos(Vector2(MODES_CHOSE_POS_X, MODES_CHOSE_POS_Y + MODES_INTERVAL_Y * i));
			Modes[i].SetColor(Color(1, 1, 1, 1.0f));
		}
		else
		{
			Modes[i].setGoalPos(Vector2(MODES_NOMAL_POS_X, MODES_NOMAL_POS_Y + MODES_INTERVAL_Y * i));
			Modes[i].SetColor(Color(1, 1, 1, MODES_IMG_ARUFA));
		}
	}
}

void SelectScene::ChangeZCount()
{
	switch (ZCount)
	{
	case -1:

		Manager::ThreadLoadScene(new ShootingTitleScene(dev, cmdList));

		break;

	case 0:

		selectMode.setGoalPos(Vector2(SELECTMODE_SECOND_POS_X, SELECTMODE_SECOND_POS_Y));
		ChangeModeNum();

		selectPlayer.setGoalPos(Vector2(SELECTPLAYER_FIRST_POS_X, SELECTPALYER_FIRST_POS_Y));
		for (int i = 0; i < (int)CHARACTER_TYPE::MAX; i++)
		{
			player[i].setGoalPos(Vector2(PLAYER_FIRST_POS_X, PLAYER_FIRST_POS_Y));
		}

		break;

	case 1:

		selectMode.setGoalPos(Vector2(SELECTMODE_THIRD_POS_X, SELECTMODE_THIRD_POS_Y));
		for (int i = 0; i < (int)MODE_TYPE::MAX; i++)
		{
			if (ModeNum == i)
			{
				Modes[i].setGoalPos(Vector2(MODES_SECOND_CHOSE_POS_X, MODES_SECOND_CHOSE_POS_Y));
				Modes[i].SetColor(Color(1, 1, 1, MODES_IMG_ARUFA));
			}
			else
			{
				Modes[i].setGoalPos(Vector2(MODES_SECOND_POS_X, FIRST_MODES_POS_Y + MODES_INTERVAL_Y * i));
			}
		}

		selectPlayer.setGoalPos(Vector2(SELECTPLAYER_SECOND_POS_X, SELECTPLAYER_SECOND_POS_Y));
		for (int i = 0; i < (int)CHARACTER_TYPE::MAX; i++)
		{
			player[i].setGoalPos(Vector2(PLAYER_SECOND_POS_X, PLAYER_SECOND_POS_Y));
		}

		selectHuda.setGoalPos(Vector2(SELECTHUDA_FIRSTPOS_X, SELECTHUDA_FIRSTPOS_Y));
		huda[0 + PlayerNum * 2].setGoalPos(Vector2(HUDA_FIRST_POS_X, FIRST_HUDA_FIRST_POS_Y));
		huda[1 + PlayerNum * 2].setGoalPos(Vector2(HUDA_FIRST_POS_X, SECOND_HUDA_FIRST_POS_Y));

		break;

	case 2:

		selectPlayer.setGoalPos(Vector2(SELECTPLAYER_THIRD_POS_X, SELECTPLAYER_THIRD_POS_Y));
		player[PlayerNum].setGoalPos(Vector2(PLAYER_THIRD_POS_X, PLAYER_THIRD_POS_Y));

		selectHuda.setGoalPos(Vector2(SELECTHUDA_SECOND_POS_X, SELECTHUDA_SECOND_POS_Y));
		huda[0 + PlayerNum * 2].setGoalPos(Vector2(HUDA_SECOND_POS_X, FIRST_HUDA_SECOND_POS_Y));
		huda[1 + PlayerNum * 2].setGoalPos(Vector2(HUDA_SECOND_POS_X, SECOND_HUDA_SECOND_POS_Y));
		
		break;

	case 3:
		switch (ModeNum)
		{
		case 0:
			Manager::ThreadLoadScene(new EasyMode_One(dev, cmdList));
			break;
		case 1:
			Manager::ThreadLoadScene(new Korone(dev, cmdList));
			break;
		case 2:
			Manager::ThreadLoadScene(new Miosha(dev, cmdList));
			break;
		}
		SoundManager::stop(SOUNDMANAGER::Fanfarle);
	}
}