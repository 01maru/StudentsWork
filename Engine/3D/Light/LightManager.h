#pragma once
#include "DirLight.h"
#include "DistanceFog.h"

#include "ConstBuff.h"
#include "ConstBuffStruct.h"

/**
* @file LightManager.h
* @brief ライト関連の処理を管理するファイル
*/

class LightManager
{
private:
	ConstBuff constBuff_;

	bool dirty_ = false;
	Vector3D ambientColor_ = { 1,1,1 };
	DirLight dirLights_[CBuff::DIRLIGHT_NUM];
	DistanceFog distanceFog_;

	Vector3D ambient_ = { 0.8f, 0.8f, 0.8f };
	Vector3D diffuse_ = { 0.7f, 0.7f, 0.7f };
	Vector3D specular_= { 0.3f, 0.3f, 0.3f };

private:
	LightManager() {};
	~LightManager() {};

	void TransferConstBuffer();
public:
	static LightManager* GetInstance();
	LightManager(const LightManager& obj) = delete;
	LightManager& operator=(const LightManager& obj) = delete;

	/**
	* @fn Initialize()
	* 初期化用関数
	*/
	void Initialize();
	/**
	* @fn Update()
	* 更新処理関数
	*/
	void Update();
	/**
	* @fn ImGuiUpdate()
	* ImGui更新処理関数
	*/
	void ImGuiUpdate();
	/**
	* @fn SetGraphicsRootCBuffView(int32_t)
	* コマンドリストのSetGraphicsRootCBuffViewを設定する関数
	* @param lootparaIdx ルートパラメータの添え字
	*/
	void SetGraphicsRootCBuffView(int32_t lootparaIdx);

#pragma region Getter

	Vector3D GetMtlAmbient()const { return ambient_; }
	Vector3D GetMtlDiffuse()const { return diffuse_; }
	Vector3D GetMtlSpecular()const { return specular_; }

#pragma endregion

#pragma region Setter

	void SetDirLightActive(int32_t index, bool active);
	void SetDirLightDir(int32_t index, const Vector3D& lightdir_);
	void SetDirLightColor(int32_t index, const Vector3D& lightcolor_);
	void SetDirLightShadow(int32_t index, bool shadowflag);

	void SetFogActive(bool active);
	void SetFogStart(float start);
	void SetFogEnd(float end);
	void SetFogNear(float fogNear);
	void SetFogFar(float fogFar);
	void SetFogColor(const Vector3D& color);

#pragma endregion
};

