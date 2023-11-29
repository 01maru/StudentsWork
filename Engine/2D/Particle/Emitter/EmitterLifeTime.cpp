#include "EmitterLifeTime.h"

void EmitterLifeTime::Initialize(Particle* particle)
{
	particle->SetLifeTime(lifetime);
}
