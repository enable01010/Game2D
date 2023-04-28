#pragma once
#include "stdafx.h"
#define PARTICLE_COUNT 500

/// <summary>
/// �G�t�F�N�g1�Ԃ�̃f�[�^
/// </summary>
struct FxParticleData
{
protected:
	Vector2 Pos;
	Vector2 Dir;
	int Timer; // �c�莞�ԁB0���Ɛ����ĂȂ�
public:
	void SetPos(Vector2  pos) { Pos = pos; }
	Vector2 GetPos() { return Pos; }
	void SetDir(Vector2 dir) { Dir = dir; }
	Vector2 GetDir() { return Dir; }
	void SetTimer(int timer) { Timer = timer; }
	int GetTimer() { return Timer; }
};

// �V�F�[�_�[�ɓn���\����
struct FxParticleUnitShaderData
{
	Color color;
	XMMATRIX mat;
};

__declspec(align(256))
struct FxParticleShaderData
{
	FxParticleUnitShaderData Data[PARTICLE_COUNT];

	FxParticleShaderData()
	{
		for (int i = 0; i < PARTICLE_COUNT; i++)
		{
			Data[i] = { Color(),XMMATRIX() };
		}
	}
};

/// <summary>
/// �G�t�F�N�g��\������
/// GPU Instance
/// </summary>
class FxParticle : public STG_Instance<FxParticleData>
{
	// �V�F�[�_�[�ɓn���萔�o�b�t�@
	FxParticleShaderData	m_ConstantBufferData = FxParticleShaderData();
	FxParticleShaderData* MapConstBuffer;


public:

	FxParticle();

	// �������B�u���_�̐ݒ�v�Ɓu�V�F�[�_�[�ǂݍ��݁v�A�u�萔�o�b�t�@�̐ݒ�v�ȊO�͒e�Ɠ���
	LRESULT Init(ID3D12Device* dev, const wchar_t* fileName, float size, bool add = false);

	/// <summary>
	/// �G�t�F�N�g���g�p��Ԃɂ��܂�
	/// </summary>
	/// <param name="pos"></param>
	void SetFx(Vector2 pos, Vector2 dir);
	/// <summary>
	/// �\��
	/// </summary>
	void Draw(ID3D12GraphicsCommandList* cmdList);
};


