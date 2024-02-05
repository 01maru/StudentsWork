#pragma once
#include "ICamera.h"
#include "FrameCounter.h"
#include "Quaternion.h"

/**
* @file GameCamera.h
* @brief プレイヤーを中心に回転するカメラの動きを管理するファイル
*/

class GameCamera :public MNE::ICamera
{
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
	* @fn Initialize(const Vector3D&, const Vector3D&, float)
	* 初期化用関数
	* @param frontVec 前方方向ベクトル
	* @param center 注視点座標
	* @param dis 注視点と視点座標の距離
	*/
	void Initialize(const Vector3D& frontVec, const Vector3D& center, float dis) override;
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
	//	操作可能か
	bool isActive_ = false;

	//	縦軸のカメラの移動可能範囲
	const float MAX_ANGLE_Y = 160.0f;
	const float MIN_ANGLE_Y = 20.0f;
	float minAngleRange_;
	float maxAngleRange_;

	float spd_ = 0.1f;

	//	ターゲットカメラ実行中
	bool targeting_ = false;
	//	ターゲットカメラの経過時間
	FrameCounter counter_;
	//	ターゲットカメラの正面ベクトルの開始方向と終了方向
	Quaternion startFront_;
	Quaternion endFront_;
	//	最大時の移動終了時間
	int32_t maxMoveTime_ = 20;
	int32_t minMoveTime_ = 5;

	//	マウスの移動最大半径の範囲
	float mouseMaxRad_ = 300.0f;

	//	一番カメラの向きから近い敵の位置
	Vector3D* pEnemyPos_ = nullptr;
	//	プレイヤーの位置
	Vector3D* pPlayerPos_ = nullptr;

private:
	/**
	* @fn TargetCameraUpdate()
	* ターゲットカメラの動きを管理する関数
	*/
	void TargetCameraUpdate();
	/**
	* @fn UnTargetUpdate()
	* プレイヤーが操作できるカメラの動きを管理する関数
	*/
	void UnTargetUpdate();

public:
#pragma region Setter

	/**
	* @fn SetEnemyPos(Vector3D*)
	* ターゲットする敵の位置を設定するための関数
	* @param pos ターゲットする敵の位置のポインタ
	*/
	void SetEnemyPos(Vector3D* pos);
	/**
	* @fn SetPlayerPos(Vector3D*)
	* プレイヤーの位置を設定するための関数
	* @param pos プレイヤーの位置のポインタ
	*/
	void SetPlayerPos(Vector3D* pos);
	/**
	* @fn SetIsActive(bool)
	* 実行中かどうかの設定をする溜めの関数
	* @param isActive 実行中かどうか
	*/
	void SetIsActive(bool isActive);

#pragma endregion
};

