#include "Enemy.h"
#include "InputManager.h"

void Enemy::Initialize(IModel* model)
{
	Object3D::Initialize();
	SetModel(model);
}

void Enemy::Update()
{
	InputKeyboard* keyboard = InputManager::GetInstance()->GetKeyboard();

	mat_.trans_ += Vector3D(keyboard->GetKey(DIK_RIGHT) - keyboard->GetKey(DIK_LEFT),
							keyboard->GetKey(DIK_UP) - keyboard->GetKey(DIK_DOWN),
							keyboard->GetKey(DIK_N) - keyboard->GetKey(DIK_M)) * 0.05f;
}
