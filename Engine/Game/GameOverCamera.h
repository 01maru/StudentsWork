#pragma once
#include "ICamera.h"
#include "FrameCounter.h"

/**
* @file GameOverCamera.h
* @brief ゲームオーバー時のカメラの動き管理ファイル
*/

class GameOverCamera :public MNE::ICamera
{
public:
	/**
	* @fn Initialize(const Vector3D&, const Vector3D&, float)
	* 初期化用関数
	* @param frontVec 前方方向ベクトル
	* @param center 注視点座標
	* @param dis 注視点と視点座標の距離
	*/
	void Initialize(const Vector3D& eye, const Vector3D& target, const Vector3D& up) override;
	/**
	* @fn Update()
	* 更新処理関数
	*/
	void Update() override;

private:
	/**
	* @fn ImGuiInfo()
	* ImGuiに表示する情報をまとめる関数
	*/
	void ImGuiInfo() override;

private:
	//	動いている時間管理カウンター
	FrameCounter counter_;
	//	カウンターの変更時間
	int32_t moveTime_ = 50;
	int32_t easePaw_;

	//	開始位置
	Vector3D eyeStart_;
	//	終了位置
	Vector3D eyeEnd_;
	float eEyeYRate_ = 0.125f;
	float eEyeLenRate_ = 0.25f;

	//	制御点
	Vector3D controlPoint_;
	float cPointYRate_ = 0.66f;
	float cPointLenRate_ = 1.33f;

public:
#pragma region Getter

	/**
	* @fn GetEndCameraMove()
	* カメラの動きが止まったか取得する関数
	* @return 終了していたらTRUEを返す
	*/
	bool GetEndCameraMove();

#pragma endregion

#pragma region Setter

	/**
	* @fn Reset()
	* カメラの初期をリセットし、再度移動させるための関数
	*/
	void Reset();
	/**
	* @fn SetPosData(const Vector3D&)
	* カメラとプレイヤーの位置から制御点と終了位置を設定するための関数
	* @param pos プレイヤーの位置(足元の座標)
	*/
	void SetPosData(const Vector3D& pos);

#pragma endregion
};

