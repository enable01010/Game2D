#pragma once
#include "stdafx.h"

/// <summary>
/// �G�t�F�N�g1�Ԃ�̃f�[�^
/// </summary>
struct STG_FxData
{
	Vector2 Pos;
	int Timer; // �c�莞�ԁB0���Ɛ����ĂȂ�
};

// �V�F�[�_�[�ɓn���\����
__declspec(align(256))
struct FxShaderData
{
	Color color;
	XMMATRIX mat[COUNT];

	FxShaderData()
	{
		color = Color();
		for (int i = 0; i < COUNT; i++)
		{
			mat[i] = XMMATRIX();
		}
	}
};

/// <summary>
/// �G�t�F�N�g��\������
/// GPU Instance
/// </summary>
class STG_Fx : public STG_Instance<STG_FxData>
{
	// �V�F�[�_�[�ɓn���萔�o�b�t�@
	FxShaderData	m_ConstantBufferData = FxShaderData();
	FxShaderData* MapConstBuffer;

public:
	STG_Fx();

	// �������B�u���_�̐ݒ�v�Ɓu�V�F�[�_�[�ǂݍ��݁v�A�u�萔�o�b�t�@�̐ݒ�v�ȊO�͒e�Ɠ���
	LRESULT Init(ID3D12Device* dev, const wchar_t* fileName, float size, bool add = false);
	void Draw(ID3D12GraphicsCommandList* cmdList);
	void Destroy();

	/// <summary>
	/// �G�t�F�N�g���g�p��Ԃɂ��܂�
	/// </summary>
	/// <param name="pos"></param>
	void SetFx(Vector2 pos);
	
	
};
