#include "Object3DAnimation.h"
#include "Matrix.h"
#include "IModel.h"
#include <vector>
#include "ConstBuffStruct.h"
#include <cassert>

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
	std::vector<Matrix> Transforms;
	model_->BoneTransform(animationTimer_, Transforms, animationIdx);

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

void MNE::Object3DAnimation::SetModel(IModel* model)
{
	model_ = model;
}
