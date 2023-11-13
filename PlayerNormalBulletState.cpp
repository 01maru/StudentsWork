#include "PlayerNormalBulletState.h"
#include "Player.h"

#include "ModelManager.h"
#include "InputManager.h"
#include "PlayerNoAttackState.h"

void PlayerNormalBulletState::Initialize()
{
	sPlayer_->SetBulletRate(20);
}

void PlayerNormalBulletState::Update()
{
	InputManager* input = InputManager::GetInstance();
	if (input->GetMouse()->GetClick(InputMouse::LeftClick) == false) {
		std::unique_ptr<PlayerAttackState> next_ = std::make_unique<PlayerNoAttackState>();
		sPlayer_->SetAttackState(next_);
	}


	if (sPlayer_->GetRateCountIsActive() == false) {
		//	弾生成
		sPlayer_->StartRateCount();

		std::unique_ptr<Bullet> bullet = std::make_unique<Bullet>();
		bullet->Initialize();
		bullet->SetLifeTime(600);
		bullet->SetSpd(1.0f);
		bullet->SetMoveVec(sPlayer_->GetFrontVec());
		bullet->SetModel(ModelManager::GetInstance()->GetModel("bullet"));
		bullet->SetPosition(sPlayer_->GetPosition());
		sPlayer_->AddBullet(bullet);
	}
}
