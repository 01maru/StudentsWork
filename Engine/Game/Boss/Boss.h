#pragma once
#include "Object3D.h"
#include "CharacterHP.h"
#include "BossState.h"
#include "BulletInfo.h"
#include "BossData.h"

/**
* @file Boss.h
* @brief ボスの動き全体を管理するファイル
*/

#pragma region 前置宣言
class Player;
class GameScene;
#pragma endregion

class Boss :public MNE::Object3D, public BossData
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
	* @fn DrawUI()
	* UI描画処理関数
	*/
	void DrawUI();
	void CollisionUpdate();
	void OnCollision(MNE::CollisionInfo& info) override;

private:
	bool isActive_ = true;

	//	体力
	CharacterHP hp_;
	//	現在のステート
	std::unique_ptr<BossState> currentState_;
	//	追加する敵の弾リスト
	std::list<EnemyBulletInfo> bullets_;
	BeamInfo beam_;

	//	体に触れたらダメージを与えるようフラグ
	bool bodyAt_ = false;
	bool isSecondForm_ = false;

	MyMath::Vector3D frontVec_ = { 0.0f,0.0f,-1.0f };

	//	プレイヤーのポインター
	Player* player_ = nullptr;
	GameScene* pGameScene_ = nullptr;

	bool isDeathState_ = false;

public:
	/**
	* @fn RotationUpdate()
	* モデルの回転更新用関数
	*/
	float RotationUpdate();
	/**
	* @fn AddBullet(EnemyBulletInfo&)
	* 追加する弾の情報をリストに加えるための関数
	* @param bullet 追加する弾の情報
	*/
	void AddBullet(EnemyBulletInfo& bullet);
	/**
	* @fn DecHP(int32_t)
	* HP減少させる用関数
	* @param damage HP減少量
	*/
	void DecHP(int32_t damage);

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

	MyMath::Vector3D GetShotPoint();
	MyMath::Vector3D GetBeamPoint();

	std::list<EnemyBulletInfo>& GetBullets();
	BeamInfo GetBeamInfo();

	bool GetIsDeathState() { return isDeathState_; }

	GameScene* GetGameScene();

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

	void SetBeamInfo(const BeamInfo& beam);

	void SetIsDeathState(bool isDeathState) { isDeathState_ = isDeathState; }

	void SetGameScene(GameScene* gameScene);

#pragma endregion
};

