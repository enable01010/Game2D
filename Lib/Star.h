#pragma once
#include "stdafx.h"
#define StarMaxCount 10

struct StarData
{
	Vector2 Pos;
};

_declspec(align(256))
struct StarShaderData
{
	Color color;
	XMFLOAT2 uv;
	XMMATRIX mat[StarMaxCount];
};

class Star :public STG_Instance<StarData>
{
	StarShaderData m_ConstantBufferData = StarShaderData();
	StarShaderData* StarConstBuffer;

public:
	Star();
	LRESULT Init(ID3D12Device* dev, const wchar_t* fileName,Vector2 pos, bool add = false);
	void Draw(ID3D12GraphicsCommandList* cmdList,int Count);

};

