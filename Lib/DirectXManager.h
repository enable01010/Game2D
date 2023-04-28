#pragma once
#include "../stdafx.h"

class DirectXManager
{
	static DirectXManager* instance;
public:
	static DirectXManager* GetInstance()
	{
		// 初期化されて無ければ自動で初期化
		if (instance == nullptr)
		{
			instance = new DirectXManager;
		}
		return instance;
	}
	// DirectXじゃなくてWindowのハンドルだけど色々使うので
	HWND hWnd;

	static void ShowDialog(LPCTSTR lpText, LPCTSTR lpCaption)
	{
		MessageBox(NULL, lpText, lpCaption, MB_OK);
	}
};
