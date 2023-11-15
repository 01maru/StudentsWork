#include "ObjectParticle.h"
#include "IModel.h"
#include "PipelineManager.h"
#include "ConstBuffStruct.h"
#include "ICamera.h"

void ObjectParticle::Initialize()
{
	Object3D::Initialize();
	mat_.Initialize();
}

void ObjectParticle::MatUpdate()
{
	Object3D::MatUpdate();
}

void ObjectParticle::Draw()
{
	Object3D::Draw();
}

//-----------------------------------------------------------------------------
// [SECTION] Getter
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// [SECTION] Setter
//-----------------------------------------------------------------------------
