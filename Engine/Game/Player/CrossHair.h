#pragma once
#include "Sprite.h"

/**
* @file CrossHair.h
* @brief クロスヘアの表示や飛ばす方向を設定するためのファイル
*/

class CrossHair
{
public:
	/**
	* @fn Update(const Vector3D&)
	* 更新処理関数
	* @param startPos 銃口の先端の位置(弾が出るところ)
	*/
	void Update(const Vector3D& startPos);
	/**
	* @fn ImGuiUpdate()
	* ImGui更新処理関数
	*/
	void ImGuiUpdate();
	/**
	* @fn Draw()
	* 描画処理関数
	*/
	void Draw();

private:
	//	クロスヘアのスプライト
	MNE::Sprite sprite_;
	//	弾の発射方向
	Vector3D dir_;
	//	地形とレイが当たらなかったときの発射の方向
	const float MAX_DISTANCE = 80.0f;

public:
#pragma region Getter

	/**
	* @fn GetDir()
	* 弾の発射方向取得関数
	* @return 弾の発射方向
	*/
	Vector3D GetDir();
	
#pragma endregion

#pragma region Setter

	/**
	* @fn SetSprite(const MNE::Sprite&)
	* クロスヘアのスプライト設定用関数
	* @param sprite クロスヘアのスプライト表示情報
	*/
	void SetSprite(const MNE::Sprite& sprite);

#pragma endregion
};

