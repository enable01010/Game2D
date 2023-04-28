#pragma once
#include "STG_Bomb.h"

class STG_Enemy;
class STG_Boss;
class STG_Unit;
class FxParticle;

class STG_Bomb_Rei :public STG_Bomb
{
	//ビームで使うもの
	VTN_Image Beam;

	//チャージで使うもの
	FxParticle Charge;

	int ChargeTime = 500;
	int MaxTime = 1500;

	const int ATTACK = 2;

	/// <summary>
	/// 画像の位置調整
	/// </summary>
	/// <param name="pos"></param>
	void PosUpdate(Vector2 pos);

	/// <summary>
	/// 時間経過の管理
	/// </summary>
	/// <param name="unit"></param>
	void TimeManager(STG_Unit** unit);

	/// <summary>
	/// 当たり判定の管理
	/// </summary>
	/// <param name="unit"></param>
	void HitCheck(STG_Unit** unit);
public:
	void Init(ID3D12Device* dev);
	void UseBomb(Vector2 pos);
	void Update(STG_Unit** unit);
	void Draw(ID3D12GraphicsCommandList* cmdList);
	void Destroy();
};

