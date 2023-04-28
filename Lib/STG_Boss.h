#pragma once
#include"stdafx.h"


class STG_Boss:public STG_Unit_Single
{
protected:
	
	int BulletDelay = 0;
	bool isExpload = false;
	bool isDeath = false;
	int State = 0;
	int WaitTime = 0;
	int StateTime = 200;
	int MoveNo = 0;

	//�ʊ֌W
	enum class BulletType
	{
		YELLOW,
		BIG,
		RED,
		BULE,
		CIRCLE,
		DIVISION,
		WAIT_ROCKON,
		HOMING,
		MAX
	};
	const float BULLET_SIZE = 3.0f;
	const int PLAYER_HIT_AREA = 300;
	const int PLAYER_GRAZE_AREA = 50;

	//Player�ɋʂ��������ď�����Ƃ��̃G�t�F�N�g
	enum class FxType
	{
		PLAYER_HIT,
		EXPLOAD,
		MAX
	};
	const float PLAYER_HIT_FX_SIZE = 1.0f;

	//�r�[���֌W
	STG_ChargeShot Charge;

	//���Ԋ֌W
	TextManager text;
	int MicroTimer = 25000;
	char TimerChar[256] = {};

	//�̗̓o�[
	VTN_Image HpGaze;
	int MaxHP;
	bool isHpGazeFull = false;
	int FirstHpGazeSize = 0;
	const int FirstHpGazeSizeMax = 100;
	const float HPGAZE_SIZE_X = 0.18f;
	const float HPGAZE_SIZE_Y = 0.05f;

	//�o���E���j�Ǘ�
	bool isSpawn = false;
	bool isTimeOver = false;
	bool isClear = false;

	//AI�Ǘ��p
	bool isEntry = false;
	bool isExit = false;
	
	/// <summary>
	/// ���Ԍv��
	/// </summary>
	void TimerCount();

	/// <summary>
	/// �{�X�̍s��
	/// </summary>
	virtual void BossAI(STG_Unit** unit) = 0;


	/// <summary>
	/// ��莞�Ԍo�ߌ�̃{�X�̍s��
	/// </summary>
	/// <param name="unit"></param>
	virtual void TimeOutAi(STG_Unit** unit) = 0;

	/// <summary>
	/// HpGaze�𒲐�����
	/// </summary>
	void BossHpGaze();

	/// <summary>
	/// ��������
	/// </summary>
	void Expload();

	/// <summary>
	/// �X�e�[�g�����ɐi�߂�֐�
	/// �J�R�����@2022/10/07
	/// </summary>
	/// <param name="state">���̃X�e�[�g</param>
	/// <param name="nextTime">�E�F�C�g����</param>
	/// <returns>�����̌���true �����@false ���s</returns>
	bool NextState(int state, int nextTime);
	void WaitState(int nextState, int waitTime);

	void HitBullet() {};
	void CheckBodyHit() {};
public:
	bool getIsSpawn() { return isSpawn; }
	bool getIsClear() { return isClear; }

	//�I�[�o�[���C�h
	virtual LRESULT Init(ID3D12Device* dev, const wchar_t* fileName ,Unit_Side mySide = Unit_Side::ENEMY, VTN_Item* item = NULL, bool add = false);
	void Update(STG_Unit** unit);
	virtual void Draw(ID3D12GraphicsCommandList* cmdList);
	virtual void DrawStatus(ID3D12GraphicsCommandList* cmdList);
	void UnitDestroy();
	virtual void BossDestroy() = 0;
	STG_Status* GetUnitStatus(int num = 0) { return this; };
	

	void Damage(int val); 

	/// <summary>
	/// �o���J�n
	/// </summary>
	void Spawn();

	/// <summary>
/// �v���C���[����e�����Ƃ��ɌĂ΂��
/// ��ɏo�Ă���ʂ�����
/// </summary>
};

