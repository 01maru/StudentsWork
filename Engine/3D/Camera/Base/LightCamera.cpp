#include "LightCamera.h"
#include "ConstBuffStruct.h"
#include <cassert>

void MNE::LightCamera::ConstBuffInitialzie()
{
	HRESULT result;

#pragma region ConstBuff

	lightMaterial_.Initialize(sizeof(MNE::CBuff::CBuffLightMaterial));
	//	定数バッファのマッピング
	result = lightMaterial_.GetResource()->Map(0, nullptr, (void**)&cLightMap_);	//	マッピング
	assert(SUCCEEDED(result));

#pragma endregion
}

void MNE::LightCamera::Initialize(const Vector3D& frontVec, const Vector3D& center, float dis)
{
	ConstBuffInitialzie();

	ICamera::Initialize(frontVec, center, dis);
}

void MNE::LightCamera::Initialize(const Vector3D& eye, const Vector3D& target, const Vector3D& up)
{
	ConstBuffInitialzie();

	ICamera::Initialize(eye, target, up);
}

void MNE::LightCamera::Update()
{
	ICamera::Update();

	cLightMap_->mLVP = GetViewProj();
	cLightMap_->cameraPos = GetEye();
}

void MNE::LightCamera::SetGraphicsRootCBuffView(uint32_t rootParaIdx)
{
	lightMaterial_.SetGraphicsRootCBuffView(rootParaIdx);
}
