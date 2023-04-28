#include "Object.h"
STG_ChargeShot::STG_ChargeShot()
{
	BulletConstBuffer = NULL;
	for (int i = 0; i < COUNT; i++) {
		Data[i].SetPos(Vector2());
		Data[i].SetBeam(0);
	}
}

LRESULT STG_ChargeShot::Init(ID3D12Device* dev, const wchar_t* fileName, bool add)
{
	Dev = dev;
	LRESULT result = LoadTex(dev, fileName);

	Init_Vertex(IMG_BASE_POSITION::SENTER_DOWN);
	Init_Blend(add);
	Init_Sampler();

	result = LoadShader(dev, L"Lib\\Shader\\BulletPS.hlsl", L"Lib\\Shader\\BulletVS.hlsl");

	InitConstBuffer<ChargeShaderData>(&m_ConstantBufferData, &BulletConstBuffer);
	for (int h = 0; h < COUNT; h++) {
		BulletConstBuffer->mat[h] = XMMatrixIdentity();
	}

	BulletConstBuffer->uv = XMFLOAT2(0, 0);
	BulletConstBuffer->color = Color(1, 1, 1, 1);

	InitFlag = true;

	Scale = XMFLOAT2(1.0f, 1.0f);
	Size = XMFLOAT2(1.0f, 1.0f);
	
	return result;
}

void STG_ChargeShot::Update(STG_Unit** unit)
{
	maxCount = 0;
	for (int h = 0; h < COUNT; h++)
	{
		if (Data[h].GetBeam() > 0)
		{
			Data[h].SetBeam(Data[h].GetBeam() - 1);
			float time = BeamMaxTime - Data[h].GetBeam();
			float rateX = (time < 10) ? (time / 10.0f / 100) : (time < BeamStartTime) ? (0.1f) : (Data[h].GetBeam() / BeamMaxTime);
			float rateY = (time < BeamStartTime) ? (time / 10.0f) : (Data[h].GetBeam() / BeamMaxTime);
			Data[h].SetScale(Vector2(0.25f * rateX, time / 30.0F * 2.5F + rateY));
			if (BeamStartTime < time)
			{
				int UnitCount = STG_Unit::GetUnitTypeCount();
				for (int i = 0; i < UnitCount; i++)
				{
					if (MySide == unit[i]->GetGroupSide())
					{
						continue;
					}

					int InstanceCount = unit[i]->GetInstanceCount();
					for (int j = 0; j < InstanceCount; j++)
					{
						STG_Status* target = unit[i]->GetUnitStatus(j);

						if (target->GetUnitPos().y < Data[h].GetRadian() * target->GetUnitPos().x + Data[h].GetPos().y - (Data[h].GetRadian() * Data[h].GetPos().x) + abs(Width / cos(Data[h].GetRotation())) &&
							target->GetUnitPos().y > Data[h].GetRadian() * target->GetUnitPos().x + Data[h].GetPos().y - (Data[h].GetRadian() * Data[h].GetPos().x) - abs(Width / cos(Data[h].GetRotation())))
						{
							Vector2 dir = Data[h].GetPos() - target->GetUnitPos();
							if (((Data[h].GetDir().x > 0 && dir.x < 0) || (Data[h].GetDir().x < 0 && dir.x > 0)) &&
								((Data[h].GetDir().y > 0 && dir.y < 0) || (Data[h].GetDir().y < 0 && dir.y > 0)))
							{

							}
							else
							{
								target->Damage(1);
							}
						}
					}
				}
			}
			maxCount = h + 1;
		}
	}
}

void STG_ChargeShot::Draw(ID3D12GraphicsCommandList* cmdList)
{
	// ビームエフェクト表示
	int id = 0;
	for (int i = 0; i < COUNT; i++)
	{
		if (Data[i].GetBeam() > 0)
		{
			if (!InitFlag)return;

			XMMATRIX* mat = &BulletConstBuffer->mat[id];
			*mat = XMMatrixIdentity();
			*mat = XMMatrixMultiply(*mat, XMMatrixScaling(2.0f / WINDOW_WIDTH * Data[i].GetScale().x, -2.0f / WINDOW_HEIGHT * Data[i].GetScale().y, 0.1f));
			*mat = XMMatrixMultiply(*mat, XMMatrixRotationZ(Data[i].GetRotation()));
			

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
}

void STG_ChargeShot::Destroy()
{
	VTN_Image::Destroy();
}

void STG_ChargeShot::SetBeam(Vector2 pos, Vector2 targetPos)
{
	for (int i = 0; i < COUNT; i++)
	{
		if (Data[i].GetBeam() == 0)
		{
			Data[i].SetPos(pos);
			Data[i].SetDir(pos - targetPos);
			if (Data[i].GetDir().x == 0)
			{
				Data[i].SetRadian(99999);
			}
			else
			{
				Data[i].SetRadian(Data[i].GetDir().y / Data[i].GetDir().x);
			}
			Data[i].SetRotation(Data[i].GetDir());
			Data[i].SetBeam(BeamMaxTime);
			return;
		}
	}
}
