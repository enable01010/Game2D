#pragma once
#include "..\stdafx.h"
#include "Lib.h"

#define DESTROY(obj)delete obj;obj == nullptr;

enum class INPUT_TYPE {
	a,
	z,
	x,
	up,
	enter,
	down,
	right,
	left,
	esc,
	MAX
};

struct InputData{
public:
	int pad;
	int key;
	int time;
};

class InputManager
{
	static InputManager* instance;

public:
	InputData InputData[(int)INPUT_TYPE::MAX];
	static InputManager* GetInstance();

	void Init();
	void Update();
	void onDestroy();

	static bool GetPush(INPUT_TYPE key);
	static bool GetKey(INPUT_TYPE ket);
	static int GetTime(INPUT_TYPE ket);
};
