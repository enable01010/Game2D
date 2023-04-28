#include "Scene.h"


void NovelScene::Init() {
	//画像
 	background.Init(dev, L"img\\huren.jpg");
	textBack.Init(dev, L"img\\Window.png");
	textBack.SetScale(Vector2(0.5f, 0.5f));
	textBack.SetPos(Vector2(0, 200));
	text.Init(dev,"＞森へ行く酒場海川フレンEルスタリオ絶賛推し中是非見てね ","test");
	num = 0;

	//サウンドのあとに画像をイニットするとバグる
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
	text.Draw(cmdList, "森へ行く", Vector2(300, 550));
	text.Draw(cmdList, "酒場へ行く", Vector2(300, 600));
	text.Draw(cmdList, "海へ行く", Vector2(300, 650));
	text.Draw(cmdList, "川へ行く", Vector2(300, 700));
	text.Draw(cmdList, "＞", Vector2(270, (550 + (50*num))));
	return true;
}
void NovelScene::onDestroy() {}