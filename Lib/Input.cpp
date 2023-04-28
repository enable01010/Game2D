#include "Input.h"

InputManager* InputManager::instance = NULL;
InputManager* InputManager::GetInstance() {
	//����������ĂȂ���Ύ����ŏ�����
	if (instance == nullptr) {
		instance = new InputManager;
		instance->Init();
	}
	return instance;
}

void InputManager::Init() {
	
	InputData[(int)INPUT_TYPE::a].key = 'A';
	InputData[(int)INPUT_TYPE::z].key = 'Z';
	InputData[(int)INPUT_TYPE::x].key = 'X';
	InputData[(int)INPUT_TYPE::enter].key = VK_RETURN;
	InputData[(int)INPUT_TYPE::up].key = VK_UP;
	InputData[(int)INPUT_TYPE::down].key = VK_DOWN;
	InputData[(int)INPUT_TYPE::right].key = VK_RIGHT;
	InputData[(int)INPUT_TYPE::left].key = VK_LEFT;
	InputData[(int)INPUT_TYPE::esc].key = VK_ESCAPE;


	InputData[(int)INPUT_TYPE::a].pad = XINPUT_GAMEPAD_A;
	InputData[(int)INPUT_TYPE::z].pad = XINPUT_GAMEPAD_A;
	InputData[(int)INPUT_TYPE::x].pad = XINPUT_GAMEPAD_A;
	InputData[(int)INPUT_TYPE::enter].pad = XINPUT_GAMEPAD_A;
	InputData[(int)INPUT_TYPE::up].pad = XINPUT_GAMEPAD_A;
	InputData[(int)INPUT_TYPE::down].pad = XINPUT_GAMEPAD_A;
	InputData[(int)INPUT_TYPE::right].pad = XINPUT_GAMEPAD_A;
	InputData[(int)INPUT_TYPE::left].pad = XINPUT_GAMEPAD_A;
	InputData[(int)INPUT_TYPE::esc].pad = XINPUT_GAMEPAD_A;

	for (int i = 0; i < (int)INPUT_TYPE::MAX; i++) {
		InputData[i].time = 0;
	}
}

void InputManager::Update() {
	XINPUT_STATE state;
	DWORD XinputState = XInputGetState(0, &state);
	for (int i = 0; i < (int)INPUT_TYPE::MAX; i++) {
		if ((XinputState == ERROR_SUCCESS && state.Gamepad.wButtons &InputData[i].pad)||GetAsyncKeyState(InputData[i].key)&0x8000) {
			InputData[i].time++;
		}
		else {
			InputData[i].time = 0;
		}
	}
}

/// <summary>
/// ��������
/// </summary>
/// <param name="key"></param>
/// <returns></returns>
bool InputManager::GetPush(INPUT_TYPE key) {
	if (instance->InputData[(int)key].time == 1){
		return true;
	}
	return false;
}

/// <summary>
/// �����Ă��
/// </summary>
/// <param name="key"></param>
/// <returns></returns>
bool InputManager::GetKey(INPUT_TYPE key) {
	if (instance->InputData[(int)key].time >= 1) {
		return true;
	}
	return false;
}

/// <summary>
/// �J�X�^���p
/// </summary>
/// <param name="key"></param>
/// <returns></returns>
int InputManager::GetTime(INPUT_TYPE key) {
	return instance->InputData[(int)key].time;
}

void InputManager::onDestroy() {
	DESTROY(instance);
}
