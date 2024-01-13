#include "EmitterLifeTime.h"

void MNE::EmitterLifeTime::Initialize(MNE::Particle* particle)
{
	particle->SetLifeTime(lifetime);
}
