#pragma once
#include "IBulletManager.h"
#include "BulletInfo.h"
#include <vector>

/**
* @file EnemyBulletManager.h
* @brief 敵の弾丸をまとめて管理するファイル
*/

class EnemyBulletManager :public IBulletManager
{
public:
	/**
	* @fn Initialize()
	* 初期化処理関数
	*/
	void Initialize();
	/**
	* @fn LoadResources()
	* リソース読み込み処理関数
	*/
	void LoadResources();
	/**
	* @fn Update()
	* 更新処理関数
	*/
	void Update(std::list<EnemyBulletInfo>& bullets, const BeamInfo& beam);

	void Draw() override;

private:
	bool beamActive_ = false;
	MNE::Object3D beamObj_;

	//	岩のオブジェクト配列
	std::vector<MNE::Object3D> rocks_;
};

