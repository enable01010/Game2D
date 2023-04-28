#pragma once
#include "stdafx.h"

/// <summary>
/// 谷山＿授業用のシューティングのシーン
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
