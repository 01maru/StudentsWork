#include "ObjModel.h"
#include <sstream>
#include <fstream>
#include <assert.h>

using namespace std;

MNE::ObjModel::ObjModel(const char* filename, bool smoothing)
{
	Initialize(filename, smoothing);
}

MNE::ObjModel::~ObjModel()
{
	meshes_.clear();

	materials_.clear();
}

void MNE::ObjModel::AddIndices(const std::vector<uint16_t>& indices, Mesh* mesh)
{
	//for (auto i : indices)
	//{
	//	mesh->AddIndex(i);
	//}
	if (indices.size() == 3)
	{
		mesh->AddIndex(indices[0]);
		mesh->AddIndex(indices[2]);
		mesh->AddIndex(indices[1]);
	}
	else {
		mesh->AddIndex(indices[0]);
		mesh->AddIndex(indices[2]);
		mesh->AddIndex(indices[1]);
		mesh->AddIndex(indices[3]);
		mesh->AddIndex(indices[5]);
		mesh->AddIndex(indices[4]);
	}
}

void MNE::ObjModel::LoadModel(const std::string& modelname, bool smoothing)
{
	std::vector<uint16_t> vertexIndices, uvIndices, normalIndices;
	std::vector<Vector3D> poss;
	std::vector<Vector2D> temp_uvs;
	std::vector<Vector3D> temp_normals;

	//	filePath
	const string directoryPath = "Resources/Model/" + modelname + "/";
	const string filename = modelname + ".obj";

	//	FileOpen
	std::ifstream file_;
	file_.open(directoryPath + filename);
	assert(!file_.fail());

	meshes_.emplace_back();
	Mesh* mesh = &meshes_.back();
	int indexCount = 0;

	//	一行ずつ読み込む
	string line;
	while (getline(file_, line)) {
		istringstream line_stream(line);

		//	行の先頭文字格納変数
		string key;
		//	半角スペース区切りで行の先頭文字列取得
		getline(line_stream, key, ' ');

		//	material
		if (key == "mtllib") {
			string filename_;
			line_stream >> filename_;
			LoadMaterial(directoryPath, filename_);
		}
		
		//	materialの割り当て
		if (key == "usemtl")
		{
			if (mesh->GetMaterial() == nullptr) {
				// マテリアルの名読み込み
				string materialName;
				line_stream >> materialName;

				// マテリアル名で検索し、マテリアルを割り当てる
				auto itr = materials_.find(materialName);
				if (itr != materials_.end()) {
					mesh->SetMaterial(itr->second.get());
				}
			}
		}

		if (key == "o") {

			// メッシュの情報が揃っているなら
			if (mesh->GetVertexCount() > 0) {
				// 頂点法線の平均によるエッジの平滑化
				if (smoothing) {
					mesh->CalcSmoothedNormals();
				}
				// 次のメッシュ生成
				meshes_.emplace_back();
				mesh = &meshes_.back();
				indexCount = 0;
			}

			//// グループ名読み込み
			//string groupName;
			//line_stream >> groupName;

			//// メッシュに名前をセット
			//mesh->SetName(groupName);
		}

		//	Vertex(頂点情報)なら
		if (key == "v") {
			Vector3D pos;
			line_stream >> pos.x;
			line_stream >> pos.y;
			line_stream >> pos.z;
			pos.z = -pos.z;
			//	頂点追加
			poss.emplace_back(pos);
		}

		//	UV(テクスチャ座標)なら
		if (key == "vt") {
			Vector2D uv;
			line_stream >> uv.x;
			line_stream >> uv.y;
			//	V方向反転
			uv.y = 1.0f - uv.y;
			//	テクスチャ座標データ追加
			temp_uvs.emplace_back(uv);
		}

		//	Normal(法線ベクトル)なら
		if (key == "vn") {
			Vector3D normal;
			line_stream >> normal.x;
			line_stream >> normal.y;
			line_stream >> normal.z;
			normal.z = -normal.z;
			//	法線データ追加
			temp_normals.emplace_back(normal);
		}

		//	face(ポリゴン)なら
		if (key == "f") {
			int32_t indexNum = 0;
			std::vector<uint16_t> indices;

			string index_string;
			while (getline(line_stream, index_string, ' '))
			{
				//	頂点ごとのIndex情報取得
				std::istringstream index_stream(index_string);
				uint16_t indexPos, indexNormal, indexUV;
				index_stream >> indexPos;
				//	スラッシュを飛ばす
				index_stream.seekg(1, ios_base::cur);
				index_stream >> indexUV;
				//	スラッシュを飛ばす
				index_stream.seekg(1, ios_base::cur);
				index_stream >> indexNormal;

				//	頂点データ追加
				MNE::ModelVertex vertex{};
				vertex.pos = poss[indexPos - 1];
				vertex.normal = temp_normals[indexNormal - 1];
				vertex.uv = temp_uvs[indexUV - 1];
				vertex.boneWeight[0] = 1.0f;			//	fbxVertex用
				mesh->AddVertex(vertex);

				if (smoothing) {
					mesh->AddSmoothData(indexPos, (unsigned short)mesh->GetVertexCount() - 1);
				}

				if (indexNum >= 3)	//	一面のindex数が3を越えたら(4つ以上は想定外)
				{
					indices.emplace_back((unsigned short)(indexCount - 1));
					indices.emplace_back((unsigned short)indexCount);
					indices.emplace_back((unsigned short)(indexCount - 3));
				}
				else
				{
					indices.emplace_back((unsigned short)indexCount);
				}
				indexNum++;
				indexCount++;
			}

			//	AddIndex
			AddIndices(indices, mesh);
		}
	}

	file_.close();

	if (smoothing) {
		mesh->CalcSmoothedNormals();
	}
}

void MNE::ObjModel::LoadMaterial(const std::string& directoryPath, const std::string& filename)
{
	//	fileOpen
	std::ifstream file;
	file.open(directoryPath + filename);
	if (file.fail()) { assert(0); }

	//	複数マテリアルに対応するために初期値nullptr
	Material* material = nullptr;
	string line;
	while (getline(file, line))
	{
		std::istringstream line_stream(line);

		string key;
		getline(line_stream, key, ' ');

		if (key[0] == '\t') {
			key.erase(key.begin());
		}

		if (key == "newmtl") {
			// 既にマテリアルがあれば
			if (material) {
				// マテリアルをコンテナに登録
				AddMaterial(material);
			}

			// 新しいマテリアルを生成
			material = Material::Create();
			// マテリアル名読み込み
			if (material) {
				line_stream >> material->name_;
			}
		}

		if (material)
		{
			if (key == "Ka") {
				line_stream >> material->ambient_.x;
				line_stream >> material->ambient_.y;
				line_stream >> material->ambient_.z;
			}

			if (key == "Kd") {
				line_stream >> material->diffuse_.x;
				line_stream >> material->diffuse_.y;
				line_stream >> material->diffuse_.z;
			}

			if (key == "Ks") {
				line_stream >> material->specular_.x;
				line_stream >> material->specular_.y;
				line_stream >> material->specular_.z;
			}

			//	画像セット
			if (key == "map_Kd") {
				line_stream >> material->textureFilename_;

				material->filepath_ = directoryPath + material->textureFilename_;
			}
		}
	}
	file.close();

	if (material) {
		// マテリアルを登録
		AddMaterial(material);
	}
}
