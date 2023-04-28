#pragma once
#include "Lib/Lib.h"


//宣言
class NovelScene :public BaseScene
{
	VTN_Image background;
	VTN_Image textBack;
	TextManager text;
	SoundManager Sound;
	int num;

public:
	NovelScene(ID3D12Device* Dev, ID3D12GraphicsCommandList* CmdList) :BaseScene(Dev, CmdList)
	{

	}

	
	//初期化(Initialize)。一回だけ実行。
	void Init();
	//メインループ
	bool Update();
	void onDestroy();
};
