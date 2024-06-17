#pragma once
#include "../Library/sceneBase.h"

class PlayScene : public SceneBase {
public:
	PlayScene();
	~PlayScene();
	void Update() override;
	void Draw() override;
private:
};