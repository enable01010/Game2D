#pragma once

class Manager
{

	static bool isFirstScene;
	static bool isEscape;
	VTN_Image EscapeIMG;
	VTN_Image cursol;
	int cursolNum;
	float flameTime;
	TextManager text;

	VTN_Image Circle;
	float CircleNum;
	float radian = 3.14f / 180;

	static BaseScene* temporaryScene;
	static bool isChengeScene;
	static bool isFadeOut;
	static bool isInitFinish;
	static bool isFadeIn;
	VTN_Image Fade;
	static float FadeNum;
	std::thread th;

public:
	static bool GetIsEscape() { return isEscape; }

	static bool getIsChengeScene() { return isChengeScene; }
	static bool getIsFadeOut() { return isFadeOut; }
	static bool getIsInitFisish() { return isInitFinish; }

	static BaseScene* currentScene;
	static BaseScene* newScene;

	static void ThreadLoadScene(BaseScene* BS);
	static void ThreadInit();
	static void ChangeScene(BaseScene* _newScene)
	{
		newScene = _newScene;
	}

	 void Init(ID3D12Device* dev);
	static void Update();
	 bool Draw(ID3D12GraphicsCommandList* cmdList);

};



