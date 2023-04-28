#pragma once
#include "stdafx.h"

class VTN_MoveImage:public VTN_Image
{
	Vector2 GoalPos;
	int speed;

public:
	LRESULT Init(ID3D12Device* dev, const wchar_t* fileName,  Vector2 Goal,int Speed = 20);

	void Update();
	bool getIsGoal();
	void setGoalPos(Vector2 Goal) { GoalPos = Goal; }
};

