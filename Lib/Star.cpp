#include "Object.h"

Star::Star()
{
	StarConstBuffer = NULL;
	for (int i = 0; i < StarMaxCount; i++)
	{
		Data[i].Pos = Vector2(0 + i * 25,0);
	}
}

LRESULT Star::Init(ID3D12Device* dev, const wchar_t* fileName,Vector2 pos, bool add)
{
	Dev = dev;
	LRESULT result = LoadTex(dev, fileName);

	Init_Vertex();
	Init_Blend(add);
	Init_Sampler();

	//
	result = LoadShader(dev, L"Lib\\Shader\\BulletPS.hlsl", L"Lib\\Shader\\StarVS.hlsl");
	//

	InitConstBuffer<StarShaderData>(&m_ConstantBufferData, &StarConstBuffer);
	for (int h = 0; h < COUNT; h++) {
		StarConstBuffer->mat[h] = XMMatrixIdentity();
	}

	StarConstBuffer->uv = XMFLOAT2(0, 0);
	StarConstBuffer->color = Color(1, 1, 1, 1);

	InitFlag = true;

	Scale = XMFLOAT2(1.0f, 1.0f);
	Size = XMFLOAT2(1.0f, 1.0f);

	for (int i = 0; i < StarMaxCount; i++)
	{
		Data[i].Pos = Vector2(pos.x + i * 40, pos.y);
	}

	return result;
}

void Star::Draw(ID3D12GraphicsCommandList* cmdList, int Count)
{
	int id = 0;
	for (int i = 0; i < Count; i++)
	{
		XMMATRIX* mat = &StarConstBuffer->mat[id];
		*mat = XMMatrixIdentity();
		*mat = XMMatrixMultiply(*mat, XMMatrixRotationZ(0));
		*mat = XMMatrixMultiply(*mat, XMMatrixScaling(2.0f / WINDOW_WIDTH * 0.5f * Scale.x, -2.0f / WINDOW_HEIGHT * 0.5f * Scale.y, 0.1f));

		mat->r[3].m128_f32[0] = Data[i].Pos.x / WINDOW_WIDTH * 2.0f;
		mat->r[3].m128_f32[1] = Data[i].Pos.y / WINDOW_HEIGHT * -2.0f;

		id++;
	}

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