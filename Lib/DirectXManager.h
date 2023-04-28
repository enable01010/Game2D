#pragma once
#include "../stdafx.h"

class DirectXManager
{
	static DirectXManager* instance;
public:
	static DirectXManager* GetInstance()
	{
		// ����������Ė�����Ύ����ŏ�����
		if (instance == nullptr)
		{
			instance = new DirectXManager;
		}
		return instance;
	}
	// DirectX����Ȃ���Window�̃n���h�������ǐF�X�g���̂�
	HWND hWnd;

	static void ShowDialog(LPCTSTR lpText, LPCTSTR lpCaption)
	{
		MessageBox(NULL, lpText, lpCaption, MB_OK);
	}
};
