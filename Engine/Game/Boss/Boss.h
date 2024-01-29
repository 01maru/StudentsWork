#pragma once
#include "Object3D.h"
#include "CharacterHP.h"
#include "EnemyState.h"
#include "EnemyBullet.h"

class IModel;
class Player;
class IGameState;

class Boss :public MNE::Object3D
{
public:
	void StatusInitialize();
	void Initialize(MNE::IModel* model);
	void Update();
	void ImGuiUpdate();
	void DrawBullets();
	void DrawUI();
	void CollisionUpdate();
	float RotationUpdate();
	void OnCollision(CollisionInfo& info) override;

private:
	bool isActive_ = true;

	CharacterHP hp_;

	std::unique_ptr<EnemyState> currentState_;
	std::list<std::unique_ptr<EnemyBullet>> bullets_;

	bool bodyAt_ = false;

	int32_t maxHP_ = 100;

	Player* player_ = nullptr;
	Vector3D frontVec_ = { 0.0f,0.0f,-1.0f };
	IGameState* pClearState_ = nullptr;
	
public:
	void AddBullet(std::unique_ptr<EnemyBullet>& bullet);
	void DecHP(int32_t damage);

	void StartClearState();

#pragma region Getter

	bool GetIsAlive();
	bool GetBodyAttack();
	Vector3D GetRot() { return mat_.angle_; }
	Vector3D GetPosition() { return mat_.trans_; }
	Vector3D* GetPositionPtr() { return &mat_.trans_; }
	Vector3D GetFrontVec() { return frontVec_; }
	Player* GetPlayer() { return player_; }

#pragma endregion

#pragma region Setter

	void SetIsActive(bool isActive) { isActive_ = isActive; }
	void SetRot(const Vector3D& rot) { mat_.angle_ = rot; }
	void SetPosition(const Vector3D& pos) { mat_.trans_ = pos; }
	void SetCurrentState(std::unique_ptr<EnemyState>& next);
	void SetPlayer(Player* player);
	void SetHPBarSprite(const MNE::Sprite& sprite);
	void SetBodyAttack(bool attackFlag);
	void SetClearState(IGameState* clearState);

#pragma endregion
};

