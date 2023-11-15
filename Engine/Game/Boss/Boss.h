#pragma once
#include "Object3D.h"
#include "CharacterHP.h"
#include "EnemyState.h"

class IModel;

class Boss :public Object3D
{
private:
	CharacterHP hp_;

	std::unique_ptr<EnemyState> currentState_;

	bool hide_ = false;

	int32_t maxHP_ = 100;

public:
	void StatusInitialize();
	void Initialize(IModel* model);
	void Update();
	void DrawUI();
	void CollisionUpdate();

	void GetDamage(int32_t damage);
	bool GetIsAlive();
	bool GetIsHide();

	void SetCurrentState(std::unique_ptr<EnemyState>& next);
	void SetIsHide(bool isHide);
};
