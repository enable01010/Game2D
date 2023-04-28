#pragma once
#include "stdafx.h"

class STG_Boss_Miosha_Last :public STG_Boss
{
	enum BOSS_STATE
	{
		DOWN_FIRST = 0,
		LEFT_LASRE,
		RIGHT_LASER,
		DOUBLE_LASRE,
		RAND_BULLET,
		FAN_BULLET,
		CIRCLE_BULLET,
		CIRCLE_BULLET_MOVE_ONE,
		CIRCLE_BULLET_MOVE_TWO,
		CiRCLE_BULLET_THREE,
		CIRCLE_ONLY,
		TIME_OUT_FIRST,
		おかゆの背負った罪はお前の罪である,
		こつこつギャンブルで稼ぐ,
		リスナーは養分なんじゃコラァァ！,
	};
	int CircleNum = 0;

	Vector2 MoveDir[4] = {
		Vector2(3,0),
		Vector2(0,3),
		Vector2(-3,0),
		Vector2(0,-3)
	};
	int MoveDirCount = sizeof(MoveDir) / sizeof(Vector2);

	bool isTimeOutStart = false;
	Vector2 FirstPosDir;
	//攻撃関係
	int AttackTime = 0;
	int BulletDelayTime = 0;

	VTN_MoveImage CutInImg[3];
	bool isSkill[3] = { false,false,false };
	int SkillTimer = 0;


	/// <summary>
	/// ボスの行動パターン
	/// </summary>
	/// <param name="unit"></param>
	void BossAI(STG_Unit** unit);

	/// <summary>
	/// 一定時間経過後のボスの行動
	/// </summary>
	/// <param name="unit"></param>
	void TimeOutAi(STG_Unit** unit);

	/// <summary>
	/// ランダムパターンに飛ぶ
	/// </summary>
	void BossRnadPatarn();

	/// <summary>
	/// 必殺技のランダム
	/// </summary>
	void BossRnadPatarn_Skiil();
public:

	LRESULT Init(ID3D12Device* dev, const wchar_t* fileName, Unit_Side mySide = Unit_Side::ENEMY, VTN_Item* item = NULL, bool add = false);
	void Draw(ID3D12GraphicsCommandList* cmdList);
	void DrawStatus(ID3D12GraphicsCommandList* cmdList);
	void BossDestroy();
};
