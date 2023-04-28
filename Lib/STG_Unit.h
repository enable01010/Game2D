#pragma once
#include "stdafx.h"

class STG_Unit_Single;
class STG_Unit;

enum class Unit_Side
{
	PlAYER,
	ENEMY,
	MAX
};

class STG_Status
{
protected:
	int Hp;
	Vector2* PosAddress;
	Vector2* SizeAddress;
	int Radius;
	Unit_Side MySide;
	float Speed;
	bool IsDrop = false;

	/// <summary>
	/// 当たり判定の計算
	/// </summary>
	/// <param name="pos">玉のPos</param>
	/// <param name="magnitude">玉の画像サイズ</param>
	/// <returns></returns>
	bool CheckHit(Vector2 pos, int magnitude, float size);
	
public:

	//ゲッターセッター
	void SetHp(int hp) { Hp = hp; }
	int GetHp() { return Hp; }
	int* GetHpAddress() { return &Hp; }
	virtual void Damage(int val) { Hp -= val; }
	void SetPosAddress(Vector2* address) { PosAddress = address; }
	void SetUnitPos(Vector2 pos) { PosAddress->x = pos.x; PosAddress->y = pos.y; }
	Vector2* GetPosAddress() {return PosAddress;}
	Vector2 GetUnitPos() { return Vector2(PosAddress->x, PosAddress->y); }
	void SetSizeAddress(Vector2* address) { SizeAddress = address; }
	void SetUnitSize(Vector2 size) { SizeAddress->x = size.x, SizeAddress->y = size.y; }
	Vector2* GetSizeAddress() { return SizeAddress; }
	Vector2 GetUnitSize() { return Vector2(SizeAddress->x, SizeAddress->y); }
	void SetRadius(int num) { Radius = num; }
	int GetRadius() { return Radius; }
	void SetMySide(Unit_Side mySide) { MySide = mySide; }
	Unit_Side GetMySide() { return MySide; }
	void SetSpeed(float speed) { Speed = speed; }
	float GetSpeed() { return Speed; }
	void SetIsDrop(bool isDrop) { IsDrop = isDrop; }
	bool GetIsDrop() { return IsDrop; }

	/// <summary>
	/// 弾の攻撃を受けたか判定
	/// </summary>
	/// <param name="unit">シーン全てのUnit</param>
	/// <returns></returns>
	int CheckBulletHit(STG_Unit** unit);

	/// <summary>
	/// 直接ユニットに触れたか判定
	/// </summary>
	/// <param name="unit"></param>
	/// <returns></returns>
	int CheckUnitHit(STG_Unit** unit);
};

class STG_Unit
{
protected:
	STG_Bullet** Bullet;
	int BulletTypeCount;
	STG_Fx* Fx;
	int FxTypeCount;
	Unit_Side GroupSide;
	int InstanceCount;
	static int UnitTypeCount;

public:
	//ゲッターセッター
	STG_Bullet** GetBullet() { return Bullet; }
	void SetBulletTypeCount(int num) { BulletTypeCount = num; }
	int GetBulletTypeCount() { return BulletTypeCount; }
	STG_Fx* GetFx() { return Fx; }
	void SetFxTypeCount(int num) { FxTypeCount = num; }
	int GetFxTypeCount() { return FxTypeCount; }
	Unit_Side GetGroupSide() { return GroupSide; }
	void SetInstanceCount(int num) { InstanceCount = num; }
	int GetInstanceCount() { return InstanceCount; }
	static void SetUnitTypeCount(int num) { UnitTypeCount = num; }
	static int GetUnitTypeCount() { return UnitTypeCount; }

	//オーバーライド
	virtual LRESULT Init(ID3D12Device* dev, const wchar_t* = NULL, Unit_Side mySide = Unit_Side::ENEMY, VTN_Item* item = NULL, bool add = false) = 0;
	virtual void Draw(ID3D12GraphicsCommandList* cmdList) = 0;
	virtual void DrawStatus(ID3D12GraphicsCommandList* cmdList) = 0;
	virtual void Update(STG_Unit** unit) = 0;
	virtual void UnitDestroy() = 0;
	virtual STG_Status* GetUnitStatus(int num = 0) = 0;

	void Destroy();
	
};

class STG_Unit_Single:public STG_Status,public STG_Unit,public VTN_Image
{
	
};

