#pragma once
#include "stdafx.h"

class STG_Status;

struct STG_BulletData
{
protected:
	Vector2 Pos;
	Vector2 Dir;
	int Timer;
	bool isGraze = false;

	Vector2* TargetPos;
	int* TargetHp;
	int Speed;

	Vector2 Scale;

public:
	void SetPos(Vector2 pos) { Pos = pos; }
	Vector2 GetPos() { return Pos; };
	void SetDir(Vector2 dir) { Dir = dir; }
	Vector2 GetDir() { return Dir; }
	void SetTimer(int timer) { Timer = timer; }
	int GetTimer() { return Timer; }
	bool GetIsGraze() { return isGraze; }
	void SetIsGraze(bool newIsGraze) { isGraze = newIsGraze; }

	void SetTargetPos(Vector2* pos) { TargetPos = pos; }
	Vector2* GetTargetPos() { return TargetPos; };
	void SetTargetHp(int* hp) { TargetHp = hp; }
	int* GetTargetHp() { return TargetHp; }
	void SetSpeed(int speed) { Speed = speed; }
	int GetSpeed() { return Speed; }

	void SetScale(Vector2 vec) { Scale = vec; }
	Vector2 GetScale() { return Scale; }
};

_declspec(align(256))
struct BulletShaderData
{
	Color color;
	XMFLOAT2 uv;
	XMMATRIX mat[COUNT];
};

class STG_Bullet :public STG_Instance<STG_BulletData>
{
protected:
	BulletShaderData m_ConstantBufferData = BulletShaderData();
	BulletShaderData* BulletConstBuffer;


	STG_Fx PlayerHitFx;
	const float PLAYER_HIT_FX_SIZE = 0.25f;

public:
	STG_Bullet();
	LRESULT Init(ID3D12Device* dev, const wchar_t* fileName, bool add = false);
	void Update();
	virtual void Draw(ID3D12GraphicsCommandList* cmdList);
	void Destroy();
	
	virtual void BulletAI(int i) = 0;
	virtual void SetBullet(Vector2 pos, Vector2 dir, STG_Status* status = NULL, int timer = 300) = 0;

	void BulletDelete();
};

