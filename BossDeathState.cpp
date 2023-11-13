#include "BossDeathState.h"
#include "Boss.h"
#include "Easing.h"

void BossDeathState::Initialize()
{
	counter_.Initialize(150, true, true);
	counter_.StartCount();
}

void BossDeathState::Update()
{
	counter_.Update();

	float scale = Easing::EaseOut(2.0f, 0.0f, counter_.GetCountPerMaxCount(), 3);
	sBoss_->SetScale({ scale,scale,scale });

	if (counter_.GetIsActive() == false) {
		sBoss_->SetIsHide(true);
	}
}
