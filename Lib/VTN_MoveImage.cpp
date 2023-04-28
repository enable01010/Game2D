#include "Object.h"

LRESULT VTN_MoveImage::Init(ID3D12Device* dev, const wchar_t* fileName,  Vector2 Goal,int Speed)
{
	LRESULT ret = VTN_Image::Init(dev, fileName);
	GoalPos = Goal;
	speed = Speed;
	return ret;
}

void VTN_MoveImage::Update()
{
	if (!getIsGoal())
	{
		Vector2 distance = Vector2(Pos.x - GoalPos.x, Pos.y - GoalPos.y);
		if (abs(distance.x) <= 10 && abs(distance.y) <= 10) {
			Pos.x = GoalPos.x;
			Pos.y = GoalPos.y;
		}
		else 
		{
			float magnitude = std::sqrt(distance.x * distance.x + distance.y * distance.y);
			Vector2 newPos = Vector2(Pos.x - distance.x / magnitude * speed, Pos.y - distance.y / magnitude * speed);
			SetPos(newPos);
		}
	}
}

bool VTN_MoveImage::getIsGoal()
{
	if (GoalPos.x == Pos.x && GoalPos.y == Pos.y)
	{
		return true;
	}
	return false;
}