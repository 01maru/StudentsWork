#pragma once

#pragma region 前置宣言

class EscapePod;

#pragma endregion

class EscPodState
{
public:
	/**
	* @fn Initialize()
	* 初期化関数
	*/
	virtual void Initialize() {};
	/**
	* @fn Update()
	* 更新処理関数
	*/
	virtual void Update() {};

protected:
	//	親ポッドのポインター
	static EscapePod* sPod_;

public:

#pragma region Setter

	/**
	* @fn SetPod(EscapePod*)
	* 親のPod設定用関数
	* @param pod 親のPod
	*/
	static void SetPod(EscapePod* pod);

#pragma endregion
};

