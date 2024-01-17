#pragma once
#include <json.hpp>
#include "JSONData.h"

/**
* @file JSONLoader.h
* @brief brenderで作成したjson型のレベルデータを読み込み、表示するためのファイル
*/

namespace MNE
{

	class JSONLoader
	{
	private:
		void LoadCameraData(JSONData& data, nlohmann::json_abi_v3_11_2::detail::iter_impl<nlohmann::json_abi_v3_11_2::json>& itr);

		void LoadEmpty(JSONData& data, nlohmann::json_abi_v3_11_2::detail::iter_impl<nlohmann::json_abi_v3_11_2::json>& itr);

		void LoadObjectData(JSONData& data, nlohmann::json_abi_v3_11_2::detail::iter_impl<nlohmann::json_abi_v3_11_2::json>& itr, ObjectData* parent);
	public:
		/**
		* @fn LoadJSON(const std::string&)
		* レベルデータ読み込み用の関数
		* @param jsonname レベルデータのファイル名、.jsonは必要ない(例 : test.json -> testのみ)
		*/
		JSONData LoadJSON(const std::string& jsonname);
	};

}
