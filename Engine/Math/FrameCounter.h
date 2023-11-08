#pragma once
#include <cstdint>

/**
* @file FrameCounter.h
* @brief フレームカウントする処理を使いやすくまとめたファイル
*/

class FrameCounter
{
private:
	//	現在の経過フレーム数
	int32_t frameCount_ = 0;
	//	フレームの範囲(0~maxFrameCount)
	int32_t maxFrameCount_ = 0;

	//	範囲外に言った回数
	int32_t count_ = 0;
	//	終了までのnowCountの回数
	int32_t maxCount_ = 1;

	bool isEndless_ = false;
	bool isLoop_ = true;
	bool isIncrement_ = true;
	bool isActive_ = false;

public:
	FrameCounter() {};

	/**
	* @fn StartCount()
	* カウントスタート時に呼ぶ関数
	*/
	void StartCount();
	/**
	* @fn ResetCount()
	* カウントリセットする関数
	*/
	void ResetCount();

	void Initialize(int32_t maxFrameCount, bool isIncrement, bool isLoop = false, int32_t maxCount = 1);
	/**
	* @fn Update()
	* 更新処理関数
	*/
	void Update();

#pragma region Getter

	/**
	* @fn GetIsActive()
	* isActive_のGetter関数
	* @return isActive_の値
	*/
	bool GetIsActive()			{ return isActive_; }
	/**
	* @fn GetIsIncrement()
	* isIncrement_のGetter関数
	* @return isIncrement_の値
	*/
	bool GetIsIncrement()		{ return isIncrement_; }
	/**
	* @fn GetCount()
	* count_のGetter関数
	* @return count_の値
	*/
	int32_t GetCount()			{ return count_; }
	/**
	* @fn GetMaxCount()
	* maxCount_のGetter関数
	* @return maxCount_の値
	*/
	int32_t GetMaxCount()		{ return maxCount_; }
	/**
	* @fn GetFrameCount()
	* frameCount_のGetter関数
	* @return frameCount_の値
	*/
	int32_t GetFrameCount()		{ return frameCount_; }
	/**
	* @fn GetMaxFrameCount()
	* maxFrameCount_のGetter関数
	* @return maxFrameCount_の値
	*/
	int32_t GetMaxFrameCount()	{ return maxFrameCount_; }
	/**
	* @fn GetCountPerMaxCount()
	* frameCount_ / maxFrameCount_の値を返す関数(返り値は0.0f～1.0f)
	* @return frameCount_ / maxFrameCount_の値(maxFrameCount_ == 0の時は0を返す)
	*/
	float GetCountPerMaxCount();

#pragma endregion

#pragma region Setter

	/**
	* @fn SetIsActive(bool)
	* isActive_の値を変更するための関数
	* @param isActive isActive_の変更後の値
	*/
	void SetIsActive(bool isActive);
	/**
	* @fn SetIsIncrement(bool)
	* isIncrement_の値を変更するための関数
	* @param isIncrement isIncrement_の変更後の値
	*/
	void SetIsIncrement(bool isIncrement) { isIncrement_ = isIncrement; }
	/**
	* @fn AddCount()
	* frameCount_をインクリメントする関数
	*/
	void AddCount();
	/**
	* @fn SubCount()
	* frameCount_をデクリメントする関数
	*/
	void SubCount();
	/**
	* @fn SetIsEndless(bool)
	* isEndless_の値を変更するための関数
	* @param isEndless isEndless_の変更後の値
	*/
	void SetIsEndless(bool isEndless) { isEndless_ = isEndless; }

	void SetMaxFrameCount(int32_t maxFrame) { maxFrameCount_ = maxFrame; }

	void SetFrameCountIsMax() { frameCount_ = maxFrameCount_; }

#pragma endregion
};

