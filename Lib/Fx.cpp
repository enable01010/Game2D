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

	// ���_���C�A�E�g
	inputLayout[0] = { "POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 };
	inputLayout[1] = { "TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 };

	// ���_�̐ݒ�
	float width = metadata.width * 0.5f * 0.25f * size;
	float height = metadata.height * 0.5f * 0.3333333f * size;

	//�k�ڕύX�̊�Ƃ��ύX�p
	vertexs[0].Set(-width, height, 0.0f, 0.0f, 0.3333333f);
	vertexs[1].Set(-width, -height, 0.0f, 0.0f, 0.0f);
	vertexs[2].Set(width, height, 0.0f, 0.25f, 0.3333333f);
	vertexs[3].Set(width, -height, 0.0f, 0.25f, 0.0f);

	Init_Blend(add);
	Init_Sampler();


	// �V�F�[�_�[�̓ǂݍ���
	result = LoadShader(dev, L"Lib\\Shader\\FxPS.hlsl", L"Lib\\Shader\\FxVS.hlsl");

	// �萔�o�b�t�@�̐ݒ�
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
	// ���ʖ����g���܂킷�ׂ�ID
	int id = 0;
	for (int i = 0; i < COUNT; i++)
	{
		if (Data[i].Timer > 0)
		{
			Data[i].Timer--;
			XMMATRIX* mat = &MapConstBuffer->mat[id];// �g���܂킷
			// ���_�ɏ�����
			*mat = XMMatrixIdentity();
			// ��]
			*mat = XMMatrixMultiply(*mat, XMMatrixRotationZ(0));
			// �g��k��
			*mat = XMMatrixMultiply(*mat, XMMatrixScaling(2.0f / WINDOW_WIDTH * 0.5f * Scale.x, -2.0f / WINDOW_HEIGHT * 0.5f * Scale.y, 0.1f));
			// �ړ�
			mat->r[3].m128_f32[0] = Data[i].Pos.x / WINDOW_WIDTH * 2.0f;
			mat->r[3].m128_f32[1] = Data[i].Pos.y / WINDOW_HEIGHT * -2.0f;
			int frame = 5;
			mat->r[3].m128_f32[2] = 11 - Data[i].Timer / frame;
			id++;
		}
	}
	// �G�t�F�N�g���������͕\�����Ȃ��B���̕ӂ͒e�̃R�[�h�Ɠ����Ȃ̂ŃR�s�[�����OK
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

