#include "Object.h"
#include "Scene.h"

VTN_Item::VTN_Item() {
	ItemConstBuffer = NULL;
}

LRESULT VTN_Item::Init(ID3D12Device* dev, const wchar_t* fileName, ENEMY_DROP MyDrop, bool add)
{
	Dev = dev;
	LRESULT result = LoadTex(dev, fileName);

	Init_Vertex();
	Init_Blend(add);
	Init_Sampler();

	//
	result = LoadShader(dev, L"Lib\\Shader\\BulletPS.hlsl", L"Lib\\Shader\\StarVS.hlsl");
	//

	myDrop = MyDrop;
	InitConstBuffer<ItemShaderData>(&m_ConstantBufferData, &ItemConstBuffer);
	for (int h = 0; h < INSTANCE_COUNT_MAX; h++) {
		ItemConstBuffer->mat[h] = XMMatrixIdentity();
	}

	ItemConstBuffer->uv = XMFLOAT2(0, 0);
	ItemConstBuffer->color = Color(1, 1, 1, 1);

	InitFlag = true;

	Scale = XMFLOAT2(1.0f, 1.0f);
	Size = XMFLOAT2(1.0f, 1.0f);

	return result;
}

void VTN_Item::Drop(Vector2 pos)
{
	for (int i = 0; i < ItemMaxCount; i++)
	{
		if (Data[i].Timer <= 0)
		{
			Data[i].Timer = 1;
			Data[i].Pos = pos;
			Data[i].Dir = Vector2(0, DROP_FIRST_FORCE);
			return;
		}
	}
}

void VTN_Item::Spawn(Vector2 pos)
{
	for (int i = 0; i < ItemMaxCount; i++)
	{
		if (Data[i].Timer <= 0)
		{
			Data[i].Timer = 1;
			Data[i].Pos = pos;
			Data[i].Dir = Vector2(0, SPAWN_FIRST_FORCE);
			return;
		}
	}
}

void VTN_Item::Update(STG_Player* player)
{
	for (int i = 0; i < ItemMaxCount; i++)
	{
		if (Data[i].Timer >= 0)
		{
			Data[i].Pos += Vector2(Data[i].Dir.x * FrameRateManager::getDeltaTime() * SPEED, Data[i].Dir.y * FrameRateManager::getDeltaTime() * SPEED);

			//Å‚‘¬“x‚É‚È‚Á‚Ä‚¢‚È‚¢ê‡‚É‰Á‘¬‚·‚é
			if (Data[i].Dir.y != MAX_SPEED)
			{
				Data[i].Dir.y += GRAVITY * FrameRateManager::getDeltaTime() * SPEED;
				if (Data[i].Dir.y > MAX_SPEED)
				{
					Data[i].Dir.y = MAX_SPEED;
				}
			}
		}
	}

	int itemCount = GetCount();
	for (int i = 0; i < INSTANCE_COUNT_MAX; i++)
	{
		VTN_ItemData* item = GetData(i);
		if (item->Timer <= 0)
		{
			continue;
		}

		Vector2 Dir = item->Pos - player->getPos();
		float len = Dir.sqrtMagnitude();
		if (len < ITEM_AREA)
		{
			item->Timer = 0;
			player->getItem(myDrop);
		}
	}

}

void VTN_Item::Draw(ID3D12GraphicsCommandList* cmdList)
{
	int id = 0;
	for (int i = 0; i < INSTANCE_COUNT_MAX; i++)
	{
		if (Data[i].Timer > 0)
		{
			XMMATRIX* mat = &ItemConstBuffer->mat[id];
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