#include "Scene.h"


void NovelScene::Init() {
	//�摜
 	background.Init(dev, L"img\\huren.jpg");
	textBack.Init(dev, L"img\\Window.png");
	textBack.SetScale(Vector2(0.5f, 0.5f));
	textBack.SetPos(Vector2(0, 200));
	text.Init(dev,"���X�֍s������C��t����E���X�^���I��^���������񌩂Ă� ","test");
	num = 0;

	//�T�E���h�̂��Ƃɉ摜���C�j�b�g����ƃo�O��
	Sound.Init();
	Sound.Load("sound\\Click.wav");

}
bool NovelScene::Update() {
	InputManager::GetInstance()->Update();
	if (InputManager::GetPush(INPUT_TYPE::a))
	{
		GameManager::ChangeScene(new TitleScene(dev, cmdList));
	}
	if (InputManager::GetPush(INPUT_TYPE::up))
	{
		if (num > 0)
		{
			num--;
		}
	}
	if (InputManager::GetPush(INPUT_TYPE::down))
	{
		if (num < 3)
		{
			num++;
		}
	}
	background.Draw(cmdList);
	textBack.Draw(cmdList);
	text.DrawStart(cmdList);
	text.Draw(cmdList, "�X�֍s��", Vector2(300, 550));
	text.Draw(cmdList, "����֍s��", Vector2(300, 600));
	text.Draw(cmdList, "�C�֍s��", Vector2(300, 650));
	text.Draw(cmdList, "��֍s��", Vector2(300, 700));
	text.Draw(cmdList, "��", Vector2(270, (550 + (50*num))));
	return true;
}
void NovelScene::onDestroy() {}