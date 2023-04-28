#pragma once
#include "stdafx.h"

struct STG_HomingBulletData
{
	Vector2 Pos;
	Vector2 Dir;
	float oldMagnitude;
	Vector2* TargetPos;
	int* TargetHP;
	bool isBreak = false;
	int Speed;
	int Timer;
	int Attack;
	bool isGraze = false;

public:
	bool GetIsGraze() { return isGraze; }
	void SetIsGraze(bool newIsGraze) { isGraze = newIsGraze; }
};

_declspec(align(256))
struct HomingBulletShaderData
{
	Color color;
	XMFLOAT2 uv;
	XMMATRIX mat[COUNT];
};

class STG_HomingBullet :public STG_Instance<STG_HomingBulletData>
{
	HomingBulletShaderData m_ConstantBufferData = HomingBulletShaderData();
	HomingBulletShaderData* HomingBulletConstBuffer;

	const int MAX_DIFFERENCE = 100;
	const int MIN_MAGNITUDE = 10;

protected:
	int HP;
	int Attack;
	STG_HomingBullet* Bullet;

public:
	STG_HomingBullet();
	LRESULT Init(ID3D12Device* dev, const wchar_t* fileName, bool add = false);
	void Update();

	void SetBullet(Vector2 pos, Vector2* targetPos,int* targetHp,int speed, int attack = 1, int timer = 100);

	void Draw(ID3D12GraphicsCommandList* cmdList);

	void Init(int hp, int attack) {
		HP = hp;
		Attack = attack;
	}

	int GetHp() { return HP; }
	void Damage(int val) { HP -= val; }
	void Shoot(Vector2 pos, Vector2 dir, int time);

};
