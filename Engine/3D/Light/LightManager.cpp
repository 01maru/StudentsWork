#include "LightManager.h"
#include "ImGuiController.h"
#include "ImGuiManager.h"
#include <cassert>

using namespace MNE::CBuff;
using namespace MyMath;

MNE::LightManager* MNE::LightManager::GetInstance()
{
	static LightManager instance;
	return &instance;
}

void MNE::LightManager::Initialize()
{
	constBuff_.Initialize((sizeof(CBuffLightData) + 0xFF) & ~0xFF);

	TransferConstBuffer();
}

//-----------------------------------------------------------------------------
// [SECTION] Update
//-----------------------------------------------------------------------------

void MNE::LightManager::TransferConstBuffer()
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
			constMap->distanceFog.centerPos = distanceFog_.GetCenterPos();
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

void MNE::LightManager::Update()
{
	if (dirty_) {
		TransferConstBuffer();
		dirty_ = false;
	}
}

void MNE::LightManager::ImGuiUpdate()
{
	if (!ImGuiController::GetInstance()->GetActiveLightManager()) return;

	ImGuiManager* imguiMan = ImGuiManager::GetInstance();

	imguiMan->BeginWindow("LightManager", true);

	int32_t id = 0;

	imguiMan->Text("Material");
	if (imguiMan->BeginChild(Vector2D(0, 90))) {
		imguiMan->PushID(id++);
		imguiMan->SetSliderFloat3("Ambient", ambient_, 0.01f, 0.0f, 1.0f);
		imguiMan->SameLine();
		if (imguiMan->SetButton("SameValue")) ambient_ = { ambient_.x,ambient_.x ,ambient_.x };
		imguiMan->PopID();

		imguiMan->PushID(id++);
		imguiMan->SetSliderFloat3("Diffuse", diffuse_, 0.01f, 0.0f, 1.0f);
		imguiMan->SameLine();
		if (imguiMan->SetButton("SameValue")) diffuse_ = { diffuse_.x,diffuse_.x ,diffuse_.x };
		imguiMan->PopID();

		imguiMan->PushID(id++);
		imguiMan->SetSliderFloat3("Specular", specular_, 0.01f, 0.0f, 1.0f);
		imguiMan->SameLine();
		if (imguiMan->SetButton("SameValue")) specular_ = { specular_.x,specular_.x ,specular_.x };
		imguiMan->PopID();

		imguiMan->EndChild();
	}

	//	方向ライト
	for (size_t i = 0; i < DIRLIGHT_NUM; i++)
	{
		imguiMan->PushID(id++);

		std::string name = "DirLight" + std::to_string(i);
		if (imguiMan->CollapsingHeader(name))
		{
			bool active = dirLights_[i].GetIsActive();
			imguiMan->CheckBox("Active", active);
			SetDirLightActive((int32_t)i, active);

			active = dirLights_[i].GetShadowing();
			imguiMan->CheckBox("Shadow", active);
			SetDirLightShadow((int32_t)i, active);

			Vector3D vec = dirLights_[i].GetLightDir();
			imguiMan->SetSliderFloat3("Dir", vec, 0.001f);
			SetDirLightDir((int32_t)i, vec);

			vec = dirLights_[i].GetLightColor();
			imguiMan->ColorPicker3("Color", vec);
			SetDirLightColor((int32_t)i, vec);
		}

		imguiMan->PopID();
	}

	std::string name = "DistanceFog";
	if (imguiMan->CollapsingHeader(name))
	{
		imguiMan->PushID(id++);

		bool active = distanceFog_.GetIsActive();
		imguiMan->CheckBox("Active", active);
		SetFogActive(active);

		float len = distanceFog_.GetStart();
		imguiMan->SetSliderFloat("Start", len, 0.001f, 0.001f);
		SetFogStart(len);

		len = distanceFog_.GetEnd();
		imguiMan->SetSliderFloat("End", len, 0.001f, 0.001f);
		SetFogEnd(len);

		len = distanceFog_.GetNear();
		imguiMan->SetSliderFloat("Near", len, 0.001f, 0.001f);
		SetFogNear(len);

		len = distanceFog_.GetFar();
		imguiMan->SetSliderFloat("Far", len, 0.001f, 0.001f);
		SetFogFar(len);

		Vector3D vec = distanceFog_.GetColor();
		imguiMan->ColorPicker3("Color", vec);
		SetFogColor(vec);

		vec = distanceFog_.GetCenterPos();
		imguiMan->SetSliderFloat3("CenterPos", vec);
		SetCenterPos(vec);

		imguiMan->PopID();
	}

	imguiMan->EndWindow();
}

//-----------------------------------------------------------------------------
// [SECTION] Getter
//-----------------------------------------------------------------------------

Vector3D MNE::LightManager::GetMtlAmbient()
{
	return ambient_;
}

Vector3D MNE::LightManager::GetMtlDiffuse()
{
	return diffuse_;
}

Vector3D MNE::LightManager::GetMtlSpecular()
{
	return specular_;
}

//-----------------------------------------------------------------------------
// [SECTION] Setter
//-----------------------------------------------------------------------------

void MNE::LightManager::SetGraphicsRootCBuffView(int32_t lootparaIdx)
{
	constBuff_.SetGraphicsRootCBuffView(static_cast<uint32_t>(lootparaIdx));
}

void MNE::LightManager::SetDirLightActive(int32_t index, bool active)
{
	assert(0 <= index && index < DIRLIGHT_NUM);
	if (dirLights_[index].GetIsActive() == active) return;

	dirLights_[index].SetActive(active);
	dirty_ = true;
}

void MNE::LightManager::SetDirLightDir(int32_t index, const Vector3D& lightdir_)
{
	assert(0 <= index && index < DIRLIGHT_NUM);
	if (dirLights_[index].GetLightDir() == lightdir_) return;

	dirLights_[index].SetLightDir(lightdir_);
	dirty_ = true;
}

void MNE::LightManager::SetDirLightColor(int32_t index, const Vector3D& lightcolor_)
{
	assert(0 <= index && index < DIRLIGHT_NUM);
	if (dirLights_[index].GetLightColor() == lightcolor_) return;

	dirLights_[index].SetLightColor(lightcolor_);
	dirty_ = true;
}

void MNE::LightManager::SetDirLightShadow(int32_t index, bool shadowflag)
{
	assert(0 <= index && index < DIRLIGHT_NUM);
	if (dirLights_[index].GetShadowing() == shadowflag) return;

	dirLights_[index].SetShadow(shadowflag);

	dirty_ = true;
}

void MNE::LightManager::SetFogActive(bool active)
{
	if (distanceFog_.GetIsActive() == active) return;

	distanceFog_.SetActive(active);
	dirty_ = true;
}

void MNE::LightManager::SetFogStart(float start)
{
	if (distanceFog_.GetStart() == start) return;

	distanceFog_.SetStart(start);
	dirty_ = true;
}

void MNE::LightManager::SetFogEnd(float end)
{
	if (distanceFog_.GetEnd() == end) return;

	distanceFog_.SetEnd(end);
	dirty_ = true;
}

void MNE::LightManager::SetFogNear(float fogNear)
{
	if (distanceFog_.GetNear() == fogNear) return;

	distanceFog_.SetNear(fogNear);
	dirty_ = true;
}

void MNE::LightManager::SetFogFar(float fogFar)
{
	if (distanceFog_.GetFar() == fogFar) return;

	distanceFog_.SetFar(fogFar);
	dirty_ = true;
}

void MNE::LightManager::SetFogColor(const Vector3D& color)
{
	if (distanceFog_.GetColor() == color) return;

	distanceFog_.SetColor(color);
	dirty_ = true;
}

void MNE::LightManager::SetCenterPos(const Vector3D& center)
{
	if (distanceFog_.GetCenterPos() == center) return;

	distanceFog_.SetCenterPos(center);
	dirty_ = true;
}
