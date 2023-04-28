#include "Object.h"

void STG_Bomb::UseCutIn()
{
	if (Timer > 500)
	{
		CutInImg.AddColor(-0.02f);
		CutInImg.AddScale(0.03f);
	}
}