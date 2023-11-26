#pragma once
#include "ICamera.h"
#include <memory>
#include <string>
#include "Object3D.h"

/**
* @file CameraManager.h
* @brief カメラをまとめて管理するファイル
*/

class CameraManager
{
private:
	bool isDebug_ = true;
	bool lightView_ = false;
	bool drawTarget_ = false;

	std::unique_ptr<ICamera> mainCamera_;
	std::unique_ptr<ICamera> debugCamera_;
	std::unique_ptr<ICamera> lightCamera_;
	//	正射投影カメラ
	std::unique_ptr<ICamera> orthoProjCamera_;

	std::unique_ptr<Object3D> targetObj_;

private:
	CameraManager() {};
	~CameraManager() {};

	void SetDebugCameraPosToMain();
	void ImGuiCameraInfo(ICamera* camera, const std::string& name);
public:
	static CameraManager* GetInstance();
	//	コピーコンストラクタ無効
	CameraManager(const CameraManager& obj) = delete;
	//	代入演算子無効
	CameraManager& operator=(const CameraManager& obj) = delete;

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
	* @fn DrawTarget()
	* 注視点描画用関数
	*/
	void DrawTarget();

private:
	void TargetUpdate();

public: 
#pragma region Getter

	/**
	* @fn GetCamera()
	* 現在描画に使用しているカメラを返す関数
	* @return 現在描画に使用しているカメラ
	*/
	ICamera* GetCamera();
	/**
	* @fn GetLightCamera()
	* ライトカメラを返す関数
	* @return ライトカメラ
	*/
	ICamera* GetLightCamera();
	/**
	* @fn GetMainCamera()
	* メインカメラを返す関数
	* @return メインカメラ
	*/
	ICamera* GetMainCamera();
	/**
	* @fn GetDebugCamera()
	* デバッグカメラを返す関数
	* @return デバッグカメラ
	*/
	ICamera* GetDebugCamera();

	ICamera* GetOrthoProjCamera();

#pragma endregion

#pragma region Setter

	/**
	* @fn SetMainCamera(std::unique_ptr<ICamera>)
	* メインカメラ設定用関数
	* @param camera 設定するメインカメラ
	*/
	void SetMainCamera(std::unique_ptr<ICamera>& camera) { mainCamera_ = std::move(camera); }
	/**
	* @fn SetMainCamera(std::unique_ptr<ICamera>)
	* ライトカメラ設定用関数
	* @param camera 設定するライトカメラ
	*/
	void SetLightCamera(std::unique_ptr<ICamera>& camera) { lightCamera_ = std::move(camera); }

	void SetOrthoProjCamera(std::unique_ptr<ICamera>& camera) { orthoProjCamera_ = std::move(camera); }

#pragma endregion
};

