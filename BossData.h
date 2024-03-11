#pragma once
#include <cstdint>
#include "Vector3D.h"

/**
* @file BossData.h
* @brief ボスのステータスやパラメーターを管理するためのファイル
*/

class BossData
{
public:
	/**
	* @fn SaveData()
	* データをファイルに保存するための関数
	*/
	void SaveData();
	/**
	* @fn LoadData()
	* データをファイルから読み込むための関数
	*/
	void LoadData();

protected:
	//	HP
	int32_t maxHP_ = 100;

	//	Move

	//	Attack

};

