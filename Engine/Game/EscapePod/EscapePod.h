#pragma once
#include "Object3D.h"
#include "EscPodState.h"
#include "PodInputUI.h"

/**
* @file EscapePod.h
* @brief 脱出ポッド周りの演出を管理するファイル
*/

#pragma region 前置宣言
namespace MNE
{
	class UIData;
}
#pragma endregion

class EscapePod :public MNE::Object3D
{
public:
	/**
	* @fn Initialize(const Vector3D&)
	* 初期化関数
	* @param pos 表示する位置(演出終了後の位置)
	*/
	void Initialize(const Vector3D& pos);
	/**
	* @fn LoadResources()
	* リソース読み込み処理関数
	*/
	void LoadResources();
	/**
	* @fn Update()
	* 更新処理関数
	*/
	void Update();
	/**
	* @fn Update()
	* 更新処理関数
	*/
	void DrawUI();

private:
	//	現在のState
	std::unique_ptr<EscPodState> currentState_;

	//	ムービー用黒帯のポインター
	MNE::UIData* pLetterBox_ = nullptr;
	//	ドアを開ける入力説明用
	PodInputUI ui_;

	//	ドアが開いたか(TRUEで操作可能に)
	bool openDoor_ = false;
	//	プレイヤー描画用フラグ
	bool drawPlayer_ = false;

public:

#pragma region Getter

	/**
	* @fn GetLetterBoxPtr()
	* ムービー用黒帯データの取得関数
	* @return ムービー用黒帯データのポインタ
	*/
	MNE::UIData* GetLetterBoxPtr();
	/**
	* @fn GetOpenDoor()
	* ドアが開いているかの取得関数
	* @return ドアが開いているか
	*/
	bool GetOpenDoor();
	/**
	* @fn GetDrawPlayer()
	* プレイヤーが表示されているかの取得関数
	* @return プレイヤーが表示されているか
	*/
	bool GetDrawPlayer();

#pragma endregion

#pragma region Setter

	/**
	* @fn SetInputUISprite(const MNE::Sprite&, const MNE::Sprite&)
	* 入力説明用スプライトの情報設定用関数
	* @param uiSprite テキストスプライトの配置情報
	* @param buttonSprite 入力スプライトの配置情報
	*/
	void SetInputUISprite(const MNE::Sprite& uiSprite, const MNE::Sprite& buttonSprite);
	/**
	* @fn StartUICounter()
	* 説明用UIのアニメーション開始用関数
	*/
	void StartUICounter();
	/**
	* @fn SetUIIsActive(bool)
	* 説明用UIの表示非表示設定用関数
	* @param isActive 表示中かどうか
	*/
	void SetUIIsActive(bool isActive);
	/**
	* @fn SetLetterBox(MNE::UIData*)
	* ムービー用の黒帯データのセット用関数
	* @param data 実際のUIDataのポインタ
	*/
	void SetLetterBox(MNE::UIData* data);
	/**
	* @fn SetOpenDoor(bool)
	* ドアが開いているかのセット用関数
	* @param openDoor ドアが開いているか
	*/
	void SetOpenDoor(bool openDoor);
	/**
	* @fn SetDrawPlayer(bool)
	* プレイヤーが表示されているかのセット用関数
	* @param drawplayer プレイヤーが表示されているか
	*/
	void SetDrawPlayer(bool drawplayer);
	/**
	* @fn ResetAnimation()
	* アニメーションを初めからに戻す関数(debug用)
	*/
	void ResetAnimation();
	/**
	* @fn SetNextState(std::unique_ptr<EscPodState>&)
	* 次のState設定用関数(Initialize()もする)
	* @param next 次のState
	*/
	void SetNextState(std::unique_ptr<EscPodState>& next);

#pragma endregion

};

