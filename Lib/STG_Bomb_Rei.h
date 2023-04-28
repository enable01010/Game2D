#pragma once
#include "STG_Bomb.h"

class STG_Enemy;
class STG_Boss;
class STG_Unit;
class FxParticle;

class STG_Bomb_Rei :public STG_Bomb
{
	//�r�[���Ŏg������
	VTN_Image Beam;

	//�`���[�W�Ŏg������
	FxParticle Charge;

	int ChargeTime = 500;
	int MaxTime = 1500;

	const int ATTACK = 2;

	/// <summary>
	/// �摜�̈ʒu����
	/// </summary>
	/// <param name="pos"></param>
	void PosUpdate(Vector2 pos);

	/// <summary>
	/// ���Ԍo�߂̊Ǘ�
	/// </summary>
	/// <param name="unit"></param>
	void TimeManager(STG_Unit** unit);

	/// <summary>
	/// �����蔻��̊Ǘ�
	/// </summary>
	/// <param name="unit"></param>
	void HitCheck(STG_Unit** unit);
public:
	void Init(ID3D12Device* dev);
	void UseBomb(Vector2 pos);
	void Update(STG_Unit** unit);
	void Draw(ID3D12GraphicsCommandList* cmdList);
	void Destroy();
};

