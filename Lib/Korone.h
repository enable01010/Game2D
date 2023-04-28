#pragma once
#include "stdafx.h"

class Korone :public BaseScene
{
	//ゲームプレイエリアの設定用
	VTN_Image background2;
	const int LEFT_SIDE_MAX = -1775;
	VTN_Image background3;
	const int RIGHT_SIDE_MAX = 1575;

	//背景の設定用
	VTN_MoveImage playArea[2];
	const int BACKGROUND_POS_X = -100;
	const int BACKGROUND_GOAL_POS_Y = 1000;
	const int BACKGROUND_SPEED = 1;
	const int BACKGROUND_POSX_Y = -1200;
	const float BACKGROUND_SIZE = 0.25f;

	//ユニット管理用
	STG_Player* Player = new STG_Player();
	STG_Enemy* Hubu = new STG_Enemy();
	STG_Enemy* Sisi = new STG_Enemy();
	STG_Boss* BossMid = new STG_Boss_Korone_Mid();
	STG_Boss* BossLast = new STG_Boss_Korone_Last();

	//ユニット管理用
	enum class UnitType
	{
		PLAYER,
		HUBU,
		SISI,
		MID_BOSS,
		LAST_BOSS,
		MAX
	};

	STG_Unit* Unit[(int)UnitType::MAX] =
	{
		Player,
		Hubu,
		Sisi,
		BossMid,
		BossLast
	};

	//エネミー管理用
	enum class EnemyType
	{
		HUBU,
		SISI,
		MAX
	};
	STG_Enemy* Enemy[(int)ENEMY_NAME::MAX] =
	{
		Hubu,
		Sisi
	};
	//ボス管理用
	enum class BossType
	{
		MID_BOSS,
		LAST_BOSS,
		MAX
	};
	STG_Boss* Boss[(int)BossType::MAX] =
	{
		BossMid,
		BossLast
	};

	//エネミー管理用
	const float HUBURS_SIZE = 0.5f;
	const float SISI_SIZE = 0.3f;
	SpawnEnemyData SpawnTable[2][48] = {
		{
			10,ENEMY_NAME::HUBU,Vector2(-200, -400), ENEMY_TYPE::D_S_D, ENEMY_MODE::RIGHT, ENEMY_DROP::RANDOM,1,
			12,ENEMY_NAME::HUBU,Vector2(-190, -400), ENEMY_TYPE::D_S_D, ENEMY_MODE::RIGHT, ENEMY_DROP::RANDOM,1,
			14,ENEMY_NAME::HUBU,Vector2(-180, -400), ENEMY_TYPE::D_S_D, ENEMY_MODE::RIGHT, ENEMY_DROP::RANDOM,1,
			16,ENEMY_NAME::HUBU,Vector2(-170, -400), ENEMY_TYPE::D_S_D, ENEMY_MODE::RIGHT, ENEMY_DROP::RANDOM,1,
			18,ENEMY_NAME::HUBU,Vector2(-160, -400), ENEMY_TYPE::D_S_D, ENEMY_MODE::RIGHT, ENEMY_DROP::RANDOM,1,
			30,ENEMY_NAME::HUBU,Vector2(10, -400), ENEMY_TYPE::D_S_D, ENEMY_MODE::LEFT, ENEMY_DROP::RANDOM,1,
			32,ENEMY_NAME::HUBU,Vector2(0, -400), ENEMY_TYPE::D_S_D, ENEMY_MODE::LEFT, ENEMY_DROP::RANDOM,1,
			34,ENEMY_NAME::HUBU,Vector2(-10, -400), ENEMY_TYPE::D_S_D, ENEMY_MODE::LEFT, ENEMY_DROP::RANDOM,1,
			36,ENEMY_NAME::HUBU,Vector2(-20, -400), ENEMY_TYPE::D_S_D, ENEMY_MODE::LEFT, ENEMY_DROP::RANDOM,1,
			38,ENEMY_NAME::HUBU,Vector2(-30, -400), ENEMY_TYPE::D_S_D, ENEMY_MODE::LEFT, ENEMY_DROP::RANDOM,1,
			40,ENEMY_NAME::HUBU,Vector2(-40, -400), ENEMY_TYPE::D_S_D, ENEMY_MODE::LEFT, ENEMY_DROP::RANDOM,1,
			42,ENEMY_NAME::HUBU,Vector2(-50, -400), ENEMY_TYPE::D_S_D, ENEMY_MODE::LEFT, ENEMY_DROP::RANDOM,1,
			44,ENEMY_NAME::HUBU,Vector2(-60, -400), ENEMY_TYPE::D_S_D, ENEMY_MODE::LEFT, ENEMY_DROP::RANDOM,1,
			46,ENEMY_NAME::HUBU,Vector2(-70, -400), ENEMY_TYPE::D_S_D, ENEMY_MODE::LEFT, ENEMY_DROP::RANDOM,1,
			60,ENEMY_NAME::HUBU,Vector2(-240, -400), ENEMY_TYPE::D_S, ENEMY_MODE::LEFT, ENEMY_DROP::RANDOM,1,
			60,ENEMY_NAME::HUBU,Vector2(40, -400), ENEMY_TYPE::D_S, ENEMY_MODE::RIGHT, ENEMY_DROP::RANDOM,1,
			62,ENEMY_NAME::HUBU,Vector2(-230, -400), ENEMY_TYPE::D_S, ENEMY_MODE::LEFT, ENEMY_DROP::RANDOM,1,
			62,ENEMY_NAME::HUBU,Vector2(30, -400), ENEMY_TYPE::D_S, ENEMY_MODE::RIGHT, ENEMY_DROP::RANDOM,1,
			64,ENEMY_NAME::HUBU,Vector2(-220, -400), ENEMY_TYPE::D_S, ENEMY_MODE::LEFT, ENEMY_DROP::RANDOM,1,
			64,ENEMY_NAME::HUBU,Vector2(20, -400), ENEMY_TYPE::D_S, ENEMY_MODE::RIGHT, ENEMY_DROP::RANDOM,1,
			66,ENEMY_NAME::HUBU,Vector2(-210, -400), ENEMY_TYPE::D_S, ENEMY_MODE::LEFT, ENEMY_DROP::RANDOM,1,
			66,ENEMY_NAME::HUBU,Vector2(10, -400), ENEMY_TYPE::D_S, ENEMY_MODE::RIGHT, ENEMY_DROP::RANDOM,1,
			68,ENEMY_NAME::HUBU,Vector2(-200, -400), ENEMY_TYPE::D_S, ENEMY_MODE::LEFT, ENEMY_DROP::RANDOM,1,
			68,ENEMY_NAME::HUBU,Vector2(0, -400), ENEMY_TYPE::D_S, ENEMY_MODE::RIGHT, ENEMY_DROP::RANDOM,1,
			70,ENEMY_NAME::HUBU,Vector2(-190, -400), ENEMY_TYPE::D_S, ENEMY_MODE::LEFT, ENEMY_DROP::RANDOM,1,
			70,ENEMY_NAME::HUBU,Vector2(-10, -400), ENEMY_TYPE::D_S, ENEMY_MODE::RIGHT, ENEMY_DROP::RANDOM,1,
			72,ENEMY_NAME::HUBU,Vector2(-180, -400), ENEMY_TYPE::D_S, ENEMY_MODE::LEFT, ENEMY_DROP::RANDOM,1,
			72,ENEMY_NAME::HUBU,Vector2(-20, -400), ENEMY_TYPE::D_S, ENEMY_MODE::RIGHT, ENEMY_DROP::RANDOM,1,
			74,ENEMY_NAME::HUBU,Vector2(-170, -400), ENEMY_TYPE::D_S, ENEMY_MODE::LEFT, ENEMY_DROP::RANDOM,1,
			74,ENEMY_NAME::HUBU,Vector2(-30, -400), ENEMY_TYPE::D_S, ENEMY_MODE::RIGHT, ENEMY_DROP::RANDOM,1,
			76,ENEMY_NAME::HUBU,Vector2(-160, -400), ENEMY_TYPE::D_S, ENEMY_MODE::LEFT, ENEMY_DROP::RANDOM,1,
			76,ENEMY_NAME::HUBU,Vector2(-40, -400), ENEMY_TYPE::D_S, ENEMY_MODE::RIGHT, ENEMY_DROP::RANDOM,1,
			78,ENEMY_NAME::HUBU,Vector2(-150, -400), ENEMY_TYPE::D_S, ENEMY_MODE::LEFT, ENEMY_DROP::RANDOM,1,
			78,ENEMY_NAME::HUBU,Vector2(-50, -400), ENEMY_TYPE::D_S, ENEMY_MODE::RIGHT, ENEMY_DROP::RANDOM,1,
			90,ENEMY_NAME::SISI,Vector2(-330, -400), ENEMY_TYPE::D_ST_A_ST_S_U, ENEMY_MODE::LEFT, ENEMY_DROP::ATTACK,1,
			94,ENEMY_NAME::SISI,Vector2(0, -400), ENEMY_TYPE::D_ST_S_U, ENEMY_MODE::RIGHT, ENEMY_DROP::ATTACK,1,
			98,ENEMY_NAME::SISI,Vector2(-200, -400), ENEMY_TYPE::D_ST_S_U, ENEMY_MODE::LEFT, ENEMY_DROP::ATTACK,1,
			102,ENEMY_NAME::SISI,Vector2(110, -400), ENEMY_TYPE::D_ST_A_ST_S_U, ENEMY_MODE::RIGHT, ENEMY_DROP::ATTACK,1,
			106,ENEMY_NAME::SISI,Vector2(-380, -400), ENEMY_TYPE::D_ST_S_U, ENEMY_MODE::LEFT, ENEMY_DROP::ATTACK,1,
			110,ENEMY_NAME::SISI,Vector2(150, -400), ENEMY_TYPE::D_ST_S_U, ENEMY_MODE::RIGHT, ENEMY_DROP::ATTACK,1,
			114,ENEMY_NAME::SISI,Vector2(-200, -400), ENEMY_TYPE::D_ST_A_ST_S_U, ENEMY_MODE::LEFT, ENEMY_DROP::ATTACK,1,
			116,ENEMY_NAME::SISI,Vector2(170, -400), ENEMY_TYPE::D_ST_S_U, ENEMY_MODE::RIGHT, ENEMY_DROP::ATTACK,1,
			118,ENEMY_NAME::SISI,Vector2(-370, -400), ENEMY_TYPE::D_ST_S_U, ENEMY_MODE::LEFT, ENEMY_DROP::ATTACK,1,
			120,ENEMY_NAME::SISI,Vector2(0, -400), ENEMY_TYPE::D_ST_A_ST_S_U, ENEMY_MODE::RIGHT, ENEMY_DROP::ATTACK,1,
			122,ENEMY_NAME::SISI,Vector2(-250, -400), ENEMY_TYPE::D_ST_A_ST_S_U, ENEMY_MODE::LEFT, ENEMY_DROP::ATTACK,1,
			122,ENEMY_NAME::SISI,Vector2(100, -400), ENEMY_TYPE::D_ST_S_U, ENEMY_MODE::RIGHT, ENEMY_DROP::ATTACK,1,
			124,ENEMY_NAME::SISI,Vector2(-300, -400), ENEMY_TYPE::D_ST_A_ST_S_U, ENEMY_MODE::LEFT, ENEMY_DROP::ATTACK,1,
			124,ENEMY_NAME::SISI,Vector2(50, -400), ENEMY_TYPE::D_ST_S_U, ENEMY_MODE::RIGHT, ENEMY_DROP::ATTACK,1,
		},
		{
			10,ENEMY_NAME::HUBU,Vector2(-200, -400), ENEMY_TYPE::D_S_D, ENEMY_MODE::RIGHT, ENEMY_DROP::RANDOM,5,
			12,ENEMY_NAME::HUBU,Vector2(-190, -400), ENEMY_TYPE::D_S_D, ENEMY_MODE::RIGHT, ENEMY_DROP::RANDOM,5,
			14,ENEMY_NAME::HUBU,Vector2(-180, -400), ENEMY_TYPE::D_S_D, ENEMY_MODE::RIGHT, ENEMY_DROP::RANDOM,5,
			16,ENEMY_NAME::HUBU,Vector2(-170, -400), ENEMY_TYPE::D_S_D, ENEMY_MODE::RIGHT, ENEMY_DROP::RANDOM,5,
			18,ENEMY_NAME::HUBU,Vector2(-160, -400), ENEMY_TYPE::D_S_D, ENEMY_MODE::RIGHT, ENEMY_DROP::RANDOM,5,
			30,ENEMY_NAME::HUBU,Vector2(10, -400), ENEMY_TYPE::D_S_D, ENEMY_MODE::LEFT, ENEMY_DROP::RANDOM,5,
			32,ENEMY_NAME::HUBU,Vector2(0, -400), ENEMY_TYPE::D_S_D, ENEMY_MODE::LEFT, ENEMY_DROP::RANDOM,5,
			34,ENEMY_NAME::HUBU,Vector2(-10, -400), ENEMY_TYPE::D_S_D, ENEMY_MODE::LEFT, ENEMY_DROP::RANDOM,5,
			36,ENEMY_NAME::HUBU,Vector2(-20, -400), ENEMY_TYPE::D_S_D, ENEMY_MODE::LEFT, ENEMY_DROP::RANDOM,5,
			38,ENEMY_NAME::HUBU,Vector2(-30, -400), ENEMY_TYPE::D_S_D, ENEMY_MODE::LEFT, ENEMY_DROP::RANDOM,5,
			40,ENEMY_NAME::HUBU,Vector2(-40, -400), ENEMY_TYPE::D_S_D, ENEMY_MODE::LEFT, ENEMY_DROP::RANDOM,5,
			42,ENEMY_NAME::HUBU,Vector2(-50, -400), ENEMY_TYPE::D_S_D, ENEMY_MODE::LEFT, ENEMY_DROP::RANDOM,5,
			44,ENEMY_NAME::HUBU,Vector2(-60, -400), ENEMY_TYPE::D_S_D, ENEMY_MODE::LEFT, ENEMY_DROP::RANDOM,5,
			46,ENEMY_NAME::HUBU,Vector2(-70, -400), ENEMY_TYPE::D_S_D, ENEMY_MODE::LEFT, ENEMY_DROP::RANDOM,5,
			60,ENEMY_NAME::HUBU,Vector2(-240, -400), ENEMY_TYPE::D_S, ENEMY_MODE::LEFT, ENEMY_DROP::RANDOM,5,
			60,ENEMY_NAME::HUBU,Vector2(40, -400), ENEMY_TYPE::D_S, ENEMY_MODE::RIGHT, ENEMY_DROP::RANDOM,5,
			62,ENEMY_NAME::HUBU,Vector2(-230, -400), ENEMY_TYPE::D_S, ENEMY_MODE::LEFT, ENEMY_DROP::RANDOM,5,
			62,ENEMY_NAME::HUBU,Vector2(30, -400), ENEMY_TYPE::D_S, ENEMY_MODE::RIGHT, ENEMY_DROP::RANDOM,5,
			64,ENEMY_NAME::HUBU,Vector2(-220, -400), ENEMY_TYPE::D_S, ENEMY_MODE::LEFT, ENEMY_DROP::RANDOM,5,
			64,ENEMY_NAME::HUBU,Vector2(20, -400), ENEMY_TYPE::D_S, ENEMY_MODE::RIGHT, ENEMY_DROP::RANDOM,5,
			66,ENEMY_NAME::HUBU,Vector2(-210, -400), ENEMY_TYPE::D_S, ENEMY_MODE::LEFT, ENEMY_DROP::RANDOM,5,
			66,ENEMY_NAME::HUBU,Vector2(10, -400), ENEMY_TYPE::D_S, ENEMY_MODE::RIGHT, ENEMY_DROP::RANDOM,5,
			68,ENEMY_NAME::HUBU,Vector2(-200, -400), ENEMY_TYPE::D_S, ENEMY_MODE::LEFT, ENEMY_DROP::RANDOM,5,
			68,ENEMY_NAME::HUBU,Vector2(0, -400), ENEMY_TYPE::D_S, ENEMY_MODE::RIGHT, ENEMY_DROP::RANDOM,5,
			70,ENEMY_NAME::HUBU,Vector2(-190, -400), ENEMY_TYPE::D_S, ENEMY_MODE::LEFT, ENEMY_DROP::RANDOM,5,
			70,ENEMY_NAME::HUBU,Vector2(-10, -400), ENEMY_TYPE::D_S, ENEMY_MODE::RIGHT, ENEMY_DROP::RANDOM,5,
			72,ENEMY_NAME::HUBU,Vector2(-180, -400), ENEMY_TYPE::D_S, ENEMY_MODE::LEFT, ENEMY_DROP::RANDOM,5,
			72,ENEMY_NAME::HUBU,Vector2(-20, -400), ENEMY_TYPE::D_S, ENEMY_MODE::RIGHT, ENEMY_DROP::RANDOM,5,
			74,ENEMY_NAME::HUBU,Vector2(-170, -400), ENEMY_TYPE::D_S, ENEMY_MODE::LEFT, ENEMY_DROP::RANDOM,5,
			74,ENEMY_NAME::HUBU,Vector2(-30, -400), ENEMY_TYPE::D_S, ENEMY_MODE::RIGHT, ENEMY_DROP::RANDOM,5,
			76,ENEMY_NAME::HUBU,Vector2(-160, -400), ENEMY_TYPE::D_S, ENEMY_MODE::LEFT, ENEMY_DROP::RANDOM,5,
			76,ENEMY_NAME::HUBU,Vector2(-40, -400), ENEMY_TYPE::D_S, ENEMY_MODE::RIGHT, ENEMY_DROP::RANDOM,5,
			78,ENEMY_NAME::HUBU,Vector2(-150, -400), ENEMY_TYPE::D_S, ENEMY_MODE::LEFT, ENEMY_DROP::RANDOM,5,
			78,ENEMY_NAME::HUBU,Vector2(-50, -400), ENEMY_TYPE::D_S, ENEMY_MODE::RIGHT, ENEMY_DROP::RANDOM,5,
			90,ENEMY_NAME::SISI,Vector2(-330, -400), ENEMY_TYPE::D_ST_A_ST_S_U, ENEMY_MODE::LEFT, ENEMY_DROP::ATTACK,5,
			94,ENEMY_NAME::SISI,Vector2(0, -400), ENEMY_TYPE::D_ST_S_U, ENEMY_MODE::RIGHT, ENEMY_DROP::ATTACK,5,
			98,ENEMY_NAME::SISI,Vector2(-200, -400), ENEMY_TYPE::D_ST_S_U, ENEMY_MODE::LEFT, ENEMY_DROP::ATTACK,5,
			102,ENEMY_NAME::SISI,Vector2(110, -400), ENEMY_TYPE::D_ST_A_ST_S_U, ENEMY_MODE::RIGHT, ENEMY_DROP::ATTACK,5,
			106,ENEMY_NAME::SISI,Vector2(-380, -400), ENEMY_TYPE::D_ST_S_U, ENEMY_MODE::LEFT, ENEMY_DROP::ATTACK,5,
			110,ENEMY_NAME::SISI,Vector2(150, -400), ENEMY_TYPE::D_ST_S_U, ENEMY_MODE::RIGHT, ENEMY_DROP::ATTACK,5,
			114,ENEMY_NAME::SISI,Vector2(-200, -400), ENEMY_TYPE::D_ST_A_ST_S_U, ENEMY_MODE::LEFT, ENEMY_DROP::ATTACK,5,
			116,ENEMY_NAME::SISI,Vector2(170, -400), ENEMY_TYPE::D_ST_S_U, ENEMY_MODE::RIGHT, ENEMY_DROP::ATTACK,5,
			118,ENEMY_NAME::SISI,Vector2(-370, -400), ENEMY_TYPE::D_ST_S_U, ENEMY_MODE::LEFT, ENEMY_DROP::ATTACK,5,
			120,ENEMY_NAME::SISI,Vector2(0, -400), ENEMY_TYPE::D_ST_A_ST_S_U, ENEMY_MODE::RIGHT, ENEMY_DROP::ATTACK,5,
			122,ENEMY_NAME::SISI,Vector2(-250, -400), ENEMY_TYPE::D_ST_A_ST_S_U, ENEMY_MODE::LEFT, ENEMY_DROP::ATTACK,5,
			122,ENEMY_NAME::SISI,Vector2(100, -400), ENEMY_TYPE::D_ST_S_U, ENEMY_MODE::RIGHT, ENEMY_DROP::ATTACK,5,
			124,ENEMY_NAME::SISI,Vector2(-300, -400), ENEMY_TYPE::D_ST_A_ST_S_U, ENEMY_MODE::LEFT, ENEMY_DROP::ATTACK,5,
			124,ENEMY_NAME::SISI,Vector2(50, -400), ENEMY_TYPE::D_ST_S_U, ENEMY_MODE::RIGHT, ENEMY_DROP::ATTACK,5
		}
	};
	int SpawnDataCount = sizeof(SpawnTable[0]) / sizeof(SpawnEnemyData);
	int StepDataCount = sizeof(SpawnTable) / (sizeof(SpawnEnemyData) * SpawnDataCount);
	int NowStep = 0;
	int NowSpawn = 0;

	//アイテム管理用
	VTN_Item Items[(int)ENEMY_DROP::NOTHING];
	const float ITEM_SIZE = 0.1f;

	//経過時間管理用
	int Timer = 0;
	int TimeCount = 0;
	bool isSpawn = false;

	//会話パート用
	bool isTalkStart = false;
	bool isTalkEnd = false;
	VTN_Image TalkWindow;
	float TalkWindowSize = 0.0f;
	TextManager text;
	char TextList[2][10][256] =
	{
		{
			"あれ〜机の上に置いていた、おにぎりがないなー",
			"ニヤニヤ",
			"おかゆーおにぎり知らない？？",
			"おにぎりならここにあるよ〜*＜おなかポンポン＞",
			"おにぎりの恨みを思い知れ！",
			"~"
		},
		{
			"おかゆ、まて〜",
			"もー。しつこいな〜*おにぎりぐらいいいじゃん",
			"おにぎりぐらいって言ったな〜*もう許さないぞ！",
			"~"
		}
	};
	VTN_Image PlayerTalk[1];
	int PlayerTalkNum = 0;
	VTN_Image EnemyTalk[1];
	int EnemyTalkNum = 0;
	int TalkWaitTime = 0;
	const int MaxTalkWaitTime = 200;

	//テキスト表示用
	int EnterCount;
	int FramerateCount = 0;
	int TextOutPutCount = 0;
	const int TextSpeed = 10;
	int TextLineCount = 0;//いじるとデバッグ時のstartの位置が変わるよ
	bool isAllOutPut = false;
	char OutPutChar[3][256] = { "","","" };
	int  OutPutCharNum = 0;
	int StartNum = 0;
	bool isTextEnd = false;

	//ゲームオーバー判定用
	bool isGameOver = false;
	VTN_Image GameOver;

	//ゲームクリア判定用
	bool isGameClear = false;
	VTN_Image GameClear;

	//ゲームオーバークリア共通
	char TototalScore[256] = "";
	VTN_Image Cursol;
	int cursolNum = 0;

	//音に関する
	bool isSoundStart = false;

	/// <summary>
	/// 時間経過トリガーのイベント管理
	/// </summary>
	void TimeControl();

	/// <summary>
	/// 背景の移動
	/// </summary>
	void BackGroundMove();

	/// <summary>
	/// 敵出現の管理
	/// </summary>
	void SpawnControl();

	/// <summary>
	/// エネミーが倒されてるかを確認
	/// </summary>
	/// <returns></returns>
	bool CheckEnemyClear();

	/// <summary>
	/// 会話パートに関する処理
	/// </summary>
	void TalkPart();

	/// <summary>
	/// テキスト関連
	/// </summary>
	void TextManagement();

	/// <summary>
	/// テキストを進める用
	/// </summary>
	void CheckEnter();

	/// <summary>
	/// ゲームオーバーになってからの処理
	/// </summary>
	void CheckGameOverAfter();

	/// <summary>
	/// ゲームクリアーになってからの処理
	/// </summary>
	void CheckGameClearAfter();
public:
	//オーバーロード
	Korone(ID3D12Device* Dev, ID3D12GraphicsCommandList* CmdList) :BaseScene(Dev, CmdList) {}
	void Init();
	void Draw();
	bool Update();
	void onDestroy();
};
