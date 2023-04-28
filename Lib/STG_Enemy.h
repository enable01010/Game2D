#pragma once
#include "stdafx.h"

class STG_Enemy;
class STG_Bullet;
class STG_Boss;
class STG_Player;
class STG_Fx;

enum class ENEMY_TYPE
{
	D,
	D_S_D,
	D_S,
	D_ST_A_ST_S_U,
	D_ST_S_U,
	Max
};

enum class ENEMY_MODE
{
	RIGHT,
	LEFT
};

enum class ENEMY_NAME
{
	HUBU,
	SISI,
	MAX
};

struct SpawnEnemyData
{
	int spawnTime;
	ENEMY_NAME name;
	Vector2 pos;
	ENEMY_TYPE type;
	ENEMY_MODE mode;
	ENEMY_DROP drop;
	int hp;
};


class STG_EnemyData: public STG_Status
{
protected:
	ENEMY_TYPE MyType;
	ENEMY_MODE MyMode;
	ENEMY_DROP MyDrop;
	int Timer;
	bool IsShoot = false;
	bool IsGraze = false;
	STG_Enemy* MyEnemy;
	
public:
	//セッターゲッター
	void SetMyType(ENEMY_TYPE myType) { MyType = myType; }
	ENEMY_TYPE GetMyType() { return MyType; }
	void SetMyMode(ENEMY_MODE myMode) { MyMode = myMode; }
	ENEMY_MODE GetMyMode() { return MyMode; }
	void SetMyDrop(ENEMY_DROP myDrop) { MyDrop = myDrop; }
	ENEMY_DROP GetMyDrop() { return MyDrop; }
	void SetTimer(int time) { Timer = time; }
	int GetTimer() { return Timer; }
	void SetIsShoot(bool isShoot) { IsShoot = isShoot; }
	bool GetIsShoot() { return IsShoot; }
	void SetIsGraze(bool isGraze) { IsGraze = isGraze; }
	bool GetIsGraze() { return IsGraze; }
	void SetMyEnemy(STG_Enemy* enemy) { MyEnemy = enemy; }
	STG_Enemy* GetMyEnemy() { return MyEnemy; }
	void Damage(int val);
};

struct EnemyUnitShaderData {
	XMMATRIX mat;
	float rate;
};

_declspec(align(256))
struct EnemyShaderData
{
	Color color;
	XMFLOAT2 uv;
	EnemyUnitShaderData Unit[800];
};

class STG_Enemy :public STG_Instance<STG_EnemyData>,public STG_Unit
{
	EnemyShaderData m_ConstantBufferData = EnemyShaderData();
	EnemyShaderData* EnemyConstBuffer;

	//攻撃関係
	enum class BulletType
	{
		STRAIGHT,
		MAX
	};
	const float BULLET_SIZE = 3.0f;

	//エフェクト
	enum class FxType
	{
		PLAYER_HIT,
		ENEMY_HIT,
		MAX
	};
	//Playerに玉が当たって消えるときのエフェクト
	const float PLAYER_HIT_FX_SIZE = 1.0f;
	const float ENEMY_HIT_FX_SIZE = 0.5f;

	//当たり判定用
	const int BULLET_HIT_AREA = 300;
	const int PLAYER_HIT_AREA = 300;
	const int PLAYER_GRAZE_AREA = 50;

	//アイテムのドロップ率調整
	const int AITEM_DROP_PARCENT = 10;
	VTN_Item* Item;
	
	//エネミーの移動関連
	const int ENEMY_MOVE_AREA_RIGHT = 230;
	const int ENEMY_MOVE_AREA_LEFT = -430;
	const int ENEMY_MOVE_AREA_UP = 360;
	const int ENEMY_MOVE_AREA_DOWN = -500;

	void MoveEnemy(STG_EnemyData* Data);
	void MoveSide(STG_EnemyData* Data, int Direction);
	void MoveDown(STG_EnemyData* Data);
	void MoveUp(STG_EnemyData* Data);
	void Stay(STG_EnemyData* Data);
	
	void HitBullet() {};
	void CheckBodyHit() {};

public:
	VTN_Item* getItem() { return Item; }

	//オーバーライド
	STG_Enemy();
	LRESULT Init(ID3D12Device* dev, const wchar_t* fileName, Unit_Side mySide = Unit_Side::ENEMY, VTN_Item* item = NULL ,bool add = false);
	void Update(STG_Unit** unit);
	void Draw(ID3D12GraphicsCommandList* cmdList);
	void DrawStatus(ID3D12GraphicsCommandList* cmdList) {}
	void UnitDestroy(); 
	STG_Status* GetUnitStatus(int num) { return &Data[num]; };

	/// <summary>
	/// 敵を作成する関数
	/// </summary>
	/// <param name="pos">作成場所</param>
	/// <param name="type">行動パターン</param>
	/// <param name="mode">右向きor左向き</param>
	/// <param name="drop">倒したときのアイテムドロップ</param>
	/// <param name="hp"></param>
	/// <param name="attack"></param>
	/// <param name="speed"></param>
	void SpawnEnemy(SpawnEnemyData spawnData,int hp = 1, float speed = 0.2f);

	/// <summary>
	/// エネミーが全員倒されているかチェック
	/// </summary>
	/// <returns></returns>
	bool CheckEnemyClear();
	
};