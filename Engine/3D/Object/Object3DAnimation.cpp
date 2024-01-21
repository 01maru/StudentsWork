#include "Object3DAnimation.h"
#include "Matrix.h"
#include "ConstBuffStruct.h"
#include "IModel.h"
#include <cassert>
#include <vector>

void MNE::Object3DAnimation::Initialize()
{
	HRESULT result;

	skinData_.Initialize(sizeof(MNE::CBuff::CBuffSkinData));
	//	定数バッファのマッピング
	result = skinData_.GetResource()->Map(0, nullptr, (void**)&cSkinMap_);	//	マッピング
	assert(SUCCEEDED(result));

	//	ボーンの初期化
	for (size_t i = 0; i < MNE::CBuff::MAX_BONES; i++)
	{
		cSkinMap_->bones[i] = Matrix();
	}
}

void MNE::Object3DAnimation::MatUpdate()
{
	if (autoPlay_ == TRUE)
	{
		animeTimer_++;
	}

	std::vector<Matrix> Transforms;
	model_->BoneTransform(animeTimer_, Transforms, animeName_, isLoop_);

	if (Transforms.empty()) return;
	for (size_t i = 0; i < model_->GetNumBones(); i++)
	{
		cSkinMap_->bones[i] = Transforms[i];
	}
}

void MNE::Object3DAnimation::SetGraphicsRootCBuffView(int32_t rootparaIndex)
{
	skinData_.SetGraphicsRootCBuffView(rootparaIndex);
}

//-----------------------------------------------------------------------------
// [SECTION] Getter
//-----------------------------------------------------------------------------

float MNE::Object3DAnimation::GetAnimationTimer() const
{
	return animeTimer_;
}

//-----------------------------------------------------------------------------
// [SECTION] Setter
//-----------------------------------------------------------------------------

void MNE::Object3DAnimation::SetAutoPlay(bool autoPlay)
{
	autoPlay_ = autoPlay;
}

void MNE::Object3DAnimation::SetIsLoop(bool isLoop)
{
	isLoop_ = isLoop;
}

void MNE::Object3DAnimation::SetAnimeName(const std::string& name)
{
	animeName_ = name;
}

void MNE::Object3DAnimation::SetAnimeTimer(float timer)
{
	animeTimer_ = timer;
}

void MNE::Object3DAnimation::SetModel(IModel* model)
{
	model_ = model;
}
