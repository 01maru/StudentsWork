#include "ParticleComponent.h"
#include "Particle.h"

//-----------------------------------------------------------------------------
// [SECTION] Setter
//-----------------------------------------------------------------------------

void ParticleComponent::SetParent(Particle* parent)
{
	parent_ = parent;
}
