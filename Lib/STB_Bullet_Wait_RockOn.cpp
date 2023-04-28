#include "Object.h"

void STB_Bullet_Wait_RockOn::BulletAI(int i)
{
	int elapsedTime = Data[i].GetSpeed() - Data[i].GetTimer();
	if (elapsedTime < 100)
	{
		Data[i].SetScale(Vector2(Data[i].GetScale().x - 0.1f, Data[i].GetScale().y - 0.1f));
	}
	else if (elapsedTime == 100)
	{
		Vector2 temp = (Data[i].GetPos() - *Data[i].GetTargetPos()) / 100;
		Data[i].SetDir(temp);
	}
	else
	{
		Data[i].SetPos(Data[i].GetPos() - Data[i].GetDir());
	}
}

void STB_Bullet_Wait_RockOn::SetBullet(Vector2 pos, Vector2 dir, STG_Status* status, int timer)
{
	if (!STG_Player::getIsDMG())
	{
		for (int i = 0; i < COUNT; i++)
		{
			if (Data[i].GetTimer() <= 0)
			{
				//Dirのｘを最初の傾きyを時計回り、反時計周りの判定に使用
				Data[i].SetPos(pos);
				Data[i].SetDir(Vector2());
				Data[i].SetTimer(timer);
				Data[i].SetSpeed(timer);
				Data[i].SetTargetPos(status->GetPosAddress());
				Data[i].SetScale(Vector2(10 + Scale.x, 10 + Scale.y));
				Data[i].SetIsGraze(false);
				return;
			}
		}
	}
}

void STB_Bullet_Wait_RockOn::Draw(ID3D12GraphicsCommandList* cmdList)
{
	int id = 0;
	for (int i = 0; i < COUNT; i++)
	{
		if (Data[i].GetTimer() > 0)
		{

			XMMATRIX* mat = &BulletConstBuffer->mat[id];
			*mat = XMMatrixIdentity();
			*mat = XMMatrixMultiply(*mat, XMMatrixRotationZ(0));
			*mat = XMMatrixMultiply(*mat, XMMatrixScaling(2.0f / WINDOW_WIDTH * 0.5f * Data[i].GetScale().x, -2.0f / WINDOW_HEIGHT * 0.5f * Data[i].GetScale().y, 0.1f));

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