#include "Object.h"

STG_Bullet::STG_Bullet()
{
	BulletConstBuffer = NULL;
	for (int i = 0; i < COUNT; i++) {
		Data[i].SetPos(Vector2());
	}
}

LRESULT STG_Bullet::Init(ID3D12Device* dev, const wchar_t* fileName, bool add)
{
	Dev = dev;
	LRESULT result = LoadTex(dev, fileName);

	Init_Vertex();
	Init_Blend(add);
	Init_Sampler();

	result = LoadShader(dev, L"Lib\\Shader\\BulletPS.hlsl", L"Lib\\Shader\\BulletVS.hlsl");

	InitConstBuffer<BulletShaderData>(&m_ConstantBufferData, &BulletConstBuffer);
	for (int h = 0; h < COUNT; h++) {
		BulletConstBuffer->mat[h] = XMMatrixIdentity();
	}

	BulletConstBuffer->uv = XMFLOAT2(0, 0);
	BulletConstBuffer->color = Color(1, 1, 1, 1);

	InitFlag = true;

	Scale = XMFLOAT2(1.0f, 1.0f);
	Size = XMFLOAT2(1.0f, 1.0f);

	PlayerHitFx.Init(dev, L"img\\Fx\\Frash.png", PLAYER_HIT_FX_SIZE, false);
	return result;
}

void STG_Bullet::Update()
{
	maxCount = 0;
	for (int h = 0; h < COUNT; h++) {
		if (Data[h].GetTimer() > 0)
		{
			Data[h].SetTimer(Data[h].GetTimer() - 1);
			BulletAI(h);
			
			if (Data[h].GetPos().x > 215		||
				Data[h].GetPos().x < -417	||
				Data[h].GetPos().y > 360		||
				Data[h].GetPos().y < -360)
			{
				Data[h].SetTimer(0);
			}
			maxCount = h + 1;
		}
	}
}

void STG_Bullet::Draw(ID3D12GraphicsCommandList* cmdList)
{
	int id = 0;
	for (int i = 0; i < COUNT; i++)
	{
		if (Data[i].GetTimer() > 0)
		{
			
			XMMATRIX* mat = &BulletConstBuffer->mat[id];
			*mat = XMMatrixIdentity();
			*mat = XMMatrixMultiply(*mat, XMMatrixRotationZ(0));
			*mat = XMMatrixMultiply(*mat, XMMatrixScaling(2.0f / WINDOW_WIDTH * 0.5f * Scale.x, -2.0f / WINDOW_HEIGHT * 0.5f * Scale.y, 0.1f));

			mat->r[3].m128_f32[0] = Data[i].GetPos().x / WINDOW_WIDTH * 2.0f;
			mat->r[3].m128_f32[1] = Data[i].GetPos().y / WINDOW_HEIGHT * -2.0f;
			
			id++;
		}
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
	PlayerHitFx.Draw(cmdList);
}

void STG_Bullet::Destroy()
{
	VTN_Image::Destroy();
	PlayerHitFx.Destroy();
}

void STG_Bullet::BulletDelete()
{
	int BulletCount = GetCount();
	for (int i = 0; i < BulletCount; i++)
	{
		STG_BulletData* bullet = GetData(i);
		if (bullet->GetTimer() != 0)
		{
			PlayerHitFx.SetFx(bullet->GetPos());
			bullet->SetTimer(0);
		}
	}
}