#include "Object.h"

STG_Fx::STG_Fx()
{
	MapConstBuffer = NULL;
	for (int i = 0; i < COUNT; i++)
	{
		Data[i].Timer = 0;
		Data[i].Pos = Vector2();
	}
}

LRESULT STG_Fx::Init(ID3D12Device* dev, const wchar_t* fileName, float size, bool add)
{
	Dev = dev;
	LRESULT result = LoadTex(dev, fileName);

	// 頂点レイアウト
	inputLayout[0] = { "POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 };
	inputLayout[1] = { "TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 };

	// 頂点の設定
	float width = metadata.width * 0.5f * 0.25f * size;
	float height = metadata.height * 0.5f * 0.3333333f * size;

	//縮尺変更の基準とう変更用
	vertexs[0].Set(-width, height, 0.0f, 0.0f, 0.3333333f);
	vertexs[1].Set(-width, -height, 0.0f, 0.0f, 0.0f);
	vertexs[2].Set(width, height, 0.0f, 0.25f, 0.3333333f);
	vertexs[3].Set(width, -height, 0.0f, 0.25f, 0.0f);

	Init_Blend(add);
	Init_Sampler();


	// シェーダーの読み込み
	result = LoadShader(dev, L"Lib\\Shader\\FxPS.hlsl", L"Lib\\Shader\\FxVS.hlsl");

	// 定数バッファの設定
	InitConstBuffer<FxShaderData>(&m_ConstantBufferData, &MapConstBuffer);
	for (int h = 0; h < COUNT; h++)
	{
		MapConstBuffer->mat[h] = XMMatrixIdentity();
	}
	MapConstBuffer->color = Color(1, 1, 1, 1);

	InitFlag = true;

	Scale = XMFLOAT2(1.0f, 1.0f);
	Size = XMFLOAT2(1.0f, 1.0f);
	return result;
}

void STG_Fx::Draw(ID3D12GraphicsCommandList* cmdList)
{
	// 無駄無く使いまわす為のID
	int id = 0;
	for (int i = 0; i < COUNT; i++)
	{
		if (Data[i].Timer > 0)
		{
			Data[i].Timer--;
			XMMATRIX* mat = &MapConstBuffer->mat[id];// 使いまわす
			// 原点に初期化
			*mat = XMMatrixIdentity();
			// 回転
			*mat = XMMatrixMultiply(*mat, XMMatrixRotationZ(0));
			// 拡大縮小
			*mat = XMMatrixMultiply(*mat, XMMatrixScaling(2.0f / WINDOW_WIDTH * 0.5f * Scale.x, -2.0f / WINDOW_HEIGHT * 0.5f * Scale.y, 0.1f));
			// 移動
			mat->r[3].m128_f32[0] = Data[i].Pos.x / WINDOW_WIDTH * 2.0f;
			mat->r[3].m128_f32[1] = Data[i].Pos.y / WINDOW_HEIGHT * -2.0f;
			int frame = 5;
			mat->r[3].m128_f32[2] = 11 - Data[i].Timer / frame;
			id++;
		}
	}
	// エフェクトが無い時は表示しない。この辺は弾のコードと同じなのでコピーすればOK
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

void STG_Fx::Destroy()
{
	VTN_Image::Destroy();
}

void STG_Fx::SetFx(Vector2 pos)
{
	for (int i = 0; i < COUNT; i++)
	{
		if (Data[i].Timer <= 0)
		{
			int frame = 5;
			Data[i].Timer = 12 * frame;
			Data[i].Pos = pos;
			return;
		}
	}
}

