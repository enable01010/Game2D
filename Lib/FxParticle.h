#pragma once
#include "stdafx.h"
#define PARTICLE_COUNT 500

/// <summary>
/// エフェクト1つぶんのデータ
/// </summary>
struct FxParticleData
{
protected:
	Vector2 Pos;
	Vector2 Dir;
	int Timer; // 残り時間。0だと生きてない
public:
	void SetPos(Vector2  pos) { Pos = pos; }
	Vector2 GetPos() { return Pos; }
	void SetDir(Vector2 dir) { Dir = dir; }
	Vector2 GetDir() { return Dir; }
	void SetTimer(int timer) { Timer = timer; }
	int GetTimer() { return Timer; }
};

// シェーダーに渡す構造体
struct FxParticleUnitShaderData
{
	Color color;
	XMMATRIX mat;
};

__declspec(align(256))
struct FxParticleShaderData
{
	FxParticleUnitShaderData Data[PARTICLE_COUNT];

	FxParticleShaderData()
	{
		for (int i = 0; i < PARTICLE_COUNT; i++)
		{
			Data[i] = { Color(),XMMATRIX() };
		}
	}
};

/// <summary>
/// エフェクトを表示する
/// GPU Instance
/// </summary>
class FxParticle : public STG_Instance<FxParticleData>
{
	// シェーダーに渡す定数バッファ
	FxParticleShaderData	m_ConstantBufferData = FxParticleShaderData();
	FxParticleShaderData* MapConstBuffer;


public:

	FxParticle();

	// 初期化。「頂点の設定」と「シェーダー読み込み」、「定数バッファの設定」以外は弾と同じ
	LRESULT Init(ID3D12Device* dev, const wchar_t* fileName, float size, bool add = false);

	/// <summary>
	/// エフェクトを使用状態にします
	/// </summary>
	/// <param name="pos"></param>
	void SetFx(Vector2 pos, Vector2 dir);
	/// <summary>
	/// 表示
	/// </summary>
	void Draw(ID3D12GraphicsCommandList* cmdList);
};


