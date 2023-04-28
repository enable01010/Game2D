#include "Scene.h"

BaseScene* Manager::currentScene = NULL;
BaseScene* Manager::newScene = NULL;
BaseScene* Manager::temporaryScene = NULL;

bool Manager::isFirstScene = true;

bool Manager::isChengeScene = false;
bool Manager::isFadeOut = false;
bool Manager::isInitFinish = false;
bool Manager::isFadeIn = false;
float Manager::FadeNum = 0.1;

bool Manager::isEscape = false;

void Manager::Init(ID3D12Device* dev)
{
	EscapeIMG.Init(dev, L"img\\Manager\\EscapeImg.png");
	EscapeIMG.SetScale(Vector2(0.3f,0.3f));
	EscapeIMG.SetColor(Color(1, 1, 1, 0.5f));
	cursol.Init(dev, L"img\\Manager\\cursol.png");
	cursol.SetScale(Vector2(0.5f, 0.5f));
	cursol.SetPos(Vector2(-300, 10));
	text.Init(dev, "0123456789fps", "Manager");
	Fade.Init(dev, L"img\\Manager\\Fade.png");
	Circle.Init(dev, L"img\\Manager\\Circle.png");
	Circle.SetScale(Vector2(0.5f,0.5f));
}

void Manager::Update() {
	if (newScene) {
		if (currentScene != NULL)
		{
			currentScene->onDestroy();
			delete currentScene;

			_CrtDumpMemoryLeaks();
		}

		if (isFirstScene) 
		{
			newScene->Init();
			isFirstScene = false;
		}
		
		currentScene = newScene;
		newScene = NULL;
	}

	if (InputManager::GetPush(INPUT_TYPE::esc))
	{
		isEscape = !isEscape;
	}
	
}

bool Manager::Draw(ID3D12GraphicsCommandList* cmdList) 
{

	if (isEscape) {
		
		if (InputManager::GetPush(INPUT_TYPE::right)) {
			cursolNum = 1;
			cursol.SetPos(Vector2(0, 10));
		}
		else if (InputManager::GetPush(INPUT_TYPE::left)) {
			cursolNum = 0;
			cursol.SetPos(Vector2(-300, 10));
		}
		else if (InputManager::GetPush(INPUT_TYPE::z)) {
			if (cursolNum == 0) {
				return false;
			}
			else if (cursolNum == 1) {
				isEscape = false;
			}
		}
		
		EscapeIMG.Draw(cmdList);
		cursol.Draw(cmdList);
	}

	if (isChengeScene)
	{
		if (!isFadeOut) 
		{
			if (FadeNum >= 1.0f) {
				isFadeOut = true;
				ChangeScene(temporaryScene);

				//ÉRÉãÅ[É`Éì
				th = std::thread(ThreadInit);
				printf("a");
				
			}
			else 
			{
				FadeNum *= 1.1f;
				Fade.SetColor(Color(1, 1, 1, FadeNum));
			}
		}
		else if(isInitFinish)
		{
			if (FadeNum < 0.1f) {
				isFadeIn = true;
			}
			else
			{
				FadeNum *= 0.95f;
				Fade.SetColor(Color(1, 1, 1, FadeNum));
			}
			if (isFadeIn)
			{
				temporaryScene = NULL;
				isChengeScene = false;
				isFadeOut = false;
				isInitFinish = false;
				isFadeIn = false;
				FadeNum = 0.1;
				th.join();
			}
		}

		Fade.Draw(cmdList);
		if (isFadeOut && !isInitFinish) 
		{
			/*CircleNum*/
			CircleNum += 5.00f;
			Circle.SetPos(Vector2(-cos((CircleNum + 90) * radian)*100,sin((CircleNum + 90) * radian)*100));
			Circle.Draw(cmdList);
		}
	}
	return true;
}

void Manager::ThreadLoadScene(BaseScene* BS)
{
	temporaryScene = BS;
	isChengeScene = true;
}

void Manager::ThreadInit() {
	temporaryScene->Init();
	isInitFinish = true;
}
