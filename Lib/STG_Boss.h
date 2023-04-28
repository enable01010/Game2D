#pragma once
#include"stdafx.h"


class STG_Boss:public STG_Unit_Single
{
protected:
	
	int BulletDelay = 0;
	bool isExpload = false;
	bool isDeath = false;
	int State = 0;
	int WaitTime = 0;
	int StateTime = 200;
	int MoveNo = 0;

	//玉関係
	enum class BulletType
	{
		YELLOW,
		BIG,
		RED,
		BULE,
		CIRCLE,
		DIVISION,
		WAIT_ROCKON,
		HOMING,
		MAX
	};
	const float BULLET_SIZE = 3.0f;
	const int PLAYER_HIT_AREA = 300;
	const int PLAYER_GRAZE_AREA = 50;

	//Playerに玉が当たって消えるときのエフェクト
	enum class FxType
	{
		PLAYER_HIT,
		EXPLOAD,
		MAX
	};
	const float PLAYER_HIT_FX_SIZE = 1.0f;

	//ビーム関係
	STG_ChargeShot Charge;

	//時間関係
	TextManager text;
	int MicroTimer = 25000;
	char TimerChar[256] = {};

	//体力バー
	VTN_Image HpGaze;
	int MaxHP;
	bool isHpGazeFull = false;
	int FirstHpGazeSize = 0;
	const int FirstHpGazeSizeMax = 100;
	const float HPGAZE_SIZE_X = 0.18f;
	const float HPGAZE_SIZE_Y = 0.05f;

	//出現・撃破管理
	bool isSpawn = false;
	bool isTimeOver = false;
	bool isClear = false;

	//AI管理用
	bool isEntry = false;
	bool isExit = false;
	
	/// <summary>
	/// 時間計測
	/// </summary>
	void TimerCount();

	/// <summary>
	/// ボスの行動
	/// </summary>
	virtual void BossAI(STG_Unit** unit) = 0;


	/// <summary>
	/// 一定時間経過後のボスの行動
	/// </summary>
	/// <param name="unit"></param>
	virtual void TimeOutAi(STG_Unit** unit) = 0;

	/// <summary>
	/// HpGazeを調整する
	/// </summary>
	void BossHpGaze();

	/// <summary>
	/// 爆発処理
	/// </summary>
	void Expload();

	/// <summary>
	/// ステートを次に進める関数
	/// 谷山陸統　2022/10/07
	/// </summary>
	/// <param name="state">次のステート</param>
	/// <param name="nextTime">ウェイト時間</param>
	/// <returns>処理の結果true 成功　false 失敗</returns>
	bool NextState(int state, int nextTime);
	void WaitState(int nextState, int waitTime);

	void HitBullet() {};
	void CheckBodyHit() {};
public:
	bool getIsSpawn() { return isSpawn; }
	bool getIsClear() { return isClear; }

	//オーバーライド
	virtual LRESULT Init(ID3D12Device* dev, const wchar_t* fileName ,Unit_Side mySide = Unit_Side::ENEMY, VTN_Item* item = NULL, bool add = false);
	void Update(STG_Unit** unit);
	virtual void Draw(ID3D12GraphicsCommandList* cmdList);
	virtual void DrawStatus(ID3D12GraphicsCommandList* cmdList);
	void UnitDestroy();
	virtual void BossDestroy() = 0;
	STG_Status* GetUnitStatus(int num = 0) { return this; };
	

	void Damage(int val); 

	/// <summary>
	/// 出現開始
	/// </summary>
	void Spawn();

	/// <summary>
/// プレイヤーが被弾したときに呼ばれる
/// 場に出ている玉を消す
/// </summary>
};

