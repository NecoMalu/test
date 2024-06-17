#include "bootScene.h"
#include "../Library/sceneManager.h"
#include "DebugScreen.h"

BootScene::BootScene()
{
	SceneBase* common = SceneManager::CommonScene();
	DebugScreen* ds = common->CreateGameObject<DebugScreen>();
	common->SetDrawOrder(ds, 10000);
}

BootScene::~BootScene()
{
}

void BootScene::Update()
{
	//SceneManager::ChangeScene("TitleScene"); // 起動が終わったらTitleを表示
	SceneManager::ChangeScene("PlayScene"); // 起動が終わったらPlayを表示
}

void BootScene::Draw()
{
}
