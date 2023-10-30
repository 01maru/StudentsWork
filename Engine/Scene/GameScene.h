#pragma once
#include "IScene.h"
#include "Object3D.h"
#include "Sprite.h"

#include "ICamera.h"
#include "Texture.h"
#include "Player.h"
#include "Enemy.h"

#include "JSONLoader.h"
#include "PauseScreen.h"

#include "FrameCounter.h"

class GameScene :public IScene
{
private:

	JSONLoader level;

	//std::unique_ptr<PauseScreen> pause_;

#pragma region Obj3D

	std::unique_ptr<Object3D> skydome_;
	std::unique_ptr<Object3D> ground_;
	std::unique_ptr<Object3D> cube_;
	std::unique_ptr<Player> player_;
	std::unique_ptr<Enemy> enemy_;

#pragma endregion

#pragma region Sprite

	std::unique_ptr<Sprite> sprite_;

#pragma endregion
	
#pragma region Texture
	Texture* reimuG;
	Texture* grassG;
#pragma endregion

private:	//	関数
	void MatUpdate() override;

public:	
	~GameScene() override {};
	void Initialize() override;
	void LoadResources() override;
	void Finalize() override;
	void FirstFrameUpdate() override;
	void Update() override;
	void ImguiUpdate() override;
	void DrawShadow() override;
	void Draw() override;
};

