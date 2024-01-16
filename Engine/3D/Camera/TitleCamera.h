#pragma once
#include "ICamera.h"
#include "FrameCounter.h"
#include "Quaternion.h"

/**
* @file TitleCamera.h
* @brief Titleでのカメラの動きや表示を管理するファイル
*/

class TitleCamera :	public MNE::ICamera
{
public:
	//	カメラの表示モード
	enum CameraMode
	{
		Menu,
		Option,
		SceneChange,
	};

public:
	/**
	* @fn Initialize(const Vector3D&, const Vector3D&, const Vector3D&)
	* 初期化用関数
	* @param eye 視点座標
	* @param target 注視点座標
	* @param up 上方向ベクトル
	*/
	void Initialize(const Vector3D& eye, const Vector3D& target, const Vector3D& up) override;
	/**
	* @fn Update()
	* 更新処理関数
	*/
	void Update() override;

private:
	//	カメラが移動中か
	bool moveCamera_ = false;
	//	現在の表示モード
	CameraMode mode_ = Menu;
	
	Vector3D startEye_;
	Vector3D endEye_;
	Vector3D startFront_;
	Vector3D endFront_;

	FrameCounter counter_;

	Vector3D titleEye_;
	Vector3D titleFront_;

	//	タイトルとオプション用
	Vector3D controlPoint_;

	Vector3D optionEye_;
	Vector3D optionFront_;

	Vector3D sceneChangeEye_;
	Vector3D sceneChangeFront_;

private:
	/**
	* @fn CameraMoveUpdate()
	* カメラの移動に関する更新処理関数
	*/
	void CameraMoveUpdate();

public:
#pragma region Getter

	/**
	* @fn GetIsCameraMove()
	* カメラが移動中かどうかの取得関数
	* @return カメラが移動中かどうか
	*/
	bool GetIsCameraMove();

#pragma endregion

#pragma region Setter

	/**
	* @fn SetAnimationTime(int32_t)
	* カメラの移動時間設定用関数
	* @param time 選択中かどうか
	*/
	void SetAnimationTime(int32_t time);
	/**
	* @fn SetCameraPos(const Vector3D&, const Vector3D&, CameraMode)
	* 表示モードそれぞれに対するカメラの位置設定用関数
	* @param eye 視点座標の位置
	* @param target ターゲットの位置
	* @param mode 設定するモード
	*/
	void SetCameraPos(const Vector3D& eye, const Vector3D& target, CameraMode mode);
	/**
	* @fn SetNextMode(CameraMode)
	* 次の表示モード設定用関数
	* @param mode 次のモード
	*/
	void SetNextMode(CameraMode mode);

#pragma endregion
};

