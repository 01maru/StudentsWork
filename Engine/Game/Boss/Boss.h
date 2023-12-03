#pragma once
#include "Object3D.h"
#include "CharacterHP.h"
#include "EnemyState.h"

#include "Quaternion.h"

class IModel;
class Player;

class Boss :public Object3D
{
private:
	CharacterHP hp_;

	std::unique_ptr<EnemyState> currentState_;

	bool hide_ = false;

	int32_t maxHP_ = 100;

	Player* player_ = nullptr;
	Vector3D frontVec_ = { 0.0f,0.0f,-1.0f };
	Quaternion q;
public:
	void StatusInitialize();
	void Initialize(IModel* model);
	void Update();
	void ImGuiUpdate();
	void DrawUI();
	void CollisionUpdate();

	void GetDamage(int32_t damage);
	bool GetIsAlive();
	bool GetIsHide();

	void SetCurrentState(std::unique_ptr<EnemyState>& next);
	void SetIsHide(bool isHide);
	void SetPlayer(Player* player);
	void SetHPBarSprite(const Sprite& sprite);
};

