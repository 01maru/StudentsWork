#include "ParticleComponent.h"
#include "Particle.h"

//-----------------------------------------------------------------------------
// [SECTION] Setter
//-----------------------------------------------------------------------------

void MNE::ParticleComponent::SetParent(MNE::Particle* parent)
{
	parent_ = parent;
}
