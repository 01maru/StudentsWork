#include "CoolTime.h"

void CoolTime::Initialize()
{
	coolTimer_.SetIsActive(false);
	coolTimer_.SetIsIncrement(true);
	coolTimer_.SetFrameCountIsMax();
	isActive_ = true;
}

void CoolTime::Update()
{
	coolTimer_.Update();

	if (coolTimer_.GetIsActive() == false) {
		isActive_ = true;
	}
}

void CoolTime::StartCount()
{
	coolTimer_.StartCount();
	isActive_ = false;
}

bool CoolTime::GetIsActive()
{
	return isActive_;
}

void CoolTime::SetMaxTime(int32_t time)
{
	coolTimer_.SetMaxFrameCount(time);
}
