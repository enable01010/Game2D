#pragma once
#include "stdafx.h"

enum class TITLE_STR
{
	SIRA,
	KAMI,
	KO,
	MA,
	KYO,
	ENGLISH,
	MAX
};

enum class MODESELECT_STR
{
	START,
	QUIT,
	MAX
};

class ShootingTitleScene :public BaseScene
{
	bool isGameEnd = false;

	//背景
	VTN_Image Background;
	const float BACKGROUND_IMG_SIZE = 1.6f;

	//タイトルの文字関係
	VTN_MoveImage Title[(int)TITLE_STR::MAX];
	bool TitleMove[(int)TITLE_STR::MAX];
	const float TITLE_IMG_SIZE = 0.5f;
	int Degree = 0;//紅の傾き管理用

	const int TITLE_FIRST_POS_X = 0;
	const int TITLE_SECOND_POS_X = -400;
	const int TITLE_SIRA_GOAL_POS_Y = -300;
	const int TITLE_KAMI_GOAL_POS_Y = -150;
	const int TITLE_KO_GOAL_POS_Y = 0;
	const int TITLE_MA_GOAL_POS_Y = 150;
	const int TITLE_KYO_GOAL_POS_Y = 300;
	const int TITLE_ENGLISH_GOAL_POS_Y = 0;

	const int TITLE_SIRA_FIRST_POS_X = 0;
	const int TITLE_SIRA_FIRST_POS_Y = 400;
	const int TITLE_KAMI_FIRST_POS_X = -700;
	const int TITLE_KAMI_FIRST_POS_Y = -150;
	const int TITLE_KO_FIRST_POS_X = -300;
	const int TITLE_KO_FIRST_POS_Y = -450;
	const int TITLE_MA_FIRST_POS_X = 700;
	const int TITLE_MA_FIRST_POS_Y = 150;
	const int TITLE_KYO_FIRST_POS_X = 0;
	const int TITLE_KYO_FIRST_POS_Y = -450;
	const int TITLE_ENGLISH_FIRST_POS_X = -400;
	const int TITLE_ENGLISH_FIRST_POS_Y = -800;

	const int TITLE_SIRA_FINAL_POS_X = -400;
	const int TITLE_SIRA_FINAL_POS_Y = 400;
	const int TITLE_KAMI_FINAL_POS_X = -700;
	const int TITLE_KAMI_FINAL_POS_Y = -150;
	const int TITLE_KO_FINAL_POS_X = -700;
	const int TITLE_KO_FINAL_POS_Y = -450;
	const int TITLE_MA_FINAL_POS_X = -700;
	const int TITLE_MA_FINAL_POS_Y = 150;
	const int TITLE_KYO_FINAL_POS_X = -400;
	const int TITLE_KYO_FINAL_POS_Y = -450;
	const int TITLE_ENGLISH_FINAL_POS_X = -800;
	const int TITLE_ENGLISH_FINAL_POS_Y = -800;

	//モードセレクトの関係
	int SelectNum = 0;
	VTN_MoveImage modeSelect[7];
	const float MODESELECT_IMG_SIZE = 0.5f;
	VTN_MoveImage modeSelectRed[7];
	const float MODESELECTRED_IMG_SIZE = 0.5f;

	const int FIRST_MODEDELECT_FIRST_POS_X = 1100;
	const int FIRST_MODEDELECT_SECOND_POS_X = 400;
	const int  FIRST_MODEDELECT_FIRST_POS_Y = 150;
	const int MODESELECT_INTERVAL_X = 0;
	const int  MODESELECT_INTERVAL_Y = 50;

	const int FIRST_MODEDELECTRED_FIRST_POS_X = 950;
	const int FIRST_MODEDELECTRED_SECOND_POS_X = 400;
	const int  FIRST_MODEDELECTRED_FIRST_POS_Y = 150;
	const int MODESELECTRED_INTERVAL_X = 0;
	const int  MODESELECTRED_INTERVAL_Y = 50;

	//シーンの進行度管理用
	int ZCount = 0;

	//入力関係用
	const int INPUT_DELAY_TIME = 50;
	const int INPUT_SPEED = 10;

	/// <summary>
	/// Zキーが押されたときの処理
	/// </summary>
	void ChengeZCount();

public:

	//オーバーロード
	ShootingTitleScene(ID3D12Device* Dev, ID3D12GraphicsCommandList* CmdList) :BaseScene(Dev, CmdList){}
	void Init();
	void Draw();
	bool Update();
	void onDestroy();
};
