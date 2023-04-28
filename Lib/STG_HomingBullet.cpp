#include "Object.h"


STG_HomingBullet::STG_HomingBullet()
{
	HomingBulletConstBuffer = NULL;
	for (int i = 0; i < COUNT; i++) {
		Data[i].Pos = Vector2();
	}
}

LRESULT STG_HomingBullet::Init(ID3D12Device* dev, const wchar_t* fileName, bool add)
{
	Dev = dev;
	LRESULT result = LoadTex(dev, fileName);

	Init_Vertex();
	Init_Blend(add);
	Init_Sampler();

	result = LoadShader(dev, L"Lib\\Shader\\BulletPS.hlsl", L"Lib\\Shader\\BulletVS.hlsl");

	InitConstBuffer<HomingBulletShaderData>(&m_ConstantBufferData, &HomingBulletConstBuffer);
	for (int h = 0; h < COUNT; h++) {
		HomingBulletConstBuffer->mat[h] = XMMatrixIdentity();
	}

	HomingBulletConstBuffer->uv = XMFLOAT2(0, 0);
	HomingBulletConstBuffer->color = Color(1, 1, 1, 1);

	InitFlag = true;

	Scale = XMFLOAT2(1.0f, 1.0f);
	Size = XMFLOAT2(1.0f, 1.0f);
	return result;
}

void STG_HomingBullet::Update()
{
	maxCount = 0;
	for (int h = 0; h < COUNT; h++) {
		if (Data[h].Timer > 0)
		{
			Data[h].Timer--;
			if (Data[h].TargetHP > 0 && !Data[h].isBreak)
			{
				Vector2 temp = *(Data[h].TargetPos) - Data[h].Pos;
				float magnitude = std::sqrt(temp.x * temp.x + temp.y * temp.y);
				if (magnitude - Data[h].oldMagnitude < MAX_DIFFERENCE && magnitude > MIN_MAGNITUDE)
				{
					Data[h].oldMagnitude = magnitude;
					temp = Vector2(temp.x / magnitude, temp.y / magnitude);
					Data[h].Dir = Vector2((Data[h].Dir.x + temp.x) / 2, (Data[h].Dir.y + temp.y) / 2);
				}
				else
				{
					Data[h].isBreak = true;
				}
			}
			else
			{
				Data[h].Timer = 0;
				Data[h].isBreak = true;
			}

			Data[h].Pos += Vector2(Data[h].Dir.x * Data[h].Speed, Data[h].Dir.y * Data[h].Speed);
			if (Data[h].Pos.x > PLAY_WINDOW_RIGHT_AREA ||
				Data[h].Pos.x < PLAY_WINDOW_LEFT_AREA ||
				Data[h].Pos.y > PLAY_WINDOW_UP_AREA ||
				Data[h].Pos.y < PLAY_WINDOW_DOWN_AREA)
			{
				Data[h].Timer = 0;
			}
			maxCount = h + 1;
		}
	}
}

void STG_HomingBullet::SetBullet(Vector2 pos, Vector2* targetPos,int* targetHp,int speed, int attack, int timer)
{
	if (!STG_Player::getIsDMG())
	{
		for (int i = 0; i < COUNT; i++)
		{
			if (Data[i].Timer <= 0)
			{
				Data[i].Pos = pos;
				Data[i].Dir = Vector2(0, -1);
				Data[i].TargetPos = targetPos;
				Data[i].TargetHP = targetHp;
				Data[i].isBreak = false;
				Data[i].Speed = speed;
				Data[i].Timer = timer;
				Data[i].Attack = attack;
				Data[i].isGraze = false;

				if (Data[i].TargetPos != NULL)
				{
					Vector2 temp = Data[i].Pos - *(Data[i].TargetPos);
					Data[i].oldMagnitude = std::sqrt(temp.x * temp.x + temp.y * temp.y);
				}
				else
				{
					Data[i].oldMagnitude = 0;
				}
				
				return;
			}
		}
	}
}

void STG_HomingBullet::Draw(ID3D12GraphicsCommandList* cmdList)
{
	int id = 0;
	for (int i = 0; i < COUNT; i++)
	{
		if (Data[i].Timer > 0)
		{

			XMMATRIX* mat = &HomingBulletConstBuffer->mat[id];
			*mat = XMMatrixIdentity();
			*mat = XMMatrixMultiply(*mat, XMMatrixRotationZ(0));
			*mat = XMMatrixMultiply(*mat, XMMatrixScaling(2.0f / WINDOW_WIDTH * 0.5f * Scale.x, -2.0f / WINDOW_HEIGHT * 0.5f * Scale.y, 0.1f));

			mat->r[3].m128_f32[0] = Data[i].Pos.x / WINDOW_WIDTH * 2.0f;
			mat->r[3].m128_f32[1] = Data[i].Pos.y / WINDOW_HEIGHT * -2.0f;

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
}