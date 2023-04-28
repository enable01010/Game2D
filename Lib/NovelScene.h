#pragma once
#include "Lib/Lib.h"


//�錾
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

	
	//������(Initialize)�B��񂾂����s�B
	void Init();
	//���C�����[�v
	bool Update();
	void onDestroy();
};
