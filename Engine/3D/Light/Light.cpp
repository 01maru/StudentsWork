#include "Light.h"
#include <cassert>

#include "ImGuiController.h"
#include "ImGuiManager.h"

using namespace CBuff;

Light* Light::GetInstance()
{
	static Light instance;
	return &instance;
}

void Light::TransferConstBuffer()
{
	HRESULT result;

	CBuffLightData* constMap = nullptr;
	result = constBuff_.GetResource()->Map(0, nullptr, (void**)&constMap);
	if (SUCCEEDED(result)) {
		constMap->ambientColor = ambientColor_;

		for (size_t i = 0; i < DIRLIGHT_NUM; i++)
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
	constBuff_.Initialize((sizeof(CBuffLightData) + 0xFF) & ~0xFF);

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
	if (!ImGuiController::GetInstance()->GetActiveLightManager()) return;

	ImGuiManager* imguiMan = ImGuiManager::GetInstance();

	imguiMan->BeginWindow("LightManager", true);

	int32_t id = 0;

	//	方向ライト
	for (size_t i = 0; i < DIRLIGHT_NUM; i++)
	{
		imguiMan->PushID(id++);

		std::string name = "DirLight" + std::to_string(i);
		if (imguiMan->CollapsingHeader(name))
		{
			bool active = dirLights_[i].GetIsActive();
			imguiMan->CheckBox("Active", active);
			dirLights_[i].SetActive(active);

			active = dirLights_[i].GetShadowing();
			imguiMan->CheckBox("Shadow", active);
			dirLights_[i].SetShadow(active);

			Vector3D vec = dirLights_[i].GetLightDir();
			imguiMan->SetSliderFloat3("Dir", vec, 0.001f);
			dirLights_[i].SetLightDir(vec);

			vec = dirLights_[i].GetLightColor();
			imguiMan->ColorPicker3("Color", vec);
			dirLights_[i].SetLightColor(vec);
		}

		imguiMan->PopID();
	}

	std::string name = "DistanceFog";
	if (imguiMan->CollapsingHeader(name))
	{
		imguiMan->PushID(id++);

		bool active = distanceFog_.GetIsActive();
		imguiMan->CheckBox("Active", active);
		distanceFog_.SetActive(active);

		float len= distanceFog_.GetStart();
		imguiMan->SetSliderFloat("Start", len, 0.001f, 0.001f);
		distanceFog_.SetStart(len);

		len= distanceFog_.GetEnd();
		imguiMan->SetSliderFloat("End", len, 0.001f, 0.001f);
		distanceFog_.SetEnd(len);

		len= distanceFog_.GetNear();
		imguiMan->SetSliderFloat("Near", len, 0.001f, 0.001f);
		distanceFog_.SetNear(len);

		len= distanceFog_.GetFar();
		imguiMan->SetSliderFloat("Far", len, 0.001f, 0.001f);
		distanceFog_.SetFar(len);

		Vector3D vec = distanceFog_.GetColor();
		imguiMan->ColorPicker3("Color", vec);
		distanceFog_.SetColor(vec);

		imguiMan->PopID();
	}

	//	無駄アリ
	TransferConstBuffer();

	imguiMan->EndWindow();
}

void Light::SetGraphicsRootCBuffView(int32_t lootparaIdx)
{
	constBuff_.SetGraphicsRootCBuffView(lootparaIdx);
}

void Light::SetDirLightActive(int32_t index, bool active)
{
	assert(0 <= index && index < DIRLIGHT_NUM);
	dirLights_[index].SetActive(active);
}

void Light::SetDirLightDir(int32_t index, const Vector3D& lightdir_)
{
	assert(0 <= index && index < DIRLIGHT_NUM);
	dirLights_[index].SetLightDir(lightdir_);
	dirty_ = true;
}

void Light::SetDirLightColor(int32_t index, const Vector3D& lightcolor_)
{
	assert(0 <= index && index < DIRLIGHT_NUM);
	dirLights_[index].SetLightColor(lightcolor_);
	dirty_ = true;
}

void Light::SetDirLightShadow(int32_t index, bool shadowflag)
{
	dirLights_[index].SetShadow(shadowflag);
}
