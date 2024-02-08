#include "FrameCounter.h"

void MNE::FrameCounter::StartCount()
{
	ResetCount();
	isActive_ = true;
}

void MNE::FrameCounter::ResetCount()
{
	if(isIncrement_)	frameCount_ = 0;
	else			frameCount_ = maxFrameCount_;
	
	count_ = 0;
}

void MNE::FrameCounter::Initialize(int32_t maxFrameCount, bool isIncrement, bool isLoop, int32_t maxCount)
{
	if (isIncrement_) frameCount_ = 0;
	else			  frameCount_ = maxFrameCount;

	maxFrameCount_ = maxFrameCount;
	maxCount_ = maxCount;
	isLoop_ = isLoop;
	isIncrement_ = isIncrement;

	isActive_ = false;
}

void MNE::FrameCounter::Update()
{
	if (isActive_) {

		if (isIncrement_)	//	インクリメント
		{
			frameCount_++;

			//	範囲外
			if (frameCount_ >= maxFrameCount_)
			{
				//	範囲外になったらインクリメント
				count_++;

				bool countContinue = isEndless_ || count_ < maxCount_;
				if (countContinue) {
					//	ループだったらデクリメントに変更
					if (isLoop_)	isIncrement_ = false;
					//	ループじゃなかったら初期化
					else		frameCount_ = 0;
				}
			}
		}
		else				//	デクリメント
		{
			frameCount_--;

			//	範囲外
			if (frameCount_ <= 0)
			{
				//	範囲外になったらインクリメント
				count_++;

				bool countContinue = isEndless_ || count_ < maxCount_;
				if (countContinue) {
					//	ループだったらインクリメントに変更
					if (isLoop_)	isIncrement_ = true;
					//	ループじゃなかったら初期化
					else		frameCount_ = maxFrameCount_;
				}
			}
		}

		//	エンドレスじゃなくcountが範囲外になったら
		if (!isEndless_ && count_ >= maxCount_)	isActive_ = false;
	}
}

float MNE::FrameCounter::GetCountPerMaxCount()
{
	if (maxFrameCount_ != 0) return (float)frameCount_ / maxFrameCount_;

	//	0で割らないように
	return 0.0f;
}

void MNE::FrameCounter::SetIsActive(bool isActive)
{
	if (isActive_ != isActive && isActive) ResetCount();

	isActive_ = isActive;
}

void MNE::FrameCounter::AddCount()
{
	if (frameCount_ < maxFrameCount_) frameCount_++;
}

void MNE::FrameCounter::SubCount()
{
	if (frameCount_ > 0) frameCount_--;
}
