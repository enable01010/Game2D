#pragma once

#define PLAY_WINDOW_LEFT_AREA -415
#define PLAY_WINDOW_RIGHT_AREA 215
#define PLAY_WINDOW_UP_AREA 360
#define PLAY_WINDOW_DOWN_AREA -360

#define FADE_FIRST_NUM 1.0f
#define FADE_IN_COEFFICIENT 1.05f
#define FADE_IN_MAX_ARUFA 0.9f
#define FADE_OUT_COEFFICIENT 0.95f
#define FADE_OUT_MIN_ARUFA 0.5f

#define INSTANCE_COUNT_MAX 800

#define DEBUG_MODE
#pragma comment (lib, "dxgi.lib")
#pragma comment (lib, "d3d12.lib")

#include <stdio.h>
#include <d3d12.h>
#include <dxgi1_4.h>
#include <string>
#include <vector>
#include <dxgi1_6.h>
#include <time.h>
#include <thread>
#include <cstdio>
#include <cstdint>

// 新しく追加する部分
#include <d3dx12.h>
#include<iostream>

#pragma comment (lib, "d3dcompiler.lib")
#include <d3dcompiler.h>
// テクスチャ（画像）を扱う
#pragma comment (lib, "DirectXTex.lib")
#include <DirectXTex.h>

#include <DirectXMath.h>
#include "Lib/Lib.h"// 次ページのLib.hを作るまでエラーになるので気にしないでください

#pragma comment(lib,"xinput.lib")
#include <Xinput.h>

#include"Lib/Lib.h"
#include "Lib/Input.h"

#include"Lib\/TextManager.h"

#include<dsound.h>
#pragma comment(lib,"dsound")
#pragma comment(lib,"winmm.lib")
#include "Lib\SoundManager.h"
#include <playsoundapi.h>
#include "Lib\DirectXManager.h"

#include "ShootingManager.h"
#include "Object.h"

//#include "NovelScene.h"
//#include "TitleScene.h"

enum class SOUND_NAME
{
	CLICK,
	FANFARRE,
	MAX
};