#pragma once
#include "IScene.h"
#include "Object3D.h"

#include "Player.h"
#include "Boss.h"

#include "PauseScreen.h"

#include "IGameState.h"

#include "EscapePod.h"
#include "UIData.h"

class GameScene :public MNE::IScene
{
private:
	std::vector<std::unique_ptr<MNE::Object3D>> objs_;

	PauseScreen pause_;

	//	ムービー中の黒帯
	MNE::UIData letterBox_;

	std::unique_ptr<IGameState> clear_;
	std::unique_ptr<IGameState> gameOver_;

#pragma region Obj3D

	std::unique_ptr<MNE::Object3D> skydome_;
	std::unique_ptr<MNE::Object3D> ground_;
	std::unique_ptr<Player> player_;
	std::unique_ptr<Boss> enemy_;
	EscapePod pod_;

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
	void DrawUIBeforeBlackScreen() override;
	void DrawUIAfterBlackScreen() override;
	void Draw() override;
};

