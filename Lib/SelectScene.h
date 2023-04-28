#pragma once
#include "stdafx.h"

enum class MODE_TYPE
{
	EASY,
	NORMAL,
	HARD,
	LUNATIC,
	MAX
};

enum class CHARACTER_TYPE
{
	SIRAKAMI,
	KUROUE,
	MAX
};

enum class SKILL_TYPE
{
	REI,
	YUME,
	MA,
	KOI,
	MAX
};


class SelectScene :public BaseScene
{
	//背景
	VTN_Image background;
	const float BACKGROUND_IMG_SIZE = 1.3f;

	//モード選択関係
	int ModeNum = 0;
	VTN_MoveImage selectMode;
	const float SELECTMODE_IMG_SIZE = 0.4f;
	const int SELECTMODE_FIRST_POS_X = 0;
	const int SELECTMODE_FIRST_POS_Y = -400;
	const int SELECTMODE_SECOND_POS_X = 0;
	const int SELECTMODE_SECOND_POS_Y = -280;
	const int SELECTMODE_THIRD_POS_X = -900;
	const int SELECTMODE_THIRD_POS_Y = -280;
	
	VTN_MoveImage Modes[(int)MODE_TYPE::MAX];
	const float MODES_IMG_SIZE = 0.25f;
	const float MODES_IMG_ARUFA = 0.5f;
	const int MODES_NOMAL_POS_X = 0;
	const int MODES_NOMAL_POS_Y = -130;
	const int MODES_CHOSE_POS_X = -30;
	const int MODES_CHOSE_POS_Y = -140;
	const int FIRST_MODES_POS_X = 800;
	const int FIRST_MODES_POS_Y = -130;
	const int MODES_INTERVAL_Y = 140;
	const int MODES_SECOND_CHOSE_POS_X = -400;
	const int MODES_SECOND_CHOSE_POS_Y = 250;
	const int MODES_SECOND_POS_X = -900;

	//キャラクター選択関係
	static int PlayerNum;
	VTN_MoveImage selectPlayer;
	const float SELECTPLAYER_IMG_SIZE = 0.4f;
	const int SELECTPLAYER_FIRST_POS_X = 0;
	const int SELECTPALYER_FIRST_POS_Y = -450;
	const int SELECTPLAYER_SECOND_POS_X = 0;
	const int SELECTPLAYER_SECOND_POS_Y = -280;
	const int SELECTPLAYER_THIRD_POS_X = 0;
	const int SELECTPLAYER_THIRD_POS_Y = -450;

	VTN_MoveImage player[(int)CHARACTER_TYPE::MAX];
	const float PLAYER_IMG_SIZE = 0.3f;
	const int PLAYER_FIRST_POS_X = 900;
	const int PLAYER_FIRST_POS_Y = -30;
	const int PLAYER_SECOND_POS_X = 300;
	const int PLAYER_SECOND_POS_Y = -30;
	const int PLAYER_THIRD_POS_X = 0;
	const int PLAYER_THIRD_POS_Y = -30;
	
	//スキル関係
	static int HudaNum;
	VTN_MoveImage selectHuda;
	const float SELECTHUDA_IMG_SIZE = 0.3f;
	const int SELECTHUDA_FIRSTPOS_X = 400;
	const int SELECTHUDA_FIRSTPOS_Y = -450;
	const int SELECTHUDA_SECOND_POS_X = 400;
	const int SELECTHUDA_SECOND_POS_Y = -200;

	VTN_MoveImage huda[4];
	const float HUDA_IMG_SIZE = 0.3f;
	const float HUDA_IMG_ARUFA = 0.5f;
	const int HUDA_FIRST_POS_X = 950;
	const int FIRST_HUDA_FIRST_POS_Y = -100;
	const int SECOND_HUDA_FIRST_POS_Y = 0;
	const int HUDA_SECOND_POS_X = 400;
	const int FIRST_HUDA_SECOND_POS_Y = -100;
	const int SECOND_HUDA_SECOND_POS_Y = 0;

	//シーンの進捗管理
	int ZCount = 0;

	/// <summary>
	/// モード選択切り替え用
	/// </summary>
	void ChangeModeNum();

	/// <summary>
	/// Zキーが押されたときの処理
	/// </summary>
	void ChangeZCount();

public:

	static int getPlayerNum() { return PlayerNum; }
	static int getHudaNum() { return HudaNum; }

	SelectScene(ID3D12Device* Dev, ID3D12GraphicsCommandList* CmdList) :BaseScene(Dev, CmdList)
	{

	}
	void Init();
	void Draw();
	bool Update();
	void onDestroy();
};