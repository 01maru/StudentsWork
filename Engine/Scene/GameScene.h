#pragma once
#include "IScene.h"
#include "Object3D.h"

#include "Player.h"
#include "PlayerBulletManager.h"
#include "Boss.h"

#include "PauseScreen.h"

#include "IGameState.h"
#include "GameOverUI.h"

#include "EscapePod.h"
#include "UIData.h"
#include <array>

class GameScene :public MNE::IScene
{
public:
	//	ステート
	enum GameSceneState
	{
		PauseState = 0,		//	ポーズ
		StartState,			//	開始時演出
		PlayState,			//	プレイ中
		EndState,			//	終了時演出
		TotalStates,		//	ステートの合計数
	};

private:
	typedef void (GameScene::* state)();
	//	関数ポインタテーブル
	std::array<state, TotalStates> stateTable_;
	//	現在のステート
	int32_t nowState_ = StartState;

public:
	//	デストラクタ
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

private:
	std::vector<std::unique_ptr<MNE::Object3D>> objs_;

	PauseScreen pause_;
	GameCamera* camera_ = nullptr;
	//	ムービー中の黒帯
	MNE::UIData letterBox_;

	std::unique_ptr<IGameState> clear_;
	std::unique_ptr<GameOverUI> gameOver_;

	//	脱出ポッドから出たらTRUE
	bool drawPlayer_ = FALSE;

#pragma region Obj3D

	std::unique_ptr<MNE::Object3D> skydome_;
	std::unique_ptr<MNE::Object3D> ground_;
	PlayerBulletManager playerBullets_;
	std::unique_ptr<Player> player_;
	std::unique_ptr<Boss> enemy_;
	EscapePod pod_;

#pragma endregion

private:
	void PauseScene();
	void StartScene();
	void PlayScene();
	void EndScene();
	void PlayGameUpdate();

private:	//	関数
	void MatUpdate() override;
	void CollisionUpdate();

public:
	int32_t GetNowState();
	void SetNextState(int32_t nextState);
	void SetDrawPlayer(bool drawPlayer);
	void ActiveGameOver();
	void ActiveClearState();
};

