#include "Scene.h"

int ShootingManager::Stage = 0;
bool ShootingManager::isEscape = false;

void ShootingManager::ChengeScene(ID3D12Device* dev, ID3D12GraphicsCommandList* cmdList) {
	switch (Stage)
	{
	case 0:
		Manager::ChangeScene(new ShootingScene(dev, cmdList));
		break;
	case 1:
		/*Manager::ChangeScene(new Test1(dev, cmdList));*/
		break;
	}
}

void ShootingManager::Update() {
	if(InputManager::GetPush(INPUT_TYPE::esc))
	{
		isEscape = !isEscape;
	}
}