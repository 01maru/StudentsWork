#pragma once
#include <string>
#include "Vector3D.h"
#include "IModel.h"
#include "Object3D.h"
#include <json.hpp>

/**
* @file JSONLoader.h
* @brief brenderで作成したjson型のレベルデータを読み込み、表示するためのファイル
*/

struct ObjectData
{
	//	ファイル名
	std::string fileName;
	//	オブジェクト情報
	Vector3D translation;
	Vector3D rotation;
	Vector3D scaling;
	//	親情報
	ObjectData* parent = nullptr;
};

struct CameraData
{
	Vector3D eye;
	int32_t targetID;
	Vector3D target;
};

struct PlayerData
{
	Vector3D pos;
	Vector3D rotation;
};

struct LevelData
{
	std::vector<ObjectData> objects;
};

class JSONLoader
{
private:
	//	モデルデータコンテナ
	std::map<std::string, std::unique_ptr<MNE::IModel>> models_;
	//	オブジェクトデータ
	std::vector<std::unique_ptr<MNE::Object3D>> objects_;
	std::unique_ptr<LevelData> levelData_;

	std::map<std::string, CameraData> cameraData_;
	PlayerData playerData_;

	void LoadObjectData(nlohmann::json_abi_v3_11_2::detail::iter_impl<nlohmann::json_abi_v3_11_2::json>& itr, ObjectData* parent);
	void LoadModel();
public:
	/**
	* @fn LoadJSON(const std::string&)
	* レベルデータ読み込み用の関数
	* @param jsonname レベルデータのファイル名、.jsonは必要ない(例 : test.json -> testのみ)
	*/
	void LoadJSON(const std::string& jsonname);

	/**
	* @fn MatUpdate()
	* 読み込んだデータの行列更新用関数
	*/
	void MatUpdate();

	/**
	* @fn Draw(bool)
	* 読み込んだデータの表示用関数
	* @param drawShadow 影描画用か変更用引数
	*/
	void Draw(bool drawShadow);

#pragma region Getter

	/**
	* @fn GetPlayerSpownPoint()
	* プレイヤーの情報を返す関数
	* @return プレイヤーの位置と回転を返す
	*/
	const PlayerData& GetPlayerSpownPoint() { return playerData_; }
	CameraData GetCameraData(const std::string& tagName) { return cameraData_[tagName]; }

#pragma endregion
};

