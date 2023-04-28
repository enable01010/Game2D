#pragma once
#include "stdafx.h"

enum class Unit_Side;

struct STG_ChargeShotData
{
protected:
	float Beam = 0;
	Vector2 Pos;
	Vector2 Scale;
	Vector2 Dir;
	float Radian = 0;
	float Rotation = 0;

public:
	void SetBeam(float num) { Beam = num; }
	float GetBeam() { return Beam; }
	void SetPos(Vector2 vec) { Pos = vec; }
	Vector2 GetPos() { return Pos; }
	void SetScale(Vector2 vec) { Scale = vec; }
	Vector2 GetScale() { return Scale; }
	void SetDir(Vector2 vec) { Dir = vec; }
	Vector2 GetDir() { return Dir; }
	void SetRadian(float num) { Radian = num; }
	float GetRadian() { return Radian; }
	void SetRotation(Vector2 dir) { Rotation = atan2(dir.x / 1280 * 720, dir.y); }
	float GetRotation() { return Rotation; }
};

_declspec(align(256))
struct ChargeShaderData
{
	Color color;
	XMFLOAT2 uv;
	XMMATRIX mat[COUNT];
};

class STG_ChargeShot : public STG_Instance<STG_ChargeShotData>
{
	ChargeShaderData m_ConstantBufferData = ChargeShaderData();
	ChargeShaderData* BulletConstBuffer;

	//ビームで使うもの
	float BeamMaxTime = 120;
	float BeamStartTime = 90;

	//当たり判定用
	Unit_Side MySide;
	int Width = 30;
public:
	STG_ChargeShot();
	LRESULT Init(ID3D12Device* dev, const wchar_t* fileName, bool add = false);
	void Draw(ID3D12GraphicsCommandList* cmdList);
	void Update(STG_Unit** unit);
	void Destroy();

	void SetMySide(Unit_Side side) { MySide = side; }
	void SetBeam(Vector2 pos,Vector2 targetPos);
};

