#include "Particle.h"

void Particle::Update()
{
	if (lifeTimer_.GetFrameCount() == lifeTimer_.GetMaxFrameCount()) {
		isEnd_ = true;
	}

	lifeTimer_.Update();

	for (auto itr = components_.begin(); itr != components_.end(); ++itr)
	{
		itr->get()->Update();
	}
}

void Particle::MatUpdate()
{
	for (auto itr = components_.begin(); itr != components_.end(); ++itr)
	{
		itr->get()->MatUpdate();
	}
}

void Particle::Draw()
{
	for (auto itr = components_.begin(); itr != components_.end(); ++itr)
	{
		itr->get()->Draw();
	}
}

//-----------------------------------------------------------------------------
// [SECTION] Getter
//-----------------------------------------------------------------------------

Blend::BlendMord Particle::GetBlendMord()
{
	return blendMord_;
}

bool Particle::GetIsEnd()
{
	return isEnd_;
}

float Particle::GetTimerPerTime()
{
	return lifeTimer_.GetCountPerMaxCount();
}

//-----------------------------------------------------------------------------
// [SECTION] Setter
//-----------------------------------------------------------------------------

void Particle::SetBlendMord(Blend::BlendMord blendMord)
{
	blendMord_ = blendMord;
}

void Particle::SetLifeTime(int32_t time)
{
	lifeTimer_.SetMaxFrameCount(time);
}

void Particle::StartTimer()
{
	lifeTimer_.StartCount();
}
