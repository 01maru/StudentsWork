#include "ShadowPostEffect.h"
#include "DirectX.h"
#include "SceneManager.h"

using namespace MNE;

void MNE::ShadowPostEffect::Update()
{
	MyDirectX* dx = MyDirectX::GetInstance();

	dx->PrevPostEffect(this);

	SceneManager::GetInstance()->DrawShadow();

	if (originalPE_ != nullptr)
	{
		originalPE_->Draw();
	}

	dx->PostEffectDraw(this);
}
