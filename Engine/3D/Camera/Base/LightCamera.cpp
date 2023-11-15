#include "LightCamera.h"
#include "ConstBuffStruct.h"
#include <cassert>

void LightCamera::ImGuiInfo()
{
}

void LightCamera::ConstBuffInitialzie()
{
	HRESULT result;

#pragma region ConstBuff

	lightMaterial_.Initialize(sizeof(CBuff::CBuffLightMaterial));
	//	定数バッファのマッピング
	result = lightMaterial_.GetResource()->Map(0, nullptr, (void**)&cLightMap_);	//	マッピング
	assert(SUCCEEDED(result));

#pragma endregion
}

void LightCamera::Initialize(const Vector3D& /*frontVec*/, const Vector3D& /*center*/, float /*dis*/)
{
	ConstBuffInitialzie();

}

void LightCamera::Initialize(const Vector3D& eye, const Vector3D& target, const Vector3D& up)
{
	ConstBuffInitialzie();

	eye_ = eye;
	target_ = target;
	up_ = up;

	MatUpdate();

	CalcDirectionVec();

	//up_ = -downVec_;

	MatUpdate();
}

void LightCamera::Update()
{
	CalcDirectionVec();

	CalcBillboard();

	MatUpdate();

	cLightMap_->mLVP = GetViewProj();
	cLightMap_->cameraPos = GetEye();
}

void LightCamera::SetGraphicsRootCBuffView(uint32_t rootParaIdx)
{
	lightMaterial_.SetGraphicsRootCBuffView(rootParaIdx);
}
