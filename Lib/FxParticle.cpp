#include "Object.h"

LRESULT FxParticle::Init(ID3D12Device* dev, const wchar_t* fileName, float size, bool add)
{
	Dev = dev;
	LRESULT result = LoadTex(dev, fileName);

	// ���_���C�A�E�g
	Init_Vertex();
	Init_Blend(add);
	Init_Sampler();


	// �V�F�[�_�[�̓ǂݍ���
	result = LoadShader(dev, L"Lib\\Shader\\FxParticlePS.hlsl", L"Lib\\Shader\\FxParticleVS.hlsl");

	// �萔�o�b�t�@�̐ݒ�
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
	// ���ʖ����g���܂킷�ׂ�ID
	int id = 0;
	for (int i = 0; i < COUNT; i++)
	{
		if (Data[i].GetTimer() > 0)
		{
			Data[i].SetTimer(Data[i].GetTimer() - 1);
			MapConstBuffer->Data[id].color.a = Data[i].GetTimer() / 20.0f;
			XMMATRIX* mat = &MapConstBuffer->Data[id].mat;// �g���܂킷
			// ���_�ɏ�����
			*mat = XMMatrixIdentity();
			// ��]
			*mat = XMMatrixMultiply(*mat, XMMatrixRotationZ(0));
			// �g��k��
			*mat = XMMatrixMultiply(*mat, XMMatrixScaling(2.0f / WINDOW_WIDTH * 0.5f * Scale.x, -2.0f / WINDOW_HEIGHT * 0.5f * Scale.y, 0.1f));
			// �ړ�
			Data[i].SetPos(Data[i].GetPos() - Data[i].GetDir());
			mat->r[3].m128_f32[0] = (Pos.x + Data[i].GetPos().x) / WINDOW_WIDTH * 2.0f;
			mat->r[3].m128_f32[1] =(Pos.y + Data[i].GetPos().y )/ WINDOW_HEIGHT * -2.0f;
			id++;
		}
	}
	// �G�t�F�N�g���������͕\�����Ȃ��B���̕ӂ͒e�̃R�[�h�Ɠ����Ȃ̂ŃR�s�[�����OKs
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