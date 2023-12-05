#pragma once
#include "Object3D.h"
#include "CharacterHP.h"
#include "EnemyState.h"
#include "EnemyBullet.h"

class IModel;
class Player;

class Boss :public Object3D
{
public:
	void StatusInitialize();
	void Initialize(IModel* model);
	void Update();
	void ImGuiUpdate();
	void DrawBullets();
	void DrawUI();
	void CollisionUpdate();
	float RotationUpdate();
	void OnCollision(CollisionInfo& info) override;

private:
	CharacterHP hp_;

	std::unique_ptr<EnemyState> currentState_;
	std::list<std::unique_ptr<EnemyBullet>> bullets_;

	bool hide_ = false;
	bool bodyAt_ = false;

	int32_t maxHP_ = 100;

	Player* player_ = nullptr;
	Vector3D frontVec_ = { 0.0f,0.0f,-1.0f };
	
public:
	void AddBullet(std::unique_ptr<EnemyBullet>& bullet);
	void DecHP(int32_t damage);

#pragma region Getter

	bool GetIsAlive();
	bool GetIsHide();
	bool GetBodyAttack();
	Vector3D GetRot() { return mat_.angle_; }
	Vector3D GetPosition() { return mat_.trans_; }
	Vector3D GetFrontVec() { return frontVec_; }
	Player* GetPlayer() { return player_; }

#pragma endregion

#pragma region Setter

	void SetRot(const Vector3D& rot) { mat_.angle_ = rot; }
	void SetPosition(const Vector3D& pos) { mat_.trans_ = pos; }
	void SetCurrentState(std::unique_ptr<EnemyState>& next);
	void SetIsHide(bool isHide);
	void SetPlayer(Player* player);
	void SetHPBarSprite(const Sprite& sprite);
	void SetBodyAttack(bool attackFlag);

#pragma endregion
};

