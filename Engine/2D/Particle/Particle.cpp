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

MNE::Blend::BlendMode MNE::Particle::GetBlendMode()
{
	return blendMode_;
}

bool MNE::Particle::GetIsEnd()
{
	return isEnd_;
}

float MNE::Particle::GetTimerPerTime()
{
	return lifeTimer_.GetCountPerMaxCount();
}

MyMath::Vector3D MNE::Particle::GetDir()
{
	return dir_;
}

//-----------------------------------------------------------------------------
// [SECTION] Setter
//-----------------------------------------------------------------------------

void MNE::Particle::SetDir(const MyMath::Vector3D& dir)
{
	dir_ = dir;
}

void MNE::Particle::SetBlendMode(MNE::Blend::BlendMode blendMord)
{
	blendMode_ = blendMord;
}

void MNE::Particle::SetLifeTime(int32_t time)
{
	lifeTimer_.SetMaxFrameCount(time);
}

void MNE::Particle::StartTimer()
{
	lifeTimer_.StartCount();
}
