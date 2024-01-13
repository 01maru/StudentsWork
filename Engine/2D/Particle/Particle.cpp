#include "Particle.h"

void MNE::Particle::Update()
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

void MNE::Particle::MatUpdate()
{
	for (auto itr = components_.begin(); itr != components_.end(); ++itr)
	{
		itr->get()->MatUpdate();
	}
}

void MNE::Particle::Draw()
{
	for (auto itr = components_.begin(); itr != components_.end(); ++itr)
	{
		itr->get()->Draw();
	}
}

//-----------------------------------------------------------------------------
// [SECTION] Getter
//-----------------------------------------------------------------------------

MNE::Blend::BlendMord MNE::Particle::GetBlendMord()
{
	return blendMord_;
}

bool MNE::Particle::GetIsEnd()
{
	return isEnd_;
}

float MNE::Particle::GetTimerPerTime()
{
	return lifeTimer_.GetCountPerMaxCount();
}

//-----------------------------------------------------------------------------
// [SECTION] Setter
//-----------------------------------------------------------------------------

void MNE::Particle::SetBlendMord(MNE::Blend::BlendMord blendMord)
{
	blendMord_ = blendMord;
}

void MNE::Particle::SetLifeTime(int32_t time)
{
	lifeTimer_.SetMaxFrameCount(time);
}

void MNE::Particle::StartTimer()
{
	lifeTimer_.StartCount();
}
