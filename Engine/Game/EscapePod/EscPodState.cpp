#include "EscPodState.h"
#include "EscapePod.h"

EscapePod* EscPodState::sPod_ = nullptr;

//-----------------------------------------------------------------------------
// [SECTION] Setter
//-----------------------------------------------------------------------------

void EscPodState::SetPod(EscapePod* pod)
{
	sPod_ = pod;
}
