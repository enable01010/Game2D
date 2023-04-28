#pragma once
#include "stdafx.h"

/// <summary>
/// �J�R�Q���Ɨp�̃V���[�e�B���O�̃V�[��
/// </summary>
class ShootingScene :public BaseScene
{

public:

	VTN_Image background;
	STG_Enemy Enemy;
	//STG_Bullet EnemyBullet;
	/*STG_Boss Boss;*/
	STG_Player player;
	STG_Fx BulletFx;
	//TextManager text;

	ShootingScene(ID3D12Device* Dev, ID3D12GraphicsCommandList* CmdList) :BaseScene(Dev, CmdList)
	{

	}
	void Init();
	void Draw();
	bool Update();
	void onDestroy();
};
