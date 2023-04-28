#include "Object.h"

LRESULT FxParticle::Init(ID3D12Device* dev, const wchar_t* fileName, float size, bool add)
{
	Dev = dev;
	LRESULT result = LoadTex(dev, fileName);

	// 頂点レイアウト
	Init_Vertex();
	Init_Blend(add);
	Init_Sampler();


	// シェーダーの読み込み
	result = LoadShader(dev, L"Lib\\Shader\\FxParticlePS.hlsl", L"Lib\\Shader\\FxParticleVS.hlsl");

	// 定数バッファの設定
	InitConstBuffer<FxParticleShaderData>(&m_ConstantBufferData, &MapConstBuffer);
	for (int h = 0; h < COUNT; h++)
	{
		MapConstBuffer->Data[h] = { Color(1),XMMatrixIdentity() };
	}

	InitFlag = true;

	Scale = XMFLOAT2(1.0f, 1.0f);
	Size = XMFLOAT2(1.0f, 1.0f);
	return result;
}

FxParticle::FxParticle()
{
	MapConstBuffer = NULL;
	for (int i = 0; i < COUNT; i++)
	{
		Data[i].SetTimer(0);
		Data[i].SetPos(Vector2());
	}
}

void FxParticle::SetFx(Vector2 pos, Vector2 dir)
{
	for (int i = 0; i < PARTICLE_COUNT; i++)
	{
		if (Data[i].GetTimer() <= 0)
		{
			Data[i].SetTimer(20);
			Data[i].SetPos(pos);
			Data[i].SetDir(dir);
			return;
		}
	}
}

void FxParticle::Draw(ID3D12GraphicsCommandList* cmdList)
{
	// 無駄無く使いまわす為のID
	int id = 0;
	for (int i = 0; i < COUNT; i++)
	{
		if (Data[i].GetTimer() > 0)
		{
			Data[i].SetTimer(Data[i].GetTimer() - 1);
			MapConstBuffer->Data[id].color.a = Data[i].GetTimer() / 20.0f;
			XMMATRIX* mat = &MapConstBuffer->Data[id].mat;// 使いまわす
			// 原点に初期化
			*mat = XMMatrixIdentity();
			// 回転
			*mat = XMMatrixMultiply(*mat, XMMatrixRotationZ(0));
			// 拡大縮小
			*mat = XMMatrixMultiply(*mat, XMMatrixScaling(2.0f / WINDOW_WIDTH * 0.5f * Scale.x, -2.0f / WINDOW_HEIGHT * 0.5f * Scale.y, 0.1f));
			// 移動
			Data[i].SetPos(Data[i].GetPos() - Data[i].GetDir());
			mat->r[3].m128_f32[0] = (Pos.x + Data[i].GetPos().x) / WINDOW_WIDTH * 2.0f;
			mat->r[3].m128_f32[1] =(Pos.y + Data[i].GetPos().y )/ WINDOW_HEIGHT * -2.0f;
			id++;
		}
	}
	// エフェクトが無い時は表示しない。この辺は弾のコードと同じなのでコピーすればOKs
	if (id > 0)
	{
		cmdList->SetPipelineState(_pipelinestate);
		cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		cmdList->IASetVertexBuffers(0, 1, &vbView);
		cmdList->IASetIndexBuffer(&ibView);
		cmdList->SetGraphicsRootSignature(rootsignature);
		cmdList->SetDescriptorHeaps(1, &basicDescHeap);

		cmdList->SetGraphicsRootDescriptorTable(0, basicDescHeap->GetGPUDescriptorHandleForHeapStart());

		cmdList->DrawIndexedInstanced(6, id, 0, 0, 0);
	}
}