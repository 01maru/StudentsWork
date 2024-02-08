#include "CharacterHP.h"
#include "Easing.h"

using namespace Easing;
using namespace MyMath;

//-----------------------------------------------------------------------------
// [SECTION] Initialize
//-----------------------------------------------------------------------------

void CharacterHP::Initialize()
{
	//	HPを最大に
	hp_ = maxHP_;
	//	生存中
	isAlive_ = TRUE;

	//	ダメージタイマーの初期化
	damageTimer_.Initialize(animationTime_, TRUE);
}

//-----------------------------------------------------------------------------
// [SECTION] Update
//-----------------------------------------------------------------------------

void CharacterHP::IsAliveUpdate()
{
	const int32_t MIN_FOR_ALIVE = 1;
	//	HPが0以下なら死亡
	if (hp_ <= MIN_FOR_ALIVE) {
		isAlive_ = FALSE;
	}
}

void CharacterHP::HPBarUpdate()
{
	//	ダメージ演出中じゃなかったら処理しない
	if (damageTimer_.GetIsActive() == FALSE) return;

	damageTimer_.Update();
	Vector2D size = damage_.GetSize();
	size.x = EaseIn(damageLen_, 0.0f, damageTimer_.GetCountPerMaxCount(), Triple);
	damage_.SetSize(size);

	damage_.Update();
}

void CharacterHP::Update()
{
	HPBarUpdate();

	bar_.Update();
}

//-----------------------------------------------------------------------------
// [SECTION] Draw
//-----------------------------------------------------------------------------

void CharacterHP::Draw()
{
	//	背景描画
	back_.Draw();

	//	ダメージ演出中だけ表示
	if (damageTimer_.GetIsActive() == TRUE)
	{
		damage_.Draw();
	}

	//	HPバー表示
	bar_.Draw();
}

void CharacterHP::DecHp(int32_t damage)
{
	if (isAlive_ == false) return;

	const int32_t DEATH_LINE = 0;
	hp_ = MyMath::mMax(DEATH_LINE, hp_ - damage);

	IsAliveUpdate();

	Vector2D size = bar_.GetSize();
	size.x = Easing::lerp(0.0f, len_, hp_ / static_cast<float>(maxHP_));
	size.x = MyMath::mMax(size.x, 0.0f);
	bar_.SetSize(size);

	if (damage > 0) {
		damageLen_ = Easing::lerp(0.0f, len_, damage / static_cast<float>(maxHP_));

		Vector2D pos = bar_.GetPosition();
		pos.x += size.x;
		damage_.SetPosition(pos);

		damageTimer_.StartCount();
	}
}

//-----------------------------------------------------------------------------
// [SECTION] Getter
//-----------------------------------------------------------------------------

int32_t CharacterHP::GetHP()
{
	return hp_;
}

int32_t CharacterHP::GetMaxHP()
{
	return maxHP_;
}

bool CharacterHP::GetIsAlive()
{
	return isAlive_;
}

//-----------------------------------------------------------------------------
// [SECTION] Setter
//-----------------------------------------------------------------------------

void CharacterHP::SetMaxHP(int32_t maxHP)
{
	maxHP_ = maxHP;
}

void CharacterHP::SetSprite(const MNE::Sprite& sprite)
{
	//	バーのスプライト設定
	bar_ = sprite;

	//	バーの最大値時の長さ保持
	len_ = bar_.GetSize().x;

	//	バーにの大きさに合わせた背景設定
	back_.Initialize();
	back_.SetSize(bar_.GetSize());
	back_.SetPosition(bar_.GetPosition());
	back_.SetAnchorPoint(bar_.GetAnchorPoint());
	MyMath::Vector4D grayColor(0.1f, 0.1f, 0.1f, 0.5f);
	back_.SetColor(grayColor);
	back_.Update();

	//	ダメージを受けたときのバーの初期化
	damage_.Initialize(bar_.GetTexture());
	damage_.SetSize(bar_.GetSize());
	damage_.SetPosition(bar_.GetPosition());
	damage_.SetAnchorPoint(bar_.GetAnchorPoint());
	MyMath::Vector4D redColor(1.0f, 0.0f, 0.0f, 0.4f);
	damage_.SetColor(redColor);
}

void CharacterHP::SetBarColor(const MyMath::Vector3D& color)
{
	bar_.SetColor(color);
}
