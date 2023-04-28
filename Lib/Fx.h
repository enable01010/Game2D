#pragma once
#include "stdafx.h"

/// <summary>
/// エフェクト1つぶんのデータ
/// </summary>
struct STG_FxData
{
	Vector2 Pos;
	int Timer; // 残り時間。0だと生きてない
};

// シェーダーに渡す構造体
__declspec(align(256))
struct FxShaderData
{
	Color color;
	XMMATRIX mat[COUNT];

	FxShaderData()
	{
		color = Color();
		for (int i = 0; i < COUNT; i++)
		{
			mat[i] = XMMATRIX();
		}
	}
};

/// <summary>
/// エフェクトを表示する
/// GPU Instance
/// </summary>
class STG_Fx : public STG_Instance<STG_FxData>
{
	// シェーダーに渡す定数バッファ
	FxShaderData	m_ConstantBufferData = FxShaderData();
	FxShaderData* MapConstBuffer;

public:
	STG_Fx();

	// 初期化。「頂点の設定」と「シェーダー読み込み」、「定数バッファの設定」以外は弾と同じ
	LRESULT Init(ID3D12Device* dev, const wchar_t* fileName, float size, bool add = false);
	void Draw(ID3D12GraphicsCommandList* cmdList);
	void Destroy();

	/// <summary>
	/// エフェクトを使用状態にします
	/// </summary>
	/// <param name="pos"></param>
	void SetFx(Vector2 pos);
	
	
};
