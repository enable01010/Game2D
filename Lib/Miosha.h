#pragma once
#include "stdafx.h"

class Miosha :public BaseScene
{
	//�Q�[���v���C�G���A�̐ݒ�p
	VTN_Image background2;
	const int LEFT_SIDE_MAX = -1775;
	VTN_Image background3;
	const int RIGHT_SIDE_MAX = 1575;

	//�w�i�̐ݒ�p
	VTN_MoveImage playArea[2];
	const int BACKGROUND_POS_X = -100;
	const int BACKGROUND_GOAL_POS_Y = 1000;
	const int BACKGROUND_SPEED = 1;
	const int BACKGROUND_POSX_Y = -1200;
	const float BACKGROUND_SIZE = 0.25f;

	//���j�b�g�Ǘ��p
	STG_Player* Player = new STG_Player();
	STG_Enemy* Hubu = new STG_Enemy();
	STG_Enemy* Sisi = new STG_Enemy();
	STG_Boss* BossMid = new STG_Boss_Miosha_Mid();
	STG_Boss* BossLast = new STG_Boss_Miosha_Last();

	//���j�b�g�Ǘ��p
	enum class UnitType
	{
		PLAYER,
		HUBU,
		SISI,
		MID_BOSS,
		LAST_BOSS,
		MAX
	};

	STG_Unit* Unit[(int)UnitType::MAX] =
	{
		Player,
		Hubu,
		Sisi,
		BossMid,
		BossLast
	};

	//�G�l�~�[�Ǘ��p
	enum class EnemyType
	{
		HUBU,
		SISI,
		MAX
	};
	STG_Enemy* Enemy[(int)ENEMY_NAME::MAX] =
	{
		Hubu,
		Sisi
	};
	//�{�X�Ǘ��p
	enum class BossType
	{
		MID_BOSS,
		LAST_BOSS,
		MAX
	};
	STG_Boss* Boss[(int)BossType::MAX] =
	{
		BossMid,
		BossLast
	};

	//�G�l�~�[�Ǘ��p
	const float HUBURS_SIZE = 0.5f;
	const float SISI_SIZE = 0.3f;
	SpawnEnemyData SpawnTable[2][48] = {
		{
			10,ENEMY_NAME::HUBU,Vector2(-200, -400), ENEMY_TYPE::D_S_D, ENEMY_MODE::RIGHT, ENEMY_DROP::RANDOM,1,
			12,ENEMY_NAME::HUBU,Vector2(-190, -400), ENEMY_TYPE::D_S_D, ENEMY_MODE::RIGHT, ENEMY_DROP::RANDOM,1,
			14,ENEMY_NAME::HUBU,Vector2(-180, -400), ENEMY_TYPE::D_S_D, ENEMY_MODE::RIGHT, ENEMY_DROP::RANDOM,1,
			16,ENEMY_NAME::HUBU,Vector2(-170, -400), ENEMY_TYPE::D_S_D, ENEMY_MODE::RIGHT, ENEMY_DROP::RANDOM,1,
			18,ENEMY_NAME::HUBU,Vector2(-160, -400), ENEMY_TYPE::D_S_D, ENEMY_MODE::RIGHT, ENEMY_DROP::RANDOM,1,
			30,ENEMY_NAME::HUBU,Vector2(10, -400), ENEMY_TYPE::D_S_D, ENEMY_MODE::LEFT, ENEMY_DROP::RANDOM,1,
			32,ENEMY_NAME::HUBU,Vector2(0, -400), ENEMY_TYPE::D_S_D, ENEMY_MODE::LEFT, ENEMY_DROP::RANDOM,1,
			34,ENEMY_NAME::HUBU,Vector2(-10, -400), ENEMY_TYPE::D_S_D, ENEMY_MODE::LEFT, ENEMY_DROP::RANDOM,1,
			36,ENEMY_NAME::HUBU,Vector2(-20, -400), ENEMY_TYPE::D_S_D, ENEMY_MODE::LEFT, ENEMY_DROP::RANDOM,1,
			38,ENEMY_NAME::HUBU,Vector2(-30, -400), ENEMY_TYPE::D_S_D, ENEMY_MODE::LEFT, ENEMY_DROP::RANDOM,1,
			40,ENEMY_NAME::HUBU,Vector2(-40, -400), ENEMY_TYPE::D_S_D, ENEMY_MODE::LEFT, ENEMY_DROP::RANDOM,1,
			42,ENEMY_NAME::HUBU,Vector2(-50, -400), ENEMY_TYPE::D_S_D, ENEMY_MODE::LEFT, ENEMY_DROP::RANDOM,1,
			44,ENEMY_NAME::HUBU,Vector2(-60, -400), ENEMY_TYPE::D_S_D, ENEMY_MODE::LEFT, ENEMY_DROP::RANDOM,1,
			46,ENEMY_NAME::HUBU,Vector2(-70, -400), ENEMY_TYPE::D_S_D, ENEMY_MODE::LEFT, ENEMY_DROP::RANDOM,1,
			60,ENEMY_NAME::HUBU,Vector2(-240, -400), ENEMY_TYPE::D_S, ENEMY_MODE::LEFT, ENEMY_DROP::RANDOM,1,
			60,ENEMY_NAME::HUBU,Vector2(40, -400), ENEMY_TYPE::D_S, ENEMY_MODE::RIGHT, ENEMY_DROP::RANDOM,1,
			62,ENEMY_NAME::HUBU,Vector2(-230, -400), ENEMY_TYPE::D_S, ENEMY_MODE::LEFT, ENEMY_DROP::RANDOM,1,
			62,ENEMY_NAME::HUBU,Vector2(30, -400), ENEMY_TYPE::D_S, ENEMY_MODE::RIGHT, ENEMY_DROP::RANDOM,1,
			64,ENEMY_NAME::HUBU,Vector2(-220, -400), ENEMY_TYPE::D_S, ENEMY_MODE::LEFT, ENEMY_DROP::RANDOM,1,
			64,ENEMY_NAME::HUBU,Vector2(20, -400), ENEMY_TYPE::D_S, ENEMY_MODE::RIGHT, ENEMY_DROP::RANDOM,1,
			66,ENEMY_NAME::HUBU,Vector2(-210, -400), ENEMY_TYPE::D_S, ENEMY_MODE::LEFT, ENEMY_DROP::RANDOM,1,
			66,ENEMY_NAME::HUBU,Vector2(10, -400), ENEMY_TYPE::D_S, ENEMY_MODE::RIGHT, ENEMY_DROP::RANDOM,1,
			68,ENEMY_NAME::HUBU,Vector2(-200, -400), ENEMY_TYPE::D_S, ENEMY_MODE::LEFT, ENEMY_DROP::RANDOM,1,
			68,ENEMY_NAME::HUBU,Vector2(0, -400), ENEMY_TYPE::D_S, ENEMY_MODE::RIGHT, ENEMY_DROP::RANDOM,1,
			70,ENEMY_NAME::HUBU,Vector2(-190, -400), ENEMY_TYPE::D_S, ENEMY_MODE::LEFT, ENEMY_DROP::RANDOM,1,
			70,ENEMY_NAME::HUBU,Vector2(-10, -400), ENEMY_TYPE::D_S, ENEMY_MODE::RIGHT, ENEMY_DROP::RANDOM,1,
			72,ENEMY_NAME::HUBU,Vector2(-180, -400), ENEMY_TYPE::D_S, ENEMY_MODE::LEFT, ENEMY_DROP::RANDOM,1,
			72,ENEMY_NAME::HUBU,Vector2(-20, -400), ENEMY_TYPE::D_S, ENEMY_MODE::RIGHT, ENEMY_DROP::RANDOM,1,
			74,ENEMY_NAME::HUBU,Vector2(-170, -400), ENEMY_TYPE::D_S, ENEMY_MODE::LEFT, ENEMY_DROP::RANDOM,1,
			74,ENEMY_NAME::HUBU,Vector2(-30, -400), ENEMY_TYPE::D_S, ENEMY_MODE::RIGHT, ENEMY_DROP::RANDOM,1,
			76,ENEMY_NAME::HUBU,Vector2(-160, -400), ENEMY_TYPE::D_S, ENEMY_MODE::LEFT, ENEMY_DROP::RANDOM,1,
			76,ENEMY_NAME::HUBU,Vector2(-40, -400), ENEMY_TYPE::D_S, ENEMY_MODE::RIGHT, ENEMY_DROP::RANDOM,1,
			78,ENEMY_NAME::HUBU,Vector2(-150, -400), ENEMY_TYPE::D_S, ENEMY_MODE::LEFT, ENEMY_DROP::RANDOM,1,
			78,ENEMY_NAME::HUBU,Vector2(-50, -400), ENEMY_TYPE::D_S, ENEMY_MODE::RIGHT, ENEMY_DROP::RANDOM,1,
			90,ENEMY_NAME::SISI,Vector2(-330, -400), ENEMY_TYPE::D_ST_A_ST_S_U, ENEMY_MODE::LEFT, ENEMY_DROP::ATTACK,1,
			94,ENEMY_NAME::SISI,Vector2(0, -400), ENEMY_TYPE::D_ST_S_U, ENEMY_MODE::RIGHT, ENEMY_DROP::ATTACK,1,
			98,ENEMY_NAME::SISI,Vector2(-200, -400), ENEMY_TYPE::D_ST_S_U, ENEMY_MODE::LEFT, ENEMY_DROP::ATTACK,1,
			102,ENEMY_NAME::SISI,Vector2(110, -400), ENEMY_TYPE::D_ST_A_ST_S_U, ENEMY_MODE::RIGHT, ENEMY_DROP::ATTACK,1,
			106,ENEMY_NAME::SISI,Vector2(-380, -400), ENEMY_TYPE::D_ST_S_U, ENEMY_MODE::LEFT, ENEMY_DROP::ATTACK,1,
			110,ENEMY_NAME::SISI,Vector2(150, -400), ENEMY_TYPE::D_ST_S_U, ENEMY_MODE::RIGHT, ENEMY_DROP::ATTACK,1,
			114,ENEMY_NAME::SISI,Vector2(-200, -400), ENEMY_TYPE::D_ST_A_ST_S_U, ENEMY_MODE::LEFT, ENEMY_DROP::ATTACK,1,
			116,ENEMY_NAME::SISI,Vector2(170, -400), ENEMY_TYPE::D_ST_S_U, ENEMY_MODE::RIGHT, ENEMY_DROP::ATTACK,1,
			118,ENEMY_NAME::SISI,Vector2(-370, -400), ENEMY_TYPE::D_ST_S_U, ENEMY_MODE::LEFT, ENEMY_DROP::ATTACK,1,
			120,ENEMY_NAME::SISI,Vector2(0, -400), ENEMY_TYPE::D_ST_A_ST_S_U, ENEMY_MODE::RIGHT, ENEMY_DROP::ATTACK,1,
			122,ENEMY_NAME::SISI,Vector2(-250, -400), ENEMY_TYPE::D_ST_A_ST_S_U, ENEMY_MODE::LEFT, ENEMY_DROP::ATTACK,1,
			122,ENEMY_NAME::SISI,Vector2(100, -400), ENEMY_TYPE::D_ST_S_U, ENEMY_MODE::RIGHT, ENEMY_DROP::ATTACK,1,
			124,ENEMY_NAME::SISI,Vector2(-300, -400), ENEMY_TYPE::D_ST_A_ST_S_U, ENEMY_MODE::LEFT, ENEMY_DROP::ATTACK,1,
			124,ENEMY_NAME::SISI,Vector2(50, -400), ENEMY_TYPE::D_ST_S_U, ENEMY_MODE::RIGHT, ENEMY_DROP::ATTACK,1,
		},
		{
			10,ENEMY_NAME::HUBU,Vector2(-200, -400), ENEMY_TYPE::D_S_D, ENEMY_MODE::RIGHT, ENEMY_DROP::RANDOM,5,
			12,ENEMY_NAME::HUBU,Vector2(-190, -400), ENEMY_TYPE::D_S_D, ENEMY_MODE::RIGHT, ENEMY_DROP::RANDOM,5,
			14,ENEMY_NAME::HUBU,Vector2(-180, -400), ENEMY_TYPE::D_S_D, ENEMY_MODE::RIGHT, ENEMY_DROP::RANDOM,5,
			16,ENEMY_NAME::HUBU,Vector2(-170, -400), ENEMY_TYPE::D_S_D, ENEMY_MODE::RIGHT, ENEMY_DROP::RANDOM,5,
			18,ENEMY_NAME::HUBU,Vector2(-160, -400), ENEMY_TYPE::D_S_D, ENEMY_MODE::RIGHT, ENEMY_DROP::RANDOM,5,
			30,ENEMY_NAME::HUBU,Vector2(10, -400), ENEMY_TYPE::D_S_D, ENEMY_MODE::LEFT, ENEMY_DROP::RANDOM,5,
			32,ENEMY_NAME::HUBU,Vector2(0, -400), ENEMY_TYPE::D_S_D, ENEMY_MODE::LEFT, ENEMY_DROP::RANDOM,5,
			34,ENEMY_NAME::HUBU,Vector2(-10, -400), ENEMY_TYPE::D_S_D, ENEMY_MODE::LEFT, ENEMY_DROP::RANDOM,5,
			36,ENEMY_NAME::HUBU,Vector2(-20, -400), ENEMY_TYPE::D_S_D, ENEMY_MODE::LEFT, ENEMY_DROP::RANDOM,5,
			38,ENEMY_NAME::HUBU,Vector2(-30, -400), ENEMY_TYPE::D_S_D, ENEMY_MODE::LEFT, ENEMY_DROP::RANDOM,5,
			40,ENEMY_NAME::HUBU,Vector2(-40, -400), ENEMY_TYPE::D_S_D, ENEMY_MODE::LEFT, ENEMY_DROP::RANDOM,5,
			42,ENEMY_NAME::HUBU,Vector2(-50, -400), ENEMY_TYPE::D_S_D, ENEMY_MODE::LEFT, ENEMY_DROP::RANDOM,5,
			44,ENEMY_NAME::HUBU,Vector2(-60, -400), ENEMY_TYPE::D_S_D, ENEMY_MODE::LEFT, ENEMY_DROP::RANDOM,5,
			46,ENEMY_NAME::HUBU,Vector2(-70, -400), ENEMY_TYPE::D_S_D, ENEMY_MODE::LEFT, ENEMY_DROP::RANDOM,5,
			60,ENEMY_NAME::HUBU,Vector2(-240, -400), ENEMY_TYPE::D_S, ENEMY_MODE::LEFT, ENEMY_DROP::RANDOM,5,
			60,ENEMY_NAME::HUBU,Vector2(40, -400), ENEMY_TYPE::D_S, ENEMY_MODE::RIGHT, ENEMY_DROP::RANDOM,5,
			62,ENEMY_NAME::HUBU,Vector2(-230, -400), ENEMY_TYPE::D_S, ENEMY_MODE::LEFT, ENEMY_DROP::RANDOM,5,
			62,ENEMY_NAME::HUBU,Vector2(30, -400), ENEMY_TYPE::D_S, ENEMY_MODE::RIGHT, ENEMY_DROP::RANDOM,5,
			64,ENEMY_NAME::HUBU,Vector2(-220, -400), ENEMY_TYPE::D_S, ENEMY_MODE::LEFT, ENEMY_DROP::RANDOM,5,
			64,ENEMY_NAME::HUBU,Vector2(20, -400), ENEMY_TYPE::D_S, ENEMY_MODE::RIGHT, ENEMY_DROP::RANDOM,5,
			66,ENEMY_NAME::HUBU,Vector2(-210, -400), ENEMY_TYPE::D_S, ENEMY_MODE::LEFT, ENEMY_DROP::RANDOM,5,
			66,ENEMY_NAME::HUBU,Vector2(10, -400), ENEMY_TYPE::D_S, ENEMY_MODE::RIGHT, ENEMY_DROP::RANDOM,5,
			68,ENEMY_NAME::HUBU,Vector2(-200, -400), ENEMY_TYPE::D_S, ENEMY_MODE::LEFT, ENEMY_DROP::RANDOM,5,
			68,ENEMY_NAME::HUBU,Vector2(0, -400), ENEMY_TYPE::D_S, ENEMY_MODE::RIGHT, ENEMY_DROP::RANDOM,5,
			70,ENEMY_NAME::HUBU,Vector2(-190, -400), ENEMY_TYPE::D_S, ENEMY_MODE::LEFT, ENEMY_DROP::RANDOM,5,
			70,ENEMY_NAME::HUBU,Vector2(-10, -400), ENEMY_TYPE::D_S, ENEMY_MODE::RIGHT, ENEMY_DROP::RANDOM,5,
			72,ENEMY_NAME::HUBU,Vector2(-180, -400), ENEMY_TYPE::D_S, ENEMY_MODE::LEFT, ENEMY_DROP::RANDOM,5,
			72,ENEMY_NAME::HUBU,Vector2(-20, -400), ENEMY_TYPE::D_S, ENEMY_MODE::RIGHT, ENEMY_DROP::RANDOM,5,
			74,ENEMY_NAME::HUBU,Vector2(-170, -400), ENEMY_TYPE::D_S, ENEMY_MODE::LEFT, ENEMY_DROP::RANDOM,5,
			74,ENEMY_NAME::HUBU,Vector2(-30, -400), ENEMY_TYPE::D_S, ENEMY_MODE::RIGHT, ENEMY_DROP::RANDOM,5,
			76,ENEMY_NAME::HUBU,Vector2(-160, -400), ENEMY_TYPE::D_S, ENEMY_MODE::LEFT, ENEMY_DROP::RANDOM,5,
			76,ENEMY_NAME::HUBU,Vector2(-40, -400), ENEMY_TYPE::D_S, ENEMY_MODE::RIGHT, ENEMY_DROP::RANDOM,5,
			78,ENEMY_NAME::HUBU,Vector2(-150, -400), ENEMY_TYPE::D_S, ENEMY_MODE::LEFT, ENEMY_DROP::RANDOM,5,
			78,ENEMY_NAME::HUBU,Vector2(-50, -400), ENEMY_TYPE::D_S, ENEMY_MODE::RIGHT, ENEMY_DROP::RANDOM,5,
			90,ENEMY_NAME::SISI,Vector2(-330, -400), ENEMY_TYPE::D_ST_A_ST_S_U, ENEMY_MODE::LEFT, ENEMY_DROP::ATTACK,5,
			94,ENEMY_NAME::SISI,Vector2(0, -400), ENEMY_TYPE::D_ST_S_U, ENEMY_MODE::RIGHT, ENEMY_DROP::ATTACK,5,
			98,ENEMY_NAME::SISI,Vector2(-200, -400), ENEMY_TYPE::D_ST_S_U, ENEMY_MODE::LEFT, ENEMY_DROP::ATTACK,5,
			102,ENEMY_NAME::SISI,Vector2(110, -400), ENEMY_TYPE::D_ST_A_ST_S_U, ENEMY_MODE::RIGHT, ENEMY_DROP::ATTACK,5,
			106,ENEMY_NAME::SISI,Vector2(-380, -400), ENEMY_TYPE::D_ST_S_U, ENEMY_MODE::LEFT, ENEMY_DROP::ATTACK,5,
			110,ENEMY_NAME::SISI,Vector2(150, -400), ENEMY_TYPE::D_ST_S_U, ENEMY_MODE::RIGHT, ENEMY_DROP::ATTACK,5,
			114,ENEMY_NAME::SISI,Vector2(-200, -400), ENEMY_TYPE::D_ST_A_ST_S_U, ENEMY_MODE::LEFT, ENEMY_DROP::ATTACK,5,
			116,ENEMY_NAME::SISI,Vector2(170, -400), ENEMY_TYPE::D_ST_S_U, ENEMY_MODE::RIGHT, ENEMY_DROP::ATTACK,5,
			118,ENEMY_NAME::SISI,Vector2(-370, -400), ENEMY_TYPE::D_ST_S_U, ENEMY_MODE::LEFT, ENEMY_DROP::ATTACK,5,
			120,ENEMY_NAME::SISI,Vector2(0, -400), ENEMY_TYPE::D_ST_A_ST_S_U, ENEMY_MODE::RIGHT, ENEMY_DROP::ATTACK,5,
			122,ENEMY_NAME::SISI,Vector2(-250, -400), ENEMY_TYPE::D_ST_A_ST_S_U, ENEMY_MODE::LEFT, ENEMY_DROP::ATTACK,5,
			122,ENEMY_NAME::SISI,Vector2(100, -400), ENEMY_TYPE::D_ST_S_U, ENEMY_MODE::RIGHT, ENEMY_DROP::ATTACK,5,
			124,ENEMY_NAME::SISI,Vector2(-300, -400), ENEMY_TYPE::D_ST_A_ST_S_U, ENEMY_MODE::LEFT, ENEMY_DROP::ATTACK,5,
			124,ENEMY_NAME::SISI,Vector2(50, -400), ENEMY_TYPE::D_ST_S_U, ENEMY_MODE::RIGHT, ENEMY_DROP::ATTACK,5
		}
	};
	int SpawnDataCount = sizeof(SpawnTable[0]) / sizeof(SpawnEnemyData);
	int StepDataCount = sizeof(SpawnTable) / (sizeof(SpawnEnemyData) * SpawnDataCount);
	int NowStep = 0;
	int NowSpawn = 0;

	//�A�C�e���Ǘ��p
	VTN_Item Items[(int)ENEMY_DROP::NOTHING];
	const float ITEM_SIZE = 0.1f;

	//�o�ߎ��ԊǗ��p
	int Timer = 0;
	int TimeCount = 0;
	bool isSpawn = false;

	//��b�p�[�g�p
	bool isTalkStart = false;
	bool isTalkEnd = false;
	VTN_Image TalkWindow;
	float TalkWindowSize = 0.0f;
	TextManager text;
	char TextList[2][10][256] =
	{
		{
			"����`���̏�ɒu���Ă����A���ɂ��肪�Ȃ��ȁ[",
			"�j���j��",
			"������[���ɂ���m��Ȃ��H�H",
			"���ɂ���Ȃ炱���ɂ����`*�����Ȃ��|���|����",
			"���ɂ���̍��݂��v���m��I",
			"~"
		},
		{
			"������A�܂ā`",
			"���[�B�������ȁ`*���ɂ��肮�炢���������",
			"���ɂ��肮�炢���Č������ȁ`*���������Ȃ����I",
			"~"
		}
	};
	VTN_Image PlayerTalk[1];
	int PlayerTalkNum = 0;
	VTN_Image EnemyTalk[1];
	int EnemyTalkNum = 0;
	int TalkWaitTime = 0;
	const int MaxTalkWaitTime = 200;

	//�e�L�X�g�\���p
	int EnterCount;
	int FramerateCount = 0;
	int TextOutPutCount = 0;
	const int TextSpeed = 10;
	int TextLineCount = 0;//������ƃf�o�b�O����start�̈ʒu���ς���
	bool isAllOutPut = false;
	char OutPutChar[3][256] = { "","","" };
	int  OutPutCharNum = 0;
	int StartNum = 0;
	bool isTextEnd = false;

	//�Q�[���I�[�o�[����p
	bool isGameOver = false;
	VTN_Image GameOver;

	//�Q�[���N���A����p
	bool isGameClear = false;
	VTN_Image GameClear;

	//�Q�[���I�[�o�[�N���A����
	char TototalScore[256] = "";
	VTN_Image Cursol;
	int cursolNum = 0;

	//���Ɋւ���
	bool isSoundStart = false;

	/// <summary>
	/// ���Ԍo�߃g���K�[�̃C�x���g�Ǘ�
	/// </summary>
	void TimeControl();

	/// <summary>
	/// �w�i�̈ړ�
	/// </summary>
	void BackGroundMove();

	/// <summary>
	/// �G�o���̊Ǘ�
	/// </summary>
	void SpawnControl();

	/// <summary>
	/// �G�l�~�[���|����Ă邩���m�F
	/// </summary>
	/// <returns></returns>
	bool CheckEnemyClear();

	/// <summary>
	/// ��b�p�[�g�Ɋւ��鏈��
	/// </summary>
	void TalkPart();

	/// <summary>
	/// �e�L�X�g�֘A
	/// </summary>
	void TextManagement();

	/// <summary>
	/// �e�L�X�g��i�߂�p
	/// </summary>
	void CheckEnter();

	/// <summary>
	/// �Q�[���I�[�o�[�ɂȂ��Ă���̏���
	/// </summary>
	void CheckGameOverAfter();

	/// <summary>
	/// �Q�[���N���A�[�ɂȂ��Ă���̏���
	/// </summary>
	void CheckGameClearAfter();
public:
	//�I�[�o�[���[�h
	Miosha(ID3D12Device* Dev, ID3D12GraphicsCommandList* CmdList) :BaseScene(Dev, CmdList) {}
	void Init();
	void Draw();
	bool Update();
	void onDestroy();
};

