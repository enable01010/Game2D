#include "Scene.h"


void ShootingScene::Init() {
	player.Init(dev, L"img\\shooting\\Player.png");
	Enemy.Init(dev, L"img\\shooting\\Enemy.png");
	for (int i = 0; i < 13; i++) {
			//Enemy.SpawnEnemy(Vector2(-600 + 100 * i,  -200) ,10);
	}
	
	/*EnemyBullet.Init(dev, L"img\\shooting\\Bullet.png");*/
	/*Boss.Init(dev, L"img\\shooting\\Boss.png", &EnemyBullet);*/
	/*Boss.ExploadFx.Init(dev, L"img\\shooting\\Expload.png", 1.0f, false);*/
	BulletFx.Init(dev, L"img\\shooting\\Expload.png", 1.0f, false);
}
void ShootingScene::Draw() {

	player.Draw(cmdList);
	Enemy.Draw(cmdList);
	/*EnemyBullet.Draw(cmdList);*/
	BulletFx.Draw(cmdList);
	//Boss.Draw(cmdList);

}
bool ShootingScene::Update() {
	//if (InputManager::GetPush(INPUT_TYPE::a)) {
	////	Enemy.SpawnEnemy(Vector2(0, -200),10);
	////}

	///*player.Update();*/
	///*Enemy.Update(&player);*/
	///*Boss.Update();*/
	///*EnemyBullet.Update();*/
	///*Enemy.Move(player.GetPos());*/

	//

	//int enemyCount = Enemy.GetCount();
	//int playerBulletCount = player.getBullet()->GetCount();
	//int enemyBulletCount = EnemyBullet.GetCount();

	//for (int i = 0; i < enemyCount; i++) {
	//	STG_EnemyData* enemy = Enemy.GetData(i);
	//	if (enemy->GetHP() <= 0) {
	//		continue;
	//	}
	//	for (int j = 0; j < playerBulletCount; j++) {
	//		STG_BulletData* bullet = player.getBullet()->GetData(j);

	//		if (bullet->Timer <= 0) {
	//			continue;
	//		}

	//		Vector2 Dir = enemy->Pos - bullet->Pos;
	//		float len = Dir.sqrtMagnitude();
	//		if(len < 100)
	//		{
	//			enemy->Damage(bullet->Attack);
	//			bullet->Timer = 0;
	//			BulletFx.SetFx(bullet->Pos);
	//		}
	//	}
	//}

	///*if (Boss.GetHP() > 0) {
	//	for (int i = 0; i < playerBulletCount; i++) {
	//		STG_BulletData* bullet = player.getBullet()->GetData(i);

	//		if (bullet->Timer <= 0) {
	//			continue;
	//		}

	//		if (Boss.GetPos().x - Boss.GetSize().x * 0.5f <= bullet->Pos.x &&
	//			Boss.GetPos().x + Boss.GetSize().x * 0.5f >= bullet->Pos.x &&
	//			Boss.GetPos().y - Boss.GetSize().y * 0.5f <= bullet->Pos.y &&
	//			Boss.GetPos().y + Boss.GetSize().y * 0.5f >= bullet->Pos.y)
	//		{
	//			Boss.Damage(bullet->Attack);
	//			printf("ƒNƒŠƒA");
	//			bullet->Timer = 0;
	//			BulletFx.SetFx(bullet->Pos);
	//		}
	//	}
	//}*/

	//if (player.GetHP() >= 0) {
	//	for (int i = 0; i < enemyBulletCount; i++) {
	//		STG_BulletData* bullet = EnemyBullet.GetData(i);


	//		if (bullet->Timer <= 0) {
	//			continue;
	//		}

	//		Vector2 Dir = player.GetPos() - bullet->Pos;
	//		float len = Dir.sqrtMagnitude();
	//		if (len < 100)
	//		{
	//			player.Damage(bullet->Attack);
	//			bullet->Timer = 0;
	//			BulletFx.SetFx(bullet->Pos);
	//		}

	//	}
	//}
	//
	return true;
}

void ShootingScene::onDestroy() {}