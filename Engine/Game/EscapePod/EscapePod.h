#pragma once
#include "Object3D.h"
#include "EscPodState.h"
#include "Sprite.h"
#include "FrameCounter.h"

/**
* @file EscapePod.h
* @brief 脱出ポッド周りの演出を管理するファイル
*/

class EscapePod :public MNE::Object3D
{
public:
	/**
	* @fn Initialize(IModel*)
	* 初期化関数
	* @param model 表示するモデルの設定
	*/
	void Initialize(MNE::IModel* model);
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
	//	UIのスプライト
	MNE::Sprite ui_;
	FrameCounter fadeCounter_;
	bool drawUI_ = false;
	bool openDoor_ = false;
	bool drawPlayer_ = false;
public:

	bool GetOpenDoor() { return openDoor_; }
	bool GetDrawPlayer() { return drawPlayer_; }
	void StartUICounter();

#pragma region Setter

	void SetDrawPlayer(bool drawplayer) { drawPlayer_ = drawplayer; }
	void SetOpenDoor(bool openDoor) { openDoor_ = openDoor; }
	void SetDrawUI(bool drawUI) { drawUI_ = drawUI; }
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

