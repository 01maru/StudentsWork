#pragma once
#include "ICamera.h"
#include "FrameCounter.h"

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
		Menu,			//	タイトル表示時
		Option,			//	オプション表示時
		SceneChange,	//	シーン遷移
	};

public:
	/**
	* @fn Initialize(const Vector3D&, const Vector3D&, const Vector3D&)
	* 初期化用関数
	* @param eye 視点座標
	* @param target 注視点座標
	* @param up 上方向ベクトル
	*/
	void Initialize(const MyMath::Vector3D& eye, const MyMath::Vector3D& target, const MyMath::Vector3D& up) override;
	/**
	* @fn Update()
	* 更新処理関数
	*/
	void Update() override;

private:
	//	現在の表示モード
	CameraMode mode_ = Menu;

	//	アニメーションの経過時間
	MNE::FrameCounter counter_;
	
	//	移動開始前と後の値格納用
	MyMath::Vector3D startEye_;
	MyMath::Vector3D endEye_;
	MyMath::Vector3D startFront_;
	MyMath::Vector3D endFront_;
	//	制御点
	MyMath::Vector3D controlPoint_;

#pragma region カメラの最終値格納変数

	//	タイトル
	MyMath::Vector3D titleEye_;
	MyMath::Vector3D titleFront_;
	//	オプション
	MyMath::Vector3D optionEye_;
	MyMath::Vector3D optionFront_;
	//	シーンチェンジ
	MyMath::Vector3D sceneChangeEye_;
	MyMath::Vector3D sceneChangeFront_;

#pragma endregion

private:
	/**
	* @fn CameraMoveUpdate()
	* カメラの移動に関する更新処理関数
	*/
	void CameraMoveUpdate();

public:
#pragma region Getter

	/**
	* @fn GetIsMoving()
	* カメラが移動中かどうかの取得関数
	* @return カメラが移動中かどうか
	*/
	bool GetIsMoving();

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
	void SetCameraPos(const MyMath::Vector3D& eye, const MyMath::Vector3D& target, CameraMode mode);
	/**
	* @fn SetNextMode(CameraMode)
	* 次の表示モード設定用関数
	* @param mode 次のモード
	*/
	void SetNextMode(CameraMode mode);

#pragma endregion
};

