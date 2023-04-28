#include "Object.h"
#include "Scene.h"

void STG_EnemyData::Damage(int val)
{
	Hp -= val;
	if (Hp <= 0)
	{
		MyEnemy->GetFx()[1].SetFx(GetUnitPos());
	}
}

STG_Enemy::STG_Enemy()
{
	EnemyConstBuffer = NULL;
	for (int i = 0; i < INSTANCE_COUNT_MAX; i++)
	{
		Data[i].SetHp(0);
	}
}

LRESULT STG_Enemy::Init(ID3D12Device* dev, const wchar_t* fileName, Unit_Side mySide, VTN_Item* item, bool add)
{

	GroupSide = Unit_Side::ENEMY;

	Dev = dev;
	LRESULT result = LoadTex(dev, fileName);
	InstanceCount = COUNT;
	Init_Vertex();
	Init_Blend(add);
	Init_Sampler();

	result = LoadShader(dev, L"Lib\\Shader\\EnemyPS.hlsl", L"Lib\\Shader\\EnemyVS.hlsl");

	InitConstBuffer<EnemyShaderData>(&m_ConstantBufferData, &EnemyConstBuffer);
	for (int h = 0; h < INSTANCE_COUNT_MAX; h++) {
		EnemyConstBuffer->Unit[h].mat = XMMatrixIdentity();
	}

	EnemyConstBuffer->uv = XMFLOAT2(0, 0);
	EnemyConstBuffer->color = Color(1, 1, 1, 1);

	InitFlag = true;

	Scale = XMFLOAT2(1.0f, 1.0f);
	Size = XMFLOAT2(1.0f, 1.0f);

	Item = item;
	
	Bullet = new STG_Bullet* [(int)BulletType::MAX]{ new STG_Bullet_Straight() };
	BulletTypeCount = (int)BulletType::MAX;
	Fx = new STG_Fx[(int)FxType::MAX]{ STG_Fx(),STG_Fx() };
	FxTypeCount = (int)FxType::MAX;
	GroupSide = mySide;
	InstanceCount = GetCount();
	
	//弾関係
	Bullet[(int)BulletType::STRAIGHT]->Init(dev, L"img\\Bullet\\Bullet.png");
	Bullet[(int)BulletType::STRAIGHT]->SetScale(Vector2(BULLET_SIZE, BULLET_SIZE));
	//エフェクト関係
	Fx[(int)FxType::PLAYER_HIT].Init(dev, L"img\\Fx\\Frash.png", PLAYER_HIT_FX_SIZE, false);
	Fx[(int)FxType::ENEMY_HIT].Init(dev, L"img\\Fx\\Expload.png", ENEMY_HIT_FX_SIZE, false);

	return result;
}

void STG_Enemy::Update(STG_Unit** unit)
{
	//スポーンしているエネミー一体一体の処理
	int enemyCount = GetCount();
	maxCount = 0;
	for (int i = 0; i < enemyCount; i++) {
		STG_EnemyData* enemy = GetData(i);
		if (enemy->GetHp() <= 0) {
			continue;
		}

		//被弾判定の処理
		int damage = enemy->CheckBulletHit(unit);
		if (damage != 0)
		{
			SoundManager::Play(SOUNDMANAGER::EnemyHit);
			enemy->Damage(damage);
			if (enemy->GetHp() <= 0 && enemy->GetMyDrop() != ENEMY_DROP::NOTHING)
			{
				if (enemy->GetMyDrop() == ENEMY_DROP::RANDOM)
				{
					int RandomNum = rand() % AITEM_DROP_PARCENT;
					switch (RandomNum)
					{
					case 0:
						break;
					case 1:
						Item[(int)ENEMY_DROP::POINT].Drop(enemy->GetUnitPos());
						break;
					case 2:
						Item[(int)ENEMY_DROP::ATTACK].Drop(enemy->GetUnitPos());
						break;
					}
				}
				else
				{
					Item[(int)enemy->GetMyDrop()].Drop(enemy->GetUnitPos());
				}
			}
			if (enemy->GetHp() <= 0) {
				ScoreManager::AddScore(1000);
				Fx[(int)FxType::ENEMY_HIT].SetFx(enemy->GetUnitPos());
				continue;
			}
		}

		//生きてるキャラクターのAI
		Data[i].SetTimer(Data[i].GetTimer() + FrameRateManager::getDeltaTime());
		MoveEnemy(&Data[i]);
		maxCount = i + 1;
	}

	for (int i = 0; i < BulletTypeCount; i++)
	{
		Bullet[i]->Update();
	}
}

void STG_Enemy::SpawnEnemy(SpawnEnemyData spawnData, int hp, float speed)
{
	for (int i = 0; i < INSTANCE_COUNT_MAX; i++)
	{
		if (Data[i].GetHp() <= 0)
		{
			//ユニットのデータ
			Data[i].SetHp(spawnData.hp);
			Data[i].SetPosAddress(new Vector2(spawnData.pos.x, spawnData.pos.y));
			Data[i].SetSizeAddress(GetSize_Address());
			Data[i].SetRadius(0);
			Data[i].SetMySide(GetGroupSide());
			Data[i].SetSpeed(speed);
			Data[i].SetIsDrop(false);
			//エネミー専用のデータ
			Data[i].SetMyType(spawnData.type);
			Data[i].SetMyMode(spawnData.mode);
			Data[i].SetMyDrop(spawnData.drop);
			Data[i].SetTimer(0);
			Data[i].SetIsShoot(false);
			Data[i].SetIsGraze(false);
			Data[i].SetMyEnemy(this);
			maxCount++;
			return;
		}
	}
}

void STG_Enemy::Draw(ID3D12GraphicsCommandList* cmdList)
{
	int id = 0;
	for (int i = 0; i < INSTANCE_COUNT_MAX; i++)
	{
		if (Data[i].GetHp() > 0)
		{
			XMMATRIX* mat = &EnemyConstBuffer->Unit[id].mat;
			*mat = XMMatrixIdentity();
			*mat = XMMatrixMultiply(*mat, XMMatrixRotationZ(0));
			*mat = XMMatrixMultiply(*mat, XMMatrixScaling(2.0f / WINDOW_WIDTH * 0.5f * Scale.x, -2.0f / WINDOW_HEIGHT * 0.5f * Scale.y, 0.1f));

			mat->r[3].m128_f32[0] = Data[i].GetUnitPos().x / WINDOW_WIDTH * 2.0f;
			mat->r[3].m128_f32[1] = Data[i].GetUnitPos().y / WINDOW_HEIGHT * -2.0f;
			/*EnemyConstBuffer->Unit[id].rate = 1.0f - ((float)Data[i].GetHP() / 10.0f);*/
			id++;
		}
	}

	if (id > 0)
	{
		cmdList->SetPipelineState(_pipelinestate);
		cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		cmdList->IASetVertexBuffers(0, 1, &vbView);
		cmdList->IASetIndexBuffer(&ibView);
		cmdList->SetGraphicsRootSignature(rootsignature);
		cmdList->SetDescriptorHeaps(1, &basicDescHeap);

		cmdList->SetGraphicsRootDescriptorTable(0, basicDescHeap->GetGPUDescriptorHandleForHeapStart());

		cmdList->DrawIndexedInstanced(6, id, 0, 0, 0);
	}

	for (int i = 0; i < BulletTypeCount; i++)
	{
		Bullet[i]->Draw(cmdList);
	}
	for (int i = 0; i < FxTypeCount; i++)
	{
		Fx[i].Draw(cmdList);
	}
}

void STG_Enemy::UnitDestroy()
{
	VTN_Image::Destroy();
}

void STG_Enemy::MoveEnemy(STG_EnemyData* Data)
{
	int Direction = 0;
	if (Data->GetMyMode() == ENEMY_MODE::RIGHT)
	{
		Direction = 1;
	}
	else if (Data->GetMyMode() == ENEMY_MODE::LEFT)
	{
		Direction = -1;
	}

	int FirstTime = 0;
	int SecondTime = 0;
	int ThirdTime = 0;
	int FourthTime = 0;
	int FifthTime = 0;
	switch (Data->GetMyType())
	{
	case ENEMY_TYPE::D:
		MoveDown(Data);
		break;

	case ENEMY_TYPE::D_S_D:
		FirstTime = 1200;
		SecondTime = 1000 + FirstTime;
		if (Data->GetTimer() >= 0&& Data->GetTimer() <= FirstTime)
		{	
			MoveDown(Data);
		}
		else if (Data->GetTimer() > FirstTime && Data->GetTimer() <= SecondTime)
		{	
			MoveSide(Data, Direction);
		}
		else if (Data->GetTimer() > SecondTime)
		{
			MoveDown(Data);
		}
		break;

	case ENEMY_TYPE::D_S:
		FirstTime = 2000;
		if (Data->GetTimer() >= 0 && Data->GetTimer() <= FirstTime)
		{
			MoveDown(Data);
		}
		else if (Data->GetTimer() > FirstTime)
		{
			MoveSide(Data, Direction);
		}
		break;

	case ENEMY_TYPE::D_ST_A_ST_S_U:
		FirstTime = 1000;
		SecondTime = 100 + FirstTime;
		ThirdTime = 32 + SecondTime;
		FourthTime = 1000 + ThirdTime;
		FifthTime = 500 + FourthTime;
		if (Data->GetTimer() >= 0 && Data->GetTimer() <= FirstTime)
		{
			MoveDown(Data);
		}
		else if (Data->GetTimer() > FirstTime && Data->GetTimer() <= SecondTime)
		{
			Stay(Data);
		}
		else if (Data->GetTimer() > SecondTime && Data->GetTimer() <= ThirdTime)
		{
			if (!Data->GetIsShoot())
			{
				Data->SetIsShoot(true);
				Bullet[(int)BulletType::STRAIGHT]->SetBullet(Data->GetUnitPos(), Vector2(0.5f * Direction, 2.5f),NULL,500);
				Bullet[(int)BulletType::STRAIGHT]->SetBullet(Data->GetUnitPos(), Vector2(1.5f * -Direction, 2), NULL,500);
				Bullet[(int)BulletType::STRAIGHT]->SetBullet(Data->GetUnitPos(), Vector2(2.5f * -Direction, 0.5f), NULL,500);
			}
		}
		else if (Data->GetTimer() > ThirdTime && Data->GetTimer() <= FourthTime)
		{
			Data->SetIsShoot(false);
			Stay(Data);
		}
		else if (Data->GetTimer() > FourthTime && Data->GetTimer() <= FifthTime)
		{
			MoveSide(Data,Direction);
		}
		else if (Data->GetTimer() > FifthTime)
		{
			MoveUp(Data);
		}
		break;

	case ENEMY_TYPE::D_ST_S_U:
		FirstTime = 1000;
		SecondTime = 1130 + FirstTime;
		ThirdTime = 100 + SecondTime;
		if (Data->GetTimer() >= 0 && Data->GetTimer() <= FirstTime)
		{
			MoveDown(Data);
		}
		else if (Data->GetTimer() > FirstTime && Data->GetTimer() <= SecondTime)
		{
			Stay(Data);
		}
		else if (Data->GetTimer() > SecondTime && Data->GetTimer() <= ThirdTime)
		{
			MoveSide(Data, Direction);
		}
		else if (Data->GetTimer() > ThirdTime)
		{
			MoveUp(Data);
		}
		break;
	}
}

void STG_Enemy::MoveDown(STG_EnemyData* Data)
{
	Data->SetUnitPos(Vector2(Data->GetUnitPos().x, Data->GetUnitPos().y + FrameRateManager::getDeltaTime() * Data->GetSpeed()));
	if (Data->GetUnitPos().y < ENEMY_MOVE_AREA_DOWN || Data->GetUnitPos().y > ENEMY_MOVE_AREA_UP)
	{
		Data->SetHp(0);
	}
}

void STG_Enemy::MoveUp(STG_EnemyData* Data)
{
	Data->SetUnitPos(Vector2(Data->GetUnitPos().x, Data->GetUnitPos().y - FrameRateManager::getDeltaTime() * Data->GetSpeed()));
	if (Data->GetUnitPos().y < ENEMY_MOVE_AREA_DOWN || Data->GetUnitPos().y > ENEMY_MOVE_AREA_UP)
	{
		Data->SetHp(0);
	}
}

void STG_Enemy::MoveSide(STG_EnemyData* Data, int Direction)
{
	Data->SetUnitPos(Vector2(Data->GetUnitPos().x + FrameRateManager::getDeltaTime() * Data->GetSpeed() * Direction, Data->GetUnitPos().y));
	if (Data->GetUnitPos().x < ENEMY_MOVE_AREA_LEFT || Data->GetUnitPos().x > ENEMY_MOVE_AREA_RIGHT)
	{
		Data->SetHp(0);
	}
}

void STG_Enemy::Stay(STG_EnemyData* Data){}

bool STG_Enemy::CheckEnemyClear()
{
	if (maxCount == 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}


