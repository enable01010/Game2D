#pragma once
#include "stdafx.h"

class STG_Enemy;

enum class PLAYER_STATUS
{
	HP,
	BOMB,
	ATK,
	GRAZE,
	POINT,
	MAX
};

enum class PLAYER_TYPE
{
	SIRAKAMI,
	KUROUE,
	MAX
};

class STG_Player :public STG_Unit_Single
{	
	//モーション用
	VTN_Image Motion[4];
	int MotionNum;

	//プレイヤーの初期位置
	const int START_POS_X = -100;
	const int START_POS_Y = 250;

	//被弾判定用
	static bool isDMG;
	int InvincibleCount = 0;
	float PlayerArufa = 1.0f;
	bool PlayerArufaUp = false;
	const int HIT_FX_DELAY = 1500;
	const int DMG_DELAY_TIME = 500;

	//白上用初期ステータス
	const int START_HP_SIRAKAMI = 2;
	const int START_BOMB_SIRAKAMI = 3;
	const float START_SPEED_SIRAKAMI = 0.5f;

	//黒上用初期ステータス
	const int START_HP_KUROUE = 1;
	const int START_BOMB_KUROUE = 4;
	const float START_SPEED_KUROUE = 0.5f;

	//メインステータス管理用
	int ATK = 0;
	int BombCount;
	int Graze = 0;
	int Point = 0;

	//メインステータス表示用
	Star Life;
	Star BombStar;
	const int LIFE_POS_X = 400;
	const int LIFE_POS_Y = -200;
	const float LIFE_IMG_SIZE = 0.5f;
	const int BOMB_POS_X = 400;
	const int BOMB_POS_Y = -150;
	const float BOMB_IMG_SIZE = 0.5f;

	//ステータス項目表示用
	VTN_Image Score;
	VTN_Image Status[(int)PLAYER_STATUS::MAX];
	TextManager text;
	char textChar[5][256];
	const int MAIN_SPACE = 50;
	const int SUB_SPACE = 80;
	const int FIRST_STATUS_POS_X = 300;
	const int FIRST_STATUS_POS_Y = -200;
	const float STATUS_STR_IMG_SIZE = 0.2f;

	//グレイズ用
	const int PLAYER_GRAZE_AREA = 50;

	//FX表示用
	enum class FxType
	{
		HITFX,
		MAX
	};
	const float HITFX_IMG_SIZE = 5.0f;

	//攻撃用(玉)
	enum class BulletType
	{
		STRAIGHT,
		HOMING,
		MAX
	};
	const int COOL_FLAME = 10;
	const int WAY_STRAT_X = 0;
	const int WAY_STRAT_Y = -10;
	const int WAY_SLANT_RIGHT_X = 1;
	const int WAY_SLANT_RIGHT_Y = -8;
	const int WAY_SLANT_LEFT_X = -1;
	const int WAY_SLANT_LEFT_Y = -8;
	const int POS_INTERVAL_STRAIGHT = 10;
	const int POS_INTERVAL_HOMING = 30;

	const int SUBBULLET_SPEED = 10;

	//ボム関係用
	STG_Bomb*  Bomb;

	/// <summary>
	/// グレイズの範囲に攻撃があったかチェック
	/// </summary>
	void CheckGraze(STG_Unit** unit);

	/// <summary>
	/// 入力を受けた場合の処理
	/// </summary>
	void CheckInput(STG_Unit** unit);

	/// <summary>
	/// 攻撃を受けた場合の処理
	/// </summary>
	void CheckInvincible(STG_Unit** unit);

	/// <summary>
	/// ボムの使用に関する処理
	/// </summary>
	void CheckBomb(STG_Unit** unit);

	void EnemyBulletDelete(STG_Unit** unit);

public:
	
	//ゲッターセッター
	static bool getIsDMG() { return isDMG; }
	int getInvincibleCount() {return InvincibleCount;}
	int getGraze() { return Graze; }
	void setGraze(int newGraze) { Graze = newGraze; }
	Vector2 getPos() { return Pos; }

	//オーバーライド
	LRESULT Init(ID3D12Device* dev, const wchar_t* fileName, Unit_Side mySide = Unit_Side::ENEMY, VTN_Item* item = NULL, bool add = false);
	void Draw(ID3D12GraphicsCommandList* cmdList);
	void DrawStatus(ID3D12GraphicsCommandList* cmdList);
	void Update(STG_Unit** unit);
	void UnitDestroy();
	STG_Status* GetUnitStatus(int num) { return this; };
	/// <summary>
	/// アイテムを拾った時の処理
	/// </summary>
	/// <param name="drop">アイテムの種類</param>
	void getItem(ENEMY_DROP drop);

	/// <summary>
	/// 敵の攻撃 or 敵　に当たったときの処理
	/// </summary>
	void Hit();

	/// <summary>
	/// 敵の攻撃 or 敵　の近くだったときの処理
	/// </summary>
	void GrazeHit();

	/// <summary>
	/// 攻撃を受けるときの関数
	/// </summary>
	/// <param name="val">受けるダメージの量</param>
	void Damage(int val);
};
