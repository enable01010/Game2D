#pragma once
#include "stdafx.h"

class STG_Enemy;
class STG_Boss;

class STG_Bomb_Yume :public STG_Bomb
{
	//画像の配列用 (BombFx)
	enum class Bomb_Direction
	{
		Up,
		Down,
		Left,
		Right,
		Max
	};

	//画像用
	VTN_Image BombFX[(int)Bomb_Direction::Max];
	Vector2 FIRST_POS[(int)Bomb_Direction::Max] = { Vector2(0,-100),Vector2(0,100), Vector2(-100,0), Vector2(100,0) };
	const float IMG_ALUFA = 0.5f;
	const float FIRST_IMG_SIZE = 0.5f;
	float ImgSize = 0.5f;

	//時間用
	const int BOMB_TIME = 3000;
	int waitTime = 0;

	//ボムの判定用
	int BombArea = 100;
	const int FIRST_BOMB_AREA = 100;
	Vector2 UsePosition;
	const int ATTACK = 1;

	/// <summary>
	/// ボムのエリアに攻撃をする
	/// </summary>
	void CheckArea(STG_Unit** unit);

	void BombAreaUp();

	/// <summary>
	/// 時間経過を測定する
	/// </summary>
	void TimeCount();


public:
	STG_Bomb_Yume() {};
	//オーバーロード
	void Init(ID3D12Device* dev);
	void UseBomb(Vector2 pos);
	void Update(STG_Unit** unit);
	void Draw(ID3D12GraphicsCommandList* cmdList);
	void Destroy();

};

