#include "Light.h"
#include <cassert>

Light* Light::GetInstance()
{
	static Light instance;
	return &instance;
}

void Light::TransferConstBuffer()
{
	HRESULT result;

	ConstBufferLightData* constMap = nullptr;
	result = constBuff_.GetResource()->Map(0, nullptr, (void**)&constMap);
	if (SUCCEEDED(result)) {
		constMap->ambientColor = ambientColor_;

		for (size_t i = 0; i < sDIRLIGHT_NUM; i++)
		{
			if (dirLights_[i].GetIsActive()) {
				constMap->dirLights[i].active = 1;
				constMap->dirLights[i].lightv = dirLights_[i].GetLightDir();
				constMap->dirLights[i].lightcolor = dirLights_[i].GetLightColor();
			}
			else {
				constMap->dirLights[i].active = 0;
			}
		}

		if (distanceFog_.GetIsActive()) {
			constMap->distanceFog.active = 1;
			constMap->distanceFog.color = distanceFog_.GetColor();
			constMap->distanceFog.start = distanceFog_.GetStart();
			constMap->distanceFog.end = distanceFog_.GetEnd();
			constMap->distanceFog.fogFar = distanceFog_.GetFar();
			constMap->distanceFog.fogNear = distanceFog_.GetNear();
		}
		else {
			constMap->distanceFog.active = 0;
		}

		constBuff_.GetResource()->Unmap(0, nullptr);
	}
}

void Light::Initialize()
{
	constBuff_.Initialize((sizeof(ConstBufferLightData) + 0xFF) & ~0xFF);

	TransferConstBuffer();
}

void Light::Update()
{
	if (dirty_) {
		TransferConstBuffer();
		dirty_ = false;
	}
}

void Light::ImGuiUpdate()
{
}

void Light::SetGraphicsRootCBuffView(int32_t lootparaIdx)
{
	constBuff_.SetGraphicsRootCBuffView(lootparaIdx);
}

void Light::SetDirLightActive(int32_t index, bool active)
{
	assert(0 <= index && index < sDIRLIGHT_NUM);
	dirLights_[index].SetActive(active);
}

void Light::SetDirLightDir(int32_t index, const Vector3D& lightdir_)
{
	assert(0 <= index && index < sDIRLIGHT_NUM);
	dirLights_[index].SetLightDir(lightdir_);
	dirty_ = true;
}

void Light::SetDirLightColor(int32_t index, const Vector3D& lightcolor_)
{
	assert(0 <= index && index < sDIRLIGHT_NUM);
	dirLights_[index].SetLightColor(lightcolor_);
	dirty_ = true;
}

void Light::SetDirLightShadow(int32_t index, bool shadowflag)
{
	dirLights_[index].SetShadow(shadowflag);
}
