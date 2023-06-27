#pragma once
#include "DirLight.h"
#include "DistanceFog.h"

#include "ConstBuff.h"
#include "ConstBuffStruct.h"

class Light
{
private:
	ConstBuff constBuff_;

	bool dirty_ = false;
	Vector3D ambientColor_ = { 1,1,1 };
	DirLight dirLights_[CBuff::DIRLIGHT_NUM];
	DistanceFog distanceFog_;

private:
	Light() {};
	~Light() {};

	void TransferConstBuffer();
public:
	static Light* GetInstance();
	Light(const Light& obj) = delete;
	Light& operator=(const Light& obj) = delete;

	void Initialize();
	void Update();
	void ImGuiUpdate();
	void SetGraphicsRootCBuffView(int32_t lootparaIdx);

	void SetDirLightActive(int32_t index, bool active);
	void SetDirLightDir(int32_t index, const Vector3D& lightdir_);
	void SetDirLightColor(int32_t index, const Vector3D& lightcolor_);
	void SetDirLightShadow(int32_t index, bool shadowflag);

	void SetFogActive(bool active) { distanceFog_.SetActive(active); }
	void SetFogStart(float start) { distanceFog_.SetStart(start); }
	void SetFogEnd(float end) { distanceFog_.SetEnd(end); }
	void SetFogNear(float fogNear) { distanceFog_.SetNear(fogNear); }
	void SetFogFar(float fogFar) { distanceFog_.SetFar(fogFar); }
};

