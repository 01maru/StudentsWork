#include "ClearUI.h"
#include "SceneManager.h"

using namespace MNE;

//-----------------------------------------------------------------------------
// [SECTION] Initialize
//-----------------------------------------------------------------------------

void ClearUI::LoadResources()
{
	//	配置データ
	data_.LoadData("Clear");
}

//-----------------------------------------------------------------------------
// [SECTION] Update
//-----------------------------------------------------------------------------

void ClearUI::Update()
{
	//	実行中じゃなかったら処理しない
	if (isActive_ == FALSE) return;

	//	アニメーションが終わったら
	if (data_.GetIsEndAnimation() == TRUE)
	{
		//	実行終了
		isActive_ = FALSE;

		//	タイトルシーンへ
		SceneManager::GetInstance()->SetNextScene("TITLESCENE");
	}

	//	データオブジェクト更新
	data_.Update();
}

//-----------------------------------------------------------------------------
// [SECTION] Draw
//-----------------------------------------------------------------------------

void ClearUI::Draw()
{
	//	実行中じゃなかったら処理しない
	if (isActive_ == FALSE) return;

	//	配置データ表示
	data_.Draw();
}
