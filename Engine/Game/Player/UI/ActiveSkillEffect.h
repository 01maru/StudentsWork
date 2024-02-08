//#pragma once
//#include "FrameCounter.h"
//#include "Sprite.h"
//
//class ActiveSkillEffect
//{
//public:
//	void Initialize();
//	void LoadResources();
//	void Update();
//	void Draw();
//
//private:
//	const int16_t SPRITE_NUM = 10;
//	const int32_t MAX_COUNT = 2;
//	int32_t effectTime_ = 120;
//
//	FrameCounter timer_;
//	//	スプライト
//	std::vector<MNE::Sprite> sprites_;
//	//	開始位置
//	Vector2D startPos_ = { 1220, 664 };
//	//	クールダウンスプライトの中心座標
//	Vector2D endPos_ = { 1220, 632 };
//	//	画像サイズ
//	Vector2D texSize_;
//
//	float maxSizeY_ = 504.0f;
//	std::vector<float> moveSizeY_;
//
//private:
//	void StartAnimeUpdate();
//	void EndAnimeUpdate();
//	void TexScaleUpdate();
//
//public:
//	void Start();
//
//};
//
