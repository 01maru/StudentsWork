#pragma once
#include "Sprite.h"
#include "FrameCounter.h"

/**
* @file CharacterHP.h
* @brief キャラのHPの計算や表示を管理するためのファイル
*/

class CharacterHP
{
public:
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
	* @fn Draw()
	* 描画処理関数
	*/
	void Draw();

protected:
	//	生存中か
	bool isAlive_ = FALSE;
	//	現在のHP
	int32_t hp_;
	//	HPの最大値
	int32_t maxHP_ = 100;

	//	バーの長さ
	float len_;

	//	DamageAnimation
	MNE::FrameCounter damageTimer_;
	int32_t animationTime_ = 30;	//	アニメーションの時間
	float damageLen_ = 0.0f;		//	ダメージを受けて実際に減少したHPの長さ

#pragma region Sprite

	MNE::Sprite back_;		//	背景
	MNE::Sprite damage_;	//	ダメージを受けたときの減少量表示
	MNE::Sprite bar_;		//	HPゲージの表示

#pragma endregion

private:
	/**
	* @fn IsAliveUpdate()
	* 生存フラグ更新処理関数
	*/
	void IsAliveUpdate();
	/**
	* @fn HPBarUpdate()
	* ダメージを受けたときのHPBarの演出処理関数
	*/
	void HPBarUpdate();

public:
	void DecHp(int32_t damage);
#pragma region Getter

	/**
	* @fn GetIsAlive()
	* 生存中かのフラグ取得関数
	* @return 生存中かどうか
	*/
	bool GetIsAlive();
	/**
	* @fn GetHP()
	* 現在のHP取得関数
	* @return 現在のHP
	*/
	int32_t GetHP();
	/**
	* @fn GetMaxHP()
	* HPの最大値取得関数
	* @return HPの最大値
	*/
	int32_t GetMaxHP();

#pragma endregion

#pragma region Setter

	/**
	* @fn SetMaxHP(int32_t)
	* HPの最大値設定用関数
	* @param maxHP HPの最大値
	*/
	void SetMaxHP(int32_t maxHP);
	/**
	* @fn SetSprite(const MNE::Sprite&)
	* HPバーのスプライト設定用関数
	* @param sprite HPバーのスプライトの情報
	*/
	void SetSprite(const MNE::Sprite& sprite);
	/**
	* @fn SetBarColor(const Vector3D&)
	* バーの色設定用関数
	* @param color バーの色変更
	*/
	void SetBarColor(const MyMath::Vector3D& color);

#pragma endregion
};

