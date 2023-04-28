#pragma once
#include "stdafx.h"
#define ItemMaxCount 800

class STG_Player;

enum class ENEMY_DROP
{
	HP,
	BOMB,
	ATTACK,
	BIG_ATTACK,
	FULL_ATTACK,
	POINT,
	NOTHING,
	RANDOM,
	MAX
};

struct VTN_ItemData 
{
	Vector2 Pos;
	Vector2 Dir;

	int Timer;

};

_declspec(align(256))
struct ItemShaderData
{
	Color color;
	XMFLOAT2 uv;
	XMMATRIX mat[ItemMaxCount];
};


class VTN_Item :public STG_Instance<VTN_ItemData>
{
	ItemShaderData m_ConstantBufferData = ItemShaderData();
	ItemShaderData* ItemConstBuffer;

	//�A�C�e���̎��
	ENEMY_DROP myDrop;

	//�����֌W
	const int DROP_FIRST_FORCE = -1;
	const int SPAWN_FIRST_FORCE = 1;
	const int MAX_SPEED = 3;
	const float GRAVITY = 0.15f;
	const float SPEED = 0.125f;

	//�A�C�e���͈̔�
	const int ITEM_AREA = 500;

public:
	//�I�[�o�[���C�h
	VTN_Item();
	LRESULT Init(ID3D12Device* dev, const wchar_t* fileName, ENEMY_DROP MyDrop, bool add = false);
	void Update(STG_Player* player);
	void Draw(ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	/// �G���A�C�e���𗎂Ƃ��Ƃ��Ɏg���֐�
	/// </summary>
	/// <param name="pos"></param>
	void Drop(Vector2 pos);

	/// <summary>
	/// ���Ԍo�߂ŃA�C�e���𗎂Ƃ��Ƃ��Ɏg���֐�
	/// </summary>
	/// <param name="pos"></param>
	void Spawn(Vector2 pos);
	
};

