#include "MoveParticle.h"
#include "Easing.h"
#include "TextureManager.h"
#include "GPipeline.h"

void MoveParticle::Initialize(const Vector3D& pos, const Vector3D& velocity, float startScale, float maxScale, int32_t lifeTime, int32_t texHandle)
{
	IParticle::Initialize();

	SetPosition(pos);
	scale_ = startScale;

	// 向き読み込み
	spd_ = velocity;
	// 大きさ読み込み
	maxScale_ = maxScale;
	startScale_ = startScale;

	maxTime_ = lifeTime;

	texHandle_ = texHandle;

	isBillboard_ = true;

	blendMord_ = Blend::ADD_BLEND;
}

void MoveParticle::Update()
{
	scale_ = (float)Easing::EaseIn(startScale_, maxScale_, (float)timer_ / (float)maxTime_, 2);

	// 移動
	IParticle::vertex_ += spd_;
	IParticle::TransferVertex();

	// 消える処理
	if (++timer_ >= maxTime_)
	{
		isEnd_ = true;
	}
}
