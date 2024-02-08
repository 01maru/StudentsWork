#pragma once
#include "Object3D.h"
#include "CharacterHP.h"
#include "BossState.h"
#include "EnemyBullet.h"

/**
* @file Boss.h
* @brief ボスの動き全体を管理するファイル
*/

#pragma region 前置宣言
class IModel;
class Player;
class IGameState;
#pragma endregion

class Boss :public MNE::Object3D
{
public:
	/**
	* @fn StatusInitialize()
	* ステータス初期化用関数
	*/
	void StatusInitialize();
	/**
	* @fn Initialize()
	* 初期化用関数
	*/
	void Initialize(MNE::IModel* model);
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
	* @fn DrawBullets()
	* 弾の描画処理関数
	*/
	void DrawBullets();
	/**
	* @fn DrawUI()
	* UI描画処理関数
	*/
	void DrawUI();
	void CollisionUpdate();
	void OnCollision(CollisionInfo& info) override;

private:
	bool isActive_ = true;

	//	体力
	int32_t maxHP_ = 100;
	CharacterHP hp_;
	//	現在のステート
	std::unique_ptr<BossState> currentState_;
	//	敵の弾リスト(後々マネージャーで管理する予定)
	std::list<std::unique_ptr<EnemyBullet>> bullets_;

	//	体に触れたらダメージを与えるようフラグ
	bool bodyAt_ = false;
	bool isSecondForm_ = false;

	MyMath::Vector3D frontVec_ = { 0.0f,0.0f,-1.0f };

	//	プレイヤーのポインター
	Player* player_ = nullptr;
	IGameState* pClearState_ = nullptr;
	
public:
	/**
	* @fn RotationUpdate()
	* モデルの回転更新用関数
	*/
	float RotationUpdate();
	/**
	* @fn AddBullet(std::unique_ptr<EnemyBullet>&)
	* リストに弾追加関数
	* @param bullet 追加する弾の情報
	*/
	void AddBullet(std::unique_ptr<EnemyBullet>& bullet);
	/**
	* @fn DecHP(int32_t)
	* HP減少させる用関数
	* @param damage HP減少量
	*/
	void DecHP(int32_t damage);

	/**
	* @fn StartClearState()
	* クリア演出開始用関数
	*/
	void StartClearState();

#pragma region Getter

	/**
	* @fn GetIsAlive()
	* 生存中かのフラグ取得用関数
	* @return 生存中か
	*/
	bool GetIsAlive();
	/**
	* @fn GetIsHPLessThanHalf()
	* HPが半分以下になったか取得用関数
	* @return HPが半分以下になったか
	*/
	bool GetIsHPLessThanHalf();
	/**
	* @fn GetIsSecondForm()
	* 第二形態かの取得用関数
	* @return 第二形態か
	*/
	bool GetIsSecondForm();
	/**
	* @fn GetBodyAttack()
	* ボディーアタック中かの取得用関数
	* @return ボディーアタック中か
	*/
	bool GetBodyAttack();
	/**
	* @fn GetPositionPtr()
	* モデルの座標のポインター取得用関数
	* @return モデルの座標のポインター
	*/
	MyMath::Vector3D* GetPositionPtr();
	/**
	* @fn GetFrontVec()
	* 前方ベクトル取得用関数
	* @return 前方ベクトル
	*/
	MyMath::Vector3D GetFrontVec();
	/**
	* @fn GetPlayerPtr()
	* プレイヤーのポインター取得用関数
	* @return プレイヤーのポインター
	*/
	Player* GetPlayerPtr();

#pragma endregion

#pragma region Setter

	/**
	* @fn SetIsSecondForm(bool)
	* 第二形態かのフラグ設定用関数
	* @param isSecondForm 第二形態か
	*/
	void SetIsSecondForm(bool isSecondForm);
	/**
	* @fn SetIsActive(bool)
	* アクティブかのフラグ設定用関数
	* @param isActive アクティブか
	*/
	void SetIsActive(bool isActive);
	/**
	* @fn SetCurrentState(std::unique_ptr<BossState>&)
	* 次のステート設定用関数
	* @param next 次のステート
	*/
	void SetCurrentState(std::unique_ptr<BossState>& next);
	/**
	* @fn SetPlayer(Player*)
	* プレイヤーのポインター設定用関数
	* @param player プレイヤーのポインター
	*/
	void SetPlayer(Player* player);
	/**
	* @fn SetHPBarSprite(const MNE::Sprite&)
	* HPBarのスプライト設定用関数
	* @param sprite 第二形態か
	*/
	void SetHPBarSprite(const MNE::Sprite& sprite);
	/**
	* @fn SetBodyAttack(bool)
	* ボディーアタック中かのフラグ設定用関数
	* @param attackFlag ボディーアタック中か
	*/
	void SetBodyAttack(bool attackFlag);
	/**
	* @fn SetClearState(IGameState*)
	* クリアステートのポインター設定用関数
	* @param clearState クリアステートのポインター
	*/
	void SetClearState(IGameState* clearState);

#pragma endregion
};

